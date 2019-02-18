//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#ifndef __CONFIGURATION_EEPROM__H__
	#define __CONFIGURATION_EEPROM__H__

	#include "customArduino.h"
	#include <EEPROM.h>
	#include "configuration.h"
  
	#define EEPROM_HDR_MAGIC                    B10101010
	#define EEPROM_HDR_MAGIC_ADDRESS            0		//1 Byte
	#define EEPROM_HDR_OPT_ADDRESS              1		//2 Byte
	#define EEPROM_HDR_OPT_SIZE					(sizeof(CONFIGURATION))
	#define EEPROM_HDR_ENTRY_COUNT_ADDRESS      EEPROM_HDR_OPT_ADDRESS + EEPROM_HDR_OPT_SIZE
	#define EEPROM_HDR_FIRST_ENTRY_ADDRESS      EEPROM_HDR_ENTRY_COUNT_ADDRESS + 1
	#define EEPROM_HDR_ENTRY_SIZE               2		//Byte
	#define EEPROM_HDR_MAX_ENTRIES              64		//2^6
	#define EEPROM_MAX_ENTRY_SIZE				64		//2^6

	extern uint8_t EEPROM_ENTRY_COUNT;
  
	#if COMPILE_EEPROM_CONFIG      
		bool eepromClear();     
		bool eepromLoad(CONFIGURATION *conf);
		bool eepromSave(CONFIGURATION *conf);
		uint16_t eepromGetEntry(uint8_t entryIndex);
		uint8_t eepromGetEntryDataSize(uint16_t entry);
		uint16_t eepromGetEntryDataPosition(uint16_t entry);
		uint8_t eepromReadEntry(uint16_t entry, char *outputBuf);
		bool eepromAddEntry(char *buf, uint8_t len);  
		bool eepromAddEntry(String *buf);  
		bool eepromRemoveLastEntry();
	#endif
#endif
