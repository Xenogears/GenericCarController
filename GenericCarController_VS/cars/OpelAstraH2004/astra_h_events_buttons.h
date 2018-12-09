#ifndef __ASTRA_H_EVENTS_BUTTONS__H__
	#define __ASTRA_H_EVENTS_BUTTONS__H__

	#include "../../configuration.h"
	#include "../../can_controller.h"
	#include "../../can_command.h"	
	#include "../../radio_command.h"
	#include "../../traccar.h"
	#include "astra_h_can_commands.h"
	#include "astra_h_dis.h"
	#include "astra_h_remote_door.h"

	#if ENABLE_BUTTON_EVENT_HANDLING
		#if COMPILE_SETTINGS_MODE
			#define ASTRA_H_SETTINGS_MODE_ENABLED	(astraHButtonsStatus & SETTINGS_MODE_MASK)
			#define ASTRA_H_ENABLE_SETTINGS_MODE()	(astraHButtonsStatus |= SETTINGS_MODE_MASK)
			#define ASTRA_H_DISABLE_SETTINGS_MODE()	(astraHButtonsStatus &= ~SETTINGS_MODE_MASK)
		#endif

		typedef struct
		{
			void(*press)(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
			void(*longPress)(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
			void(*release)(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
		} ASTRA_H_BUTTON_EVENT_HANDLER;

		typedef struct
		{
			uint8_t buttonMask;
			const PROGMEM ASTRA_H_BUTTON_EVENT_HANDLER *defaultHandler;

			#if COMPILE_SETTINGS_MODE
				const PROGMEM ASTRA_H_BUTTON_EVENT_HANDLER *settingsHandler;
			#endif
		} ASTRA_H_BUTTON_EVENT;


		extern uint8_t astraHButtonsStatus;

	#else
		typedef struct
		{
			const CAN_COMMAND *canCmd;
			const RADIO_COMMAND *radioCmd;
			unsigned short int radioCmdInterval;

		} RADIO_CAN_COMMAND;

		const RADIO_CAN_COMMAND RADIO_CAN_COMMANDS[]
		{
			{ &CMD_STEERING_WHEEL_VOLUME_WHEEL_UP,			&RADIO_CMD_VOLUME_UP,    RADIO_COMMAND_MIN_INTERVAL },
			{ &CMD_STEERING_WHEEL_VOLUME_WHEEL_DOWN,		&RADIO_CMD_VOLUME_DOWN,  RADIO_COMMAND_MIN_INTERVAL },
			{ &CMD_STEERING_WHEEL_RIGHT_TOP_PRESS,			&RADIO_CMD_NEXT_TRACK,   0 },
			{ &CMD_STEERING_WHEEL_RIGHT_TOP_RELEASE,		NULL,                    0 },
			{ &CMD_STEERING_WHEEL_RIGHT_BOTTOM_PRESS,		&RADIO_CMD_PREV_TRACK,   0 },
			{ &CMD_STEERING_WHEEL_RIGHT_BOTTOM_RELEASE,		NULL,                    0 },
			{ &CMD_STEERING_WHEEL_LEFT_TOP_PRESS,			&RADIO_CMD_FOLDER_DOWN,  0 },
			{ &CMD_STEERING_WHEEL_LEFT_TOP_RELEASE,			NULL,                    0 },
			{ &CMD_STEERING_WHEEL_LEFT_BOTTOM_PRESS,		&RADIO_CMD_FOLDER_UP,    0 },
			{ &CMD_STEERING_WHEEL_LEFT_BOTTOM_RELEASE,		NULL,                    0 },
		};
	#endif

	bool astraHHandleButtonEvent(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
#endif
