//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#ifndef __CONFIGURATION_H__
	#define __CONFIGURATION_H__
	
	#include "customArduino.h"
	#include "constants.h"
	#include "configuration_secret.h"
	
	/********* CARS **********/
	#define NONE_CAR					0
	#define OPEL_ASTRA_H_2004_CAR		1
	#define FORD_FOCUS_2004_CAR			2
	#define TOYOTA_COROLLA_2000_CAR		3
	#define OPEL_ASTRA_H_2004_MIN_CAR	4

	/********* CONFIG **********/
	#define CAR_MODEL								OPEL_ASTRA_H_2004_CAR
	#define BOARD_VERSION							43

	#define COMPILE_DEBUG							0
	#define COMPILE_CAN								1
	#define COMPILE_CAN_PIN_CMD_AVAILABLE			1
	#define COMPILE_CAN_INTERRUPT					1
	#define COMPILE_CAN_WHITELIST					1
	#define COMPILE_CAN_BLACKLIST					0
	#define COMPILE_CAN_PERIODICAL					1
	#define COMPILE_RADIO_CONTROL					1
	#define COMPILE_POTENTIOMETER					1
	#define COMPILE_SERIAL_CONTROL					1
	#define COMPILE_EEPROM_CONFIG					1
	#define COMPILE_TRACCAR							0
	#define COMPILE_GPS								0
	#define COMPILE_GSM_MODEM						0
	#define COMPILE_CAR_STATUS_MONITOR				0	//It occupies around 1KB of code and consumes 40 bytes of RAM	

	//INJECTION	
	#define ENABLE_INJECTION						0
	#define INJECT_SEQUENTIAL_COMMANDS				0

	//RADIO
	#define RADIO_COMMAND_MIN_INTERVAL				50	//ms
	#define RADIO_COMMAND_MAX_INTERVAL				110 //ms
	#define RADIO_USE_TIP_OPTOCOUPLERS				0	

	//POT
	#define POT_SHOW_KOHMS							0		

	//CAN
	#define CAN_CLOCKSET							MCP_8MHZ	
	#define CAN_MODE_LOOPBACK						0
	#define CAN_RX_QUEUE_LENGTH						10		//items
	#define CAN_TX_QUEUE_LENGTH						14		//items
	#define CAN_INIT_MAX_RETRIES					5
	#define CAN_MIN_COMMAND_INTERVAL				10		//ms
	#define CAN_DISABLE_INTERRUPTS_AT_TRANSFER		0
	#define CAN_ENABLE_MIN_DELAY_BETWEEN_COMMANDS	1
	#define CAN_ENABLE_TX_QUEUE						1
	#define CAN_TX_RETRY_TIMEOUT					10		//ms
	#define CAN_HANDLE_ERRORS_ON_ONTERRUPT			0
	#define CAN_INTERRUPT_MODE_FLAG					1
	#define CAN_INTERRUPT_MODE_FETCH				2
	#define CAN_INTERRUPT_MODE						CAN_INTERRUPT_MODE_FETCH
	#define CAN_DEBUG_PRINT_QUEUE_STATS				1

	//SERIAL
	#define DEBUG_INVERT_SERIAL				1
	#define DEFAULT_BAUDRATE                57600	//115200 fails (NeoIC)		
	#define SC_FULL                         3		//GSM + GPS + POT CALIBRATION + POT SET + [MID]
	#define SC_MID                          2		//RAD + BC + CAN + [MIN]
	#define SC_MIN                          1		//SYS + POW + LOG + OPT +  DIS
	#define SERIAL_CONTROL_MODE             SC_FULL
	#define CMD_SERIAL_BUFFER_SIZE			64
	#define CMD_USE_SERIAL_INTERRUPTS		1
	#define MAX_SERIAL_HANDLERS				14
	#define MAX_PARAMETERS					9
	#define HW_SERIAL						1
	#define IC_SERIAL						2
	#define SW_SERIAL						3	

	//GPS
	#define GPS_BAUDRATE					9600
	#define GPS_SERIAL_ALWAYS_ENABLED		0

	//GSM
	#define GSM_BAUDRATE							9600 //115200 fails (NeoHW)	
	#define GSM_GET_MAX_TIMEOUT						25 * 1000L	//ms
	#define GSM_GET_MAX_ERRORS_BEFORE_RESET			3
	#define GSM_SETUP_MAX_TIMEOUT					60 * 1000L	//ms -> Prev = 30s //TODO -> CHECK	
	#define GSM_SETUP_RESET_LOW_DELAY				100 //ms
	#define GSM_DISABLE_LEDS						1
	#define GSM_SERIAL_BUFFER_SIZE					32
	#define GSM_USE_SERIAL_INTERRUPTS				1
	#define GSM_USE_FLASH_STRINGS					1	//if disabled, consumes 198 bytes of RAM	
	#define GSM_COMMAND_QUEUE_LENGTH				3		

	#if GSM_USE_FLASH_STRINGS
		#define GSM_STR(A) PSTR(A)
	#else
		#define GSM_STR(A) A
	#endif

	//MEM
	#define USE_STATIC_MEM					1

	//POWER
	#define POW_ENABLE_WATCHDOG				0
	#define POW_WATCHDOG_TIMEOUT			WDTO_60MS

	//CAR STATUS MONITOR
	#define MON_FETCH_VOLTAGE				0
	#define MON_CHECK_VOLTAGE				0
	#define MON_MIN_VOLTAGE					12
	#define MON_MAX_VOLTAGE					15
	#define MON_REPORT_VOLTAGE				1	
	#define MON_REPORT_VOLTAGE_LENGTH		6 //VX.YZ<space>
	#define MON_REPORT_VOLTAGE_TRACCAR		1	

	#define MON_FETCH_ENGINE_TEMP			1
	#define MON_CHECK_ENGINE_TEMP			1
	#define MON_MAX_ENGINE_TEMP				105
	#define MON_REPORT_ENGINE_TEMP			1
	#define MON_REPORT_ENGINE_TEMP_LENGTH	5 //VXYZ<space>	
	#define MON_REPORT_ENGINE_TEMP_TRACCAR	1	

	#define MON_FETCH_SPEED					0
	#define MON_CHECK_SPEED					1
	#define MON_MAX_SPEED					120
	#define MON_REPORT_SPEED				1	
	#define MON_REPORT_SPEED_LENGTH			5 //SXYZ<space>
	#define MON_REPORT_SPEED_TRACCAR		1

	//TRACCAR
	#define TRACCAR_NOTIFICATION_INTERVAL_ON		30	//s
	#define TRACCAR_NOTIFICATION_INTERVAL_OFF		180	//s
	#define TRACCAR_SEND_TIMESTAMP					1
	#if COMPILE_CAR_STATUS_MONITOR
		#define TRACCAR_MAX_HTTP_URL_LEN				115
	#else
		#define TRACCAR_MAX_HTTP_URL_LEN				85
	#endif
	#define TRACCAR_MAX_REQUEST_RETRIES				2
	#define TRACCAR_USE_TIMER_TWO					0
	#define TRACCAR_NOTIFICATION_QUEUE_LENGTH		5	//items
	#define TRACCAR_COMPILE_ALARM_MODE				0
	#define TRACCAR_NOTIFICATION_INTERVAL_ALARM		10	//s
	#if TRACCAR_COMPILE_ALARM_MODE
		#define TRACCAR_TIMER_INTERVAL				TRACCAR_NOTIFICATION_INTERVAL_ALARM * 1000L   //ms
	#else
		#define TRACCAR_TIMER_INTERVAL				TRACCAR_NOTIFICATION_INTERVAL_ON * 1000L   //ms
	#endif

	#if BOARD_VERSION == 1 || BOARD_VERSION == 10
		#error BOARD v1 NOT SUPPORTED
	#elif BOARD_VERSION == 2 || BOARD_VERSION == 20
		#define CMD_SERIAL_MODE		HW_SERIAL

		#if defined(__AVR_ATmega328P__)	//UNO, NANO
			#define CAN_A_CS_PIN        10  //D10
			#define POT_CS_PIN          8   //D8
			#define TIP_CONNECTION_PIN  4   //D4
			#define RING_CONNECTION_PIN 5   //D5  
			#define ARDUINO_RESET_PIN   3   //D3
			#define POT_BASE_OHM        620
			#define POT_STEP_OHM        750
		#else
			#error BOARD NOT SUPPORTED
		#endif	

		#undef COMPILE_CAN_PIN_CMD_AVAILABLE
		#undef COMPILE_CAN_INTERRUPT
		#undef COMPILE_GPS
		#undef COMPILE_GSM_MODEM
		#define COMPILE_CAN_PIN_CMD_AVAILABLE    0
		#define COMPILE_CAN_INTERRUPT            0
		#define COMPILE_GPS                      0
		#define COMPILE_GSM_MODEM                0
	#elif BOARD_VERSION == 3 || BOARD_VERSION == 30
		#define CMD_SERIAL_MODE		HW_SERIAL

		#if defined(__AVR_ATmega328P__)	//UNO, NANO
			#define CAN_A_CS_PIN        A0  //A0
			#define POT_CS_PIN          A1  //A1
			#define TIP_CONNECTION_PIN  4   //D4
			#define RING_CONNECTION_PIN 5   //D5  
			#define ARDUINO_RESET_PIN   2   //D2
		#else
			#error BOARD NOT SUPPORTED
		#endif	

		#undef COMPILE_CAN_PIN_CMD_AVAILABLE
		#undef COMPILE_CAN_INTERRUPT
		#undef COMPILE_GPS
		#undef COMPILE_GSM_MODEM
		#define COMPILE_CAN_PIN_CMD_AVAILABLE    0
		#define COMPILE_CAN_INTERRUPT            0
		#define COMPILE_GPS                      0
		#define COMPILE_GSM_MODEM                0
	#elif BOARD_VERSION == 4 || BOARD_VERSION == 40
		#define CMD_SERIAL_MODE		HW_SERIAL
	
		#define GPS_SERIAL_MODE		SW_SERIAL
		#define GSM_SERIAL_MODE		SW_SERIAL

		#if defined(__AVR_ATmega328P__)	//UNO, NANO
			#define CAN_A_CS_PIN        A0  //A0  
			#define POT_CS_PIN          A5  //A5
			#define GSM_TX_PIN          10  //D10
			#define GSM_RX_PIN          9   //D9
			#define GSM_RESET_PIN       8   //D8
			#define GPS_TX_PIN          7   //D7
			#define GPS_RX_PIN          6   //D6
			#define TIP_CONNECTION_PIN  4   //D4
			#define CAN_A_INT_PIN       3   //D3
			#define RING_CONNECTION_PIN 5   //D5  
			#define ARDUINO_RESET_PIN   2   //D2	
		#else
			#error BOARD NOT SUPPORTED
		#endif	
	#elif BOARD_VERSION == 41
		#define CMD_SERIAL_MODE				HW_SERIAL

		#define GPS_SERIAL_MODE				SW_SERIAL
		#define GSM_SERIAL_MODE				SW_SERIAL
		#define GSM_IS_PRIORITARY_SW_SERIAL

		#if defined(__AVR_ATmega328P__) //UNO, NANO
			#define CAN_A_CS_PIN        A0  
			#define POT_CS_PIN          A5  
			#define GSM_RING_PIN        A1
			#define GSM_TX_PIN          8
			#define GSM_RX_PIN          9
			#define GSM_RESET_PIN       7
			#define GPS_RX_PIN          5
			#define GPS_TX_PIN          6    
			#define TIP_CONNECTION_PIN  3
			#define CAN_A_INT_PIN       2
			#define RING_CONNECTION_PIN 4
			#define ARDUINO_RESET_PIN   A3 
			#define ARDUINO_RANDOM_PIN  A6
			#define GPIO1_PIN			A4
		#else
			#error BOARD NOT SUPPORTED
		#endif
	#elif BOARD_VERSION == 42 || BOARD_VERSION == 43
		#if DEBUG_INVERT_SERIAL == 0
			#define CMD_SERIAL_MODE		IC_SERIAL
			#define GPS_SERIAL_MODE		SW_SERIAL
			#define GSM_SERIAL_MODE		HW_SERIAL
		#else
			#define CMD_SERIAL_MODE			HW_SERIAL
			#define CMD_ALT_SERIAL_MODE		IC_SERIAL		
			
			//#define GPS_SERIAL_MODE		SW_SERIAL
			//#define GSM_SERIAL_MODE		IC_SERIAL			
		#endif
		
		#if defined(__AVR_ATmega328P__) //UNO, NANO
			#define CAN_A_CS_PIN        A0  
			#define POT_CS_PIN          A2  
			#define RING_CONNECTION_PIN A3	
			#define GPIO1_PIN			A4
			#define GPIO2_PIN			A5
			#define ARDUINO_RANDOM_PIN  A7
			#define GSM_RING_PIN        2			
			#define CAN_A_INT_PIN       3			
			#define ARDUINO_RESET_PIN   4 
			#define GSM_RESET_PIN       5
			#define GSM_TX_PIN          6    			
			#define GSM_RX_PIN          0	
			#define GPS_TX_PIN          1    			
			#define GPS_RX_PIN          7															
			#define CMD_RX_PIN			8
			#define CMD_TX_PIN			9
		#else
			#error BOARD NOT SUPPORTED
		#endif
				
		#undef	RADIO_USE_TIP_OPTOCOUPLERS		
		#define RADIO_USE_TIP_OPTOCOUPLERS		0			
	#endif
		
	//MCP4131
	#if RADIO_USE_TIP_OPTOCOUPLERS 
		#define POT_BASE_OHM        620
		#define POT_STEP_OHM        750 
	#else
		#define POT_BASE_OHM        135
		#define POT_STEP_OHM        755
	#endif

	#if ENABLE_INJECTION
		#undef COMPILE_CAN_WHITELIST
		#define COMPILE_CAN_WHITELIST 0

		#undef	CAN_ENABLE_MIN_DELAY_BETWEEN_COMMANDS
		#define CAN_ENABLE_MIN_DELAY_BETWEEN_COMMANDS 0
	#endif

	#define	PRINT_CAN_DATA						(1 << 0)
	#define PRINT_POTENTIOMETER_INFO			(1 << 1)
	#define PRINT_STATUS_INFO					(1 << 2)

	#ifdef COMPILE_TRACCAR
		#define PRINT_TRACCAR_INFO				(1 << 3)
		#define ENABLE_TRACCAR_NOTIFICATION		(1 << 6)
	#endif
	#ifdef COMPILE_GSM_MODEM
		#define PRINT_GSM_INFO					(1 << 4)
	#endif
	#ifdef COMPILE_GPS
		#define PRINT_GPS_INFO					(1 << 5)
	#endif
	#ifdef COMPILE_CAR_STATUS_MONITOR
		#define PRINT_CAR_STATUS_INFO			(1 << 6)
	#endif
	#ifdef COMPILE_DEBUG
		#define PRINT_DEBUG						(1 << 7)
	#endif

	#define CONFIG_BASE_TYPE uint16_t

	class CONFIGURATION
	{
		private: 
			CONFIG_BASE_TYPE params;

		public:
			CONFIG_BASE_TYPE getAll()		__attribute__((always_inline));
			bool get(CONFIG_BASE_TYPE i)	__attribute__((always_inline));
			void setDefaults()				__attribute__((always_inline));
			void clear()					__attribute__((always_inline));

			void set(CONFIG_BASE_TYPE mask, bool value);
			void set(CONFIG_BASE_TYPE value)		__attribute__((always_inline));
			void set(uint8_t shift, uint8_t value)	__attribute__((always_inline));
	};

	extern CONFIGURATION CONFIG;
	
	#if(CAR_MODEL == OPEL_ASTRA_H_2004_CAR)
		#include "cars/OpelAstraH2004/car_configuration.h"
	#elif(CAR_MODEL == FORD_FOCUS_2004_CAR)
		#include "cars/FordFocus2004/car_configuration.h"
	#elif(CAR_MODEL == TOYOTA_COROLLA_2000_CAR)
		#include "cars/ToyotaCorolla2000/car_configuration.h"
	#elif(CAR_MODEL == OPEL_ASTRA_H_2004_MIN_CAR)
		#include "cars/OpelAstraH2004-RPI/car_configuration.h"
	#else
		#include "cars/DefaultCar/car_configuration.h"
	#endif

	#if COMPILE_GPS == 0
		#undef GPS_SERIAL_MODE
		#define GPS_SERIAL_MODE 0
	#endif
	#if COMPILE_GSM_MODEM == 0
		#undef GSM_SERIAL_MODE
		#define GSM_SERIAL_MODE 0
	#endif

	#pragma region SERIAL COUNT

	#if CMD_SERIAL_MODE == SW_SERIAL
		#define TMP_COUNT_CMD 1
	#else
		#define TMP_COUNT_CMD 0
	#endif

	#if GPS_SERIAL_MODE == SW_SERIAL
		#define TMP_COUNT_GPS 1
	#else
		#define TMP_COUNT_GPS 0
	#endif

	#if GSM_SERIAL_MODE == SW_SERIAL
		#define TMP_COUNT_GSM 1
	#else
		#define TMP_COUNT_GSM 0
	#endif

	#if (TMP_COUNT_CMD + TMP_COUNT_GPS + TMP_COUNT_GSM) > 1
		#define MULTIPLE_SW_SERIALS	1
	#else
		#define MULTIPLE_SW_SERIALS	0	
	#endif

	#pragma endregion

	#if !defined(CAR_MODEL)
		#define CAR_MODEL						NONE_CAR
	#endif
	#if !defined(CAN_BAUDRATE)
		#define CAN_BAUDRATE					CAN_500KBPS
	#endif
	#if !defined(CAN_BLACKLIST_LENGTH)
		#define CAN_BLACKLIST_LENGTH			0
	#endif
	#if !defined(CAN_PERIODICAL_CMDS_LENGTH)
		#define CAN_PERIODICAL_CMDS_LENGTH		0
	#endif
	#if !defined(CAN_A_INT_PIN)
		#define CAN_A_INT_PIN 0
	#endif
#endif