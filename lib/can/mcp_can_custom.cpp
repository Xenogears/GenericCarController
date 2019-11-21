#include "mcp_can_custom.h"

#if COMPILE_CAN_INTERRUPT
	MCP_CAN_CUSTOM::MCP_CAN_CUSTOM(uint8_t _CS = 0, int8_t interruptID = NOT_AN_INTERRUPT) : MCP_CAN::MCP_CAN(_CS), interruptID(interruptID)
	{
	}
#endif

byte MCP_CAN_CUSTOM::init_Filt(byte num, byte ext, unsigned long ulData)
{
	uint8_t reg;
	if (num == 0)reg = MCP_RXF0SIDH;
	else if (num == 1)reg = MCP_RXF1SIDH;
	else if (num == 2)reg = MCP_RXF2SIDH;
	else if (num == 3)reg = MCP_RXF3SIDH;
	else if (num == 4)reg = MCP_RXF4SIDH;
	else if (num == 5)reg = MCP_RXF5SIDH;
	else return MCP2515_FAIL;

	if (this->mcp2515_setCANCTRL_Mode(MODE_CONFIG) != MCP2515_OK)
	{
		delay(10); //?
		return MCP2515_FAIL;
	}

	this->mcp2515_write_id(reg, ext, ulData);

	byte rExt;
	unsigned long rUlData;
	this->mcp2515_read_id(reg, &rExt, &rUlData);

	uint8_t res = this->mcp2515_setCANCTRL_Mode(MODE_NORMAL);
	delay(10);

	if (res != MCP2515_OK || ulData != rUlData)
		return MCP2515_FAIL;
	else 
		return MCP2515_OK;
}

void MCP_CAN_CUSTOM::SPI_BEGIN(void)
{
	#if COMPILE_CAN_INTERRUPT
		pSPI->usingInterrupt(this->interruptID);
	#endif

	pSPI->beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
}