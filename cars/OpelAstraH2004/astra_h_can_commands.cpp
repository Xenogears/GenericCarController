//
// Author:	�lvaro Graci� Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#include "../../configuration.h"

#if CAR_MODEL == OPEL_ASTRA_H_2004_CAR 
	#include "astra_h.h"
	#include "../../can_command.h"	

	/*EHU*/
	const CAN_COMMAND CMD_EHU_BC_PRESS 					= CAN_COMMAND(ASTRA_H_EHU_BUTTONS_ID, 0x01, 0x01);
	const CAN_COMMAND CMD_EHU_BC_RELEASE				= CAN_COMMAND(ASTRA_H_EHU_BUTTONS_ID, 0x00, 0x01);
	const CAN_COMMAND CMD_EHU_SETTINGS_PRESS			= CAN_COMMAND(ASTRA_H_EHU_BUTTONS_ID, 0x01, 0xFF);
	const CAN_COMMAND CMD_EHU_SETTINGS_RELEASE			= CAN_COMMAND(ASTRA_H_EHU_BUTTONS_ID, 0x00, 0xFF);
  
	const CAN_COMMAND CMD_EHU_ARROW_UP_PRESS			= CAN_COMMAND(ASTRA_H_EHU_BUTTONS_ID, 0x01, 0x6C);
	const CAN_COMMAND CMD_EHU_ARROW_UP_RELEASE			= CAN_COMMAND(ASTRA_H_EHU_BUTTONS_ID, 0x00, 0x6C);
	const CAN_COMMAND CMD_EHU_ARROW_DOWN_PRESS			= CAN_COMMAND(ASTRA_H_EHU_BUTTONS_ID, 0x01, 0x6D);
	const CAN_COMMAND CMD_EHU_ARROW_DOWN_RELEASE		= CAN_COMMAND(ASTRA_H_EHU_BUTTONS_ID, 0x00, 0x6D);
	const CAN_COMMAND CMD_EHU_ARROW_RIGHT_PRESS			= CAN_COMMAND(ASTRA_H_EHU_BUTTONS_ID, 0x01, 0x6F);
	const CAN_COMMAND CMD_EHU_ARROW_RIGHT_RELEASE		= CAN_COMMAND(ASTRA_H_EHU_BUTTONS_ID, 0x00, 0x6F);
	const CAN_COMMAND CMD_EHU_ARROW_LEFT_PRESS			= CAN_COMMAND(ASTRA_H_EHU_BUTTONS_ID, 0x01, 0x6E);
	const CAN_COMMAND CMD_EHU_ARROW_LEFT_RELEASE		= CAN_COMMAND(ASTRA_H_EHU_BUTTONS_ID, 0x00, 0x6E);

	/*STEERING WHEEL*/	
	#if ENABLE_BUTTON_EVENT_HANDLING == 0				
		const CAN_COMMAND CMD_STEERING_WHEEL_BC_WHEEL_PRESS			= CAN_COMMAND(ASTRA_H_CIM_STEERING_WHEEL_BUTTONS_ID, 0x01, 0x84);
		const CAN_COMMAND CMD_STEERING_WHEEL_BC_WHEEL_RELEASE		= CAN_COMMAND(ASTRA_H_CIM_STEERING_WHEEL_BUTTONS_ID, 0x00, 0x84);
		const CAN_COMMAND CMD_STEERING_WHEEL_RIGHT_TOP_PRESS		= CAN_COMMAND(ASTRA_H_CIM_STEERING_WHEEL_BUTTONS_ID, 0x01, 0x91); 
		const CAN_COMMAND CMD_STEERING_WHEEL_RIGHT_TOP_RELEASE		= CAN_COMMAND(ASTRA_H_CIM_STEERING_WHEEL_BUTTONS_ID, 0x00, 0x91); 
		const CAN_COMMAND CMD_STEERING_WHEEL_RIGHT_BOTTOM_PRESS     = CAN_COMMAND(ASTRA_H_CIM_STEERING_WHEEL_BUTTONS_ID, 0x01, 0x92); 
		const CAN_COMMAND CMD_STEERING_WHEEL_RIGHT_BOTTOM_RELEASE   = CAN_COMMAND(ASTRA_H_CIM_STEERING_WHEEL_BUTTONS_ID, 0x00, 0x92); 
		const CAN_COMMAND CMD_STEERING_WHEEL_LEFT_TOP_PRESS			= CAN_COMMAND(ASTRA_H_CIM_STEERING_WHEEL_BUTTONS_ID, 0x01, 0x81); 
		const CAN_COMMAND CMD_STEERING_WHEEL_LEFT_TOP_RELEASE		= CAN_COMMAND(ASTRA_H_CIM_STEERING_WHEEL_BUTTONS_ID, 0x00, 0x81); 
		const CAN_COMMAND CMD_STEERING_WHEEL_LEFT_BOTTOM_PRESS		= CAN_COMMAND(ASTRA_H_CIM_STEERING_WHEEL_BUTTONS_ID, 0x01, 0x82); 
		const CAN_COMMAND CMD_STEERING_WHEEL_LEFT_BOTTOM_RELEASE	= CAN_COMMAND(ASTRA_H_CIM_STEERING_WHEEL_BUTTONS_ID, 0x00, 0x82);
	#endif
	#if (ENABLE_INJECTION && INJECT_PERIODICAL_CAR_COMMANDS) || ENABLE_BUTTON_EVENT_HANDLING == 0
		const CAN_COMMAND CMD_STEERING_WHEEL_VOLUME_WHEEL_UP		= CAN_COMMAND(ASTRA_H_CIM_STEERING_WHEEL_BUTTONS_ID, 0x08, 0x93, 0x01);
		const CAN_COMMAND CMD_STEERING_WHEEL_VOLUME_WHEEL_DOWN		= CAN_COMMAND(ASTRA_H_CIM_STEERING_WHEEL_BUTTONS_ID, 0x08, 0x93, 0xFF);
		const CAN_COMMAND CMD_STEERING_WHEEL_BC_WHEEL_UP			= CAN_COMMAND(ASTRA_H_CIM_STEERING_WHEEL_BUTTONS_ID, 0x08, 0x83, 0x01);
		const CAN_COMMAND CMD_STEERING_WHEEL_BC_WHEEL_DOWN			= CAN_COMMAND(ASTRA_H_CIM_STEERING_WHEEL_BUTTONS_ID, 0x08, 0x83, 0xFF);
	#endif	

	/*** RADIO EMULATION ***/
	#if COMPILE_TEST_MODE || COMPILE_BC_ECHO
		const CAN_COMMAND CMD_EHU_REQ_BC_STATUS		= CAN_COMMAND(ASTRA_H_EHU_UNIT_ID, 0x61, 0x00, 0x42, 0x12, 0x21, 0x00, 0x40, 0x01);
	#endif

	//REQUIRED FOR DIS AND TEST MODE
	const CAN_COMMAND CMD_EHU_UNK					= CAN_COMMAND(ASTRA_H_UNK_ID, 0x41, 0x00, 0x60, 0x02, 0x82, 0x00, 0x00, 0x2E);
	const CAN_COMMAND_PERIODICAL PER_EHU_UNK		{ (CAN_COMMAND*)&CMD_EHU_UNK, 2500, 0 };

	/*** INJECTION ***/
	#if ENABLE_INJECTION 
		#if INJECT_PERIODICAL_CAR_COMMANDS
			const CAN_COMMAND CMD_BC_STATUS_BROADCAST						= CAN_COMMAND(ASTRA_H_BC_UNIT_ID, 0x16, 0x00, 0x42, 0x15, 0xA4, 0x00, 0x00, 0x01);
			const CAN_COMMAND_PERIODICAL PER_BC_STATUS_BROADCAST			{ &CMD_BC_STATUS_BROADCAST, 200, 0 };

			const CAN_COMMAND CMD_BC_MODE_BROADCAST							= CAN_COMMAND(ASTRA_H_TEST_MODE_ID,	0x01, 0x02, 0x03, 0x03, 0x05, 0x06, 0x07, 0x08);
			const CAN_COMMAND_PERIODICAL PER_BC_MODE_BROADCAST				{ &CMD_BC_MODE_BROADCAST, 800, 0 };

			/*** STEERING BUTTONS ***/
			const CAN_COMMAND_PERIODICAL PER_STEERING_VOL_PLUS				{ &CMD_STEERING_WHEEL_VOLUME_WHEEL_UP, 1000, 0 };
			const CAN_COMMAND_PERIODICAL PER_STEERING_BC_WHEEL_UP			{ &CMD_STEERING_WHEEL_BC_WHEEL_UP, 2000, 0 };
			const CAN_COMMAND_PERIODICAL PER_STEERING_BC_WHEEL_DOWN			{ &CMD_STEERING_WHEEL_BC_WHEEL_DOWN, 770, 0 };
		#endif

		#if INJECT_DIS_BC_RESPONSE
			/*** DIS ***/
			const CAN_COMMAND CMD_DIS_REQUEST_SET_TEXT					= CAN_COMMAND(ASTRA_H_DIS_SET_ID, 0x10);
			const CAN_COMMAND CMD_DIS_RESPONSE_SET_TEXT					= CAN_COMMAND(ASTRA_H_DIS_REQUEST_ID, 0x30, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00);
		#endif

		#if INJECT_DOOR_OPEN
			const CAN_COMMAND CMD_STEERING_WHEEL_LEFT_TOP_PRESS			= CAN_COMMAND(ASTRA_H_CIM_STEERING_WHEEL_BUTTONS_ID, 0x01, 0x81, 0x01);
			const CAN_COMMAND CMD_STEERING_WHEEL_LEFT_TOP_RELEASE		= CAN_COMMAND(ASTRA_H_CIM_STEERING_WHEEL_BUTTONS_ID, 0x00, 0x81, 0x01);
			const CAN_COMMAND CMD_STEERING_WHEEL_RIGHT_TOP_PRESS		= CAN_COMMAND(ASTRA_H_CIM_STEERING_WHEEL_BUTTONS_ID, 0x01, 0x91, 0x01);
			const CAN_COMMAND CMD_STEERING_WHEEL_RIGHT_TOP_RELEASE		= CAN_COMMAND(ASTRA_H_CIM_STEERING_WHEEL_BUTTONS_ID, 0x00, 0x91, 0x01);

			const CAN_COMMAND_PERIODICAL PER_STEERING_WHEEL_LEFT_TOP_PRESS		{ &CMD_STEERING_WHEEL_LEFT_TOP_PRESS, 1000, 0 };			
			const CAN_COMMAND_PERIODICAL PER_STEERING_WHEEL_LEFT_TOP_RELEASE	{ &CMD_STEERING_WHEEL_LEFT_TOP_RELEASE, 1000, 0 };
			const CAN_COMMAND_PERIODICAL PER_STEERING_WHEEL_RIGHT_TOP_PRESS		{ &CMD_STEERING_WHEEL_RIGHT_TOP_PRESS, 1000, 0 };
			const CAN_COMMAND_PERIODICAL PER_STEERING_WHEEL_RIGHT_TOP_RELEASE	{ &CMD_STEERING_WHEEL_RIGHT_TOP_RELEASE, 1000, 0 };			
		#endif
	#endif

	/*** MEASURING BLOCKS ***/
	/*
		REQ DIS IDENTIFY:
		Received:2036708 8 0x101 - FE,1,3E,0,0,0,0,0,
		Received:2037024 8 0x246 - 1,20,0,0,0,0,0,0,
		
		DIS CLEAR ERRORS:
		Received:2038128 8 0x246 - 1,4,0,0,0,0,0,0,
	*/

#endif