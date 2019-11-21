//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#ifndef __CMD_SERIAL__H__
	#define __CMD_SERIAL__H__

	#include "configuration.h"

	#if CMD_SERIAL_MODE == HW_SERIAL || CMD_ALT_SERIAL_MODE == HW_SERIAL
		#include "lib/serial/NeoHWSerial.h"   		
		#define HW_SERIAL_AVAILABLE
	#endif	
	#if CMD_SERIAL_MODE == IC_SERIAL || CMD_ALT_SERIAL_MODE == IC_SERIAL
		#include "lib/serial/NeoICSerial.h"    
		#define IC_SERIAL_AVAILABLE
	#endif
	#if CMD_SERIAL_MODE == SW_SERIAL || CMD_ALT_SERIAL_MODE == SW_SERIAL
		#include "lib/serial/NeoSWSerial.h"    
		#define SW_SERIAL_AVAILABLE
	#endif        	

	#ifdef CMD_SERIAL_MODE
		#define CMD_SERIAL_AVAILABLE 1
	#else
		#define CMD_SERIAL_AVAILABLE 0
	#endif

	#ifdef CMD_ALT_SERIAL_MODE
		#define CMD_ALT_SERIAL_AVAILABLE 1
	#else
		#define CMD_ALT_SERIAL_AVAILABLE 0
	#endif

	#define CMD_SERIALS_LENGTH (CMD_SERIAL_AVAILABLE + CMD_ALT_SERIAL_AVAILABLE)

	void handleSerial_ISR(uint8_t c, void *isr_param);
	bool setupCMDSerial();
	void checkCMDSerial();

	class CMD_SERIAL
	{
		private: 
			volatile bool isInputStringValid;
			uint8_t type;
			uint32_t baudrate;

			#if USE_STATIC_MEM 
				char inputString[CMD_SERIAL_BUFFER_SIZE];
			#else
				char *inputString;
			#endif
			uint8_t inputStringLength = 0;

		public:
			CMD_SERIAL(uint32_t baudrate, uint8_t type);
			CMD_SERIAL(uint32_t baudrate, uint8_t type, uint8_t rxPin, uint8_t txPin);
			~CMD_SERIAL();
			bool setup();
			void check();	

			Stream *serial;

			friend void handleSerial_ISR(uint8_t c, void *isr_param);
	};

	void cmdSerialFlush();
	void cmdSerialFlush(Stream *o);

	void cmdSerialPrint(int value);
	void cmdSerialPrint(Stream *o, int value);
	void cmdSerialPrint(int value, int base);
	void cmdSerialPrint(Stream *o, int value, int base);
	void cmdSerialPrint(float value);
	void cmdSerialPrint(Stream *o, float value);
	void cmdSerialPrint(unsigned long value);
	void cmdSerialPrint(Stream *o, unsigned long value);

	void cmdSerialPrint(const char *text);
	void cmdSerialPrint(Stream *o, const char *text);

	void cmdSerialPrintln();
	void cmdSerialPrintln(Stream *o);
	void cmdSerialPrintln(const char *text);
	void cmdSerialPrintln(Stream *o, const char *text);

	void cmdSerialPrint_P(const PROGMEM char *text);
	void cmdSerialPrint_P(Stream *o, const PROGMEM char *text);

	void cmdSerialPrintln_P(const PROGMEM char *text);
	void cmdSerialPrintln_P(Stream *o, const PROGMEM char *text);
#endif
