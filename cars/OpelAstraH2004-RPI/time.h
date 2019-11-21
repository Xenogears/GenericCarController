//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#ifndef __ASTRA_H_MIN_TIME__H__
	#define __ASTRA_H_MIN_TIME__H__

	#include "../../customArduino.h"
	#include "../../gps.h"

	#if CAR_MODEL == OPEL_ASTRA_H_2004_MIN_CAR && COMPILE_GPS && COMPILE_TRACCAR
		void onGPSFixAcquired(float lat = 0, float lon = 0, uint32_t date = 0);
	#endif
#endif
