//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#ifndef __ASTRA_H_TEST_MODE__H__
	#define __ASTRA_H_TEST_MODE__H__

	#include "../../configuration.h"
	#include "astra_h.h"

	#if COMPILE_CAN && COMPILE_TEST_MODE
		extern bool astraHTestModeEnabled;
		extern bool astraHTestModeEnabling;

		bool astraHTestModeSetup();
		void astraHTestModeTryEnable()  __attribute__((always_inline));
		void astraHTestModeStep();
	#endif
#endif
