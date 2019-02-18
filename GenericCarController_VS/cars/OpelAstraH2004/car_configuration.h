//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

//Timer0 --> Millis, nanos
//Timer1 --> POW On -> DIS || POW Off -> Alarm LED
//Timer2 --> GPS notification task

//INJECTION	
#define INJECT_PERIODICAL_CAR_COMMANDS	1
#define INJECT_DIS_BC_RESPONSE			1
#define INJECT_DOOR_OPEN				1

//BUTTON EVENTS
#define ENABLE_BUTTON_EVENT_HANDLING	1
#define COMPILE_SETTINGS_MODE			1
#define ENABLE_DIS_BUTTON_FLAGS			1

#define NONE_MASK						0
#define LEFT_WHEEL_BUTTON_MASK			(1 << 5)
#define RIGHT_TOP_BUTTON_MASK			(1 << 4)
#define RIGHT_BOTTOM_BUTTON_MASK		(1 << 3)
#define LEFT_TOP_BUTTON_MASK			(1 << 2) //DIS -> REG
#define LEFT_BOTTOM_BUTTON_MASK			(1 << 1) //DIS -> RDS
#define SETTINGS_MODE_MASK				(1 << 0) //DIS -> AS

//MSCAN 
#define CAN_BAUDRATE					CAN_95KBPS
#define CAN_WHITELIST_LENGTH            6  //items
#define CAN_BLACKLIST_LENGTH            0  //items
#define CAN_PERIODICAL_CMDS_LENGTH      1  //items
#define CAN_FRIEND_FUNCTION				bool astraHDISRequestText(CAN_CONTROLLER *can)

//ALARM LED
#define COMPILE_ALARM_LED				0
#define ALARM_LED_TICK_INTERVAL         200 //ms
#define ALARM_LED_ON_TICKS              1  
#define ALARM_LED_OFF_TICKS             20 
#define ALARM_LED_USE_TIMER_ONE			0
#define ALARM_LED_PIN					GPIO1_PIN  

//STATUS MONITOR
#define MON_ON_ALARM_STATUS_CHANGED_HANDLER		astraHOnAlarmStatusChanged

#define MON_VOLTAGE_MEASURING_BLOCK_ID			0x00 //UNKNOWN
#define MON_VOLTAGE_DIS_REPORT					1

#define MON_TEMP_MEASURING_BLOCK_ID				0x0B
#define MON_TEMP_DIS_REPORT						1

#define MON_SPEED_MEASURING_BLOCK_ID			0x00 //UNKNOWN
#define MON_SPEED_DIS_REPORT					1

//BC
#define COMPILE_BC_ECHO					0	//Required to avoid MS-CAN OPCOM errors

//DIS
const char PROGMEM DIS_MESSAGE[]		= { "Opel Astra H" };
#define COMPILE_DIS						1
#define DIS_MESSAGE_USES_PROGMEM		1
#define DIS_TICK_INTERVAL               200 //ms
#define DIS_REFRESH_TICKS               15  //MAX 17
#define DIS_TEXT_TIMEOUT_TICKS          50	//10s
#define DIS_TEXT_START_END_TICKS        5	//1s
#define DIS_TEXT_STEP_TICKS             1	//200ms
#define DIS_MAX_CHARACTERS              14 
#define DIS_MAX_TEXT_LENGTH             32  
#define DIS_SEND_COMMANDS_IMMEDIATELY	0
#define DIS_USE_TIMER_ONE               0 //IF ENABLED, CAN TX QUEUE MUST DISABLE INTERRUPTIONS
#define DIS_SIMULATE_BC_RESPONSE        0

//TEST MODE
#define COMPILE_TEST_MODE				1
#define TEST_MODE_TX_INTERVAL			40 //ms
#define TEST_MODE_INITIAL_DELAY			500 //ms

//REMOTE DOOR
#define REMOTE_DOOR_MODE_NONE			0
#define REMOTE_DOOR_MODE_RC				1
#define REMOTE_DOOR_MODE_SWITCH			2

#ifdef GPIO1_PIN
	#define COMPILE_REMOTE_DOOR								1
	#define REMOTE_DOOR_MODE								REMOTE_DOOR_MODE_SWITCH
#else
	#define COMPILE_REMOTE_DOOR								0
	#define REMOTE_DOOR_MODE								REMOTE_DOOR_MODE_NONE
#endif

const char PROGMEM REMOTE_DOOR_0_DIS_MESSAGE[] =		{ "Entrada garaje" };
#ifdef GPIO2_PIN
	const char PROGMEM REMOTE_DOOR_1_DIS_MESSAGE[] =		{ "Salida garaje" };
#endif
#define REMOTE_DOOR_SIGNAL_SEND_INTERVAL				1000 //ms
#define REMOTE_DOOR_MODE_RC_PULSE_LENGTH				321
#define REMOTE_DOOR_MODE_RC_REPEAT_TRANSMIT				10
#define REMOTE_DOOR_MODE_RC_PIN							GPIO1_PIN

#if ENABLE_INJECTION
	#undef COMPILE_DIS
	#undef COMPILE_TEST_MODE
	#undef COMPILE_REMOTE_DOOR
	#undef COMPILE_ALARM_LED
	#undef CAN_PERIODICAL_CMDS_LENGTH

	#if INJECT_PERIODICAL_CAR_COMMANDS
		#define PER_CAR_COMMANDS_LENGTH 5
	#else
		#define PER_CAR_COMMANDS_LENGTH 0
	#endif

	#if INJECT_DOOR_OPEN
		#define PER_DOOR_OPEN_LENGTH 4
	#else
		#define PER_DOOR_OPEN_LENGTH 0
	#endif

	#define COMPILE_DIS					0
	#define COMPILE_TEST_MODE			0
	#define COMPILE_REMOTE_DOOR			0
	#define COMPILE_ALARM_LED			0
	#define CAN_PERIODICAL_CMDS_LENGTH	(PER_CAR_COMMANDS_LENGTH + PER_DOOR_OPEN_LENGTH)
#endif

//CONFIGURATION
#define PRINT_DISPLAY_INFO				(1 << 8)
#define ENABLE_POWER_NOTIFICATION		(1 << 9)
#define ENABLE_RADIO_SIMULATION			(1 << 10)
#define ENABLE_TEST_MODE				(1 << 11)
#define ENABLE_DISPLAY					(1 << 12)
#define ENABLE_RANDOM_TEXTS				(1 << 13)
#define ENABLE_ALARM_LED				(1 << 14)
#define ENABLE_CAR_STATUS_MONITOR		(1 << 15)