//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#include <avr/pgmspace.h>
#include "../../configuration.h"

#if CAR_MODEL == OPEL_ASTRA_H_2004_MIN_CAR 
	extern const PROGMEM char sRAD[];
	extern const PROGMEM char sRD[];
	extern const PROGMEM char sVolPlus[];
	extern const PROGMEM char sVolMinus[];
	extern const PROGMEM char sNext[];
	extern const PROGMEM char sPrev[];
	extern const PROGMEM char sMute[];
	extern const PROGMEM char sDIS_Lower[];
	extern const PROGMEM char sSrc[];
	extern const PROGMEM char sBand[];
	extern const PROGMEM char sFolderUp[];
	extern const PROGMEM char sFolderDown[];
	extern const PROGMEM char sVoice[];
	extern const PROGMEM char sCal[];
	extern const PROGMEM char sEcho[];
#endif