//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#ifndef __POTENTIOMETER__H__
	#define __POTENTIOMETER__H__

	#include "customArduino.h"
	#include <SPI.h>
	#include "lib/others/FastGPIO.h"	
	#include "configuration.h"
	#include "utils.h"	
  
	#if BOARD_VERSION == 42
		#define OPTO_ENABLED		HIGH
		#define OPTO_DISABLED		LOW    
	#else
		#define OPTO_ENABLED		LOW
		#define OPTO_DISABLED		HIGH    
	#endif

	#define MCP4131_CS_ENABLED		LOW
	#define MCP4131_CS_DISABLED		HIGH

	//MCP4131       
	#define MCP4131_WIPER0_ADDRESS		0x00  
	#define MCP4131_TCON_ADDRESS		0x40
	#define MCP4131_POT_STEPS_COUNT		129  	
	#define MCP4131_TCON_ENABLED		0xBB
	#define MCP4131_TCON_DISABLED		0x88
	#define MCP4131_STEP(A)				((((A * 1000.0f) - POT_BASE_OHM) / POT_STEP_OHM) + 0.5f)
	#define MCP4131_DEFAULT_STEP		MCP4131_STEP(100.0f)

	void potDisableResistor();    
	void potSetResistor(uint8_t step, const uint16_t interval = 0, bool ring = false);
	void checkPOT();
	bool setupPOT();
#endif
