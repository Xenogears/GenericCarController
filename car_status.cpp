//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#include "configuration.h"

#if COMPILE_CAR_STATUS_MONITOR
	#include "car_status.h"
	struct CAR_STATUS currentCarStatus;
	static uint8_t currentAlarms = 0;

	void checkCarStatus()
	{
		uint8_t newAlarms = 0;
		if (!currentCarStatus.updated)
			return;

		currentCarStatus.updated = false;

		#if MON_FETCH_VOLTAGE && MON_CHECK_VOLTAGE				
			if (currentCarStatus.voltage <= MON_MIN_VOLTAGE || currentCarStatus.voltage >= MON_MAX_VOLTAGE)
				newAlarms |= MON_VOLTAGE_BITMASK;
		#endif
		#if MON_FETCH_ENGINE_TEMP && MON_CHECK_ENGINE_TEMP
			if (currentCarStatus.engineTemp >= MON_MAX_ENGINE_TEMP)
				newAlarms |= MON_ENGINE_TEMP_BITMASK;
		#endif
		#if MON_FETCH_SPEED && MON_CHECK_SPEED
			if (currentCarStatus.speed >= MON_MAX_SPEED)
				newAlarms |= MON_SPEED_BITMASK;
		#endif

		if (newAlarms ^ currentAlarms) //CHANGED
			MON_ON_ALARM_STATUS_CHANGED_HANDLER(newAlarms);

		currentAlarms = newAlarms;
	}
#endif
