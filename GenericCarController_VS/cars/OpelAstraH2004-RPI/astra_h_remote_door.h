#ifndef __ASTRA_H_MIN_REMOTE_DOOR__H__   
	#define __ASTRA_H_MIN_REMOTE_DOOR__H__

	#include "../../configuration.h"

	#define REMOTE_DOOR_MODE_SWITCH_DISABLED_LOGIC_LEVEL	HIGH
	#define REMOTE_DOOR_MODE_SWITCH_ENABLED_LOGIC_LEVEL		LOW

	#if REMOTE_DOOR_MODE == REMOTE_DOOR_MODE_RC
		#include "../../lib/remote/RCSwitch.h"

		typedef struct
		{
			uint8_t protocol;
			unsigned long code;
			uint8_t codeLength;

		} ASTRA_H_REMOTE_DOOR_ACTION;

		const ASTRA_H_REMOTE_DOOR_ACTION DOOR_0_OPEN	= { 1, 1234, 4 };

		void astraHRemoteDoorSendAction(const ASTRA_H_REMOTE_DOOR_ACTION *open);
	#elif REMOTE_DOOR_MODE == REMOTE_DOOR_MODE_SWITCH
		typedef struct
		{
			const uint8_t pin;
			const uint8_t logicLevel;

		} ASTRA_H_REMOTE_DOOR_ACTION;
		
		extern const ASTRA_H_REMOTE_DOOR_ACTION DOOR_0_OPEN;
		extern const ASTRA_H_REMOTE_DOOR_ACTION DOOR_0_CLOSE;

		#ifdef GPIO2_PIN
			extern const ASTRA_H_REMOTE_DOOR_ACTION DOOR_1_OPEN;
			extern const ASTRA_H_REMOTE_DOOR_ACTION DOOR_1_CLOSE;

			const uint8_t DOOR_PINS[] = { GPIO1_PIN, GPIO2_PIN };
		#else
			#ifdef GPIO1_PIN
				const uint8_t DOOR_PINS[] = { GPIO1_PIN };
			#else	
				const uint8_t DOOR_PINS[] = { 0 };
			#endif
		#endif

		void astraHRemoteDoorSendAction(const ASTRA_H_REMOTE_DOOR_ACTION *open, const ASTRA_H_REMOTE_DOOR_ACTION *close);
		void astraHRemoteDoorCheck();
	#endif

	bool astraHRemoteDoorSetup();	
#endif
