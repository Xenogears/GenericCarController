//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#include "configuration.h"
#include "serial_handler.h"

#ifndef __SERIAL_CONTROLLER__H__
	#define __SERIAL_CONTROLLER__H__

	#include "customArduino.h"
  
	class SERIAL_CONTROLLER
	{
		private:
			SERIAL_HANDLER *handlers[MAX_SERIAL_HANDLERS];			
			uint8_t handlersCount;
  
		public:
			SERIAL_CONTROLLER();
      
			bool addHandler(SERIAL_HANDLER *h);
			void clearAllHandlers();
			bool handleMessage(Stream *serial, String *in);
			bool handleMessage(Stream *serial, char *in, uint8_t len);
	};
  
	extern SERIAL_CONTROLLER SerialController;

#endif
