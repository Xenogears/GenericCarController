#ifndef __ASTRA_H_EVENTS_BUTTONS__H__
	#define __ASTRA_H_EVENTS_BUTTONS__H__

	#include "../../configuration.h"
	#include "../../can_controller.h"
	#include "../../can_command.h"	
	#include "../../radio_command.h"

	#include "utils.h"

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
	} ASTRA_H_BUTTON_EVENT;


	extern uint8_t astraHButtonsStatus;	

	bool astraHHandleButtonEvent(CAN_CONTROLLER *can, CAN_COMMAND *cmd);
#endif
