//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#include "radio_command.h"

const RADIO_COMMAND RADIO_CMD_SOURCE          { MCP4131_STEP(1.2f),	  false}; //Source/Off
const RADIO_COMMAND RADIO_CMD_MUTE            { MCP4131_STEP(3.5),	  false}; //Mute/ATT
const RADIO_COMMAND RADIO_CMD_DISPLAY         { MCP4131_STEP(5.75f),  false}; //Display
const RADIO_COMMAND RADIO_CMD_NEXT_TRACK      { MCP4131_STEP(8.0f),	  false}; //Next Track
const RADIO_COMMAND RADIO_CMD_PREV_TRACK      { MCP4131_STEP(11.25f), false}; //Prev Track
const RADIO_COMMAND RADIO_CMD_VOLUME_UP       { MCP4131_STEP(16.0f),  false}; //Volume Up
const RADIO_COMMAND RADIO_CMD_VOLUME_DOWN     { MCP4131_STEP(25.0f),  false}; //Volume Down
const RADIO_COMMAND RADIO_CMD_BAND_ESCAPE     { MCP4131_STEP(62.75f), false}; //Band/Escape
const RADIO_COMMAND RADIO_CMD_FOLDER_UP       { MCP4131_STEP(8.0f),	  true};  //Folder Up
const RADIO_COMMAND RADIO_CMD_FOLDER_DOWN     { MCP4131_STEP(11.25f), true};  //Folder Down
const RADIO_COMMAND RADIO_CMD_VOICE_CONTROL   { MCP4131_STEP(62.75f), true};  //Voice control

void radEnableCmd(const RADIO_COMMAND *radioCmd, const uint16_t interval = 0)
{
    potSetResistor(radioCmd->potStep, interval, radioCmd->ringEnabled);           
}

void radDisableCmd()
{		
	potDisableResistor();
}

/*
RADIO_COMMAND::RADIO_COMMAND(const uint8_t p, const bool r) : potStep(p), ringEnabled(r)
{

}

void RADIO_COMMAND::enable(unsigned short int interval = 0) const
{
	potSetResistor(this->potStep, interval, this->ringEnabled);
}

void RADIO_COMMAND::disable()
{
	potSetResistor(POT_DEFAULT_STEP, 0, POT_DEFAULT_RING_ENABLED);
}*/