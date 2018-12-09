#ifndef __CAN_COMMAND__H__
	#define __CAN_COMMAND__H__
  
	#include "customArduino.h"
	#include "configuration.h"
	#include "utils.h"
	#include "lib/can/can.h"

	class CAN_COMMAND : public can_frame
	{
		public:
			CAN_COMMAND();
			CAN_COMMAND(const CAN_COMMAND *cmd);

			CAN_COMMAND(canid_t id);
			CAN_COMMAND(canid_t id, uint8_t data0);
			CAN_COMMAND(canid_t id, uint8_t data0, uint8_t data1);
			CAN_COMMAND(canid_t id, uint8_t data0, uint8_t data1, uint8_t data2);
			CAN_COMMAND(canid_t id, uint8_t data0, uint8_t data1, uint8_t data2, uint8_t data3);
			CAN_COMMAND(canid_t id, uint8_t data0, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4);
			CAN_COMMAND(canid_t id, uint8_t data0, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4, uint8_t data5);
			CAN_COMMAND(canid_t id, uint8_t data0, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4, uint8_t data5, uint8_t data6);
			CAN_COMMAND(canid_t id, uint8_t data0, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4, uint8_t data5, uint8_t data6, uint8_t data7);

			void print(Stream *s = NULL) const;
			void print(const char *prefix, Stream *s = NULL) const;
			void print_P(const PROGMEM char *prefix, Stream *s = NULL) const;
      
			bool isPrefix(CAN_COMMAND *cmd) const;
			bool equals(CAN_COMMAND *cmd) const;

			uint16_t hashCode() const;
			uint16_t hashCode(uint8_t firstBufPos, uint8_t lastBufPos) const;
	};
    
	typedef struct
	{    
		CAN_COMMAND  *cmd;       
		uint16_t interval;    
		unsigned long nextMillis;
   
	} CAN_COMMAND_PERIODICAL;
  
#endif


