//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#include <SPI.h>
#include "configuration.h"
#include "configuration_eeprom.h"
#include "potentiometer.h"
#include "radio_command.h"
#include "can_command.h"
#include "can_controller.h"
#include "utils.h"
#include "power.h"
#include "gsm.h"
#include "gps.h"
#include "traccar.h"
#include "cmd_serial.h"
#include "serial_controller.h"
#include "base_serial_handlers.h"
#include "injection.h"
#include "car_status.h"

void setup();
void loop();
void carAfterLoop();
void carBeforeSetup();
void carAfterSetup();

CONFIGURATION CONFIG;

#if COMPILE_CAN
	void carAfterCmdReceived(CAN_CONTROLLER *can, CAN_COMMAND *receivedCmd);
	CAN_CONTROLLER DEFAULT_CAN	= CAN_CONTROLLER(CAN_A_CS_PIN, CAN_A_INT_PIN, carAfterCmdReceived);
#endif

void setup()
{	
	setupPOW();
	setupCMDSerial();

	#if COMPILE_EEPROM_CONFIG	
		if (!eepromLoad(&CONFIG))
		{
			CONFIG.setDefaults();
			eepromSave(&CONFIG);
		}
	#endif

	carBeforeSetup(); //CAR SETUP
     
	#if COMPILE_POTENTIOMETER
		if (CONFIG.get(PRINT_STATUS_INFO))
		{			
			cmdSerialPrint_P(sPOT);
			cmdSerialPrint_P(sSpace);

			if (setupPOT()) cmdSerialPrintln_P(sOK);
			else cmdSerialPrintln_P(sERROR);
		}
		else setupPOT();
	#endif

	#if COMPILE_GPS
		if (CONFIG.get(PRINT_STATUS_INFO))
		{			
			utilsPrint_P(sGPS);
			utilsPrint_P(sSpace);

			if (setupGPS()) utilsPrintln_P(sOK);
			else utilsPrintln_P(sERROR);
		}
		else setupGPS();
	#endif  
  
	#if COMPILE_GSM_MODEM
		if (CONFIG.get(PRINT_STATUS_INFO))
		{
			utilsPrint_P(sGSM);
			utilsPrint_P(sSpace);

			if (setupGSM()) utilsPrintln_P(sOK);
			else utilsPrintln_P(sERROR);
		}
		else setupGSM();
	#endif
  
	#if COMPILE_TRACCAR
		if (CONFIG.get(PRINT_STATUS_INFO))
		{
			utilsPrint_P(sTRC);
			utilsPrint_P(sSpace);

			if (setupTraccar()) utilsPrintln_P(sOK);
			else utilsPrintln_P(sERROR);
		}
		else setupTraccar();
	#endif
  
	#if COMPILE_CAN   
		DEFAULT_CAN.setupPins();
		if (CONFIG.get(PRINT_STATUS_INFO))
		{
			cmdSerialPrint_P(sCAN);
			cmdSerialPrint_P(sSpace);
		}
		
		if (!DEFAULT_CAN.begin(CAN_BAUDRATE, CAN_CLOCKSET))
		{
			if (CONFIG.get(PRINT_STATUS_INFO))
			{
				cmdSerialPrint_P(sERROR);
				cmdSerialFlush();
			}

			utilsResetSystem();
		}
		else if (CONFIG.get(PRINT_STATUS_INFO))
			cmdSerialPrintln_P(sOK);
	#endif
      
	#if COMPILE_SERIAL_CONTROL    
		#if SERIAL_CONTROL_MODE >= SC_MIN
			SerialController.addHandler(new SERIAL_HANDLER(sSYS, SYSHandleConfCmd));
       
			#if COMPILE_EEPROM_CONFIG
				SerialController.addHandler(new SERIAL_HANDLER(sLOG, LOGHandleConfCmd));
				SerialController.addHandler(new SERIAL_HANDLER(sOPT, OPTHandleConfCmd));
				SerialController.addHandler(new SERIAL_HANDLER(sROM, ROMHandleConfCmd));
			#endif
		#endif

		#if SERIAL_CONTROL_MODE >= SC_MID
			#if COMPILE_CAN
				SerialController.addHandler(new SERIAL_HANDLER(sCAN, CANHandleConfCmd));
			#endif
		#endif
	#endif
	
	#if ENABLE_INJECTION
		setupInjection();
	#endif

	carAfterSetup(); //CAR SETUP 
}

#if COMPILE_DEBUG
	static unsigned long prev = 0;
	void prePrint()
	{
		prev = micros();
	}

	void postPrint(const PROGMEM char *type)
	{
		cmdSerial.print(micros() - prev);
		utilsPrint_P(sSpaceCommaSpace);
		utilsPrintln_P(type);
	}
#endif

void loop()
{
	#if COMPILE_DEBUG
		if (CONFIG.get(PRINT_DEBUG))
		{
			prePrint();
			checkPOW();
			postPrint(sPOW);

			#if COMPILE_CAN
				prePrint();
				DEFAULT_CAN.checkCAN();
				postPrint(sCAN);
			#endif

			#if COMPILE_TRACCAR
				prePrint();
				checkTraccar();
				postPrint(sTRC);
			#endif

			#if COMPILE_GSM_MODEM
				prePrint();
				checkGSM();   
				postPrint(sGSM);
			#endif

			#if COMPILE_POTENTIOMETER
				prePrint();
				checkPOT();
				postPrint(sPOT);
			#endif
  
			#if COMPILE_CAR_STATUS_MONITOR
				prePrint();
				checkCarStatus();
				postPrint(sSTA);
			#endif

			#if COMPILE_SERIAL_CONTROL
				prePrint();
				checkCMDSerial();	
				postPrint(PSTR("CMD"));
			#endif

			#if ENABLE_INJECTION
				prePrint();
				checkInjection();		
				postPrint(PSTR("INJ"));
			#endif

			prePrint();
			carAfterLoop();
			postPrint(sCAR);
		}
		else
	#endif
	{
		checkPOW();

		#if COMPILE_CAN			
			DEFAULT_CAN.checkCAN();			
		#endif

		#if COMPILE_TRACCAR		
			checkTraccar();			
		#endif

		#if COMPILE_GSM_MODEM			
			checkGSM();   			
		#endif

		#if COMPILE_POTENTIOMETER			
			checkPOT();			
		#endif
  
		#if COMPILE_CAR_STATUS_MONITOR			
			checkCarStatus();			
		#endif

		#if COMPILE_SERIAL_CONTROL			
			#ifdef CMD_SERIAL_MODE
				checkCMDSerial();				
			#endif	
		#endif

		#if ENABLE_INJECTION			
			checkInjection();					
		#endif
  		
		carAfterLoop();
	}
}
