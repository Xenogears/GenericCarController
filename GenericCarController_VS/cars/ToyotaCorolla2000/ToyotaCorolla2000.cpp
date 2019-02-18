//
// Author:	�lvaro Graci� Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

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