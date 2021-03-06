//
// Author:	�lvaro Graci� Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#include "../../configuration.h"

#if COMPILE_CAN && CAR_MODEL == OPEL_ASTRA_H_2004_CAR 
	#include "astra_h_power.h"

	void astraHPowerOnChange(uint8_t previousStatus, uint8_t newStatus)
	{
		if(CONFIG.get(PRINT_STATUS_INFO))
		{
			cmdSerialPrint_P(sPOW);
			cmdSerialPrint_P(sTwoDots);		
			cmdSerialPrint_P(newStatus & POWER_MODE_KEY_IN ? sIn : sOut);
			cmdSerialPrint_P(sComma);

			if (newStatus == POWER_MODE_OFF || newStatus == POWER_MODE_KEY_IN)
				cmdSerialPrintln_P(sOff);
			else
				cmdSerialPrintln_P(sOn);
		}

		
		if(!(previousStatus & POWER_MODE_IGNITION) && (newStatus & POWER_MODE_IGNITION)) //changed to on
		{   			
			#if COMPILE_ALARM_LED
				astraHAlarmLEDDisable();
				if (CONFIG.get(ENABLE_ALARM_LED))
					astraHAlarmLEDSetOff();
			#endif

			#if COMPILE_CAR_STATUS_MONITOR
				if (CONFIG.get(ENABLE_CAR_STATUS_MONITOR))
					astraHRequestMeasuringBlocks(&DEFAULT_CAN);
			#endif
       
			#if COMPILE_CAN_PERIODICAL
				if (CONFIG.get(ENABLE_RADIO_SIMULATION))
					DEFAULT_CAN.addPeriodicalCmd((CAN_COMMAND_PERIODICAL*)&PER_EHU_UNK);
			#endif

			#if COMPILE_DIS
				astraHDISSetup();
				if (CONFIG.get(ENABLE_DISPLAY))
					astraHDISStart();
			#endif

			#if COMPILE_TEST_MODE
				astraHTestModeSetup();
				if (CONFIG.get(ENABLE_TEST_MODE))
				{			
					DEFAULT_CAN.sendCmd((CAN_COMMAND*)&CMD_EHU_REQ_BC_STATUS); //CAUSES ERROR ON MS CAN
					astraHTestModeTryEnable();				
				}
			#endif

			#if COMPILE_TRACCAR
				if (CONFIG.get(ENABLE_POWER_NOTIFICATION))
					traccarAddNotification();				
			#endif
		}
		else if((previousStatus & POWER_MODE_IGNITION) && !(newStatus & POWER_MODE_IGNITION)) //changed to off
		{
			#if COMPILE_DIS
				astraHDISStop();
			#endif
        
			#if COMPILE_ALARM_LED
				astraHAlarmLEDSetup();
			#endif        

			#if COMPILE_CAN_PERIODICAL
				DEFAULT_CAN.clearPeriodicalCmds();
			#endif

			#if COMPILE_TRACCAR
				if (CONFIG.get(ENABLE_POWER_NOTIFICATION))
					traccarAddNotification();
			#endif
		}
	}  
#endif
