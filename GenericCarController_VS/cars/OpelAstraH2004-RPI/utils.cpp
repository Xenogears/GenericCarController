//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#include "utils.h"
#include "../../cmd_serial.h"

#if CAR_MODEL == OPEL_ASTRA_H_2004_MIN_CAR 
	//////////////////////// COMMAND HELPER ////////////////////////	
	void astraHSendCommand(const char type, const char *cmd, char *params[] = NULL, uint8_t paramsLength = 0, Stream *s = NULL, bool autoFlush = true)
	{
		if (s == NULL)
			s = &cmdSerial;

		#if CONFIG_COMMAND_FORMAT == COMMAND_FORMAT_TEXT
			s->print(type);
			s->print(":");
			s->print(cmd);
			
			if (paramsLength > 0)
			{
				s->print('(');

				for (uint8_t i = 0; i < paramsLength; i++)
				{
					s->print(params[i]);

					if (i < (paramsLength - 1))
						s->print(";");
				}

				s->println(");");
			}
			else s->println(';');
		#else
			#error "Not implemented yet";
		#endif

		if (autoFlush)
			s->flush();
	}
#endif