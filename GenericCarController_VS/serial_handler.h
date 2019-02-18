//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#ifndef __SERIAL_HANDLER__H__
	#define __SERIAL_HANDLER__H__

	#include "customArduino.h"
  
	class SERIAL_HANDLER
	{
		public:
			const bool (*handler)(char *cmd, char *params[], uint8_t paramsLength);
			const PROGMEM char *type;
  
			SERIAL_HANDLER(const PROGMEM char *t, bool (*h)(char*, char**, uint8_t));
	};

#endif
