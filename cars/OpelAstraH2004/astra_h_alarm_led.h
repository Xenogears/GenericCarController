//
// Author:	�lvaro Graci� Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#ifndef __ALARM_LED__H__
	#define __ALARM_LED__H__

	#include "../../customArduino.h"
	#include "../../lib/others/FastGPIO.h"
	#include "../../configuration.h"

	#ifdef ALARM_LED_PIN
		#include <stdint.h>
		#if ALARM_LED_USE_TIMER_ONE
			#include "../../lib/timer/TimerOne.h"
		#else
			void astraHAlarmLEDCheck();
		#endif
  
		extern volatile bool astraHAlarmLEDIsOn;    
           
		void astraHAlarmLEDSetOff();  
		void astraHAlarmLEDSetOn();
		bool astraHAlarmLEDSetup();
		void astraHAlarmLEDEnable() __attribute__((always_inline));
		void astraHAlarmLEDDisable() __attribute__((always_inline));
	#endif
#endif
