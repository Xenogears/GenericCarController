//
// Author:	�lvaro Graci� Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#ifndef __ASTRA_H_EVENTS_CAN_COMMANDS__H__
	#define __ASTRA_H_EVENTS_CAN_COMMANDS__H__

	#include "../../can_command.h"	

	/*EHU*/
	extern const CAN_COMMAND CMD_EHU_BC_PRESS;
	extern const CAN_COMMAND CMD_EHU_BC_RELEASE;
	extern const CAN_COMMAND CMD_EHU_SETTINGS_PRESS;
	extern const CAN_COMMAND CMD_EHU_SETTINGS_RELEASE;
  
	extern const CAN_COMMAND CMD_EHU_ARROW_UP_PRESS;
	extern const CAN_COMMAND CMD_EHU_ARROW_UP_RELEASE;
	extern const CAN_COMMAND CMD_EHU_ARROW_DOWN_PRESS;
	extern const CAN_COMMAND CMD_EHU_ARROW_DOWN_RELEASE;
	extern const CAN_COMMAND CMD_EHU_ARROW_RIGHT_PRESS;
	extern const CAN_COMMAND CMD_EHU_ARROW_RIGHT_RELEASE;
	extern const CAN_COMMAND CMD_EHU_ARROW_LEFT_PRESS;
	extern const CAN_COMMAND CMD_EHU_ARROW_LEFT_RELEASE;

	/*STEERING WHEEL*/
	#if ENABLE_BUTTON_EVENT_HANDLING == 0
		extern const CAN_COMMAND CMD_STEERING_WHEEL_VOLUME_WHEEL_UP;
		extern const CAN_COMMAND CMD_STEERING_WHEEL_VOLUME_WHEEL_DOWN;
		extern const CAN_COMMAND CMD_STEERING_WHEEL_BC_WHEEL_UP;
		extern const CAN_COMMAND CMD_STEERING_WHEEL_BC_WHEEL_DOWN;
		extern const CAN_COMMAND CMD_STEERING_WHEEL_BC_WHEEL_PRESS;
		extern const CAN_COMMAND CMD_STEERING_WHEEL_BC_WHEEL_RELEASE;
		extern const CAN_COMMAND CMD_STEERING_WHEEL_RIGHT_TOP_PRESS;
		extern const CAN_COMMAND CMD_STEERING_WHEEL_RIGHT_TOP_RELEASE;
		extern const CAN_COMMAND CMD_STEERING_WHEEL_RIGHT_BOTTOM_PRESS;
		extern const CAN_COMMAND CMD_STEERING_WHEEL_RIGHT_BOTTOM_RELEASE;
		extern const CAN_COMMAND CMD_STEERING_WHEEL_LEFT_TOP_PRESS;
		extern const CAN_COMMAND CMD_STEERING_WHEEL_LEFT_TOP_RELEASE;
		extern const CAN_COMMAND CMD_STEERING_WHEEL_LEFT_BOTTOM_PRESS;
		extern const CAN_COMMAND CMD_STEERING_WHEEL_LEFT_BOTTOM_RELEASE;
	#endif

	/*** RADIO EMULATION ***/	
	#if COMPILE_TEST_MODE || COMPILE_BC_ECHO
		extern const CAN_COMMAND CMD_EHU_REQ_BC_STATUS;
	#endif

	extern const CAN_COMMAND CMD_EHU_UNK;			  //REQUIRED FOR DIS AND TEST MODE
	extern const CAN_COMMAND_PERIODICAL PER_EHU_UNK;  //REQUIRED FOR DIS AND TEST MODE

	/*** INJECTION ***/
	#if ENABLE_INJECTION
		#if INJECT_PERIODICAL_CAR_COMMANDS
			extern const CAN_COMMAND CMD_BC_STATUS_BROADCAST;
			extern const CAN_COMMAND_PERIODICAL PER_BC_STATUS_BROADCAST;

			extern const CAN_COMMAND CMD_BC_MODE_BROADCAST;
			extern const CAN_COMMAND_PERIODICAL PER_BC_MODE_BROADCAST;

			/*** STEERING BUTTONS ***/
			extern const CAN_COMMAND_PERIODICAL PER_STEERING_VOL_PLUS;
			extern const CAN_COMMAND_PERIODICAL PER_STEERING_BC_WHEEL_UP;
			extern const CAN_COMMAND_PERIODICAL PER_STEERING_BC_WHEEL_DOWN;
		#endif

		/*** DIS ***/
		#if INJECT_DIS_BC_RESPONSE			
			extern const CAN_COMMAND CMD_DIS_REQUEST_SET_TEXT;
			extern const CAN_COMMAND CMD_DIS_RESPONSE_SET_TEXT;
		#endif

		/*** REMOTE DOOR ***/
		#if INJECT_DOOR_OPEN
			extern const CAN_COMMAND CMD_STEERING_WHEEL_LEFT_TOP_PRESS;
			extern const CAN_COMMAND CMD_STEERING_WHEEL_LEFT_TOP_RELEASE;
			extern const CAN_COMMAND CMD_STEERING_WHEEL_RIGHT_TOP_PRESS;
			extern const CAN_COMMAND CMD_STEERING_WHEEL_RIGHT_TOP_RELEASE;

			extern const CAN_COMMAND_PERIODICAL PER_STEERING_WHEEL_LEFT_TOP_PRESS;
			extern const CAN_COMMAND_PERIODICAL PER_STEERING_WHEEL_LEFT_TOP_RELEASE;
			extern const CAN_COMMAND_PERIODICAL PER_STEERING_WHEEL_RIGHT_TOP_PRESS;
			extern const CAN_COMMAND_PERIODICAL PER_STEERING_WHEEL_RIGHT_TOP_RELEASE;
		#endif
	#endif
#endif