#include "../../configuration.h"

#if CAR_MODEL == FORD_FOCUS_2004_CAR 
	#include "FordFocus2004.h"

	#if COMPILE_CAN    
		void carAfterCmdReceived(CAN_CONTROLLER *can, CAN_COMMAND *receivedCmd) {}
	#endif
	
	void carAfterLoop() {}

	void carBeforeSetup() {}

	void carAfterSetup() {}
#endif