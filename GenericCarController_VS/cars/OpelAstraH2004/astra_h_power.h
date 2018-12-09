#ifndef __ASTRA_H_POWER__H__
  #define __ASTRA_H_POWER__H__  
	#include "../../configuration.h"  
	#include "../../power.h"
	#include "../../can_command.h"
	#include "../../radio_command.h"
	#include "../../traccar.h"  
	#include "astra_h.h"  
	#include "astra_h_dis.h"   
	#include "astra_h_alarm_led.h"

	#if COMPILE_CAN
		void astraHPowerOnChange(uint8_t previousStatus, uint8_t newStatus);
	#endif
#endif