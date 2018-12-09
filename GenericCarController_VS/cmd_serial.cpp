#include "cmd_serial.h"
#include "serial_controller.h"

#if CMD_SERIAL_MODE == SW_SERIAL
	NeoSWSerial cmdSerial(CMD_RX_PIN, CMD_TX_PIN);
#elif CMD_SERIAL_MODE == IC_SERIAL		
	NeoICSerial cmdSerial;	
#endif     

	static volatile bool cmdIsInputStringValid = false;
	static char cmdInputString[CMD_SERIAL_BUFFER_SIZE];
	static uint8_t cmdInputStringLength = 0;

	static void cmdHandleSerial_ISR(uint8_t c)
	{
		if (cmdIsInputStringValid)
			return;

		if (cmdInputStringLength >= (CMD_SERIAL_BUFFER_SIZE - 1))
			cmdInputStringLength = 0;

		if (c == '\n')
			cmdIsInputStringValid = true;
		else if (c != '\r') //IGNORE \r
			cmdInputString[cmdInputStringLength++] = c;
	}

void checkCMDSerial()
{
	#if CMD_USE_SERIAL_INTERRUPTS == 0
		while(cmdSerial.available())		
			cmdHandleSerial_ISR(cmdSerial.read());		
	#endif

	if (cmdIsInputStringValid)
	{
		cmdInputString[cmdInputStringLength] = '\0';
		if (cmdInputStringLength > 0 && cmdInputString[0] != '\r')
			SerialController.handleMessage(cmdInputString, cmdInputStringLength);

		cmdInputStringLength = 0;
		cmdIsInputStringValid = false;
	}
}

bool setupCMDSerial()
{
	#if CMD_USE_SERIAL_INTERRUPTS
		cmdSerial.attachInterrupt(cmdHandleSerial_ISR);
	#endif

	cmdSerial.begin(DEFAULT_BAUDRATE);	
	return true;
}