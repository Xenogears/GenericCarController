//
// Author:	�lvaro Graci� Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#ifndef __ASTRA_H_MIN__H__
	#define __ASTRA_H_MIN__H__
	#include "../../configuration.h"
  
	/*IDs*/
	#define ASTRA_H_CIM_STEERING_WHEEL_BUTTONS_ID		0x206	
	#define ASTRA_H_POWER_MODE_ID						0x450 //https://sites.google.com/site/astracarpc/ms-can

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
	#include "astra_h_power.h" 
	#include "astra_h_remote_door.h"
	#include "astra_h_events.h"
	#include "astra_h_events_buttons.h"
#endif