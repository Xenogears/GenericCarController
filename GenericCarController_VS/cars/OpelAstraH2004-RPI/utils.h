#ifndef __ASTRA_H_MIN_UTILS__H__
	#define __ASTRA_H_MIN_UTILS__H__

	#include "../../customArduino.h"

	#if CAR_MODEL == OPEL_ASTRA_H_2004_MIN_CAR 
		void astraHSendCommand(const char type, const char *cmd, char *params[] = NULL, uint8_t paramsLength = 0, Stream *s = NULL, bool autoFlush = true);
	#endif
#endif
