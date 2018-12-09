#ifndef __ASTRA_H__H__
	#define __ASTRA_H__H__
	#include "../../configuration.h"
  
	/*IDs*/
	#define ASTRA_H_CIM_STEERING_WHEEL_BUTTONS_ID		0x206
	#define ASTRA_H_TEST_MODE_ID						0x454
	#define ASTRA_H_DIS_REQUEST_ID						0x2C1
	#define ASTRA_H_POWER_MODE_ID						0x450 //https://sites.google.com/site/astracarpc/ms-can
	#define ASTRA_H_DIS_SET_ID							0x6C1 //0x6C7, 0x6C8 for different lines (CID DISPLAY) (FAILS IN BID)
	#define ASTRA_H_EHU_BUTTONS_ID						0x201
	#define ASTRA_H_EHU_UNIT_ID							0x501
	#define ASTRA_H_BC_UNIT_ID							0x506
																
	#define ASTRA_H_ECU_IGNITION_STATE_ID				0x188 //https://github.com/Trueffelwurm/MPi3/blob/master/MPi3.py
	#define ASTRA_H_ECC_DIAGNOSTICS_ID					0x548 //https://github.com/Trueffelwurm/MPi3/blob/master/MPi3.py	
	#define ASTRA_H_DIS_MEASURING_BLOCKS_RESPONSE_ID	0x546
	#define ASTRA_H_EHU_DIAGNOSTICS_ID					0x241
	#define ASTRA_H_DIS_DIAGNOSTICS_ID					0x246
	#define ASTRA_H_UHP_DIAGNOSTICS_ID					0x247
	#define ASTRA_H_DAB_DIAGNOSTICS_ID					0x24D
	#define ASTRA_H_ECC_DIAGNOSTICS_ID					0x248 //https://www.zafiraklub.pl/viewtopic.php?p=268777&sid=dfcd39bbfb78f797d4e97df01abb1d35#p268777
	#define ASTRA_H_UNK_ID								0x691

	/* CAN BUS */
	/*
	High Speed Can (HSCAN - 500 Kbit/s) & LSCAN
	- Engine
	- Transmission
	- Chassis
		- ABS/TC/ESP ECU (Electronic Control Unit)
		- TPMS (Tyre Pressure Monitoring System)
		- EHPS (Electrohydraulic Power Steering)
		- Electric Power Steering
		- CDC (Continuous Damping Control)
	- Body
		- PEPS (Open & Start)
		- Immobilizer
		- IPC ECU (Instrument Control Unit)
		- Electrohydraulic Hardtop
		- SDM (Airbag)
		- ECC (Electronic Climate Control)
		- ECC / SAC ( Electronic Climate Control)
		- AHL / AFL /Headlamp Levelling)
		- PAS (Park Pilot)
		- EHS (Electrical Heater System)
		- CIM (Steering Column Module)
		- UEC (Underhood Electrical Centre)
		- REC (Rear Electrical Centre)
		- SLM (Shift Lever Module)
	Mid Speed Can (MSCAN - 96 Kbit/s)
	- Infotainment System
		- EHU (Entertainmet Head Unit)
		- DIS (INFO DISPLAY TID BID GID CID)
		- UHP (Mobile Phone Portal)
		- DAB (Digital Audio Broadcast)

		LSCAN (GMLAN)
		https://blog.dimitarmk.com/2017/02/05/gmlan-sniffing-arduino-mcp2515/
		https://docs.google.com/spreadsheets/d/1qEwOXSr3bWoc2VUhpuIam236OOZxPc2hxpLUsV0xkn0/edit#gid=3
	*/

	/*** ADDONS **/
	#include "astra_h_can_commands.h"
	#include "astra_h_alarm_led.h"
	#include "astra_h_dis.h"
	#include "astra_h_test_mode.h"
	#include "astra_h_power.h" 
	#include "astra_h_status.h"
	#include "astra_h_remote_door.h"
	#include "astra_h_events.h"
	#include "astra_h_events_buttons.h"
#endif