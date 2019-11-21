#ifndef __MCP_CAN_CUSTOM__H__
	#define __MCP_CAN_CUSTOM__H__

	#include "mcp_can.h"
	#include "../../configuration.h"

	class MCP_CAN_CUSTOM : public MCP_CAN
	{
		private:			
			#if COMPILE_CAN_INTERRUPT
				int8_t interruptID;
			#endif

		public:
			#if COMPILE_CAN_INTERRUPT
				MCP_CAN_CUSTOM(uint8_t _CS = 0, int8_t interruptID = NOT_AN_INTERRUPT);
			#else
				using MCP_CAN::MCP_CAN;
			#endif

			byte init_Filt(byte num, byte ext, unsigned long ulData);

			void SPI_BEGIN(void);
	};

#endif