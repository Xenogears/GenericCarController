//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#include "potentiometer.h"
#include "cmd_serial.h"

static bool potTimeoutEnabled = false;
static unsigned long potTimeout = 0;

static void potDigitalWrite(uint8_t value, uint8_t tcon);

#define SPI_BEGIN()		SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0))
#define SPI_END()       SPI.endTransaction()

//PUBLIC FUNCTIONS
void potDisableResistor()
{    
	potTimeoutEnabled = false;

	#if RADIO_USE_TIP_OPTOCOUPLERS		
		FastGPIO::Pin<TIP_CONNECTION_PIN>::setOutputValue(OPTO_DISABLED);
	#endif

	FastGPIO::Pin<RING_CONNECTION_PIN>::setOutput(OPTO_DISABLED);

	potDigitalWrite(MCP4131_DEFAULT_STEP, MCP4131_TCON_DISABLED);
	
    if(CONFIG.get(PRINT_POTENTIOMETER_INFO))
		cmdSerial.println(F("P-"));
}

bool setupPOT()
{  
	#if RADIO_USE_TIP_OPTOCOUPLERS
		FastGPIO::Pin<TIP_CONNECTION_PIN>::setOutput(OPTO_DISABLED);
	#endif
	
	FastGPIO::Pin<POT_CS_PIN>::setOutput(MCP4131_CS_DISABLED);
	FastGPIO::Pin<RING_CONNECTION_PIN>::setOutput(OPTO_DISABLED);

	SPI.begin();
	potDigitalWrite(MCP4131_DEFAULT_STEP, MCP4131_TCON_DISABLED);

	return true;
}

void potSetResistor(uint8_t step, const uint16_t interval = 0, bool ring = false)
{
	if(CONFIG.get(PRINT_POTENTIOMETER_INFO))
	{        
		utilsPrint_P(sPOT);
		utilsPrint_P(sTwoDots);
		cmdSerial.print(step);

		#if POT_SHOW_KOHMS
			utilsPrint_P(sComma);
			cmdSerial.print(((step * POT_STEP_OHM) + POT_BASE_OHM) / 1000.0f);
		#endif

		if (ring)
		{
			utilsPrint_P(sComma);
			utilsPrintln_P(sWithRing);
		}
		else cmdSerial.println();
	}

	potDigitalWrite(step, MCP4131_TCON_ENABLED);

	FastGPIO::Pin<RING_CONNECTION_PIN>::setOutputValue((ring ? OPTO_ENABLED : OPTO_DISABLED));

	#if RADIO_USE_TIP_OPTOCOUPLERS
		FastGPIO::Pin<TIP_CONNECTION_PIN>::setOutputValue(OPTO_ENABLED);
	#endif

	if(interval > 0)
	{
		potTimeout = (millis() + interval);
		potTimeoutEnabled = true;
	}
	else potTimeoutEnabled = false;
}  

void checkPOT()
{
	if(!potTimeoutEnabled)
		return;

	if (millis() >= potTimeout)
		potDisableResistor();
}


//PRIVATE FUNCTIONS
static void potDigitalWrite(uint8_t value, uint8_t tcon)
{
	SPI_BEGIN();

	FastGPIO::Pin<POT_CS_PIN>::setOutput(MCP4131_CS_ENABLED);
	
	SPI.transfer16((MCP4131_WIPER0_ADDRESS << 8) | value);
	SPI.transfer16((MCP4131_TCON_ADDRESS << 8) | tcon);	

	FastGPIO::Pin<POT_CS_PIN>::setOutput(MCP4131_CS_DISABLED);

	SPI_END();
}