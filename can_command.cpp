//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#include "can_command.h"
#include "cmd_serial.h"

#pragma region CONSTRUCTOR
CAN_COMMAND::CAN_COMMAND()
{

}

CAN_COMMAND::CAN_COMMAND(canid_t id)
{
	this->can_id = id;
}

CAN_COMMAND::CAN_COMMAND(canid_t id, uint8_t data0) : CAN_COMMAND(id)
{
	this->can_dlc = 1;
	this->data[0] = data0;
}

CAN_COMMAND::CAN_COMMAND(canid_t id, uint8_t data0, uint8_t data1) : CAN_COMMAND(id)
{
	this->can_dlc = 2;
	this->data[0] = data0;
	this->data[1] = data1;
}

CAN_COMMAND::CAN_COMMAND(canid_t id, uint8_t data0, uint8_t data1, uint8_t data2) : CAN_COMMAND(id)
{
	this->can_dlc = 3;
	this->data[0] = data0;
	this->data[1] = data1;
	this->data[2] = data2;
}

CAN_COMMAND::CAN_COMMAND(canid_t id, uint8_t data0, uint8_t data1, uint8_t data2, uint8_t data3) : CAN_COMMAND(id)
{
	this->can_dlc = 4;
	this->data[0] = data0;
	this->data[1] = data1;
	this->data[2] = data2;
	this->data[3] = data3;
}

CAN_COMMAND::CAN_COMMAND(canid_t id, uint8_t data0, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4) : CAN_COMMAND(id)
{
	this->can_dlc = 5;
	this->data[0] = data0;
	this->data[1] = data1;
	this->data[2] = data2;
	this->data[3] = data3;
	this->data[4] = data4;
}

CAN_COMMAND::CAN_COMMAND(canid_t id, uint8_t data0, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4, uint8_t data5) : CAN_COMMAND(id)
{
	this->can_dlc = 6;
	this->data[0] = data0;
	this->data[1] = data1;
	this->data[2] = data2;
	this->data[3] = data3;
	this->data[4] = data4;
	this->data[5] = data5;
}

CAN_COMMAND::CAN_COMMAND(canid_t id, uint8_t data0, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4, uint8_t data5, uint8_t data6) : CAN_COMMAND(id)
{
	this->can_dlc = 7;
	this->data[0] = data0;
	this->data[1] = data1;
	this->data[2] = data2;
	this->data[3] = data3;
	this->data[4] = data4;
	this->data[5] = data5;
	this->data[6] = data6;
}

CAN_COMMAND::CAN_COMMAND(canid_t id, uint8_t data0, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4, uint8_t data5, uint8_t data6, uint8_t data7) : CAN_COMMAND(id)
{
	this->can_dlc = 8;
	this->data[0] = data0;
	this->data[1] = data1;
	this->data[2] = data2;
	this->data[3] = data3;
	this->data[4] = data4;
	this->data[5] = data5;
	this->data[6] = data6;
	this->data[7] = data7;
}

CAN_COMMAND::CAN_COMMAND(const CAN_COMMAND *cmd)
{
	this->can_id = cmd->can_id;
	this->can_dlc = cmd->can_dlc;
	memcpy(this->data, cmd->data, cmd->can_dlc);
}
#pragma endregion

void CAN_COMMAND::print() const
{     	
	cmdSerialPrint(millis());
	cmdSerialPrint_P(sSpace);
	cmdSerialPrint((int)this->can_dlc);
	cmdSerialPrint_P(sSpace);
	cmdSerialPrint(this->can_id, HEX);
	cmdSerialPrint_P(sSpaceCommaSpace);

	for(uint8_t i = 0; i < this->can_dlc; i++)
	{		
		cmdSerialPrint(this->data[i], HEX);		
		cmdSerialPrint_P(sComma);
	}

	cmdSerialPrintln();
	cmdSerialFlush();
}

void CAN_COMMAND::print(const char *prefix) const
{
	if (prefix != NULL)
	{
		cmdSerialPrint(prefix);
		cmdSerialPrint_P(sTwoDots);
	}

	this->print();
}

void CAN_COMMAND::print_P(const PROGMEM char *prefix) const
{
	if (prefix != NULL)
	{
		cmdSerialPrint_P(prefix);
		cmdSerialPrint_P(sTwoDots);
	}

	this->print();
}

bool CAN_COMMAND::isPrefix(CAN_COMMAND *cmd) const
{
	return (this->can_id == cmd->can_id && memcmp(this->data, cmd->data, sizeof(uint8_t) * this->can_dlc) == 0);
}

bool CAN_COMMAND::equals(CAN_COMMAND *cmd) const
{
	return (this->can_id == cmd->can_id && this->can_dlc == cmd->can_dlc && memcmp(this->data, cmd->data, sizeof(uint8_t) * this->can_dlc) == 0);
}

uint16_t CAN_COMMAND::hashCode() const
{
	return this->hashCode(0, this->can_dlc);
}


uint16_t CAN_COMMAND::hashCode(uint8_t firstBufPos, uint8_t lastBufPos) const
{
	uint16_t res = 0;
	int8_t i = (lastBufPos < this->can_dlc ? lastBufPos : this->can_dlc - 1);

	for (uint8_t shift = 0; i >= 0; shift += 8, i--)	
		res |= (data[i] << shift);

	return res;
}