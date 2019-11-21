//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#ifndef __UTILS__H__
	#define __UTILS__H__

	#include "customArduino.h"
	#include <stdarg.h>
	#include "configuration.h"
	#include "lib/others/FastGPIO.h"	

	int utilsFreeRAM();
	bool utilsResetSystem();
	
	bool utilsStartWith(const char *str1, const char *str2);
	bool utilsEndsWith(const char *str1, const char *str2);
	bool utilsEquals(const char *str1, const char *str2);

	bool utilsStartWith_P(const char *str1, const PROGMEM char *str2);
	bool utilsEndsWith_P(const char *str1, const PROGMEM char *str2);
	bool utilsEquals_P(const char *str1, const PROGMEM char *str2);

	uint8_t utilsFirstIndexOf(const char *str1, const char *str2);
	uint8_t utilsFirstIndexOf_P(const char *str1, const PROGMEM char *str2);

	uint8_t hexStrToByte(char *in, uint32_t *out);
#endif
