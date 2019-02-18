//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#include "traccar.h"
#include "cmd_serial.h"

volatile static uint8_t traccarStatus = 0;
volatile static uint8_t traccarTimerTicks = 0;  

static uint8_t getTimerThreshold();
static void traccarTimer_ISR();

static TRACCAR_NOTIFICATION traccarCurrentNotification;
static Queue<TRACCAR_NOTIFICATION, TRACCAR_NOTIFICATION_QUEUE_LENGTH, false> traccarNotifications = Queue<TRACCAR_NOTIFICATION, TRACCAR_NOTIFICATION_QUEUE_LENGTH, false>(FIFO, true);

#if COMPILE_GSM_MODEM
	static void traccarNotificationOnSuccess(char *path);
	static bool traccarNotificationOnError(GSM_GET_ERROR err, char *path);
#endif

#if COMPILE_GPS
	#ifdef TRACCAR_ON_GPS_ACQUIRED_CUSTOM_CALLBACK
		void TRACCAR_ON_GPS_ACQUIRED_CUSTOM_CALLBACK(gps_fix *fix);
	#endif
	
	static bool traccarAddNotification(float lat = 0, float lon = 0, uint32_t date = 0);
#else
	static bool traccarAddNotification();
#endif

#if TRACCAR_USE_TIMER_TWO == 0
	static bool traccarTimerEnabled = false;
	static unsigned long traccarNextTickMillis = 0;
#endif
  
#if USE_STATIC_MEM
	static char traccarURL[TRACCAR_MAX_HTTP_URL_LEN];
#endif

//PUBLIC FUNCTIONS
bool traccarCanNotifyLocation()
{
	#if COMPILE_GSM_MODEM
		if (!gsmIsConnectedToInternet() || gsmGetHasActiveRequest())
			return false;		
	#endif	

	return !(traccarStatus & TRCSTAT_SENDING) && !traccarNotifications.isEmpty();
}

bool traccarNotifyLocation(TRACCAR_NOTIFICATION *notification)
{  
	char temp[16];
	#if USE_STATIC_MEM == 0
		char *traccarURL = (char*)malloc(TRACCAR_MAX_HTTP_URL_LEN);
		if(traccarURL == NULL)        
			return false;   
	#endif
  
	traccarURL[0] = '\0'; 
  
	strcat_P(traccarURL, PSTR("/?id="));
	strcat_P(traccarURL, PSTR(TRACCAR_DEVICE_ID)); 
  
	#if TRACCAR_COMPILE_ALARM_MODE
		if(traccarIsAlarmModeEnabled())
		{
			strcat_P(traccarURL, PSTR("&alarm="));
			strcat_P(traccarURL, sAlarm);
		}
	#endif
  
	strcat_P(traccarURL, PSTR("&ignition="));
	strcat_P(traccarURL, (powIsPowerOn() ? PSTR("true") : PSTR("false")));    

	#if COMPILE_GPS
		if(notification->lat != 0.0f && notification->lon != 0.0f)
		{      
			dtostrf(notification->lat, 8, 6, temp);
			strcat_P(traccarURL, PSTR("&lat="));
			strcat(traccarURL, temp);

			dtostrf(notification->lon, 8, 6, temp);
			strcat_P(traccarURL, PSTR("&lon="));
			strcat(traccarURL, temp);
		} 
	
		#if TRACCAR_SEND_TIMESTAMP
			if(notification->timestamp > 0)
			{    
				strcat_P(traccarURL, PSTR("&timestamp="));
				ltoa(notification->timestamp, temp, 10);
				strcat(traccarURL, temp);    
			}
		#endif
	#endif

	#if COMPILE_CAR_STATUS_MONITOR
		#if MON_FETCH_VOLTAGE && MON_REPORT_VOLTAGE_TRACCAR
			strcat_P(traccarURL, PSTR("&batt="));
			dtostrf(currentCarStatus.voltage, 2, 2, temp);			
			strcat(traccarURL, temp);
		#endif
		#if MON_FETCH_ENGINE_TEMP && MON_REPORT_ENGINE_TEMP_TRACCAR
			strcat_P(traccarURL, PSTR("&temp="));
			itoa(currentCarStatus.engineTemp, temp, 10);
			strcat(traccarURL, temp);
		#endif
		#if MON_FETCH_SPEED && MON_REPORT_SPEED_TRACCAR
			strcat_P(traccarURL, PSTR("&speed="));			
			itoa(currentCarStatus.speed, temp, 10);
			strcat(traccarURL, temp);
		#endif
	#endif

	if(CONFIG.get(PRINT_TRACCAR_INFO))	
		cmdSerial.println(traccarURL);

	#if COMPILE_GSM_MODEM
		return gsmHTTPGet(TRACCAR_HOST, TRACCAR_PORT, traccarURL, NULL, traccarNotificationOnSuccess, traccarNotificationOnError);
	#else
		traccarStatus &= TRCSTAT_SENDING;

		#if USE_STATIC_MEM == 0
			free(traccarURL);
		#endif
    
		return true;
	#endif  
}

