#include "injection.h"
#include "can_command.h"
#include "can_controller.h"

static uint16_t injSeqID = 0;

void checkInjection()
{
	#if COMPILE_CAN
		if (DEFAULT_CAN.getTXQueueAvailableSlots() <= 0)
			return;

		CAN_COMMAND newCmd;	

		#if INJECT_SEQUENTIAL_COMMANDS		
			newCmd.can_id = injSeqID;
			newCmd.can_dlc = 8;

			for (uint8_t i = 0; i < 8; i++)
				newCmd.data[i] = i;

			if(DEFAULT_CAN.sendCmd(&newCmd))
				injSeqID = (++injSeqID & 0x7FF);
		#endif
	#endif
}

bool setupInjection()
{
	return true;
}