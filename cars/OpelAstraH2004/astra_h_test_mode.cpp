//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#include "../../configuration.h"

#if CAR_MODEL == OPEL_ASTRA_H_2004_CAR  && COMPILE_CAN && COMPILE_TEST_MODE
	#include "astra_h_test_mode.h"

	bool astraHTestModeEnabled = false;		//false = Normal | true = Test
	bool astraHTestModeEnabling = false;	//True if test mode is setting up
  
	static uint8_t astraHTestModeCount = 0;
	static unsigned long astraHTestNextTXMillis = 0;

	bool astraHTestModeSetup()
	{
		astraHTestModeCount = 0;		
		astraHTestModeEnabled = false;        
		astraHTestModeEnabling = false;
	}
  
	void astraHTestModeTryEnable()
	{
		astraHTestModeEnabling = true;
		astraHTestNextTXMillis = (millis() + TEST_MODE_INITIAL_DELAY);
	}
  
	void astraHTestModeStep()
	{          		
		if (!astraHTestModeEnabled)
		{
			if (astraHTestModeCount < 0xFF) //255 cmds     
			{
				unsigned long now = millis();
				if (astraHTestNextTXMillis <= now)
				{					
					if (DEFAULT_CAN.sendCmd((CAN_COMMAND*)&CMD_EHU_SETTINGS_PRESS, astraHTestModeCount))
					{
						astraHTestNextTXMillis = (now + TEST_MODE_TX_INTERVAL);
						astraHTestModeCount++;
					}
				}
			}
			else //ERROR
			{
				astraHTestModeCount = 0;
				astraHTestModeEnabling = false;      
			}
		}
		else //OK!
		{            
			if (DEFAULT_CAN.sendCmdSequence((CAN_COMMAND*)&CMD_EHU_BC_PRESS, (CAN_COMMAND*)&CMD_EHU_BC_RELEASE, 1, true))
			{
				astraHTestModeCount = 0;
				astraHTestModeEnabling = false;
			}
		}
	} 
#endif