void traccarEnableNotificationTask()
{
	#if TRACCAR_USE_TIMER_TWO
		MsTimer2::start();
	#else
		traccarTimerEnabled = true;
	#endif	  
}

void traccarDisableNotificationTask()
{
	#if TRACCAR_USE_TIMER_TWO
		MsTimer2::stop();
	#else
		traccarTimerEnabled = false;
	#endif	  
}

void checkTraccar()
{
	#if TRACCAR_USE_TIMER_TWO == 0		
		unsigned long now = millis();
		if(traccarTimerEnabled && traccarNextTickMillis <= now)
		{
			traccarNextTickMillis = (now + TRACCAR_TIMER_INTERVAL);
			traccarTimer_ISR();
		}
	#endif
			
	if ((traccarStatus & TRCSTAT_ENQUED_TO_BE_SENT) && traccarCanNotifyLocation()) //READY TO SEND HAS THE HIGHER PRIORITY
	{
		if (traccarNotifications.pull(&traccarCurrentNotification))
		{		
			traccarStatus &= ~TRCSTAT_ENQUED_TO_BE_SENT;
			traccarStatus |= TRCSTAT_SENDING;

			traccarNotifyLocation(&traccarCurrentNotification);
		}		
	}

	if ((traccarStatus & TRCSTAT_PENDING))
	{
		#if MULTIPLE_SW_SERIALS && GSM_SERIAL_MODE == SW_SERIAL
			if (traccarStatus &= TRCSTAT_SENDING)
				return;
		#endif

		traccarStatus &= ~TRCSTAT_PENDING; //REMOVE PENDING FLAG

		#if COMPILE_GPS				
			gpsAcquireFix();
			traccarStatus |= TRCSTAT_ACQUIRING_GPS_FIX; //SET ACQUIRING GPS FIX FLAG
		#else			
			if(traccarAddNotification())
				traccarStatus |= TRCSTAT_READY_TO_SEND; //SET READY FLAG
		#endif		
	}

	#if COMPILE_GPS		
	if ((traccarStatus & TRCSTAT_ACQUIRING_GPS_FIX))
	{
		gps_fix *fix = gpsGetCurrentFix();
		if (fix != NULL)
		{
			#ifdef TRACCAR_ON_GPS_ACQUIRED_CUSTOM_CALLBACK
				TRACCAR_ON_GPS_ACQUIRED_CUSTOM_CALLBACK(fix);
			#endif

			#if TRACCAR_SEND_TIMESTAMP
				if (!traccarAddNotification(fix->latitude(), fix->longitude(), fix->dateTime))
					return;
			#else
				if (!traccarAddNotification(fix->latitude(), fix->longitude()))
					return;
			#endif

			#if MULTIPLE_SW_SERIALS && GSM_SERIAL_MODE == SW_SERIAL && defined(GSM_IS_PRIORITARY_SW_SERIAL)
				gsmEnableListenSerial();				
			#endif

			traccarStatus &= TRCSTAT_ACQUIRING_GPS_FIX;
			traccarStatus |= TRCSTAT_ENQUED_TO_BE_SENT;
		}
	}
	#endif
}

