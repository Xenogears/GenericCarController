//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#include "gps.h"
#include "cmd_serial.h"

#if COMPILE_GPS  	
	#if GPS_SERIAL_MODE == SW_SERIAL		
		static NeoSWSerial gpsSerial(GPS_RX_PIN, GPS_TX_PIN);
	#elif GPS_SERIAL_MODE == IC_SERIAL
		static NeoICSerial gpsSerial;
		static bool gpsListenSerial = false;	
	#elif GPS_SERIAL_MODE == HW_SERIAL
		static bool gpsListenSerial = false;
		#define gpsSerial GPS_HW_SERIAL;
	#endif
  
	static NMEAGPS gpsHandler;   
	static gps_fix currentGPSFix;
	//static volatile bool gpsFixAvailable = false;

	static bool(*gpsAcquireCallback)(gps_fix *fix);

	//PRIVATE FUNCTIONS	
	void gpsSerial_ISR(uint8_t c)
	{
		#if GPS_SERIAL_MODE == HW_SERIAL || GPS_SERIAL_MODE == IC_SERIAL
			if (!gpsListenSerial)
				return;
		#endif		
  
		if(CONFIG.get(PRINT_GPS_INFO))
			cmdSerial.print((char)c);

		gpsHandler.handle(c);

		#if !GPS_SERIAL_ALWAYS_ENABLED
			if(gpsHandler.available())
				gpsDisableListenSerial();
		#endif
	}
  
	//PUBLIC FUNCTIONS
	void gpsAcquireFix()
	{
		#if !GPS_SERIAL_ALWAYS_ENABLED
			#if MULTIPLE_SW_SERIALS	&& GPS_SERIAL_MODE == SW_SERIAL
				NeoSWSerial::disableCurrentListener();
			#endif

			gpsEnableListenSerial();
		#endif
	}

	void gpsEnableListenSerial()
	{
		#if GPS_SERIAL_MODE == SW_SERIAL
			gpsSerial.listen();
		#else
			gpsListenSerial = false;
		#endif
	}

	void gpsDisableListenSerial()
	{
		#if GPS_SERIAL_MODE == SW_SERIAL
			gpsSerial.ignore();
		#else
			gpsListenSerial = false;
		#endif
	}

	bool gpsIsFixAvailable()
	{
		return gpsHandler.available();
	}

	gps_fix *gpsGetCurrentFix()
	{
		if (gpsHandler.available())
		{
			currentGPSFix = gpsHandler.read();
			return &currentGPSFix;
		}
		else return NULL;
	}

	bool gpsHandleSerial(String *cmd, String *params)
	{               
		if(cmd->equals(F("write")))
		{
		  gpsSerial.print(*params);
		  gpsSerial.print(F("\r\n"));
		}
       
		return true;
	}

	bool setupGPS()
	{    
		gpsSerial.begin(GPS_BAUDRATE);    
		gpsSerial.attachInterrupt(gpsSerial_ISR);		

		#if !GPS_SERIAL_ALWAYS_ENABLED
			gpsDisableListenSerial();
		#endif

		#if COMPILE_SERIAL_CONTROL && SERIAL_CONTROL_MODE >= SC_FULL
			SerialController.addHandler(new SERIAL_HANDLER(sGPS, gpsHandleSerial));
		#endif
    
		return true;
	}
#endif
