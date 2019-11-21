//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#include "serial_controller.h"
#include "utils.h"

SERIAL_CONTROLLER SerialController;

SERIAL_CONTROLLER::SERIAL_CONTROLLER()
{
	this->handlersCount = 0;  
}

bool SERIAL_CONTROLLER::addHandler(SERIAL_HANDLER *h)
{
	if (this->handlersCount >= MAX_SERIAL_HANDLERS)
		return false;

	this->handlers[this->handlersCount++] = h;
	return true;
}  

void SERIAL_CONTROLLER::clearAllHandlers()
{                
	for(uint8_t i = 0; i < this->handlersCount; i++)
		delete this->handlers[i];
  	
	this->handlersCount = 0;  
}

#include "cmd_serial.h"
/**
 * Returns true if the message is handled
 */
bool SERIAL_CONTROLLER::handleMessage(Stream *serial, String *in)
{
	return this->handleMessage(serial, (char*)in->c_str(), in->length());
}

bool SERIAL_CONTROLLER::handleMessage(Stream *serial, char *in, uint8_t len)
{      
	uint8_t sepPos = utilsFirstIndexOf(in, ":");
	if(sepPos < 0)
		return false;
   
	uint8_t parSPos = utilsFirstIndexOf(in, "(");
	uint8_t parEPos = len - (in[len-1] == ';' ? 2 : 1);
	
	in[parSPos] = '\0';
	in[parEPos] = '\0';

	char *cmd = &in[sepPos + 1];	
	char *paramsStr = &in[parSPos + 1];

	#pragma region SPLIT PARAMETERS
		char *params[MAX_PARAMETERS]; //MAX PARAMS
		params[0] = paramsStr; //FIRST PARAM
		uint8_t paramsLength = (paramsStr[0] == '\0' ? 0 : 1);

		for (uint8_t i = 0; paramsLength < MAX_PARAMETERS; i++)
		{
			char c = paramsStr[i];
			if (c == '\0')
				break;

			if (c == ';')
			{
				paramsStr[i] = '\0';
				params[paramsLength++] = &paramsStr[i + 1];
			}
		}
	#pragma endregion

	for(uint8_t i = 0; i < this->handlersCount; i++)
	{
		if (strncmp_P(in, this->handlers[i]->type, sepPos) == 0)		
			if (this->handlers[i]->handler(serial, cmd, params, paramsLength) == true)
				return true;	
	}

	return false;
}
