//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#ifndef __ASTRA_H_MIN_UTILS__H__
	#define __ASTRA_H_MIN_UTILS__H__

	#include "../../customArduino.h"

	#if CAR_MODEL == OPEL_ASTRA_H_2004_MIN_CAR 
		void astraHSendCommand(const char type, const char *cmd, char *params[] = NULL, uint8_t paramsLength = 0, Stream *s = NULL, bool autoFlush = true);
	#endif
#endif