bool setupTraccar()
{
	#if TRACCAR_USE_TIMER_TWO
		MsTimer2::set(TRACCAR_TIMER_INTERVAL, traccarTimer_ISR);
	#else               
		traccarNextTickMillis = (millis() + TRACCAR_TIMER_INTERVAL);
	#endif

	if (CONFIG.get(ENABLE_TRACCAR_NOTIFICATION))
		traccarEnableNotificationTask();
	else
		traccarDisableNotificationTask();

	return true;
}

bool traccarHasPendingNotification()
{
	return (traccarStatus & TRCSTAT_PENDING);
}

#if TRACCAR_COMPILE_ALARM_MODE
	static bool traccarAlarmModeStatus = false;

	void traccarEnableAlarmMode()
	{
		traccarAlarmModeStatus = true;
	}

	void traccarDisableAlarmMode()
	{
		traccarAlarmModeStatus = false;
	}

	void traccarToggleAlarmMode()
	{
		traccarAlarmModeStatus = !traccarAlarmModeStatus;
	}

	bool traccarIsAlarmModeEnabled()
	{
		return traccarAlarmModeStatus;
	}
#endif

//PRIVATE FUNCTIONS
#if COMPILE_GPS
	bool traccarAddNotification(float lat = 0, float lon = 0, uint32_t date = 0)
	{
		TRACCAR_NOTIFICATION n;
		n.sendRetries = 0;

		n.lat = lat;
		n.lon = lon;

		#if TRACCAR_SEND_TIMESTAMP
			n.timestamp = date;
		#endif

		return traccarNotifications.push(&n);
	}
#else
	bool traccarAddNotification()
	{
		TRACCAR_NOTIFICATION n;
		n.sendRetries = 0;

		return traccarNotifications.push(&n);
	}
#endif

static void traccarSetPendingNotification()
{
	traccarStatus |= TRCSTAT_PENDING;
}

static uint8_t getTimerThreshold()
{
	#if TRACCAR_COMPILE_ALARM_MODE
		return (traccarAlarmModeStatus ? TRACCAR_NOTIFICATION_INTERVAL_ALARM : (powIsPowerOn() ? TRACCAR_NOTIFICATION_INTERVAL_ON : TRACCAR_NOTIFICATION_INTERVAL_OFF)) / (TRACCAR_TIMER_INTERVAL / 1000);
	#else
		return (powIsPowerOn() ? TRACCAR_NOTIFICATION_INTERVAL_ON : TRACCAR_NOTIFICATION_INTERVAL_OFF) / (TRACCAR_TIMER_INTERVAL / 1000);
	#endif
}

static void traccarTimer_ISR()
{      
	if(traccarTimerTicks < 0xFF) //PREVENT OVERFLOW
		traccarTimerTicks++;

	uint8_t traccarThreshold = getTimerThreshold();	
	if (traccarTimerTicks < traccarThreshold)
		return;

	traccarSetPendingNotification();
	traccarTimerTicks = 0;
}

#if COMPILE_GSM_MODEM
	static bool traccarNotificationOnError(GSM_GET_ERROR err, char *path)
	{
		traccarStatus &= TRCSTAT_SENDING;

		if (err != ERROR_GET_TIMEOUT)
			return false;

		traccarCurrentNotification.sendRetries++;
		if (traccarCurrentNotification.sendRetries < TRACCAR_MAX_REQUEST_RETRIES)
			return true; //RESEND NOTIFICATION
		else
			return false; //IGNORE CURRENT NOTIFICATION
	}

	static void traccarNotificationOnSuccess(char *path)
	{
		traccarStatus &= TRCSTAT_SENDING;
	}
#endif