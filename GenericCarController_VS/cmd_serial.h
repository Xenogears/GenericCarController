//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#ifndef __CMD_SERIAL__H__
	#define __CMD_SERIAL__H__

	#include "configuration.h"

	#if CMD_SERIAL_MODE == HW_SERIAL
		#include "lib/serial/NeoHWSerial.h"   
		#define cmdSerial CMD_HW_SERIAL
	#elif  CMD_SERIAL_MODE == IC_SERIAL
		#include "lib/serial/NeoICSerial.h"    
		extern NeoICSerial cmdSerial;	
	#elif CMD_SERIAL_MODE == SW_SERIAL
		#include "lib/serial/NeoSWSerial.h"    
	#endif        	

	void checkCMDSerial();
	bool setupCMDSerial();
#endif
