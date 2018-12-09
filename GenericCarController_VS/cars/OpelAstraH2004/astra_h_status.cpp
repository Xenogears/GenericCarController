#include "../../configuration.h"

#if COMPILE_CAN && CAR_MODEL == OPEL_ASTRA_H_2004_CAR && COMPILE_CAR_STATUS_MONITOR
	#include "astra_h.h"
	#include "astra_h_status.h"
	#include "astra_h_can_commands.h"

	#include "../../cmd_serial.h"

	void astraHRequestMeasuringBlocks(CAN_CONTROLLER *can)
	{
		CAN_COMMAND reqMeasuringBlocks = CAN_COMMAND(ASTRA_H_DIS_MEASURING_BLOCKS_REQ_ID, 0x10, 0x08, 0xAA, 0x03, 0x01, 0x0B, 0x0E, 0x11);
		/*#if MON_FETCH_VOLTAGE
			reqMeasuringBlocks.data[reqMeasuringBlocks.can_dlc++] = MON_VOLTAGE_MEASURING_BLOCK_ID;
		#endif
		#if MON_FETCH_ENGINE_TEMP
			reqMeasuringBlocks.data[reqMeasuringBlocks.can_dlc++] = MON_TEMP_MEASURING_BLOCK_ID;
		#endif
		#if MON_FETCH_SPEED
			reqMeasuringBlocks.data[reqMeasuringBlocks.can_dlc++] = MON_SPEED_MEASURING_BLOCK_ID;
		#endif*/
		//?

		DEFAULT_CAN.sendCmd(&reqMeasuringBlocks);
	}

	void astraHHandleStatusCmd(CAN_CONTROLLER *can, CAN_COMMAND *cmd)
	{
		switch(cmd->data[0])
		{
			#if MON_FETCH_VOLTAGE
				case MON_VOLTAGE_MEASURING_BLOCK_ID:
					currentCarStatus.voltage = (cmd->data[2] / 10);
					break;
			#endif

			#if MON_FETCH_ENGINE_TEMP
				case MON_TEMP_MEASURING_BLOCK_ID:
					currentCarStatus.engineTemp = (cmd->data[5] - 40);
					break;
			#endif

			#if MON_FETCH_SPEED
				case MON_SPEED_MEASURING_BLOCK_ID:
					currentCarStatus.speed = cmd->data[4];
					break;
			#endif
		}

		currentCarStatus.updated = true;
	}

	void astraHOnAlarmStatusChanged(uint8_t alarm)
	{
		#if COMPILE_DIS
			if (alarm)
				astraHPrintStatus(&DEFAULT_CAN);				
			else
				astraHDISSetDefaultText(&DEFAULT_CAN);
		#endif
	}

	#if COMPILE_DIS
		void astraHPrintStatus(CAN_CONTROLLER *can, CAR_STATUS *status = &currentCarStatus)
		{
			const uint8_t msgLength = (MON_REPORT_VOLTAGE ? MON_REPORT_VOLTAGE_LENGTH : 0)
									+ (MON_REPORT_ENGINE_TEMP ? MON_REPORT_ENGINE_TEMP_LENGTH : 0)
									+ (MON_REPORT_SPEED ? MON_REPORT_SPEED_LENGTH : 0)
									+ 1; //NULL CHAR
			char msg[msgLength] = { 0x00 };
			char temp[6];
			bool first = true;

			#if MON_FETCH_VOLTAGE && MON_REPORT_VOLTAGE
				if (!first)
					strcat_P(msg, sSpace);
				else
					first = false;

				dtostrf(status->voltage, 2, 2, temp);

				strcat_P(msg, PSTR("V"));
				strcat(msg, temp);
				strcat_P(msg, sSpace);
				first = false;
			#endif

			#if MON_FETCH_ENGINE_TEMP && MON_REPORT_ENGINE_TEMP
				if (!first)
					strcat_P(msg, sSpace);
				else
					first = false;

				itoa(status->engineTemp, temp, 10);

				strcat_P(msg, PSTR("T"));
				strcat(msg, temp);
			#endif

			#if MON_FETCH_SPEED && MON_REPORT_SPEED
				if (!first)
					strcat_P(msg, sSpace);
				else
					first = false;

				itoa(status->speed, temp, 10);

				strcat_P(msg, PSTR("S"));
				strcat(msg, temp);				
			#endif
			

			if (CONFIG.get(PRINT_CAR_STATUS_INFO))
			{
				cmdSerial.print(millis());
				utilsPrint_P(sSpace);
				cmdSerial.println(msg);
			}

			astraHDISSetText(&DEFAULT_CAN, msg);
		}
	#endif
#endif
