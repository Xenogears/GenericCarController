#include "../../configuration.h"

#if CAR_MODEL == TOYOTA_COROLLA_2000_CAR 
	#include "ToyotaCorolla2000.h"

	#if COMPILE_CAN    
		void carAfterCmdReceived(CAN_CONTROLLER *can, CAN_COMMAND *receivedCmd) {}
	#endif
	
	void carAfterLoop() {}


	void carBeforeSetup() {}

	void carAfterSetup() {}
#endif