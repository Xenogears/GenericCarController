#include "../../configuration.h"

#if COMPILE_REMOTE_DOOR && CAR_MODEL == OPEL_ASTRA_H_2004_MIN_CAR
	#include "astra_h_remote_door.h"

	#ifdef GPIO1_PIN
		const ASTRA_H_REMOTE_DOOR_ACTION DOOR_0_OPEN	= { GPIO1_PIN, REMOTE_DOOR_MODE_SWITCH_ENABLED_LOGIC_LEVEL };
		const ASTRA_H_REMOTE_DOOR_ACTION DOOR_0_CLOSE	= { GPIO1_PIN, REMOTE_DOOR_MODE_SWITCH_DISABLED_LOGIC_LEVEL };
	#endif

	#ifdef GPIO2_PIN
		const ASTRA_H_REMOTE_DOOR_ACTION DOOR_1_OPEN	= { GPIO2_PIN, REMOTE_DOOR_MODE_SWITCH_ENABLED_LOGIC_LEVEL };
		const ASTRA_H_REMOTE_DOOR_ACTION DOOR_1_CLOSE	= { GPIO2_PIN, REMOTE_DOOR_MODE_SWITCH_DISABLED_LOGIC_LEVEL };
	#endif

	#if REMOTE_DOOR_MODE == REMOTE_DOOR_MODE_RC
		static RCSwitch astraHDoorSwitch = RCSwitch();
	#else
		static const ASTRA_H_REMOTE_DOOR_ACTION *astraHRemoteDoorNextAction = NULL;
		static unsigned long astraHRemoteDoorNextActionMillis = 0;
	#endif
	
	static void astraHRemoteDoorSendCmd(const ASTRA_H_REMOTE_DOOR_ACTION *act);

	bool astraHRemoteDoorSetup()
	{
		#if REMOTE_DOOR_MODE == REMOTE_DOOR_MODE_RC
			astraHDoorSwitch.enableTransmit(REMOTE_DOOR_MODE_RC_PIN);
			astraHDoorSwitch.setPulseLength(REMOTE_DOOR_MODE_RC_PULSE_LENGTH);

			#if REMOTE_DOOR_MODE_RC_REPEAT_TRANSMIT != 10
				astraHDoorSwitch.setRepeatTransmit(REMOTE_DOOR_MODE_RC_REPEAT_TRANSMIT);
			#endif
		#elif REMOTE_DOOR_MODE == REMOTE_DOOR_MODE_SWITCH
			for (uint8_t i = 0; i < sizeof(DOOR_PINS) / sizeof(uint8_t); i++)
			{				
				digitalWrite(DOOR_PINS[i], REMOTE_DOOR_MODE_SWITCH_DISABLED_LOGIC_LEVEL);
				pinMode(DOOR_PINS[i], OUTPUT);
			}
		#endif
	}

	#if REMOTE_DOOR_MODE == REMOTE_DOOR_MODE_RC
		void astraHRemoteDoorSendAction(const ASTRA_H_REMOTE_DOOR_ACTION *open)
		{
			astraHRemoteDoorSendCmd(open);
		}
	#elif REMOTE_DOOR_MODE == REMOTE_DOOR_MODE_SWITCH
		void astraHRemoteDoorSendAction(const ASTRA_H_REMOTE_DOOR_ACTION *open, const ASTRA_H_REMOTE_DOOR_ACTION *close)
		{
			if(astraHRemoteDoorNextAction != NULL)
				astraHRemoteDoorSendCmd(astraHRemoteDoorNextAction);

			astraHRemoteDoorNextAction = close;
			astraHRemoteDoorNextActionMillis = (millis() + REMOTE_DOOR_SIGNAL_SEND_INTERVAL);
			astraHRemoteDoorSendCmd(open);
		}

		void astraHRemoteDoorCheck()
		{
			if (astraHRemoteDoorNextAction != NULL && millis() >= astraHRemoteDoorNextActionMillis)
			{
				astraHRemoteDoorSendCmd(astraHRemoteDoorNextAction);
				astraHRemoteDoorNextAction = NULL;
			}
		}
	#endif	

	static void astraHRemoteDoorSendCmd(const ASTRA_H_REMOTE_DOOR_ACTION *action)
	{
		#if REMOTE_DOOR_MODE == REMOTE_DOOR_MODE_RC

			astraHDoorSwitch.setProtocol(action->protocol);
			astraHDoorSwitch.send(action->code, action->codeLength);

		#elif REMOTE_DOOR_MODE == REMOTE_DOOR_MODE_SWITCH

			digitalWrite(action->pin, action->logicLevel);

		#endif
	}
#endif