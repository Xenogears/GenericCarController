#include "configuration.h"
#include "potentiometer.h"
#include "can_command.h"
#include <stdint.h>

#ifndef __RADIO_COMMAND__H__
  #define __RADIO_COMMAND__H__

	typedef struct
	{
		const uint8_t potStep;
		const bool ringEnabled;
    
	} RADIO_COMMAND;

	extern const RADIO_COMMAND RADIO_CMD_SOURCE;
	extern const RADIO_COMMAND RADIO_CMD_MUTE;
	extern const RADIO_COMMAND RADIO_CMD_DISPLAY;
	extern const RADIO_COMMAND RADIO_CMD_NEXT_TRACK;
	extern const RADIO_COMMAND RADIO_CMD_PREV_TRACK;
	extern const RADIO_COMMAND RADIO_CMD_VOLUME_UP;
	extern const RADIO_COMMAND RADIO_CMD_VOLUME_DOWN;
	extern const RADIO_COMMAND RADIO_CMD_BAND_ESCAPE;
	extern const RADIO_COMMAND RADIO_CMD_FOLDER_UP;
	extern const RADIO_COMMAND RADIO_CMD_FOLDER_DOWN;
	extern const RADIO_COMMAND RADIO_CMD_VOICE_CONTROL;

	void radEnableCmd(const RADIO_COMMAND *radioCmd, const uint16_t interval = 0) __attribute__((always_inline));
	void radDisableCmd() __attribute__((always_inline));

	/*class RADIO_COMMAND
	{
		public:
			const uint8_t potStep;
			const bool ringEnabled;

			RADIO_COMMAND::RADIO_COMMAND(const uint8_t p, const bool r);

			void enable(unsigned short int interval = 0) const;
			static void disable();
	};

	const RADIO_COMMAND RADIO_CMD_SOURCE(MCP4131_STEP(1.2f), false); //Source/Off
	const RADIO_COMMAND RADIO_CMD_MUTE(MCP4131_STEP(3.5), false); //Mute/ATT
	const RADIO_COMMAND RADIO_CMD_DISPLAY(MCP4131_STEP(5.75f), false); //Display
	const RADIO_COMMAND RADIO_CMD_NEXT_TRACK(MCP4131_STEP(8.0f), false); //Next Track
	const RADIO_COMMAND RADIO_CMD_PREV_TRACK(MCP4131_STEP(11.25f), false); //Prev Track
	const RADIO_COMMAND RADIO_CMD_VOLUME_UP(MCP4131_STEP(16.0f), false); //Volume Up
	const RADIO_COMMAND RADIO_CMD_VOLUME_DOWN(MCP4131_STEP(25.0f), false); //Volume Down
	const RADIO_COMMAND RADIO_CMD_BAND_ESCAPE(MCP4131_STEP(62.75f), false); //Band/Escape

	const RADIO_COMMAND RADIO_CMD_FOLDER_UP(MCP4131_STEP(8.0f),	true); //Folder Up
	const RADIO_COMMAND RADIO_CMD_FOLDER_DOWN(MCP4131_STEP(11.25f), true); //Folder Down
	const RADIO_COMMAND RADIO_CMD_VOICE_CONTROL(MCP4131_STEP(62.75f), true); //Voice control
	const RADIO_COMMAND RADIO_CMD_NONE(MCP4131_STEP(70.0f), true); //None*/

#endif


