//
// Author:	�lvaro Graci� Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

//Timer0 --> Millis, nanos
//Timer1 --> POW On -> DIS || POW Off -> Alarm LED
//Timer2 --> GPS notification task

//OPTIONS
#define VOLUME_MODE_RADIO				1
#define VOLUME_MODE_COMMAND				2
#define CONFIG_VOLUME_MODE				VOLUME_MODE_RADIO

#define COMMAND_FORMAT_TEXT				1
#define COMMAND_FORMAT_BINARY			2
#define CONFIG_COMMAND_FORMAT			COMMAND_FORMAT_TEXT

//TIME
#define TRACCAR_ON_GPS_ACQUIRED_CUSTOM_CALLBACK		onGPSFixAcquired

//MSCAN 
#define CAN_BAUDRATE					CAN_95KBPS
#define CAN_BLACKLIST_LENGTH            0  //items
#define CAN_PERIODICAL_CMDS_LENGTH      0  //items

//BUTTON EVENTS
#define NONE_MASK						0
#define LEFT_WHEEL_BUTTON_MASK			(1 << 5)
#define RIGHT_TOP_BUTTON_MASK			(1 << 4)
#define RIGHT_BOTTOM_BUTTON_MASK		(1 << 3)
#define LEFT_TOP_BUTTON_MASK			(1 << 2)
#define LEFT_BOTTOM_BUTTON_MASK			(1 << 1)

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

#define REMOTE_DOOR_SIGNAL_SEND_INTERVAL				1000 //ms
#define REMOTE_DOOR_MODE_RC_PULSE_LENGTH				321
#define REMOTE_DOOR_MODE_RC_REPEAT_TRANSMIT				10
#define REMOTE_DOOR_MODE_RC_PIN							GPIO1_PIN

//CONFIGURATION
#define ENABLE_POWER_NOTIFICATION		(1 << 9)
#define ENABLE_CAR_STATUS_MONITOR		(1 << 15)