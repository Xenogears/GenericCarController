//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#include "time.h"
#include "utils.h"

#if CAR_MODEL == OPEL_ASTRA_H_2004_MIN_CAR && COMPILE_GPS && COMPILE_TRACCAR
	//////////////////////// COMMAND HELPER ////////////////////////	
	void onGPSFixAcquired(gps_fix *fix)
	{
		if (fix == NULL || !fix->dateTime.is_valid())
			return;

		unsigned long date = fix->dateTime;
		if (date <= 1538352000) //10/01/2018 @ 12:00am (UTC) (MM/DD/YYYY)
			return;

		char ts[11];
		ltoa(fix->dateTime, (char*)&ts, 10);

		char *params[] = { (char*)&ts };
		astraHSendCommand('T', "SET", params, 1);
	}
#endif