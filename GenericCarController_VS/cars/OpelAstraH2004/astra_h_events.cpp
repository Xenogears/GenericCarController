//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#include "../../configuration.h"

#if CAR_MODEL == OPEL_ASTRA_H_2004_CAR 
	#include "astra_h_events.h"
	#include "../../cmd_serial.h"

	#if COMPILE_SERIAL_CONTROL    
		bool astraHLOGHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength);
		bool astraHOPTHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength);
		bool astraHDISHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength);
		bool astraHBCHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength);
		bool astraHPOWHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength);

		#if COMPILE_REMOTE_DOOR
			bool astraHRDHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength);
		#endif
		#if COMPILE_RADIO_CONTROL
			bool astraHRADHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength);
		#endif
	#endif

	#if COMPILE_CAN    
		void carAfterCmdReceived(CAN_CONTROLLER *can, CAN_COMMAND *receivedCmd)
		{		
			#if ENABLE_INJECTION	
				#if INJECT_DIS_BC_RESPONSE
					if (CMD_DIS_REQUEST_SET_TEXT.isPrefix(receivedCmd)) //DIS REQUEST	
						can->sendCmd((CAN_COMMAND*)&CMD_DIS_RESPONSE_SET_TEXT);					
				#endif
			#else
				uint8_t p;
				switch (receivedCmd->can_id)
				{
					case ASTRA_H_CIM_STEERING_WHEEL_BUTTONS_ID:
						astraHHandleButtonEvent(can, receivedCmd);
						return;

					case ASTRA_H_POWER_MODE_ID:
						p = (receivedCmd->data[2] & B00000111); //3 LSB bits
						if (p != currentPowerMode)
						{
							astraHPowerOnChange(currentPowerMode, p);
							currentPowerMode = p;
						}
						return;
					
					#if COMPILE_CAR_STATUS_MONITOR
					case ASTRA_H_DIS_DIAGNOSTICS_ID:
						astraHHandleStatusCmd(can, receivedCmd);
						return;
					#endif

					#if COMPILE_BC_ECHO
					case ASTRA_H_BC_UNIT_ID:
						if (CONFIG.get(ENABLE_RADIO_SIMULATION) && receivedCmd->data[0] == 0x16)
							can->sendCmd((CAN_COMMAND*)&CMD_EHU_REQ_BC_STATUS);
						return;
					#endif

					#if COMPILE_TEST_MODE
					case ASTRA_H_TEST_MODE_ID:					
						astraHTestModeEnabled = (receivedCmd->data[4] == 0x03);
						return;						
					#endif

					#if COMPILE_DIS
					case ASTRA_H_DIS_REQUEST_ID:
						if (CONFIG.get(ENABLE_DISPLAY) && receivedCmd->can_id == ASTRA_H_DIS_REQUEST_ID && receivedCmd->data[0] == 0x30)
							astraHDISShowText(can);		
						return;
					#endif
				}
			#endif
		}
	#endif

	void carAfterLoop()
	{
		if(powIsPowerOn())
		{
			#if COMPILE_CAN			
				#if COMPILE_TEST_MODE
					if(CONFIG.get(ENABLE_TEST_MODE) && astraHTestModeEnabling)
						astraHTestModeStep();
				#endif

				#if COMPILE_DIS
					checkDIS();					
				#endif
			#endif
		}

		#if COMPILE_ALARM_LED && ALARM_LED_USE_TIMER_ONE == 0
			else astraHAlarmLEDCheck();
		#endif

		#if COMPILE_REMOTE_DOOR && REMOTE_DOOR_MODE == REMOTE_DOOR_MODE_SWITCH
			astraHRemoteDoorCheck();
		#endif
	}

	void carBeforeSetup()
	{
		#if COMPILE_ALARM_LED  
			astraHAlarmLEDSetup();
		#endif

		#if COMPILE_REMOTE_DOOR
			astraHRemoteDoorSetup();
		#endif

		#if COMPILE_CAN && COMPILE_CAN_WHITELIST
			DEFAULT_CAN.setWhitelistID(ASTRA_H_CIM_STEERING_WHEEL_BUTTONS_ID, MCP2515::RXF::RXF0);	//CIM - Steering Wheel (High priority)  
			DEFAULT_CAN.setWhitelistID(ASTRA_H_POWER_MODE_ID, MCP2515::RXF::RXF2);					//Power mode (Low priority)

			#if COMPILE_BC_ECHO
				DEFAULT_CAN.setWhitelistID(ASTRA_H_BC_UNIT_ID, MCP2515::RXF::RXF4);					//BC Status Broadcast (Low priority)
			#endif
			#if COMPILE_CAR_STATUS_MONITOR
				DEFAULT_CAN.setWhitelistID(ASTRA_H_DIS_DIAGNOSTICS_ID, MCP2515::RXF::RXF5);			//Measuring Blocks (Low priority)
			#endif
			#if COMPILE_TEST_MODE
				DEFAULT_CAN.setWhitelistID(ASTRA_H_TEST_MODE_ID, MCP2515::RXF::RXF3);				//Test mode (Low priority)
			#endif				
			#if COMPILE_DIS
				DEFAULT_CAN.setWhitelistID(ASTRA_H_DIS_REQUEST_ID, MCP2515::RXF::RXF1);				//Display response (High priority)  
			#endif 
			#if ENABLE_INJECTION && INJECT_DIS_BC_RESPONSE
				DEFAULT_CAN.setWhitelistID(ASTRA_H_DIS_SET_ID, MCP2515::RXF::RXF1);					//Display request (High priority)  
			#endif

		#endif
	}

	void carAfterSetup() 
	{		
		#if COMPILE_CAN && ENABLE_INJECTION
			#if INJECT_PERIODICAL_CAR_COMMANDS
				DEFAULT_CAN.addPeriodicalCmd((CAN_COMMAND_PERIODICAL*)&PER_BC_STATUS_BROADCAST);
				DEFAULT_CAN.addPeriodicalCmd((CAN_COMMAND_PERIODICAL*)&PER_BC_MODE_BROADCAST);
				DEFAULT_CAN.addPeriodicalCmd((CAN_COMMAND_PERIODICAL*)&PER_STEERING_VOL_PLUS);
				DEFAULT_CAN.addPeriodicalCmd((CAN_COMMAND_PERIODICAL*)&PER_STEERING_BC_WHEEL_UP);
				DEFAULT_CAN.addPeriodicalCmd((CAN_COMMAND_PERIODICAL*)&PER_STEERING_BC_WHEEL_DOWN);
			#endif

			#if INJECT_DOOR_OPEN
				DEFAULT_CAN.addPeriodicalCmd((CAN_COMMAND_PERIODICAL*)&PER_STEERING_WHEEL_LEFT_TOP_PRESS, 0);				
				DEFAULT_CAN.addPeriodicalCmd((CAN_COMMAND_PERIODICAL*)&PER_STEERING_WHEEL_RIGHT_TOP_PRESS, 80);
				DEFAULT_CAN.addPeriodicalCmd((CAN_COMMAND_PERIODICAL*)&PER_STEERING_WHEEL_RIGHT_TOP_RELEASE, 160);
				DEFAULT_CAN.addPeriodicalCmd((CAN_COMMAND_PERIODICAL*)&PER_STEERING_WHEEL_LEFT_TOP_RELEASE, 240);
			#endif
		#endif

		#if COMPILE_SERIAL_CONTROL
			#if SERIAL_CONTROL_MODE >= SC_MIN
				#if COMPILE_CAN					
					SerialController.addHandler(new SERIAL_HANDLER(sPOW, astraHPOWHandleConfCmd));
					SerialController.addHandler(new SERIAL_HANDLER(sOPT, astraHOPTHandleConfCmd));

					#if COMPILE_DIS
						SerialController.addHandler(new SERIAL_HANDLER(sLOG, astraHLOGHandleConfCmd));
						SerialController.addHandler(new SERIAL_HANDLER(sDIS, astraHDISHandleConfCmd));
					#endif
				#endif
			#endif
			#if SERIAL_CONTROL_MODE >= SC_MID	
				#if COMPILE_CAN
					SerialController.addHandler(new SERIAL_HANDLER(sBC, astraHBCHandleConfCmd));
				#endif
				#if COMPILE_REMOTE_DOOR
					SerialController.addHandler(new SERIAL_HANDLER(sRD, astraHRDHandleConfCmd));
				#endif				
				#if COMPILE_RADIO_CONTROL        
					SerialController.addHandler(new SERIAL_HANDLER(sRAD, astraHRADHandleConfCmd));
				#endif
			#endif
		#endif
	}

	
	//////////////////////// MESSAGE HANDLERS  ////////////////////////
	bool astraHOPTHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength)
	{
		bool handled;

		if(utilsEquals_P(cmd, sGet))
		{			
			handled = true;
			char *param = params[0];

			if (utilsEquals_P(param, sSIM))				cmdSerial.println(CONFIG.get(ENABLE_RADIO_SIMULATION) ? 1 : 0);
			#if COMPILE_ALARM_LED
				else if (utilsEquals_P(param, sLED))	cmdSerial.println(CONFIG.get(ENABLE_ALARM_LED) ? 1 : 0);
			#endif
			#if COMPILE_CAR_STATUS_MONITOR
				else if (utilsEquals_P(param, sCAR))	cmdSerial.println(CONFIG.get(ENABLE_CAR_STATUS_MONITOR) ? 1 : 0);
			#endif
			#if COMPILE_TRACCAR
				else if (utilsEquals_P(param, sPOW))	cmdSerial.println(CONFIG.get(ENABLE_POWER_NOTIFICATION) ? 1 : 0);
			#endif
			#if COMPILE_TEST_MODE
				else if (utilsEquals_P(param, sTST))	cmdSerial.println(CONFIG.get(ENABLE_TEST_MODE) ? 1 : 0);
			#endif
			#if COMPILE_DIS
				else if (utilsEquals_P(param, sDIS))	cmdSerial.println(CONFIG.get(ENABLE_DISPLAY) ? 1 : 0);
				else if (utilsEquals_P(param, sRND))	cmdSerial.println(CONFIG.get(ENABLE_RANDOM_TEXTS) ? 1 : 0);
			#endif			
			else handled = false;
		}
		else if (paramsLength >= 2)
		{			
			handled = false;
			
			bool all = utilsEquals_P(params[0], sALL);
			bool v = !utilsEquals_P(params[1], sZero);

			if (all || utilsEquals_P(params[0], sSIM)) {		CONFIG.set(ENABLE_RADIO_SIMULATION, v);		handled = true; }
			#if COMPILE_ALARM_LED
				if (all || utilsEquals_P(params[0], sLED)) {	CONFIG.set(ENABLE_ALARM_LED, v);			handled = true; }
			#endif
			#if COMPILE_CAR_STATUS_MONITOR
				if (all || utilsEquals_P(params[0], sCAR)) {	CONFIG.set(ENABLE_CAR_STATUS_MONITOR, v);	handled = true; }
			#endif
			#if COMPILE_TRACCAR
				if (all || utilsEquals_P(params[0], sPOW)) {	CONFIG.set(ENABLE_POWER_NOTIFICATION, v);	handled = true; }
			#endif
			#if COMPILE_TEST_MODE
				if (all || utilsEquals_P(params[0], sTST)) {	CONFIG.set(ENABLE_TEST_MODE, v);			handled = true; }
			#endif
			#if COMPILE_DIS
				if (all || utilsEquals_P(params[0], sDIS)) {	CONFIG.set(ENABLE_DISPLAY, v);				handled = true; }
				if (all || utilsEquals_P(params[0], sRND)) {	CONFIG.set(ENABLE_RANDOM_TEXTS, v);			handled = true; }
			#endif						

			if (handled)
				utilsPrintln_P(sOK);

			return (!all & handled);
		}
	}	

	#if COMPILE_CAN	
		#if COMPILE_DIS
			bool astraHLOGHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength)
			{
				if (utilsEquals_P(cmd, sGet))
				{					
					if (utilsEquals_P(params[0], sDIS))
					{
						cmdSerial.println(CONFIG.get(PRINT_DISPLAY_INFO) ? 1 : 0);
						return true;
					}						
					else return false;
				}
				else if(paramsLength >= 2) //SET
				{					
					bool handled = true;
					bool all = utilsEquals_P(params[0], sALL);
					bool v = !utilsEquals_P(params[1], sZero);
					
					if (all || utilsEquals_P(params[0], sDIS))CONFIG.set(PRINT_DISPLAY_INFO, v);
					else handled = false;

					if (handled)
						utilsPrintln_P(sOK);

					return (!all & handled);
				}
			}

			bool astraHDISHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength)
			{					
				if (CONFIG.get(ENABLE_DISPLAY) && utilsEquals_P(cmd, sSet) && paramsLength >= 1)
				{
					astraHDISSetText(&DEFAULT_CAN, params[0]);
					return true;
				}				
				else return false;
			}
		#endif
	
		bool astraHBCHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength)
		{			
			bool handled = true;
			
			if(utilsEquals_P(cmd, sSettings))			DEFAULT_CAN.sendCmdSequence((CAN_COMMAND*)&CMD_EHU_SETTINGS_PRESS,		(CAN_COMMAND*)&CMD_EHU_SETTINGS_RELEASE, 1, true);
			else if(utilsEquals_P(cmd, sBC_Lower))		DEFAULT_CAN.sendCmdSequence((CAN_COMMAND*)&CMD_EHU_BC_PRESS,			(CAN_COMMAND*)&CMD_EHU_BC_RELEASE, 1, true);
			else if(utilsEquals_P(cmd, sUp))			DEFAULT_CAN.sendCmdSequence((CAN_COMMAND*)&CMD_EHU_ARROW_UP_PRESS,		(CAN_COMMAND*)&CMD_EHU_ARROW_UP_RELEASE, 1, true);
			else if(utilsEquals_P(cmd, sDown))			DEFAULT_CAN.sendCmdSequence((CAN_COMMAND*)&CMD_EHU_ARROW_DOWN_PRESS,	(CAN_COMMAND*)&CMD_EHU_ARROW_DOWN_RELEASE, 1, true);
			else if(utilsEquals_P(cmd, sLeft))			DEFAULT_CAN.sendCmdSequence((CAN_COMMAND*)&CMD_EHU_ARROW_LEFT_PRESS,	(CAN_COMMAND*)&CMD_EHU_ARROW_LEFT_RELEASE, 1, true);
			else if(utilsEquals_P(cmd, sRight))			DEFAULT_CAN.sendCmdSequence((CAN_COMMAND*)&CMD_EHU_ARROW_RIGHT_PRESS,	(CAN_COMMAND*)&CMD_EHU_ARROW_RIGHT_RELEASE, 1, true);
			else handled = false;

			return handled;			
		}
	
		bool astraHPOWHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength)
		{		
			CAN_COMMAND canCmd = CAN_COMMAND(0x450, 0x00, 0x00, 0x00);

			if(utilsEquals_P(cmd, sOn))
				canCmd.data[2] = 0x07;   
			//else off
		    
			carAfterCmdReceived(&DEFAULT_CAN, &canCmd); 
			return true;			
		}
	#endif

	#if COMPILE_REMOTE_DOOR
		bool astraHRDHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength)
		{
			if (utilsEquals_P(cmd, sSend))
			{
				if (utilsEquals_P(params[0], sZero))
					astraHRemoteDoorSendAction(&DOOR_0_OPEN, &DOOR_0_CLOSE);
				#ifdef GPIO2_PIN
					else
						astraHRemoteDoorSendAction(&DOOR_1_OPEN, &DOOR_1_CLOSE);
				#endif	

				utilsPrintln_P(sOK);

				return true;
			}
			else return false;
		}
	#endif

	#if COMPILE_RADIO_CONTROL
		bool astraHRADHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength)
		{
			bool handled = true;

			if(utilsEquals_P(cmd, sVolPlus))		radEnableCmd(&RADIO_CMD_VOLUME_UP, RADIO_COMMAND_MIN_INTERVAL);
			else if(utilsEquals_P(cmd, sVolMinus))  radEnableCmd(&RADIO_CMD_VOLUME_DOWN, RADIO_COMMAND_MIN_INTERVAL);
			else if(utilsEquals_P(cmd, sNext))		radEnableCmd(&RADIO_CMD_NEXT_TRACK, RADIO_COMMAND_MIN_INTERVAL);
			else if(utilsEquals_P(cmd, sPrev))		radEnableCmd(&RADIO_CMD_PREV_TRACK, RADIO_COMMAND_MIN_INTERVAL);
			else if(utilsEquals_P(cmd, sDIS_Lower)) radEnableCmd(&RADIO_CMD_DISPLAY, RADIO_COMMAND_MIN_INTERVAL);
			else if(utilsEquals_P(cmd, sMute))		radEnableCmd(&RADIO_CMD_MUTE, RADIO_COMMAND_MIN_INTERVAL);
			else if(utilsEquals_P(cmd, sSrc))		radEnableCmd(&RADIO_CMD_SOURCE, RADIO_COMMAND_MIN_INTERVAL);
			else if(utilsEquals_P(cmd, sBand))		radEnableCmd(&RADIO_CMD_BAND_ESCAPE, RADIO_COMMAND_MIN_INTERVAL);
			else if(utilsEquals_P(cmd, sFolderUp))	radEnableCmd(&RADIO_CMD_FOLDER_UP, RADIO_COMMAND_MIN_INTERVAL);
			else if(utilsEquals_P(cmd, sFolderDown))radEnableCmd(&RADIO_CMD_FOLDER_DOWN, RADIO_COMMAND_MIN_INTERVAL);
			else if(utilsEquals_P(cmd, sVoice))		radEnableCmd(&RADIO_CMD_VOICE_CONTROL, RADIO_COMMAND_MIN_INTERVAL);
			#if SERIAL_CONTROL_MODE >= SC_FULL
				else if(utilsEquals_P(cmd, sCal))
				{
					potSetResistor(0, 0, true);
					delay(10000);
					potSetResistor(1, 0, true);
					delay(10000);
					potSetResistor(2, 0, true);
					delay(10000);
					radDisableCmd();      
				}
			#endif		
			else handled = false;

			return handled;
		}
	#endif
#endif