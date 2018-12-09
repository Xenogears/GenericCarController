#ifndef __GPS__H__
	#define __GPS__H__

	#include "configuration.h"
  
	#if COMPILE_GPS
		#include "serial_controller.h"
		#include "traccar.h"
		#include "lib/gps/NMEAGPS.h" 
		#include "lib/queue/Queue_static.h"

		#if GPS_SERIAL_MODE == HW_SERIAL
			#include "lib/serial/NeoHWSerial.h"  
		#elif GPS_SERIAL_MODE == IC_SERIAL
			#include "lib/serial/NeoICSerial.h"    
			extern NeoICSerial gpsSerial;
		#elif GPS_SERIAL_MODE == SW_SERIAL
			#include "lib/serial/NeoSWSerial.h"    
			extern NeoSWSerial gpsSerial;
		#endif 

		extern NMEAGPS gpsHandler;   

		void gpsAcquireFix();
		bool gpsIsFixAvailable();
		gps_fix *gpsGetCurrentFix();

		void gpsEnableListenSerial();
		void gpsDisableListenSerial();
		bool gpsHandleSerial(String *cmd, String *params);
		bool setupGPS();    
	#endif
#endif
