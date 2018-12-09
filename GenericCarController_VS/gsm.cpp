#include "gsm.h"
#include "cmd_serial.h"

#if COMPILE_GSM_MODEM
	#if GSM_SERIAL_MODE == SW_SERIAL		
		static NeoSWSerial gsmSerial(GSM_RX_PIN, GSM_TX_PIN);		
	#elif GSM_SERIAL_MODE == IC_SERIAL
		static NeoICSerial gsmSerial;		
	#elif GSM_SERIAL_MODE == HW_SERIAL
		#define gsmSerial GSM_HW_SERIAL	
	#endif

	static Queue<char*, GSM_COMMAND_QUEUE_LENGTH, true> gsmCommandQueue;
	static char gsmInputString[GSM_SERIAL_BUFFER_SIZE];
	static uint8_t gsmInputStringLength = 0;
	static void gsmHandleSerial_ISR(uint8_t c);

	static GSM_STATUS gsmStat;
	static HTTP_GET_STATUS gsmGetStat = HTTP_NONE;
	static const char *gsmGetHost = NULL;
	static char *gsmGetPath = NULL;
	static char *gsmGetBody = NULL;
	static uint16_t gsmGetPort = 0;
	static unsigned long gsmGetTimeoutMillis = 0;
	static uint8_t gsmGetTotalErrors = 0;							//Cuenta el nº de errores SEGUIDOS en todas las solicitudes GET
	static void (*gsmGetOnSuccessCallback)(char *path);
	static bool (*gsmGetOnErrorCallback)(GSM_GET_ERROR err, char *path);

	static unsigned long gsmSetupTimeoutMillis = 0;
	static unsigned long gsmSetupPinTimeout = 0;
	static unsigned long gsmWaitAttachedLastCheckMillis = 0;
	static bool gsmIsAttachedToNetwork = false;

	static void gsmHTTPClose();
	static void gsmHTTPClear();
	static void gsmGetOnSuccess(char *path);
	static void gsmGetOnError(GSM_GET_ERROR error, char *path);
	static void gsmHandleCommand(char *r);
	static void gsmHTTPGetStep(HTTP_GET_STATUS stat, void *param1, void *param2, void *param3);

	#if GSM_USE_FLASH_STRINGS
		#define FUNC_STRSTR(A,B)	strstr_P(A,B) 		
		#define FUNC_PRINT(A)		utilsPrint_P(&gsmSerial, A)
		#define FUNC_PRINT_S(A,B)	utilsPrint_P(A, B)
	#else
		#define FUNC_STRSTR(A,B)	strstr(A,B)
		#define FUNC_PRINT(A)		gsmSerial.print(A);
		#define FUNC_PRINT_S(A, B)	A->print(B);
	#endif

	//PUBLIC FUNCTIONS
	void gsmEnableListenSerial()
	{
		#if GSM_SERIAL_MODE == SW_SERIAL
			gsmSerial.listen();
		#endif 
	}

	void gsmDisableListenSerial()
	{
		#if GSM_SERIAL_MODE == SW_SERIAL
			gsmSerial.ignore();
		#endif 
	}

	bool gsmIsConnectedToInternet()
	{
		return gsmStat == GSM_CONNECTED_WITH_INET;
	}

	bool gsmGetRequestHasTimeout()
	{
		return gsmGetStat != HTTP_NONE && gsmGetTimeoutMillis <= millis();
	}

	bool gsmSetupHasTimeout()
	{
		return gsmSetupTimeoutMillis > 0 && millis() >= gsmSetupTimeoutMillis && !(gsmStat == GSM_CONNECTED_WITH_INET);
	}

	bool gsmGetHasActiveRequest()
	{		
		return (gsmGetStat != HTTP_NONE);
	}

	void gsmReset()
	{	 
		memset(gsmInputString, 0, GSM_SERIAL_BUFFER_SIZE);
		gsmInputStringLength = 0;
		
		gsmStat = GSM_RESTARTING;
		
		long now = millis();
		gsmSetupTimeoutMillis = (now + GSM_SETUP_MAX_TIMEOUT);		
		gsmSetupPinTimeout = (now + GSM_SETUP_RESET_LOW_DELAY);
		gsmWaitAttachedLastCheckMillis = 0;
		gsmIsAttachedToNetwork = false;

		gsmGetTotalErrors = 0;
		gsmEnableListenSerial();
		
		FastGPIO::Pin<GSM_RESET_PIN>::setOutputLow();

		char *cmd;
		while(!gsmCommandQueue.isEmpty() && gsmCommandQueue.pull(&cmd))		
			delete cmd;		

		gsmSerial.end();
		gsmSerial.begin(GSM_BAUDRATE);
	}

	#if COMPILE_SERIAL_CONTROL && SERIAL_CONTROL_MODE >= SC_MID
		bool gsmHandleConfCmd(char *cmd, char *params[], uint8_t paramsLength)
		{               	  		
			if (utilsEquals_P(cmd, sReset))
				gsmReset();
			else if (utilsEquals_P(cmd, sWrite))
			{			
				gsmSerial.print(params[0]); 
				utilsPrintln_P(&gsmSerial, sLineFeedLineBreak);
			}
			else return false;

			return true;
		}
	#endif

	void checkGSM()
	{        		
		long now = millis();

		if (gsmSetupHasTimeout())//SETUP TIMEOUT HANDLER
		{
			if (CONFIG.get(PRINT_GSM_INFO))
				utilsPrintln_P(sGSMResetSetup);

			gsmReset();
			return;
		}		
		else if (gsmStat == GSM_RESTARTING && now >= gsmSetupPinTimeout)//RESTART DELAY
		{
			FastGPIO::Pin<GSM_RESET_PIN>::setOutputHigh();
			gsmStat = GSM_SETTING_UP_BAUDRATE;
		}
		else if (gsmStat == GSM_SETTING_UP_BAUDRATE && now >= gsmSetupPinTimeout)//RESTART DELAY
		{			
			FUNC_PRINT(GSM_STR("AT\r\n"));
			gsmSetupPinTimeout += 250; //250ms interval
		}
		else if (gsmStat == GSM_SETTING_NETWORK_WAIT_FOR_GSM_ATTACHED && (now - gsmWaitAttachedLastCheckMillis) > 1000)
		{			
			gsmWaitAttachedLastCheckMillis = now;
			FUNC_PRINT(GSM_STR("AT+CGATT?\r\n"));
		}
		
		//CHECK GET TIMEOUT
		if (gsmGetRequestHasTimeout())		
			gsmGetOnError(ERROR_GET_TIMEOUT, gsmGetPath);

		#if GSM_USE_SERIAL_INTERRUPTS == 0
			while (gsmSerial.available())
				gsmHandleSerial_ISR(gsmSerial.read());
		#endif

		char *cmd = NULL;
		if (gsmCommandQueue.pull(&cmd))
		{			
			gsmHandleCommand(cmd);
			delete cmd;
		}
	}

	//TO CHANGE SIM800L DEFAULT BAUD RATE:
	//AT+IPR=<rate>
	//AT&W

	//DISABLE ECHO
	//ATE0
	//AT&W
	bool setupGSM() //3.7v a 4.2v -> Funciona correctamente con 5v (POWERBANK)
	{        
		#if GSM_USE_SERIAL_INTERRUPTS
			gsmSerial.attachInterrupt(gsmHandleSerial_ISR);			
		#endif
				
		gsmReset();
  
		#if COMPILE_SERIAL_CONTROL && SERIAL_CONTROL_MODE >= SC_MID
			SerialController.addHandler(new SERIAL_HANDLER(sGSM, gsmHandleConfCmd));
		#endif
  
		return true;
	}

	bool gsmHTTPGet(const char *host, unsigned int port, char *path, char *body, void(*successCb)(char*), bool(*errorCb)(GSM_GET_ERROR, char*), bool forceRequest = false)
	{  
		if (!gsmIsConnectedToInternet())
			return false;

		if (gsmGetHasActiveRequest())
		{
			if (forceRequest)
				gsmHTTPAbort();
			else
				return false;
		}		

		gsmGetHost = host;
		gsmGetBody = body;
		gsmGetPort = port;
		gsmGetPath = path;		
		gsmGetOnSuccessCallback = successCb;
		gsmGetOnErrorCallback = errorCb;
		gsmGetTimeoutMillis = (millis() + GSM_GET_MAX_TIMEOUT);

		gsmHTTPGetStep(HTTP_INIT, (void*)host, (void*)&port, (void*)path);  

		return true;
	}

	bool gsmHTTPRetryRequest(bool forceRequest = false)
	{
		if (!gsmIsConnectedToInternet())
			return false;		

		gsmGetTimeoutMillis = (millis() + GSM_GET_MAX_TIMEOUT);
		
		gsmHTTPGetStep(HTTP_INIT, (void*)gsmGetHost, (void*)&gsmGetPort, (void*)gsmGetPath);
		
		return true;
	}

	void gsmHTTPAbort()
	{
		gsmHTTPClose();

		if (gsmGetOnErrorCallback != NULL)
			gsmGetOnErrorCallback(ERROR_REQUEST_ABORTED, gsmGetPath);
				
		gsmHTTPClear();
	}

	//PRIVATE FUNCTIONS
	static void gsmHTTPClose()
	{
		FUNC_PRINT(GSM_STR("AT+CIPCLOSE"));
		utilsPrintln_P(&gsmSerial, sLineFeedLineBreak);
		gsmSerial.flush();

		gsmGetStat = HTTP_NONE;
	}

	static void gsmHTTPClear()
	{
		#if USE_STATIC_MEM == 0
			if (gsmGetPath != NULL)
				free(gsmGetPath);
		#endif   
		
		gsmGetPath = NULL;
		gsmGetHost = NULL;
		gsmGetPort = 0;
		gsmGetOnSuccessCallback = NULL;
		gsmGetOnErrorCallback = NULL;
		gsmGetTimeoutMillis = 0;
	}

	static void gsmGetOnSuccess(char *path)
	{
		gsmGetTotalErrors = 0;

		if(gsmGetOnSuccessCallback != NULL)
			gsmGetOnSuccessCallback(path);
	}
  
	static void gsmGetOnError(GSM_GET_ERROR err, char *path)
	{
		gsmGetTotalErrors++;

		if(gsmGetTotalErrors >= GSM_GET_MAX_ERRORS_BEFORE_RESET)
		{
			if (CONFIG.get(PRINT_GSM_INFO))
				utilsPrintln_P(sGSMRequestMaxErrors);

			gsmReset();	
		}

		if (gsmGetOnErrorCallback != NULL && gsmGetOnErrorCallback(err, path)) //RETRY
		{
			gsmHTTPClose();

			if (gsmHTTPRetryRequest())
			{
				if(CONFIG.get(PRINT_GSM_INFO))
					utilsPrintln_P(sGSMGetRetry);
			}
			else
			{
				gsmGetOnErrorCallback(ERROR_CANNOT_RETRY_REQUEST, path);
				gsmHTTPClear();
			}
		}					
		else gsmHTTPAbort();
	}

	static void gsmHandleCommand(char *r)
	{
		if(CONFIG.get(PRINT_GSM_INFO))
		{
			utilsPrint_P(sGSM);
			cmdSerial.print(gsmStat);
			utilsPrint_P(sComma);
			cmdSerial.print(gsmGetStat);
			utilsPrint_P(sTwoDots);
			cmdSerial.println(r);
			cmdSerial.flush();
		}

		#if COMPILE_TRACCAR && TRACCAR_COMPILE_ALARM_MODE			
			if(FUNC_STRSTR(r, GSM_STR("NO CARRIER")) != NULL) //llamada colgada      
			{
				traccarToggleAlarmMode();
				if (CONFIG.get(PRINT_STATUS_INFO))
				{
					utilsPrint_P(sAlarm);
					utilsPrint_P(sSpace);
					utilsPrint_P(traccarIsAlarmModeEnabled() ? sOn : sOff);
				}
			}
			else 
		#endif

		if (FUNC_STRSTR(r, GSM_STR("PDP DEACT")) != NULL)
		{
			gsmReset();
			return;
		}
		else if (FUNC_STRSTR(r, GSM_STR("+CPIN: SIM PIN")) != NULL)
		{
			gsmStat = GSM_SETTING_PIN;
			FUNC_PRINT(GSM_STR("AT+CPIN="));
			gsmSerial.print(GSM_PIN);
			utilsPrintln_P(&gsmSerial, sLineFeedLineBreak);
		}
		else if (gsmStat == GSM_SETTING_UP_BAUDRATE && FUNC_STRSTR(r, GSM_STR("OK")))
		{
			gsmStat = GSM_WAITING_FOR_PIN_REQUEST;
		}		
		else if (gsmStat == GSM_SETTING_PIN && FUNC_STRSTR(r, GSM_STR("CPIN: READY")) != NULL)
		{
			gsmStat = GSM_SETTING_NETWORK_WAIT_FOR_GSM_ATTACHED;
			gsmWaitAttachedLastCheckMillis = millis();			
		}
		else if (gsmStat == GSM_SETTING_NETWORK_WAIT_FOR_GSM_ATTACHED && FUNC_STRSTR(r, GSM_STR("CGATT: 1")) != NULL)
		{
			gsmIsAttachedToNetwork = true;
		}
		else if (gsmStat == GSM_SETTING_NETWORK_WAIT_FOR_GSM_ATTACHED && FUNC_STRSTR(r, GSM_STR("OK")) != NULL && gsmIsAttachedToNetwork)
		{
			gsmStat = GSM_SETTING_NETWORK_SET_APN;
			FUNC_PRINT(GSM_STR("AT+CSTT=\""));
			FUNC_PRINT(GSM_STR(GSM_APN));
			FUNC_PRINT(GSM_STR("\""));

			#ifdef GSM_APN_USER
				FUNC_PRINT(GSM_STR(",\""));
				FUNC_PRINT(GSM_STR(GSM_APN_USER));
				FUNC_PRINT(GSM_STR("\""));
			#endif

			#ifdef GSM_APN_PASSWORD
				FUNC_PRINT(GSM_STR(",\""));	
				FUNC_PRINT(GSM_STR(GSM_APN_PASSWORD));
				FUNC_PRINT(GSM_STR("\""));
			#endif

			utilsPrintln_P(&gsmSerial, sLineFeedLineBreak);			
		}
		else if (gsmStat == GSM_SETTING_NETWORK_SET_APN && FUNC_STRSTR(r, GSM_STR("OK")) != NULL)
		{
			gsmStat = GSM_SETTING_NETWORK_BRING_UP_WIRELESS_GPRS;
			FUNC_PRINT(GSM_STR("AT+CIICR"));
			utilsPrintln_P(&gsmSerial, sLineFeedLineBreak);
		}
		else if (gsmStat == GSM_SETTING_NETWORK_BRING_UP_WIRELESS_GPRS && FUNC_STRSTR(r, GSM_STR("OK")) != NULL)
		{
			gsmStat = GSM_SETTING_NETWORK_GET_LOCAL_IP;
			FUNC_PRINT(GSM_STR("AT+CIFSR"));			
			utilsPrintln_P(&gsmSerial, sLineFeedLineBreak);
		}
		else if(gsmStat == GSM_SETTING_NETWORK_GET_LOCAL_IP && FUNC_STRSTR(r, GSM_STR(".")) != NULL)
		{
			#if GSM_DISABLE_LEDS
				gsmStat = GSM_DISABLING_LED;
				FUNC_PRINT(GSM_STR("AT+CNETLIGHT=0"));
				utilsPrintln_P(&gsmSerial, sLineFeedLineBreak);
			#else
				gsmStat = GSM_CONNECTED_WITH_INET;
				gsmSetupTimeoutMillis = 0; //DISABLE SETUP TIMEOUT
			#endif
		}
		#if GSM_DISABLE_LEDS
			else if(gsmStat == GSM_DISABLING_LED && FUNC_STRSTR(r, GSM_STR("OK")) != NULL)
			{			
				gsmStat = GSM_CONNECTED_WITH_INET;
				gsmSetupTimeoutMillis = 0; //DISABLE SETUP TIMEOUT
			}
		#endif	 
		else if(gsmStat == GSM_CONNECTED_WITH_INET && gsmGetStat != HTTP_NONE)
		{     
			switch(gsmGetStat)
			{
				case HTTP_INIT:
					if (FUNC_STRSTR(r, GSM_STR("ALREADY")) != NULL || FUNC_STRSTR(r, GSM_STR("CONNECT OK")) != NULL)
						gsmHTTPGetStep(HTTP_CONNECTED, NULL, NULL, NULL);					
					break;

				case HTTP_CONNECTED:
					if(r[0] == '>')
						gsmHTTPGetStep(HTTP_CONNECTED_REQUEST_LENGTH_SET, NULL, NULL, NULL);
					break;

				case HTTP_CONNECTED_REQUEST_LENGTH_SET:
					if(FUNC_STRSTR(r, GSM_STR("OK")) != NULL)
					{ 				
						gsmGetOnSuccess(gsmGetPath);
						gsmHTTPGetStep(HTTP_CONNECTED_REQUEST_SENT, NULL, NULL, NULL);
						gsmGetStat = HTTP_NONE;
					}
					break;
			}    
		}

		gsmSerial.flush();
	}
	
	static void gsmHandleSerial_ISR(uint8_t c)
	{		
		if (gsmInputStringLength >= (GSM_SERIAL_BUFFER_SIZE-1))	
			gsmInputStringLength = 0;

		if (c == '\r') //IGNORE
			return;		
		
		if (c == '\n' || c == '>')  //END OF LINE     
		{
			if(c == '>')
				gsmInputString[gsmInputStringLength++] = c;
			else
				if(gsmInputStringLength <= 0)
					return;

			char *cmd = new char[gsmInputStringLength+1];
			memcpy(cmd, gsmInputString, gsmInputStringLength);
			cmd[gsmInputStringLength] = '\0';
			
			gsmCommandQueue.push(&cmd);
			gsmInputStringLength = 0;
		}
		else gsmInputString[gsmInputStringLength++] = c;
	}

	static void gsmHTTPGetStep(HTTP_GET_STATUS stat, void *param1, void *param2, void *param3)
	{         
		Stream *s = &gsmSerial;

		switch(stat)
		{
			case HTTP_INIT:		 
				FUNC_PRINT_S(s, GSM_STR("AT+CIPSTART=\"TCP\",\""));
				s->print((const char*)param1);
				FUNC_PRINT_S(s, GSM_STR("\",\""));
				s->print(*((unsigned int*)param2));
				FUNC_PRINT_S(s, GSM_STR("\""));
				utilsPrintln_P(s, sLineFeedLineBreak);
				break;
    
			case HTTP_CONNECTED: //SET REQUEST LENGTH
				FUNC_PRINT_S(s, GSM_STR("AT+CIPSEND"));
				utilsPrintln_P(s, sLineFeedLineBreak);
				break;

			case HTTP_CONNECTED_REQUEST_LENGTH_SET: //SEND REQUEST     
				FUNC_PRINT_S(s, GSM_STR("GET "));
				s->print(gsmGetPath);
				FUNC_PRINT_S(s, GSM_STR(" HTTP/1.0"));
				utilsPrintln_P(s, sLineFeedLineBreak);
			
				if (gsmGetBody != NULL)
				{					
					utilsPrintln_P(s, sLineFeedLineBreak);
					s->print(gsmGetBody);
				}

				utilsPrintln_P(s, sLineFeedLineBreak);
				utilsPrintln_P(s, sLineFeedLineBreak);
				s->write(0x1A);
				break;
      
			case HTTP_CONNECTED_REQUEST_SENT: //END REQUEST
				gsmHTTPClear();
				break;
		}
    
		gsmGetStat = stat;		
		s->flush();
	}

#endif