#include "DefaultCar.h"

#if CAR_MODEL == NONE_CAR
	#if COMPILE_CAN    
		void carAfterCmdReceived(CAN_CONTROLLER *can, CAN_COMMAND *receivedCmd)
		{

		}
	#endif

	void carAfterLoop()
	{

	}


	void carBeforeSetup()
	{

	}

	void carAfterSetup() {}
#endif