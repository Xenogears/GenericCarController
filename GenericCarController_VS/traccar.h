#ifndef __TRACCAR_H__
	#define __TRACCAR_H__  

	#include "customArduino.h"
	#include <avr/pgmspace.h>  
	#include "configuration.h"
	#include "power.h"
	#include "gps.h"
	#include "gsm.h"
	#include "car_status.h"
	#include "lib/queue/Queue_static.h"

	#if TRACCAR_USE_TIMER_TWO
		#include "lib/timer/MsTimer2.h"
	#endif

	typedef enum TRACCAR_STATUS 
	{
		TRCSTAT_PENDING					= 1,
		TRCSTAT_ACQUIRING_GPS_FIX		= 2,
		TRCSTAT_ENQUED_TO_BE_SENT		= 4,
		TRCSTAT_SENDING					= 8,
	} TRACCAR_STATUS;

	typedef struct TRACCAR_NOTIFICATION
	{
		#if COMPILE_GPS
			float lat;
			float lon;

			#if defined(GPS_FIX_DATE) && defined(GPS_FIX_TIME)
				uint32_t timestamp;
			#endif
		#endif

		uint8_t sendRetries;
	} TRACCAR_NOTIFICATION;

	#if TRACCAR_COMPILE_ALARM_MODE
		void traccarEnableAlarmMode();
		void traccarDisableAlarmMode();
		bool traccarIsAlarmModeEnabled();
		void traccarToggleAlarmMode();
	#endif	
  
	#if COMPILE_GPS
		bool traccarAddNotification(float lat = 0, float lon = 0, uint32_t date = 0);
	#else
		bool traccarAddNotification();
	#endif

	bool traccarCanNotifyLocation();
	bool traccarNotifyLocation(TRACCAR_NOTIFICATION *notification);
	void traccarEnableNotificationTask();
	void traccarDisableNotificationTask();
	void traccarSetPendingNotification();
	bool traccarHasPendingNotification();
	void checkTraccar();
	bool setupTraccar();
#endif
