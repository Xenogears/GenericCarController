//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#include "base_serial_handlers.h"
#include "cmd_serial.h"
#include "can_command.h"
#include "can_controller.h"

bool SYSHandleConfCmd(Stream *serial, char *cmd, char *params[], uint8_t paramsLength)
{
	if (utilsEquals_P(cmd, sReset))
	{
		utilsResetSystem();
		return true;
	}
	else if (utilsEquals_P(cmd, sFree))
	{
		serial->println(utilsFreeRAM());
		return true;
	}

	return false;
}

bool LOGHandleConfCmd(Stream *serial, char *cmd, char *params[], uint8_t paramsLength)
{
	bool handled;	

	if(utilsEquals_P(cmd, sGet))
	{		
		handled = true;
		char *param = params[0];

		cmdSerialPrintln(serial);
		cmdSerialPrint_P(serial, sRES);
		cmdSerialPrint_P(serial, sTwoDots);
		cmdSerialPrint_P(serial, sLOG);
		cmdSerialPrint_P(serial, sTwoDots);
		cmdSerialPrint(serial, param);
		cmdSerialPrint_P(serial, sEquals);
                  
		if (utilsEquals_P(param, sCAN))			serial->println(CONFIG.get(PRINT_CAN_DATA) ? 1 : 0);
		else if(utilsEquals_P(param, sPOT))		serial->println(CONFIG.get(PRINT_POTENTIOMETER_INFO) ? 1 : 0);
		else if(utilsEquals_P(param, sSTA))		serial->println(CONFIG.get(PRINT_STATUS_INFO) ? 1 : 0);
		else if(utilsEquals_P(param, sTRC))		serial->println(CONFIG.get(PRINT_TRACCAR_INFO) ? 1 : 0);
		else if(utilsEquals_P(param, sGSM))		serial->println(CONFIG.get(PRINT_GSM_INFO) ? 1 : 0);
		else if(utilsEquals_P(param, sGPS))		serial->println(CONFIG.get(PRINT_GPS_INFO) ? 1 : 0);
		#if COMPILE_CAR_STATUS_MONITOR
			else if (utilsEquals_P(param, sCAR))	serial->println(CONFIG.get(PRINT_CAR_STATUS_INFO) ? 1 : 0);
		#endif
		#if COMPILE_DEBUG
			else if (utilsEquals_P(param, sDBG))	serial->println(CONFIG.get(PRINT_DEBUG) ? 1 : 0);
		#endif
		else handled = false;

		return handled;
	}
	else if (paramsLength >= 2)
	{    
		handled = false;

		bool all = utilsEquals_P(params[0], sALL);
		bool v = !utilsEquals_P(params[1], sZero);		

		if(all || utilsEquals_P(params[0], sCAN)) {		CONFIG.set(PRINT_CAN_DATA, v);				handled = true;	}
		if(all || utilsEquals_P(params[0], sPOT)) {		CONFIG.set(PRINT_POTENTIOMETER_INFO, v);	handled = true;	}
		if(all || utilsEquals_P(params[0], sSTA)) {		CONFIG.set(PRINT_STATUS_INFO, v);			handled = true;	}
		if(all || utilsEquals_P(params[0], sTRC)) {		CONFIG.set(PRINT_TRACCAR_INFO, v);			handled = true;	}
		if(all || utilsEquals_P(params[0], sGSM)) {		CONFIG.set(PRINT_GSM_INFO, v);				handled = true;	}
		if(all || utilsEquals_P(params[0], sGPS)) {		CONFIG.set(PRINT_GPS_INFO, v);				handled = true;	}
		#if COMPILE_CAR_STATUS_MONITOR
			if (all || utilsEquals_P(params[0], sCAR)) { CONFIG.set(PRINT_CAR_STATUS_INFO, v);		handled = true; }
		#endif
		#if COMPILE_DEBUG
			if (all || utilsEquals_P(params[0], sDBG)) { CONFIG.set(PRINT_DEBUG, v);				handled = true; }
		#endif

		if (handled)
			cmdSerialPrintln_P(serial, sOK);	

		return (!all & handled);
	}
}

