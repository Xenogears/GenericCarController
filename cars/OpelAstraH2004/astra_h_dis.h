//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#ifndef __ASTRA_H_DIS__H__
  #define __ASTRA_H_DIS__H__
	  #include "../../can_command.h"
	  #include "../../can_controller.h"
	  #include "../../configuration.h"
	  #include "../../configuration_eeprom.h"

	  #if DIS_USE_TIMER_ONE
		#include "../../lib/timer/TimerOne.h"
	  #endif

	#if COMPILE_CAN && COMPILE_DIS
		/*
		data[0] = 0x10. The first byte of the packet identifies a multi-packet message.
		data[1] = length
		data[2] = command[1]
		data[3] = command[0]
		data[4] = ? --> 0x4F o 0x37 !!!! --> Estaba a 0x4F
			- Funciona igual con cualquiera de los dos
		data[5] = ? --> Siempre 0x03 (Main display). Fallan: (0x01, 0x02, 0x04, 0x05)
		data[6] = ? --> Siempre 0x10. Fallan: (0x01, 0x02, 0x11, 0x12, 0x23)
		data[7] = ? --> 0x11, 0x26, 0x1A !!!! -> Estaba a 0x11. 
			- Con 0x26 y 0x1A funciona pero no parece que haya diferencia

		99.1Mhz
		162368 - 0x000006C1 -> ,10,52,C0,00,4F,03,10,26

		FM1 M80Radio
		262425 - 0x000006C1 -> ,10,74,40,00,37,03,10,1A

		FM2 99.0Mhz
		289740 - 0x000006C1 -> ,10,52,C0,00,4F,03,10,26		

		//MODO?
		Received (B): 15604ms - 0x4C1 -> 41,21,9B,99,00,00,00,00,


		http://jb0.de/?p=199
		https://github.com/Trueffelwurm/Car-CAN-Message-DB/blob/master/Opel/Astra/H/MS-CAN/Info-Display.md
		The first byte of the packet identifies a multi-packet message. It starts with 0x10, than follows 0x2X where X increments from 1 to F. So after 0x2F comes 0x20, 0x21…
		The second Byte of the first packet is the number in Bytes. The last packet is always 8 Byte long but the rest is ignored (these are either filled with 0x00 or the same then the previous package).
		Than are two byte of command or mode. This is 0x4000, 0xC000, 0x5000 or 0xA000. Don’t know what that means… Than comes the size and type of the following container. Type 0x03 seems to update the main screen.
		Now follow some strings, starting with an ID (here 0x01,0x10,0x11 and 0x12), the number of characters and the characters in UTF-16 (where only very few unicode-chars are supported but that comes in the next post).
		*/

		extern const CAN_COMMAND CMD_DIS_SET_MODE;
		#define DIS_MODE_NONE	0
		#define DIS_MODE_AS		(1 << 0)					//SETTINGS MODE
		#define DIS_MODE_RDS	(1 << 1)					//LEFT BOTTOM BUTTON
		#define DIS_MODE_REG	(1 << 2)					//LEFT TOP BUTTON
		
		void checkDIS();
		bool astraHDISSetup();
		void astraHDISStart();
		void astraHDISStop();
		bool astraHDISIsRunning();
				
		void astraHDISSetText(CAN_CONTROLLER *can, String *msg, uint8_t tickTimeout = DIS_TEXT_TIMEOUT_TICKS, bool request = true);
		void astraHDISSetText(CAN_CONTROLLER *can, char *msg, uint8_t tickTimeout = DIS_TEXT_TIMEOUT_TICKS, bool request = true);		
		void astraHDISSetText_P(CAN_CONTROLLER *can, const PROGMEM char *msg, uint8_t tickTimeout = DIS_TEXT_TIMEOUT_TICKS, bool request = true);
		void astraHDISSetDefaultText(CAN_CONTROLLER *can = &DEFAULT_CAN);
		void astraHDISSetRandomText(CAN_CONTROLLER *can, uint8_t tickTimeout = DIS_TEXT_TIMEOUT_TICKS, bool request = true);
		bool astraHDISRequestText(CAN_CONTROLLER *can);    
		bool astraHDISShowText(CAN_CONTROLLER *can);
	#endif
#endif 
