//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#include "cmd_serial.h"
#include "serial_controller.h"

static CMD_SERIAL *cmdSerials[CMD_SERIALS_LENGTH];

bool setupCMDSerial()
{
	uint8_t index = 0;

	#if CMD_SERIAL_AVAILABLE
		cmdSerials[index] = new CMD_SERIAL(DEFAULT_BAUDRATE, CMD_SERIAL_MODE);
		cmdSerials[index++]->setup();
	#endif

	#if CMD_ALT_SERIAL_AVAILABLE
		cmdSerials[index] = new CMD_SERIAL(DEFAULT_BAUDRATE, CMD_ALT_SERIAL_MODE);
		cmdSerials[index++]->setup();
	#endif
}

void checkCMDSerial()
{
	for (uint8_t i = 0; i < CMD_SERIALS_LENGTH; i++)
		cmdSerials[i]->check();
}


#pragma region //******* SERIAL PRINT / FLUSH ********// 

void cmdSerialFlush()
{
	for (uint8_t i = 0; i < CMD_SERIALS_LENGTH; i++)
		cmdSerialFlush(cmdSerials[i]->serial);
}

void cmdSerialFlush(Stream *o)
{
	o->flush();
}

void cmdSerialPrint(int value)
{
	for (uint8_t i = 0; i < CMD_SERIALS_LENGTH; i++)
		cmdSerialPrint(cmdSerials[i]->serial, value);
}

void cmdSerialPrint(Stream *o, int value)
{
	o->print(value);
}

void cmdSerialPrint(int value, int base)
{
	for (uint8_t i = 0; i < CMD_SERIALS_LENGTH; i++)
		cmdSerialPrint(cmdSerials[i]->serial, value, base);
}

void cmdSerialPrint(Stream *o, int value, int base)
{
	o->print(value, base);
}

void cmdSerialPrint(float value)
{
	for (uint8_t i = 0; i < CMD_SERIALS_LENGTH; i++)
		cmdSerialPrint(cmdSerials[i]->serial, value);
}

void cmdSerialPrint(Stream *o, float value)
{
	o->print(value);
}

void cmdSerialPrint(unsigned long value)
{
	for (uint8_t i = 0; i < CMD_SERIALS_LENGTH; i++)
		cmdSerialPrint(cmdSerials[i]->serial, value);
}

void cmdSerialPrint(Stream *o, unsigned long value)
{
	o->print(value);
}

void cmdSerialPrint_P(Stream *o, const PROGMEM char *text)
{
	o->print((__FlashStringHelper*)text);
}

void cmdSerialPrintln_P(Stream *o, const PROGMEM char *text)
{
	o->println((__FlashStringHelper*)text);
}

void cmdSerialPrint(Stream *o, const char *text)
{
	o->print(text);
}

void cmdSerialPrintln(Stream *o, const char *text)
{
	o->println(text);
}

void cmdSerialPrintln()
{
	for (uint8_t i = 0; i < CMD_SERIALS_LENGTH; i++)
		cmdSerialPrintln(cmdSerials[i]->serial);
}

void cmdSerialPrintln(Stream *o)
{
	o->println();
}

void cmdSerialPrintln(const char *text)
{
	for(uint8_t i = 0; i < CMD_SERIALS_LENGTH; i++)
		cmdSerialPrintln(cmdSerials[i]->serial, text);
}

void cmdSerialPrint(const char *text)
{
	for (uint8_t i = 0; i < CMD_SERIALS_LENGTH; i++)
		cmdSerialPrint(cmdSerials[i]->serial, text);
}

void cmdSerialPrint_P(const PROGMEM char *text)
{
	for (uint8_t i = 0; i < CMD_SERIALS_LENGTH; i++)
		cmdSerialPrint_P(cmdSerials[i]->serial, text);
}

void cmdSerialPrintln_P(const PROGMEM char *text)
{
	for (uint8_t i = 0; i < CMD_SERIALS_LENGTH; i++)
		cmdSerialPrintln_P(cmdSerials[i]->serial, text);
}
#pragma endregion

#pragma region //******* CLASS CMD_SERIAL ********// 
CMD_SERIAL::CMD_SERIAL(uint32_t baudrate, uint8_t type) : CMD_SERIAL(baudrate, type, 0, 0)
{

}

CMD_SERIAL::CMD_SERIAL(uint32_t baudrate, uint8_t type, uint8_t rxPin, uint8_t txPin)
{
	this->isInputStringValid = false;
	this->inputStringLength = 0;
	this->baudrate = baudrate;
	this->type = type;

	#if USE_STATIC_MEM == 0
		this->inputString = new char[CMD_SERIAL_BUFFER_SIZE];
	#endif

	switch (type)
	{
		#ifdef HW_SERIAL_AVAILABLE
		case HW_SERIAL:
			this->serial = &NeoSerial;
			break;
		#endif	

		#ifdef IC_SERIAL_AVAILABLE
		case IC_SERIAL:
			this->serial = new NeoICSerial();;
			break;
		#endif	

		#ifdef SW_SERIAL_AVAILABLE
		case SW_SERIAL:
			this->serial = new NeoSWSerial(rxPin, txPin);
			break;
		#endif	

		default:
			this->serial = NULL;
	}
}

CMD_SERIAL::~CMD_SERIAL()
{
	if (this->type == IC_SERIAL || this->type == SW_SERIAL)
		delete this->serial;

	#if USE_STATIC_MEM == 0
		delete this->inputString;
	#endif
}

void handleSerial_ISR(uint8_t c, void *isr_param)
{
	CMD_SERIAL *obj = (CMD_SERIAL*)isr_param;

	if (obj->isInputStringValid)
		return;

	if (obj->inputStringLength >= (CMD_SERIAL_BUFFER_SIZE - 1))
		obj->inputStringLength = 0;

	if (c == '\n')
		obj->isInputStringValid = true;
	else if (c != '\r') //IGNORE \r
		obj->inputString[obj->inputStringLength++] = c;
}

bool CMD_SERIAL::setup()
{
	#if CMD_USE_SERIAL_INTERRUPTS	
		#ifdef HW_SERIAL_AVAILABLE
			if (this->type == HW_SERIAL)	
			{
				((NeoHWSerial*)this->serial)->attachInterrupt(handleSerial_ISR, this);
				((NeoHWSerial*)this->serial)->begin(this->baudrate);
			}
			else
		#endif
		
		#ifdef IC_SERIAL_AVAILABLE
			if(this->type == IC_SERIAL)
			{
				((NeoICSerial*)this->serial)->attachInterrupt(handleSerial_ISR, this);
				((NeoICSerial*)this->serial)->begin(this->baudrate);
			}
			else 
		#endif
				
		#ifdef SW_SERIAL_AVAILABLE
			if(this->type == SW_SERIAL)
			{
				((NeoSWSerial*)this->serial)->attachInterrupt(handleSerial_ISR, this);
				((NeoSWSerial*)this->serial)->begin(this->baudrate);
			}
			else 
		#endif
			return false;		
	#endif

	return true;
}

void CMD_SERIAL::check()
{	
	#if CMD_USE_SERIAL_INTERRUPTS == 0
		while (this->serial->available())
			handleSerial_ISR(this->serial->read(), this);
	#endif

	if (this->isInputStringValid)
	{
		this->inputString[this->inputStringLength] = '\0';
		if (this->inputStringLength > 0 && this->inputString[0] != '\r')
			SerialController.handleMessage(this->serial, this->inputString, this->inputStringLength);

		this->inputStringLength = 0;
		this->isInputStringValid = false;
	}
}
#pragma endregion