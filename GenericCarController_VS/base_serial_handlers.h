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
	
	bool SYSHandleConfCmd(Stream *serial, char *cmd, char *params[], uint8_t paramsLength);
	bool LOGHandleConfCmd(Stream *serial, char *cmd, char *params[], uint8_t paramsLength);
	bool OPTHandleConfCmd(Stream *serial, char *cmd, char *params[], uint8_t paramsLength);
	bool ROMHandleConfCmd(Stream *serial, char *cmd, char *params[], uint8_t paramsLength);

	#if COMPILE_CAN
		bool CANHandleConfCmd(Stream *serial, char *cmd, char *params[], uint8_t paramsLength);
	#endif
#endif
