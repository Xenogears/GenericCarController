//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#ifndef __ASTRA_H_MIN_POWER__H__
  #define __ASTRA_H_MIN_POWER__H__  
	#include "../../configuration.h"  
	#include "../../power.h"
	#include "../../can_command.h"
	#include "../../radio_command.h"
	#include "../../traccar.h"  
	#include "astra_h.h"  

	#if COMPILE_CAN
		void astraHPowerOnChange(uint8_t previousStatus, uint8_t newStatus);
	#endif
#endif
