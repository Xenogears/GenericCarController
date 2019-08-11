//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#include "serial_handler.h"

SERIAL_HANDLER::SERIAL_HANDLER(const PROGMEM char *t, bool (*h)(Stream*, char*, char**, uint8_t))
{
	this->type = t;
	this->handler = h;
}    
