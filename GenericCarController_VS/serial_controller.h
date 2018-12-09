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
			bool handleMessage(String *in);
			bool handleMessage(char *in, uint8_t len);
	};
  
	extern SERIAL_CONTROLLER SerialController;

#endif
