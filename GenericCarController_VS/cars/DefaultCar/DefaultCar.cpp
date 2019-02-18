//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

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