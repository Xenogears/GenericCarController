//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#ifndef __CAR_STATUS__H__
	#define __CAR_STATUS__H__  
	#include "configuration.h"  
	#include "can_controller.h"
	#include "can_command.h"

	#define MON_VOLTAGE_BITMASK			(1 << 0)
	#define MON_ENGINE_TEMP_BITMASK		(1 << 1)
	#define MON_SPEED_BITMASK			(1 << 2)

	#if COMPILE_CAR_STATUS_MONITOR
		typedef struct CAR_STATUS
		{
			#if MON_FETCH_VOLTAGE
				float voltage = 0.0f;
			#endif
			#if MON_FETCH_ENGINE_TEMP
				uint8_t engineTemp = 0;
			#endif
			#if MON_FETCH_SPEED
				uint8_t speed = 0;
			#endif

			bool updated = false;
		} CAR_STATUS;

		#ifdef MON_ON_ALARM_STATUS_CHANGED_HANDLER
			void MON_ON_ALARM_STATUS_CHANGED_HANDLER(uint8_t alarm);
		#endif

		extern struct CAR_STATUS currentCarStatus;
		void checkCarStatus();
	#endif
#endif