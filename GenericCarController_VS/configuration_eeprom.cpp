//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#include "configuration_eeprom.h"

#if COMPILE_EEPROM_CONFIG    
	uint8_t EEPROM_ENTRY_COUNT = 0;

	bool eepromClear()
	{
		EEPROM_ENTRY_COUNT = 0;
		EEPROM.update(EEPROM_HDR_MAGIC_ADDRESS, 0);
		EEPROM.update(EEPROM_HDR_ENTRY_COUNT_ADDRESS, 0);     

		return true;
	}
 
	bool eepromLoad(CONFIGURATION *conf)
	{   
		if (EEPROM.read(EEPROM_HDR_MAGIC_ADDRESS) != EEPROM_HDR_MAGIC)
			return false;

		for (uint8_t i = 0; i < (sizeof(CONFIGURATION)); i++)
			conf->set(i, EEPROM.read(EEPROM_HDR_OPT_ADDRESS + i));

		EEPROM_ENTRY_COUNT = EEPROM.read(EEPROM_HDR_ENTRY_COUNT_ADDRESS);
	
		return true;
	}

	bool eepromSave(CONFIGURATION *conf)
	{
		EEPROM.update(EEPROM_HDR_MAGIC_ADDRESS, EEPROM_HDR_MAGIC);
	  
		uint16_t params = conf->getAll();
		for (uint8_t i = 0; i < (sizeof(CONFIGURATION)); i++)	  
			EEPROM.update(EEPROM_HDR_OPT_ADDRESS + i, (params >> i * 8) & 0xFF);	  
    
		EEPROM.update(EEPROM_HDR_ENTRY_COUNT_ADDRESS, EEPROM_ENTRY_COUNT);

		return true;
	}

	uint16_t eepromGetEntry(uint8_t entryIndex)
	{
		uint16_t base = EEPROM_HDR_FIRST_ENTRY_ADDRESS + (entryIndex * EEPROM_HDR_ENTRY_SIZE);
		return EEPROM.read(base) << 8 | EEPROM.read(base + 1);      
	}

	uint8_t eepromGetEntryDataSize(uint16_t entry) { return entry >> 10; }

	uint16_t eepromGetEntryDataPosition(uint16_t entry) { return entry & 0x3FF; }

	uint8_t eepromReadEntry(uint16_t entry, char *outputBuf)
	{
		uint8_t entrySize = eepromGetEntryDataSize(entry);
		uint16_t entryDataPos = eepromGetEntryDataPosition(entry);
    
		for(uint8_t i = 0; i < entrySize; i++)
			outputBuf[i] = EEPROM.read(entryDataPos+i);

		return entrySize;
	}

	bool eepromAddEntry(char *buf, uint8_t len)
	{
		if(EEPROM_ENTRY_COUNT >= EEPROM_HDR_MAX_ENTRIES || len > EEPROM_MAX_ENTRY_SIZE)
			return false;
      
		uint16_t dataIndex;
		if(EEPROM_ENTRY_COUNT > 0)
		{
			uint16_t lastEntry = eepromGetEntry(EEPROM_ENTRY_COUNT-1);
			dataIndex = eepromGetEntryDataPosition(lastEntry) + eepromGetEntryDataSize(lastEntry);
		}
		else dataIndex = EEPROM_HDR_FIRST_ENTRY_ADDRESS + (EEPROM_HDR_MAX_ENTRIES * EEPROM_HDR_ENTRY_SIZE) + 1;
    
		uint16_t newEntry = dataIndex | (len << 10);
		uint16_t baseAddr = EEPROM_HDR_FIRST_ENTRY_ADDRESS + ((EEPROM_ENTRY_COUNT++) * EEPROM_HDR_ENTRY_SIZE);
   
		EEPROM.update(baseAddr, (newEntry >> 8));
		EEPROM.update(baseAddr + 1, (newEntry & 0xFF));

		for(uint8_t i = 0; i < len; i++)
			EEPROM.update(dataIndex+i, buf[i]);
      
		EEPROM.update(EEPROM_HDR_ENTRY_COUNT_ADDRESS, EEPROM_ENTRY_COUNT);

		return true;
	}   

	bool eepromAddEntry(String *buf)
	{
		return eepromAddEntry((char*)buf->c_str(), (uint8_t)buf->length());
	}   

	bool eepromRemoveLastEntry()
	{
		if(EEPROM_ENTRY_COUNT <= 0)
			return false;
      
		EEPROM.update(EEPROM_HDR_ENTRY_COUNT_ADDRESS, --EEPROM_ENTRY_COUNT);

		return true;
	}
  
#endif
