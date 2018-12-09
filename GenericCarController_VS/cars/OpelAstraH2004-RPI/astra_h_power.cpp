#include "../../configuration.h"

#if COMPILE_CAN && CAR_MODEL == OPEL_ASTRA_H_2004_MIN_CAR 
	#include "astra_h_power.h"

	void astraHPowerOnChange(uint8_t previousStatus, uint8_t newStatus)
	{		
		char temp[4];
		itoa(newStatus, (char*)&temp, 10);

		char *params[] = { (char*)&temp };
		astraHSendCommand('P', "STAT", params, 1);

		
		if(!(previousStatus & POWER_MODE_IGNITION) && (newStatus & POWER_MODE_IGNITION)) //changed to on
		{   						
			#if COMPILE_TRACCAR
				if (CONFIG.get(ENABLE_POWER_NOTIFICATION))
					traccarAddNotification();				
			#endif
		}
		else if((previousStatus & POWER_MODE_IGNITION) && !(newStatus & POWER_MODE_IGNITION)) //changed to off
		{			
			#if COMPILE_TRACCAR
				if (CONFIG.get(ENABLE_POWER_NOTIFICATION))
					traccarAddNotification();
			#endif
		}
	}  
#endif
