#include "base_serial_handlers.h"
#include "cmd_serial.h"
#include "can_command.h"
#include "can_controller.h"

bool BTHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength)
{
  if(utilsEquals_P(cmd, sSetup))
  {
   /* Serial.end();
    Serial.begin(BLUETOOTH_PREV_BAUDRATE);*/
    
	cmdSerial.print(F("AT+ORGL"));
    delay(2000);   
	cmdSerial.print(F("AT+PIN"));
	cmdSerial.print(String(BLUETOOTH_PIN));
    delay(2000);
	cmdSerial.print(F("AT+NAME"));
	cmdSerial.print(F(BLUETOOTH_NAME));
    delay(2000);
	cmdSerial.print(F("AT+ROLE=0"));
    delay(2000);
	cmdSerial.print(F("AT+POLAR=1,0"));
    delay(2000);
	/*cmdSerial.print(F("AT+UART="));
	cmdSerial.print(BLUETOOTH_NEW_BAUDRATE);
	cmdSerial.print(F(",0,0"));
    delay(2000);*/
	cmdSerial.print(F("AT+BAUD"));
	cmdSerial.print(BLUETOOTH_NEW_BAUDRATE_INDEX);
    delay(2000);
	cmdSerial.print(F("AT+INIT"));
	cmdSerial.println(sOK);

    /*Serial.end();
    Serial.begin(DEFAULT_BAUDRATE);*/
  }

  return true;
}

bool SYSHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength)
{
	if (utilsEquals_P(cmd, sReset))
	{
		utilsResetSystem();
		return true;
	}
	else if (utilsEquals_P(cmd, sFree))
	{
		cmdSerial.println(utilsFreeRAM());
		return true;
	}

	return false;
}

bool LOGHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength)
{
	bool handled;	

	if(utilsEquals_P(cmd, sGet))
	{		
		handled = true;
		char *param = params[0];

		utilsPrintln();
		utilsPrint_P(sRES);
		utilsPrint_P(sTwoDots);
		utilsPrint_P(sLOG);
		utilsPrint_P(sTwoDots);
		utilsPrint(param);
		utilsPrint_P(sEquals);
                  
		if (utilsEquals_P(param, sCAN))			cmdSerial.println(CONFIG.get(PRINT_CAN_DATA) ? 1 : 0);
		else if(utilsEquals_P(param, sPOT))		cmdSerial.println(CONFIG.get(PRINT_POTENTIOMETER_INFO) ? 1 : 0);
		else if(utilsEquals_P(param, sSTA))		cmdSerial.println(CONFIG.get(PRINT_STATUS_INFO) ? 1 : 0);
		else if(utilsEquals_P(param, sTRC))		cmdSerial.println(CONFIG.get(PRINT_TRACCAR_INFO) ? 1 : 0);
		else if(utilsEquals_P(param, sGSM))		cmdSerial.println(CONFIG.get(PRINT_GSM_INFO) ? 1 : 0);
		else if(utilsEquals_P(param, sGPS))		cmdSerial.println(CONFIG.get(PRINT_GPS_INFO) ? 1 : 0);
		#if COMPILE_CAR_STATUS_MONITOR
			else if (utilsEquals_P(param, sCAR))	cmdSerial.println(CONFIG.get(PRINT_CAR_STATUS_INFO) ? 1 : 0);
		#endif
		#if COMPILE_DEBUG
			else if (utilsEquals_P(param, sDBG))	cmdSerial.println(CONFIG.get(PRINT_DEBUG) ? 1 : 0);
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
			utilsPrintln_P(sOK);	

		return (!all & handled);
	}
}

bool OPTHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength)
{
	bool handled = true;
	
	if(utilsEquals_P(cmd, sGet))
	{    
		char *param = params[0];

		utilsPrintln();
		utilsPrint_P(sRES);
		utilsPrint_P(sTwoDots);
		utilsPrint_P(sOPT);
		utilsPrint_P(sTwoDots);
		utilsPrint(param);
		utilsPrint_P(sEquals);

		if (utilsEquals_P(param, sTRC))
			cmdSerial.println(CONFIG.get(ENABLE_TRACCAR_NOTIFICATION) ? 1 : 0);
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
			utilsPrintln_P(sOK);
	}

	return handled;
}

bool ROMHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength)
{   
  #if COMPILE_EEPROM_CONFIG
    if(utilsEquals_P(cmd, sSave))			utilsPrintln_P(eepromSave(&CONFIG)		? sOK : sERROR);
    else if(utilsEquals_P(cmd, sLoad))		utilsPrintln_P(eepromLoad(&CONFIG)		? sOK : sERROR);
	else if (utilsEquals_P(cmd, sRemove))	utilsPrintln_P(eepromRemoveLastEntry()	? sOK : sERROR);
	else if (utilsEquals_P(cmd, sClear))
	{
		CONFIG.clear();
		utilsPrintln_P(eepromClear() ? sOK : sERROR);
	}
    else if(utilsEquals_P(cmd, sAdd))
    {
		if(eepromAddEntry(params[0], strlen(params[0])))
			utilsPrintln_P(sOK);
		else
			utilsPrintln_P(sERROR);
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
  
			utilsPrint_P(sENTRY);
			utilsPrint_P(sTwoDots);
			cmdSerial.print(i);
			utilsPrint_P(sEquals);
			utilsPrintln(tmp);
  
			free(tmp);
		}
    }     
  #endif
  
  return true;   
}

#if COMPILE_CAN
	bool CANHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength)
	{
		if (utilsEquals_P(cmd, sSend))
		{					
			if (paramsLength <= 0)
				return;

			CAN_COMMAND cmd = CAN_COMMAND();
			cmd.can_dlc = (paramsLength - 1);

			for (uint8_t i = 0; i < paramsLength; i++)
			{
				uint16_t data = 0;
				if (hexStrToByte(params[i], (uint32_t*)&data) <= 0)
				{
					utilsPrintln_P(sERROR);
					return true;
				}

				if(i == 0)
					cmd.can_id = (data & 0x07FF);
				else
					cmd.data[i-1] = (data & 0xFF);
			}

			if (!DEFAULT_CAN.sendCmd(&cmd))
				utilsPrintln_P(sERROR);
			else
				utilsPrintln_P(sOK);

			return true;
		}
		else return true;
	}
#endif