#ifndef __ASTRA_H_STATUS__H__
	#define __ASTRA_H_STATUS__H__  
	#include "../../configuration.h"  
	#include "../../can_controller.h"
	#include "../../can_command.h"
	#include "../../car_status.h"

	/*
	A/C MEASURING BLOCKS:
	248 # 06 AA 01 01 07 10 11 - Ask the A / C controller (0x0248) for measuringblocks 01, 07, 10 and 11
	Answer:
	548 # 01 03 A5 00 00 01 9C 00 - 01: 0x03A5 : Solar sensor : 4.665V - 0x019C : Indoor temp sensor : 2.06V
	548 # 07 00 90 04 3D FE 70 00 - 07: 0x90 : Voltage : 14.4V
	548 # 10 00 91 02 B2 03 1E 96 - 10: 0x0091 : Out - temp : 14.5°C - 0x02B2 : Engine temp : 69.0°C
	548 # 11 08 ED 00 30 01 FE 23 - 11: 0x08ED : RPM : 2285 - 0x30 : Speed : 48 km / h - 0x23 : LED : 35 %

	DIS MEASURING BLOCKS:
	0x246 - 10,8,AA,3,1,B,E,11 - Ask the DIS controller (0x0246) for measuring blocks 1, B, E and 11
	Answer:
	0x546 - 1,E,0,0,0,0,0,0,
	0x546 - B,17,2,C0,84,42,BF,70, - 0x0B: 0x42 Engine Temp (+40)
	0x546 - E,0,0,19,0,0,0,E4,
	0x546 - 11,6F,43,3C,42,42,43,0,
	0x546 - 19,0,D7,0,0,0,0,0,
	0x546 - 5E,A6,0,0,0,0,0,0,
	End Measuring Blocks:
	0x246 - 2,AA,0,0,0,0,0,0,
	*/

	#if COMPILE_CAR_STATUS_MONITOR
		void astraHRequestMeasuringBlocks(CAN_CONTROLLER *can);
		void astraHHandleStatusCmd(CAN_CONTROLLER *can, CAN_COMMAND *cmd);	
		void astraHOnAlarmStatusChanged(uint8_t alarm);

		#if COMPILE_DIS
			void astraHPrintStatus(CAN_CONTROLLER *can, CAR_STATUS *status = &currentCarStatus);
		#endif
	#endif
#endif