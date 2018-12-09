#include "../../configuration.h"

#if CAR_MODEL == OPEL_ASTRA_H_2004_MIN_CAR 
	#include "astra_h_events.h"
	#include "astra_h_events_buttons.h"
	#include "astra_h_power.h"
	#include "../../cmd_serial.h"

	#if COMPILE_SERIAL_CONTROL    
		bool astraHLOGHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength);
		bool astraHOPTHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength);
		bool astraHPOWHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength);
		bool astraHSYSHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength);

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
			}
		}
	#endif

	void carAfterLoop()
	{
		#if COMPILE_REMOTE_DOOR && REMOTE_DOOR_MODE == REMOTE_DOOR_MODE_SWITCH
			astraHRemoteDoorCheck();
		#endif
	}

	void carBeforeSetup()
	{
		#if COMPILE_REMOTE_DOOR
			astraHRemoteDoorSetup();
		#endif

		#if COMPILE_CAN && COMPILE_CAN_WHITELIST
			DEFAULT_CAN.setWhitelistID(ASTRA_H_CIM_STEERING_WHEEL_BUTTONS_ID, MCP2515::RXF::RXF0);	//CIM - Steering Wheel (High priority)  
			DEFAULT_CAN.setWhitelistID(ASTRA_H_POWER_MODE_ID, MCP2515::RXF::RXF2);					//Power mode (Low priority)

			#if COMPILE_CAR_STATUS_MONITOR
				DEFAULT_CAN.setWhitelistID(ASTRA_H_DIS_DIAGNOSTICS_ID, MCP2515::RXF::RXF5);			//Measuring Blocks (Low priority)
			#endif
		#endif
	}

	void carAfterSetup() 
	{				
		#if COMPILE_SERIAL_CONTROL
			#if SERIAL_CONTROL_MODE >= SC_MIN
				SerialController.addHandler(new SERIAL_HANDLER(sSYS, astraHSYSHandleConfCmd));
				#if COMPILE_CAN				
					SerialController.addHandler(new SERIAL_HANDLER(sPOW, astraHPOWHandleConfCmd));
					SerialController.addHandler(new SERIAL_HANDLER(sOPT, astraHOPTHandleConfCmd));					
				#endif
			#endif
			#if SERIAL_CONTROL_MODE >= SC_MID					
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
	bool astraHSYSHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength)
	{
		if (utilsEquals_P(cmd, sEcho))
		{
			for (uint8_t i = 0; i < paramsLength; i++)
				cmdSerial.print(params[i]);

			cmdSerial.println();
			return true;
		}
		else return false;
	}

	bool astraHPOWHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength)
	{
		if (utilsEquals_P(cmd, sGet))
		{
			char temp[4];
			itoa(currentPowerMode, (char*)&temp, 10);

			char *params[] = { (char*)&temp };
			astraHSendCommand('P', "STAT", params, 1);

			return true;
		}
		else if (utilsEquals_P(cmd, sSet) && paramsLength >= 1)
		{
			CAN_COMMAND canCmd = CAN_COMMAND(0x450, 0x00, 0x00, 0x00);

			if (utilsEquals_P(params[0], sOn))
				canCmd.data[2] = 0x07;
			else if (utilsEquals_P(params[0], sOff))
				canCmd.data[2] = 0x00;
			else //numeric representation
			{
				uint8_t n = (atoi(params[0]) & B00000111);
				canCmd.data[2] = n;
			}

			carAfterCmdReceived(&DEFAULT_CAN, &canCmd);
			return true;
		}

		return false;
	}

	bool astraHOPTHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength)
	{
		bool handled;

		if(utilsEquals_P(cmd, sGet))
		{			
			handled = true;
			char *param = params[0];
			
			if (1 == 1);
			#if COMPILE_CAR_STATUS_MONITOR
				else if (utilsEquals_P(param, sCAR))	cmdSerial.println(CONFIG.get(ENABLE_CAR_STATUS_MONITOR) ? 1 : 0);
			#endif
			#if COMPILE_TRACCAR
				else if (utilsEquals_P(param, sPOW))	cmdSerial.println(CONFIG.get(ENABLE_POWER_NOTIFICATION) ? 1 : 0);
			#endif
			else handled = false;
		}
		else if (paramsLength >= 2)
		{			
			handled = false;
			
			bool all = utilsEquals_P(params[0], sALL);
			bool v = !utilsEquals_P(params[1], sZero);
			
			#if COMPILE_CAR_STATUS_MONITOR
				if (all || utilsEquals_P(params[0], sCAR)) {	CONFIG.set(ENABLE_CAR_STATUS_MONITOR, v);	handled = true; }
			#endif
			#if COMPILE_TRACCAR
				if (all || utilsEquals_P(params[0], sPOW)) {	CONFIG.set(ENABLE_POWER_NOTIFICATION, v);	handled = true; }
			#endif						

			if (handled)
				utilsPrintln_P(sOK);

			return (!all & handled);
		}
	}		

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

			uint16_t timeout = RADIO_COMMAND_MIN_INTERVAL;
			if (paramsLength > 0)
				timeout = atoi(params[0]);

			if(utilsEquals_P(cmd, sVolPlus))		radEnableCmd(&RADIO_CMD_VOLUME_UP, timeout);
			else if(utilsEquals_P(cmd, sVolMinus))  radEnableCmd(&RADIO_CMD_VOLUME_DOWN, timeout);
			else if(utilsEquals_P(cmd, sNext))		radEnableCmd(&RADIO_CMD_NEXT_TRACK, timeout);
			else if(utilsEquals_P(cmd, sPrev))		radEnableCmd(&RADIO_CMD_PREV_TRACK, timeout);
			else if(utilsEquals_P(cmd, sDIS_Lower)) radEnableCmd(&RADIO_CMD_DISPLAY, timeout);
			else if(utilsEquals_P(cmd, sMute))		radEnableCmd(&RADIO_CMD_MUTE, timeout);
			else if(utilsEquals_P(cmd, sSrc))		radEnableCmd(&RADIO_CMD_SOURCE, timeout);
			else if(utilsEquals_P(cmd, sBand))		radEnableCmd(&RADIO_CMD_BAND_ESCAPE, timeout);
			else if(utilsEquals_P(cmd, sFolderUp))	radEnableCmd(&RADIO_CMD_FOLDER_UP, timeout);
			else if(utilsEquals_P(cmd, sFolderDown))radEnableCmd(&RADIO_CMD_FOLDER_DOWN, timeout);
			else if(utilsEquals_P(cmd, sVoice))		radEnableCmd(&RADIO_CMD_VOICE_CONTROL, timeout);
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

			if(handled)
				utilsPrintln_P(sOK);

			return handled;
		}
	#endif
#endif