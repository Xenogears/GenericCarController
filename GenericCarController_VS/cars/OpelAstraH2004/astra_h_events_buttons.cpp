//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#include "../../configuration.h"

#if CAR_MODEL == OPEL_ASTRA_H_2004_CAR && COMPILE_CAN
	#include "astra_h_events_buttons.h"

	#if ENABLE_BUTTON_EVENT_HANDLING
		uint8_t astraHButtonsStatus = 0;
	
		static void loadEvent(const PROGMEM ASTRA_H_BUTTON_EVENT *fEvent, ASTRA_H_BUTTON_EVENT *res);
		static void loadEventHandler(const PROGMEM ASTRA_H_BUTTON_EVENT_HANDLER *fHandler, ASTRA_H_BUTTON_EVENT_HANDLER *res);

		void astraHOnLeftWheelButtonPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
		void astraHOnVolumeWheelMove(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
		void astraHOnRightTopButtonLongPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
		void astraHOnRightBottomButtonPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
		void astraHOnRightBottomButtonLongPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
		void astraHButtonEventDisableRadioCmd(CAN_CONTROLLER *can, CAN_COMMAND *cmd);

		const static PROGMEM ASTRA_H_BUTTON_EVENT_HANDLER LEFT_WHEEL_BUTTON_HANDLER		= { astraHOnLeftWheelButtonPress,				NULL,									NULL };
		const static PROGMEM ASTRA_H_BUTTON_EVENT_HANDLER RIGHT_TOP_BUTTON_HANDLER		= { astraHOnRightTopButtonLongPress,			astraHOnRightTopButtonLongPress,		astraHButtonEventDisableRadioCmd };
		const static PROGMEM ASTRA_H_BUTTON_EVENT_HANDLER RIGHT_BOTTOM_BUTTON_HANDLER	= { astraHOnRightBottomButtonPress,				astraHOnRightBottomButtonLongPress,		astraHButtonEventDisableRadioCmd };
		const static PROGMEM ASTRA_H_BUTTON_EVENT_HANDLER VOLUME_WHEEL_HANDLER			= { astraHOnVolumeWheelMove,					NULL,									NULL };
		
		#if COMPILE_SETTINGS_MODE
			void astraHOnVolumeWheelSettingsMove(CAN_CONTROLLER *can, CAN_COMMAND *cmd);			
			void astraHOnLeftTopButtonSettingsPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
			void astraHOnLeftTopButtonSettingsRelease(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
			void astraHOnLeftBottomButtonSettingsPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
			void astraHOnLeftBottomButtonSettingsRelease(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
			void astraHOnRightTopButtonSettingsPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
			void astraHOnRightTopButtonSettingsRelease(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
			void astraHOnRightBottomButtonSettingsPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
			void astraHOnRightBottomButtonSettingsRelease(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
		
			const static PROGMEM ASTRA_H_BUTTON_EVENT_HANDLER LEFT_WHEEL_BUTTON_SETTINGS_HANDLER	= { astraHOnLeftWheelButtonPress,				NULL,									NULL };
			const static PROGMEM ASTRA_H_BUTTON_EVENT_HANDLER LEFT_TOP_BUTTON_SETTINGS_HANDLER		= { astraHOnLeftTopButtonSettingsPress,			astraHOnLeftTopButtonSettingsPress,		astraHOnLeftTopButtonSettingsRelease };
			const static PROGMEM ASTRA_H_BUTTON_EVENT_HANDLER LEFT_BOTTOM_BUTTON_SETTINGS_HANDLER	= { astraHOnLeftBottomButtonSettingsPress,		astraHOnLeftBottomButtonSettingsPress,	astraHOnLeftBottomButtonSettingsRelease };
			const static PROGMEM ASTRA_H_BUTTON_EVENT_HANDLER RIGHT_TOP_BUTTON_SETTINGS_HANDLER		= { astraHOnRightTopButtonSettingsPress,		astraHOnRightTopButtonSettingsPress,	astraHOnRightTopButtonSettingsRelease };
			const static PROGMEM ASTRA_H_BUTTON_EVENT_HANDLER RIGHT_BOTTOM_BUTTON_SETTINGS_HANDLER	= { astraHOnRightBottomButtonSettingsPress,		astraHOnRightBottomButtonSettingsPress, astraHOnRightBottomButtonSettingsRelease };
			const static PROGMEM ASTRA_H_BUTTON_EVENT_HANDLER VOLUME_WHEEL_SETTINGS_HANDLER			= { astraHOnVolumeWheelSettingsMove,			NULL,									NULL };
		#endif


		const static ASTRA_H_BUTTON_EVENT LEFT_WHEEL_BUTTON_EVENT PROGMEM =
		{ 
			LEFT_WHEEL_BUTTON_MASK,			//0 -> Left Wheel Button
			&LEFT_WHEEL_BUTTON_HANDLER,
			#if COMPILE_SETTINGS_MODE
				&LEFT_WHEEL_BUTTON_SETTINGS_HANDLER
			#endif
		};

		const static ASTRA_H_BUTTON_EVENT LEFT_TOP_BUTTON_EVENT PROGMEM =
		{ 
			LEFT_TOP_BUTTON_MASK,			//1 -> Left Top
			NULL,
			#if COMPILE_SETTINGS_MODE
				&LEFT_TOP_BUTTON_SETTINGS_HANDLER
			#endif
		};

		const static ASTRA_H_BUTTON_EVENT LEFT_BOTTOM_BUTTON_EVENT PROGMEM =
		{ 
			LEFT_BOTTOM_BUTTON_MASK,		//2 -> Left Bottom
			NULL,
			#if COMPILE_SETTINGS_MODE
				&LEFT_BOTTOM_BUTTON_SETTINGS_HANDLER
			#endif
		};

		const static ASTRA_H_BUTTON_EVENT NONE_EVENT PROGMEM =
		{ 
			NONE_MASK,						//3 -> Left Wheel Up/Down  || 4 -> NONE
			NULL,
			#if COMPILE_SETTINGS_MODE
				NULL
			#endif
		};

		const static ASTRA_H_BUTTON_EVENT RIGHT_TOP_BUTTON_EVENT PROGMEM =
		{
			RIGHT_TOP_BUTTON_MASK,			//5 -> Right Top
			&RIGHT_TOP_BUTTON_HANDLER,
			#if COMPILE_SETTINGS_MODE
				&RIGHT_TOP_BUTTON_SETTINGS_HANDLER
			#endif
		};

		const static ASTRA_H_BUTTON_EVENT RIGHT_BOTTOM_BUTTON_EVENT PROGMEM =
		{
			RIGHT_BOTTOM_BUTTON_MASK,		//6 -> Right Bottom	
			&RIGHT_BOTTOM_BUTTON_HANDLER,
			#if COMPILE_SETTINGS_MODE
				&RIGHT_BOTTOM_BUTTON_SETTINGS_HANDLER
			#endif
		};

		const static ASTRA_H_BUTTON_EVENT VOLUME_WHEEL_MOVE_EVENT PROGMEM =
		{
			NONE_MASK,						//7 -> Volume Wheel Up/Down	
			&VOLUME_WHEEL_HANDLER,
			#if COMPILE_SETTINGS_MODE
				&VOLUME_WHEEL_SETTINGS_HANDLER
			#endif
		};
		
		const static ASTRA_H_BUTTON_EVENT *ASTRA_H_BUTTON_HANDLERS[]
		{
			&LEFT_WHEEL_BUTTON_EVENT,
			&LEFT_TOP_BUTTON_EVENT,
			&LEFT_BOTTOM_BUTTON_EVENT,	
			&NONE_EVENT,
			&NONE_EVENT,
			&RIGHT_TOP_BUTTON_EVENT,
			&RIGHT_BOTTOM_BUTTON_EVENT,
			&VOLUME_WHEEL_MOVE_EVENT
		};
	#endif

	//PUBLIC FUNCTIONS
	bool astraHHandleButtonEvent(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
	{
		#if ENABLE_BUTTON_EVENT_HANDLING == 0
			#if (COMPILE_RADIO_CONTROL && COMPILE_POTENTIOMETER)
				for (unsigned char i = 0; i < sizeof(RADIO_CAN_COMMANDS) / sizeof(RADIO_CAN_COMMAND); i++)
				{
					if (RADIO_CAN_COMMANDS[i].canCmd->equals(cmd))
					{
						const RADIO_CAN_COMMAND *foundCmd = &RADIO_CAN_COMMANDS[i];
						if (foundCmd->radioCmd != NULL)
							radEnableCmd(foundCmd->radioCmd, foundCmd->radioCmdInterval);
						else
							radDisableCmd();
      
						break;
					}
				}
			#endif
		#else
			uint8_t prevButtonStatus = astraHButtonsStatus;
			uint8_t index =  (cmd->data[1] & B00000011) |
							((cmd->data[1] & B00010000) >> 2);

			ASTRA_H_BUTTON_EVENT ev;
			loadEvent(ASTRA_H_BUTTON_HANDLERS[index], &ev);	

			#if COMPILE_SETTINGS_MODE
				const ASTRA_H_BUTTON_EVENT_HANDLER *handlerPtr = (ASTRA_H_SETTINGS_MODE_ENABLED ? ev.settingsHandler : ev.defaultHandler);
			#else
				const ASTRA_H_BUTTON_EVENT_HANDLER *handlerPtr = ev.defaultHandler;
			#endif		

			ASTRA_H_BUTTON_EVENT_HANDLER handler;
			loadEventHandler(handlerPtr, &handler);

			if (cmd->data[0] != 0x00) //IF PRESSED
			{
				astraHButtonsStatus |= ev.buttonMask; //SET BUTTON FLAG
						
				if (handlerPtr != NULL)
				{
					if (prevButtonStatus & ev.buttonMask) //AND WAS PRESSED	
					{
						if (handler.longPress != NULL)
							handler.longPress(can, cmd);
					}
					else if (handler.press != NULL) //WAS NOT PRESSED			
						handler.press(can, cmd);
				}
			}
			else //IF RELEASED
			{						
				astraHButtonsStatus &= ~ev.buttonMask; //CLEAR BUTTON FLAG
							
				if (handlerPtr != NULL && handler.release != NULL)
					handler.release(can, cmd);				
			}

			#if ENABLE_DIS_BUTTON_FLAGS && COMPILE_DIS
				if (CONFIG.get(ENABLE_DISPLAY))
				{
					uint8_t mask = (LEFT_TOP_BUTTON_MASK | LEFT_BOTTOM_BUTTON_MASK | SETTINGS_MODE_MASK);
					uint8_t currentButtonStatus = astraHButtonsStatus & mask;
					prevButtonStatus &= mask;

					if (prevButtonStatus != currentButtonStatus)
						DEFAULT_CAN.sendCmd((CAN_COMMAND*)&CMD_DIS_SET_MODE, currentButtonStatus);
				}
			#endif
		#endif
	}

	#if ENABLE_BUTTON_EVENT_HANDLING
		static void loadEvent(const PROGMEM ASTRA_H_BUTTON_EVENT *fEvent, ASTRA_H_BUTTON_EVENT *res)
		{
			memcpy_P((void*)res, fEvent, sizeof(ASTRA_H_BUTTON_EVENT));
		}

		static void loadEventHandler(const PROGMEM ASTRA_H_BUTTON_EVENT_HANDLER *fHandler, ASTRA_H_BUTTON_EVENT_HANDLER *res)
		{
			memcpy_P((void*)res, fHandler, sizeof(ASTRA_H_BUTTON_EVENT_HANDLER));
		}

		//////////////////////// BUTTON HANDLERS ////////////////////////		
		static void astraHOnLeftWheelButtonPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
		{
			#if COMPILE_SETTINGS_MODE
				if (ASTRA_H_SETTINGS_MODE_ENABLED)
					ASTRA_H_DISABLE_SETTINGS_MODE();
				else				
					ASTRA_H_ENABLE_SETTINGS_MODE();				
			#endif			
		}

		static void astraHOnRightTopButtonLongPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
		{
			#if COMPILE_RADIO_CONTROL
				if (astraHButtonsStatus & LEFT_BOTTOM_BUTTON_MASK)
					radEnableCmd(&RADIO_CMD_FOLDER_DOWN, RADIO_COMMAND_MAX_INTERVAL);
				else if (astraHButtonsStatus & LEFT_TOP_BUTTON_MASK)
				{
					#if COMPILE_REMOTE_DOOR && defined(GPIO2_PIN)						
						astraHRemoteDoorSendAction(&DOOR_1_OPEN, &DOOR_1_CLOSE); //Exit door

						#if COMPILE_DIS
							if (CONFIG.get(ENABLE_DISPLAY))
								astraHDISSetText_P(&DEFAULT_CAN, REMOTE_DOOR_1_DIS_MESSAGE);
						#endif
					#else
						radEnableCmd(&RADIO_CMD_MUTE, RADIO_COMMAND_MAX_INTERVAL);
					#endif
				}					
				else
					radEnableCmd(&RADIO_CMD_NEXT_TRACK, RADIO_COMMAND_MAX_INTERVAL);
			#endif		
		}

		static void astraHOnRightBottomButtonPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
		{
			#if COMPILE_REMOTE_DOOR && defined(GPIO1_PIN)
				if (astraHButtonsStatus & LEFT_TOP_BUTTON_MASK)
				{
					astraHRemoteDoorSendAction(&DOOR_0_OPEN, &DOOR_0_CLOSE); //Entrance door

					#if COMPILE_DIS
						if (CONFIG.get(ENABLE_DISPLAY))
							astraHDISSetText_P(&DEFAULT_CAN, REMOTE_DOOR_0_DIS_MESSAGE);
					#endif

					#if COMPILE_TRACCAR
						traccarSetPendingNotification();
					#endif
				}
				else 
			#endif			

			astraHOnRightBottomButtonLongPress(can, cmd);
		}	

		static void astraHOnRightBottomButtonLongPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
		{
			#if COMPILE_RADIO_CONTROL
				#if COMPILE_REMOTE_DOOR == 0
					if (astraHButtonsStatus & LEFT_BOTTOM_BUTTON_MASK)
						radEnableCmd(&RADIO_CMD_VOICE_CONTROL, RADIO_COMMAND_MAX_INTERVAL);
					else
				#endif

				if (astraHButtonsStatus & LEFT_BOTTOM_BUTTON_MASK)
					radEnableCmd(&RADIO_CMD_FOLDER_UP, RADIO_COMMAND_MAX_INTERVAL);
				else if (!(astraHButtonsStatus & LEFT_TOP_BUTTON_MASK))
					radEnableCmd(&RADIO_CMD_PREV_TRACK, RADIO_COMMAND_MAX_INTERVAL);
			#endif
		}

		static void astraHButtonEventDisableRadioCmd(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
		{			
			#if COMPILE_RADIO_CONTROL
				radDisableCmd();
			#endif	
		}		

		static void astraHOnVolumeWheelMove(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
		{			
			#if COMPILE_RADIO_CONTROL
				if (cmd->data[2] == 0x01) //UP
					radEnableCmd(&RADIO_CMD_VOLUME_UP, RADIO_COMMAND_MIN_INTERVAL);
				else //DOWN
					radEnableCmd(&RADIO_CMD_VOLUME_DOWN, RADIO_COMMAND_MIN_INTERVAL);				
			#endif
		}

		//SETTINGS
		#if COMPILE_SETTINGS_MODE
			static void astraHOnRightTopButtonSettingsPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
			{
				DEFAULT_CAN.sendCmd((CAN_COMMAND*)&CMD_EHU_ARROW_RIGHT_PRESS, cmd->data[2]);
			}

			static void astraHOnRightTopButtonSettingsRelease(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
			{
				DEFAULT_CAN.sendCmd((CAN_COMMAND*)&CMD_EHU_ARROW_RIGHT_RELEASE, cmd->data[2]);
			}

			static void astraHOnRightBottomButtonSettingsPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
			{
				DEFAULT_CAN.sendCmd((CAN_COMMAND*)&CMD_EHU_ARROW_LEFT_PRESS, cmd->data[2]);
			}

			static void astraHOnRightBottomButtonSettingsRelease(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
			{
				DEFAULT_CAN.sendCmd((CAN_COMMAND*)&CMD_EHU_ARROW_LEFT_RELEASE, cmd->data[2]);
			}
			
			static void astraHOnLeftTopButtonSettingsPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
			{
				DEFAULT_CAN.sendCmd((CAN_COMMAND*)&CMD_EHU_SETTINGS_PRESS, cmd->data[2]);
			}

			static void astraHOnLeftTopButtonSettingsRelease(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
			{
				DEFAULT_CAN.sendCmd((CAN_COMMAND*)&CMD_EHU_SETTINGS_RELEASE, cmd->data[2]);
			}

			static void astraHOnLeftBottomButtonSettingsPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
			{
				DEFAULT_CAN.sendCmd((CAN_COMMAND*)&CMD_EHU_BC_PRESS, cmd->data[2]);
			}

			static void astraHOnLeftBottomButtonSettingsRelease(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
			{				
				DEFAULT_CAN.sendCmd((CAN_COMMAND*)&CMD_EHU_BC_RELEASE, cmd->data[2]);
			}

			static void astraHOnVolumeWheelSettingsMove(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
			{
				if (cmd->data[2] == 0x01) //UP
					DEFAULT_CAN.sendCmdSequence((CAN_COMMAND*)&CMD_EHU_ARROW_UP_PRESS, (CAN_COMMAND*)&CMD_EHU_ARROW_UP_RELEASE, 1, true);
				else //DOWN					
					DEFAULT_CAN.sendCmdSequence((CAN_COMMAND*)&CMD_EHU_ARROW_DOWN_PRESS, (CAN_COMMAND*)&CMD_EHU_ARROW_DOWN_RELEASE, 1, true);
			}
		#endif
	#endif
#endif