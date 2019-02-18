//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#ifndef __POWER__H__
	#define __POWER__H__

	#include "configuration.h"
	#include <avr/power.h>
	#include <avr/sleep.h>
	#include <avr/wdt.h>

	enum POWER_MODE
	{
		POWER_MODE_OFF        = 0,
		POWER_MODE_KEY_IN     = 4,
		POWER_MODE_CONTACT    = 1,
		POWER_MODE_IGNITION   = 2,
		POWER_MODE_ON         = 3,
	};
  
	extern volatile uint8_t currentPowerMode;

	bool powIsPowerOn() __attribute__((always_inline));
	bool setupPOW();
	void checkPOW();
#endif
