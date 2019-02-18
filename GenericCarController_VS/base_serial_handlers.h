//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#ifndef __BASE_SERIAL_HANDLERS__H__
	#define __BASE_SERIAL_HANDLERS__H__

	#include "customArduino.h"
	#include <avr/pgmspace.h>

	#include "configuration.h"
	#include "configuration_eeprom.h"
	#include "traccar.h"  
	#include "utils.h"

	bool BTHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength);    
	bool SYSHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength);
	bool LOGHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength);
	bool OPTHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength);
	bool ROMHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength);

	#if COMPILE_CAN
		bool CANHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength);
	#endif
#endif