bool OPTHandleConfCmd(Stream *serial, char *cmd, char *params[], uint8_t paramsLength)
{
	bool handled = true;
	
	if(utilsEquals_P(cmd, sGet))
	{    
		char *param = params[0];

		cmdSerialPrintln(serial);
		cmdSerialPrint_P(serial, sRES);
		cmdSerialPrint_P(serial, sTwoDots);
		cmdSerialPrint_P(serial, sOPT);
		cmdSerialPrint_P(serial, sTwoDots);
		cmdSerialPrint(serial, param);
		cmdSerialPrint_P(serial, sEquals);

		if (utilsEquals_P(param, sTRC))
			serial->println(CONFIG.get(ENABLE_TRACCAR_NOTIFICATION) ? 1 : 0);
		else
			handled = false;
	}
	else if(paramsLength >= 2)
	{		
		bool all = utilsEquals_P(params[0], sALL);
		bool v = !utilsEquals_P(params[1], sZero);

		if (all || utilsEquals_P(params[0], sTRC))	CONFIG.set(ENABLE_TRACCAR_NOTIFICATION, v);
		else handled = false;	

		if (handled)
			cmdSerialPrintln_P(serial, sOK);
	}

	return handled;
}

bool ROMHandleConfCmd(Stream *serial, char *cmd, char *params[], uint8_t paramsLength)
{   
  #if COMPILE_EEPROM_CONFIG
    if(utilsEquals_P(cmd, sSave))			cmdSerialPrintln_P(serial, eepromSave(&CONFIG)		? sOK : sERROR);
    else if(utilsEquals_P(cmd, sLoad))		cmdSerialPrintln_P(serial, eepromLoad(&CONFIG)		? sOK : sERROR);
	else if (utilsEquals_P(cmd, sRemove))	cmdSerialPrintln_P(serial, eepromRemoveLastEntry()	? sOK : sERROR);
	else if (utilsEquals_P(cmd, sClear))
	{
		CONFIG.clear();
		cmdSerialPrintln_P(serial, eepromClear() ? sOK : sERROR);
	}
    else if(utilsEquals_P(cmd, sAdd))
    {
		if(eepromAddEntry(params[0], strlen(params[0])))
			cmdSerialPrintln_P(serial, sOK);
		else
			cmdSerialPrintln_P(serial, sERROR);
    }
    else if(utilsEquals_P(cmd, sShow))
    {
		for(unsigned char i = 0; i < EEPROM_ENTRY_COUNT; i++)
		{
			uint16_t entry = eepromGetEntry(i);
			uint8_t s = eepromGetEntryDataSize(entry);
			char *tmp = (char*)malloc(s + 1);              
			eepromReadEntry(entry, tmp);
			tmp[s] = '\0';
  
			cmdSerialPrint_P(serial, sENTRY);
			cmdSerialPrint_P(serial, sTwoDots);
			serial->print(i);
			cmdSerialPrint_P(serial, sEquals);
			cmdSerialPrintln(serial, tmp);
  
			free(tmp);
		}
    }     
  #endif
  
  return true;   
}

#if COMPILE_CAN
	bool CANHandleConfCmd(Stream *serial, char *cmd, char *params[], uint8_t paramsLength)
	{
		if (utilsEquals_P(cmd, sSend) && paramsLength > 0)
		{					
			CAN_COMMAND cmd = CAN_COMMAND();
			cmd.can_dlc = (paramsLength - 1);

			for (uint8_t i = 0; i < paramsLength; i++)
			{
				uint16_t data = 0;
				if (hexStrToByte(params[i], (uint32_t*)&data) <= 0)
				{
					cmdSerialPrintln_P(serial, sERROR);
					return true;
				}

				if(i == 0)
					cmd.can_id = (data & 0x07FF);
				else
					cmd.data[i-1] = (data & 0xFF);
			}

			if (!DEFAULT_CAN.sendCmd(&cmd))
				cmdSerialPrintln_P(serial, sERROR);
			else
				cmdSerialPrintln_P(serial, sOK);

			return true;
		}
		else return true;
	}
#endif