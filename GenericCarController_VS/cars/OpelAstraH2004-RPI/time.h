#ifndef __ASTRA_H_MIN_TIME__H__
	#define __ASTRA_H_MIN_TIME__H__

	#include "../../customArduino.h"
	#include "../../gps.h"

	#if CAR_MODEL == OPEL_ASTRA_H_2004_MIN_CAR && COMPILE_GPS && COMPILE_TRACCAR
		void onGPSFixAcquired(float lat = 0, float lon = 0, uint32_t date = 0);
	#endif
#endif
