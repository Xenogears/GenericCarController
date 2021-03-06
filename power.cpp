//
// Author:	�lvaro Graci� Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#include "power.h"

volatile uint8_t currentPowerMode = POWER_MODE_OFF;

bool powIsPowerOn()
{ 
	return (currentPowerMode & POWER_MODE_IGNITION); 
}

bool setupPOW()
{       		
	#if POW_ENABLE_WATCHDOG		
		MCUSR = 0;
		wdt_disable();

		wdt_enable(POW_WATCHDOG_TIMEOUT); //System Reset Mode
	#endif
}

void checkPOW()
{
	#if POW_ENABLE_WATCHDOG
		wdt_reset();
	#endif
}
