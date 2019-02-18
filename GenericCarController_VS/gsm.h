//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#ifndef __GSM__H__
	#define __GSM__H__

	#include "configuration.h"
	#include "utils.h"
	#include "lib/others/FastGPIO.h"	
  
	#if COMPILE_GSM_MODEM 
		#include "serial_controller.h"

		#if COMPILE_TRACCAR
			#include "traccar.h"
		#endif

		#if GSM_SERIAL_MODE == HW_SERIAL
			#include "lib/serial/NeoHWSerial.h"  
		#elif GSM_SERIAL_MODE == IC_SERIAL
			#include "lib/serial/NeoICSerial.h"    
			extern NeoICSerial gsmSerial;
		#elif GSM_SERIAL_MODE == SW_SERIAL
			#include "lib/serial/NeoSWSerial.h"    
			extern NeoSWSerial gsmSerial;
		#endif 

		typedef enum HTTP_GET_STATUS 
		{
			HTTP_NONE,
			HTTP_INIT,
			HTTP_CONNECTED,
			HTTP_CONNECTED_REQUEST_LENGTH_SET,
			HTTP_CONNECTED_REQUEST_SENT,
		} HTTP_GET_STATUS;
  
		typedef enum GSM_STATUS 
		{
			GSM_RESTARTING,
			GSM_SETTING_UP_BAUDRATE,
			GSM_WAITING_FOR_PIN_REQUEST,
			GSM_SETTING_PIN,
			GSM_SETTING_NETWORK_WAIT_FOR_GSM_ATTACHED,
			GSM_SETTING_NETWORK_SET_APN,			
			GSM_SETTING_NETWORK_BRING_UP_WIRELESS_GPRS,
			GSM_SETTING_NETWORK_GET_LOCAL_IP,
			#if GSM_DISABLE_LEDS
				GSM_DISABLING_LED,
			#endif
			GSM_CONNECTED_WITH_INET    
		} GSM_STATUS;

		typedef enum GSM_GET_ERROR
		{
			ERROR_GET_TIMEOUT,
			ERROR_SETUP_TIMEOUT,
			ERROR_REQUEST_ABORTED,
			ERROR_CANNOT_RETRY_REQUEST
		} GSM_GET_ERROR;

		#if COMPILE_SERIAL_CONTROL && SERIAL_CONTROL_MODE >= SC_MID
			bool gsmHandleConfCmd(char *cmd, char *params);
		#endif

		bool gsmHTTPGet(const char *host, unsigned int port, char *path, char *body, void(*successCb)(char*), bool(*errorCb)(GSM_GET_ERROR, char*), bool forceRequest = false);
		bool gsmHTTPRetryRequest(bool forceRequest = false);
		void gsmHTTPAbort();
		bool gsmIsConnectedToInternet();
		bool gsmGetRequestHasTimeout();
		bool gsmGetHasActiveRequest();
		void gsmReset();
		void checkGSM();
		bool setupGSM();   
		void gsmEnableListenSerial();
		void gsmDisableListenSerial();
	#endif
#endif
