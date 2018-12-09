#include "../../configuration.h"

#if CAR_MODEL == OPEL_ASTRA_H_2004_MIN_CAR && COMPILE_CAN
	#include "astra_h_events_buttons.h"

	uint8_t astraHButtonsStatus = 0;
	
	static void loadEvent(const PROGMEM ASTRA_H_BUTTON_EVENT *fEvent, ASTRA_H_BUTTON_EVENT *res);
	static void loadEventHandler(const PROGMEM ASTRA_H_BUTTON_EVENT_HANDLER *fHandler, ASTRA_H_BUTTON_EVENT_HANDLER *res);

	void astraHOnLeftWheelMove(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
	void astraHOnVolumeWheelMove(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
	void astraHOnLeftWheelButtonPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
	void astraHOnLeftWheelButtonRelease(CAN_CONTROLLER *can, CAN_COMMAND *cmd);	
	void astraHOnLeftTopButtonPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
	void astraHOnLeftTopButtonRelease(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
	void astraHOnLeftBottomButtonPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
	void astraHOnLeftBottomButtonRelease(CAN_CONTROLLER *can, CAN_COMMAND *cmd);	
	void astraHOnRightTopButtonPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
	void astraHOnRightTopButtonRelease(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
	void astraHOnRightBottomButtonPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
	void astraHOnRightBottomButtonRelease(CAN_CONTROLLER *can, CAN_COMMAND *cmd);

	const static PROGMEM ASTRA_H_BUTTON_EVENT_HANDLER LEFT_WHEEL_MOVE_HANDLER		= { astraHOnLeftWheelMove,				NULL,									NULL };
	const static PROGMEM ASTRA_H_BUTTON_EVENT_HANDLER VOLUME_WHEEL_MOVE_HANDLER		= { astraHOnVolumeWheelMove,			NULL,									NULL };
	const static PROGMEM ASTRA_H_BUTTON_EVENT_HANDLER LEFT_TOP_BUTTON_HANDLER		= { astraHOnLeftTopButtonPress,			astraHOnLeftTopButtonPress,				astraHOnLeftTopButtonRelease };
	const static PROGMEM ASTRA_H_BUTTON_EVENT_HANDLER LEFT_BOTTOM_BUTTON_HANDLER	= { astraHOnLeftBottomButtonPress,		astraHOnLeftBottomButtonPress,			astraHOnLeftBottomButtonRelease };
	const static PROGMEM ASTRA_H_BUTTON_EVENT_HANDLER LEFT_WHEEL_BUTTON_HANDLER		= { astraHOnLeftWheelButtonPress,		astraHOnLeftWheelButtonPress,			astraHOnLeftWheelButtonRelease };
	const static PROGMEM ASTRA_H_BUTTON_EVENT_HANDLER RIGHT_TOP_BUTTON_HANDLER		= { astraHOnRightTopButtonPress,		astraHOnRightTopButtonPress,			astraHOnRightTopButtonRelease };
	const static PROGMEM ASTRA_H_BUTTON_EVENT_HANDLER RIGHT_BOTTOM_BUTTON_HANDLER	= { astraHOnRightBottomButtonPress,		astraHOnRightBottomButtonPress,			astraHOnRightBottomButtonRelease };
	

	const static ASTRA_H_BUTTON_EVENT LEFT_WHEEL_BUTTON_EVENT PROGMEM =
	{
		LEFT_WHEEL_BUTTON_MASK,			//0 -> Left Wheel Button
		&LEFT_WHEEL_BUTTON_HANDLER
	};

	const static ASTRA_H_BUTTON_EVENT LEFT_WHEEL_MOVE_EVENT PROGMEM =
	{
		NONE_MASK,						//3 -> Left Wheel Up/Down  || 4 -> NONE
		&LEFT_WHEEL_MOVE_HANDLER
	};

	const static ASTRA_H_BUTTON_EVENT LEFT_TOP_BUTTON_EVENT PROGMEM =
	{ 
		LEFT_TOP_BUTTON_MASK,			//1 -> Left Top
		&LEFT_TOP_BUTTON_HANDLER
	};

	const static ASTRA_H_BUTTON_EVENT LEFT_BOTTOM_BUTTON_EVENT PROGMEM =
	{ 
		LEFT_BOTTOM_BUTTON_MASK,		//2 -> Left Bottom
		&LEFT_BOTTOM_BUTTON_HANDLER
	};

	const static ASTRA_H_BUTTON_EVENT RIGHT_TOP_BUTTON_EVENT PROGMEM =
	{
		RIGHT_TOP_BUTTON_MASK,			//5 -> Right Top
		&RIGHT_TOP_BUTTON_HANDLER
	};

	const static ASTRA_H_BUTTON_EVENT RIGHT_BOTTOM_BUTTON_EVENT PROGMEM =
	{
		RIGHT_BOTTOM_BUTTON_MASK,		//6 -> Right Bottom	
		&RIGHT_BOTTOM_BUTTON_HANDLER
	};

	const static ASTRA_H_BUTTON_EVENT VOLUME_WHEEL_MOVE_EVENT PROGMEM =
	{
		NONE_MASK,						//7 -> Volume Wheel Up/Down	
		&VOLUME_WHEEL_MOVE_HANDLER
	};
		
	const static ASTRA_H_BUTTON_EVENT *ASTRA_H_BUTTON_HANDLERS[]
	{
		&LEFT_WHEEL_BUTTON_EVENT,
		&LEFT_TOP_BUTTON_EVENT,
		&LEFT_BOTTOM_BUTTON_EVENT,	
		&LEFT_WHEEL_MOVE_EVENT,
		NULL,
		&RIGHT_TOP_BUTTON_EVENT,
		&RIGHT_BOTTOM_BUTTON_EVENT,
		&VOLUME_WHEEL_MOVE_EVENT
	};



	
	//PUBLIC FUNCTIONS
	bool astraHHandleButtonEvent(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
	{		
		uint8_t prevButtonStatus = astraHButtonsStatus;
		uint8_t index =  (cmd->data[1] & B00000011) |
						((cmd->data[1] & B00010000) >> 2);

		if (ASTRA_H_BUTTON_HANDLERS[index] == NULL)
			return;

		ASTRA_H_BUTTON_EVENT ev;
		loadEvent(ASTRA_H_BUTTON_HANDLERS[index], &ev);	

		const ASTRA_H_BUTTON_EVENT_HANDLER *handlerPtr = ev.defaultHandler;

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
	}
	
	static void loadEvent(const PROGMEM ASTRA_H_BUTTON_EVENT *fEvent, ASTRA_H_BUTTON_EVENT *res)
	{
		memcpy_P((void*)res, fEvent, sizeof(ASTRA_H_BUTTON_EVENT));
	}

	static void loadEventHandler(const PROGMEM ASTRA_H_BUTTON_EVENT_HANDLER *fHandler, ASTRA_H_BUTTON_EVENT_HANDLER *res)
	{
		memcpy_P((void*)res, fHandler, sizeof(ASTRA_H_BUTTON_EVENT_HANDLER));
	}


	//////////////////////// BUTTON HANDLERS ////////////////////////			
	void astraHOnLeftWheelButtonPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
	{
		char temp[3];		
		itoa(cmd->data[2], (char*)&temp, 16);

		char *params[] = { (char*)&temp };
		astraHSendCommand('B', "LWP", params, 1);
	}

	void astraHOnLeftWheelButtonRelease(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
	{
		astraHSendCommand('B', "LWR");
	}

	void astraHOnLeftTopButtonPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
	{
		char temp[3];
		itoa(cmd->data[2], (char*)&temp, 16);

		char *params[] = { (char*)&temp };
		astraHSendCommand('B', "LTP", params, 1);
	}

	void astraHOnLeftTopButtonRelease(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
	{
		astraHSendCommand('B', "LTR");
	}

	void astraHOnLeftBottomButtonPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
	{
		char temp[3];
		itoa(cmd->data[2], (char*)&temp, 16);

		char *params[] = { (char*)&temp };
		astraHSendCommand('B', "LBP", params, 1);
	}

	void astraHOnLeftBottomButtonRelease(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
	{
		astraHSendCommand('B', "LBR");
	}

	void astraHOnRightTopButtonPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
	{
		char temp[3];
		itoa(cmd->data[2], (char*)&temp, 16);

		char *params[] = { (char*)&temp };
		astraHSendCommand('B', "RTP", params, 1);
	}

	void astraHOnRightTopButtonRelease(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
	{
		astraHSendCommand('B', "RTR");
	}

	void astraHOnRightBottomButtonPress(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
	{
		char temp[3];
		itoa(cmd->data[2], (char*)&temp, 16);

		char *params[] = { (char*)&temp };
		astraHSendCommand('B', "RBP", params, 1);
	}

	void astraHOnRightBottomButtonRelease(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
	{
		astraHSendCommand('B', "RBR");
	}

	static void astraHOnLeftWheelMove(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
	{
		if (cmd->data[2] == 0xFF) //UP
			astraHSendCommand('B', "LWU");
		else //DOWN
			astraHSendCommand('B', "LWD");
	}

	static void astraHOnVolumeWheelMove(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
	{			
		#if CONFIG_VOLUME_MODE == VOLUME_MODE_RADIO && COMPILE_RADIO_CONTROL
			if (cmd->data[2] == 0x01) //UP
				radEnableCmd(&RADIO_CMD_VOLUME_UP, RADIO_COMMAND_MIN_INTERVAL);
			else //DOWN
				radEnableCmd(&RADIO_CMD_VOLUME_DOWN, RADIO_COMMAND_MIN_INTERVAL);				
		#else
			if (cmd->data[2] == 0x01) //UP
				astraHSendCommand('B', "RWU");
			else
				astraHSendCommand('B', "RWD");
		#endif
	}		
#endif