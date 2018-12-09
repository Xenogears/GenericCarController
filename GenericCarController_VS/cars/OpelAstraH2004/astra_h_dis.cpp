#include "../../configuration.h"

#if CAR_MODEL == OPEL_ASTRA_H_2004_CAR && COMPILE_CAN && COMPILE_DIS
	#include "astra_h.h"
	#include "../../cmd_serial.h"

	const CAN_COMMAND CMD_DIS_SET_MODE = CAN_COMMAND(ASTRA_H_DIS_SET_ID, 0x07, 0x50, 0x00, 0x04, 0x03, 0x01, 0x07); //data[7] = Mode	

	#if USE_STATIC_MEM
		static char astraHDISText[DIS_MAX_TEXT_LENGTH]; //FULL TEXT
	#else
		static char *astraHDISText = NULL; //FULL TEXT
	#endif
	static uint8_t astraHDISTextPos = 0;
	static uint8_t astraHDISTextTotalLength = 0;    
	static uint8_t astraHDISTextRequestedLength = 0;

	static uint8_t astraHDISTextTimeoutTicksCount = 0;
	static uint8_t astraHDISTextTimeoutTicksLimit = 0;

	static uint32_t astraHDISRandomSeed = 0;
	static uint8_t  astraHDISLastRandomNumber = 0xFF;

	#if COMPILE_EEPROM_CONFIG == 0
		const char *DIS_MESSAGES[] { DIS_MESSAGE };  
	#endif

	static bool astraHDISTickTimerEnabled = false;
	
	#if DIS_USE_TIMER_ONE == 0
		static unsigned long astraHDISNextTickMillis = 0;
	#endif

	volatile static uint8_t astraHDISTicks = 0;

	//PRIVATE DECLARATIONS
	static void astraHDISPerformTick();
	static void astraHDISSetTextBase(uint8_t len, uint8_t tickTimeout);
	static uint8_t astraHDISGetRandomTextsCount();
	static bool astraHDISShowText(CAN_CONTROLLER *can, const char *inMsg);
	static uint8_t astraHDISRandom(uint8_t min, uint8_t max, boolean notRepeated);
	static uint8_t astraHDISGetRandomTextsCount();
	static bool astraHDISMarqueeIsFirstChar();
	static bool astraHDISMarqueeIsLastChar();
	static bool astraHDISMarqueeHasRemainingText();
	static bool astraHDISMarqueeCheck();

	//PUBLIC FUNCTIONS
	bool astraHDISSetup()
	{
		#if defined(ARDUINO_RANDOM_PIN) && ARDUINO_RANDOM_PIN != A6 && ARDUINO_RANDOM_PIN != A7
			pinMode(ARDUINO_RANDOM_PIN, INPUT);
		#endif  

		astraHDISStop();
		astraHDISTicks = 0;
	}

	void astraHDISStart()
	{		
		#if DIS_USE_TIMER_ONE
			Timer1.initialize(DIS_TICK_INTERVAL * 1000L);
			Timer1.attachInterrupt(astraHDISPerformTick);
			Timer1.start();
		#else               
			astraHDISNextTickMillis = millis();
		#endif

		astraHDISTickTimerEnabled = true;

		if (CONFIG.get(ENABLE_RANDOM_TEXTS) && astraHDISGetRandomTextsCount() > 0)
			astraHDISSetRandomText(&DEFAULT_CAN);
		else
			astraHDISSetDefaultText(&DEFAULT_CAN);
	}

	void astraHDISStop()
	{
		#if DIS_USE_TIMER_ONE
			Timer1.stop();
		#endif

		astraHDISTickTimerEnabled = false;
	}

	void checkDIS()
	{
		#if DIS_USE_TIMER_ONE == 0
			unsigned long now = millis();
			if (astraHDISTickTimerEnabled && astraHDISNextTickMillis <= now)
			{
				astraHDISNextTickMillis = (now + DIS_TICK_INTERVAL);
				astraHDISPerformTick();
			}
		#endif
	}

	bool astraHDISIsRunning()
	{
		return astraHDISTickTimerEnabled;
	}	
	
	void astraHDISSetRandomText(CAN_CONTROLLER *can, uint8_t tickTimeout = DIS_TEXT_TIMEOUT_TICKS, bool request = true)
	{
		uint8_t randN = astraHDISRandom(0, astraHDISGetRandomTextsCount(), true);   

		#if COMPILE_EEPROM_CONFIG   
			uint16_t entry = eepromGetEntry(randN);
			astraHDISSetTextBase(eepromGetEntryDataSize(entry), tickTimeout);
    
			#if USE_STATIC_MEM  == 0
				astraHDISText = (char*)malloc(astraHDISTextLength + 1);
			#endif

			eepromReadEntry(entry, (char*)&astraHDISText);

			if (request)
				astraHDISRequestText(can);
		#else  
			astraHDISSetText(can, DIS_MESSAGES[randN], request);
		#endif		
	}

	void astraHDISSetText(CAN_CONTROLLER *can, char *msg, uint8_t tickTimeout = DIS_TEXT_TIMEOUT_TICKS, bool request = true)
	{      
		astraHDISSetTextBase(strlen(msg), tickTimeout);

		#if USE_STATIC_MEM == 0
			if(astraHDISText != NULL)
				free(astraHDISText);

			astraHDISText = (char*)malloc(astraHDISTextLength + 1);
			memcpy(astraHDISText, msg, astraHDISTextLength);
		#else
			memcpy(astraHDISText, msg, astraHDISTextTotalLength);
		#endif 

		if (request)
			astraHDISRequestText(can);
	} 

	void astraHDISSetText_P(CAN_CONTROLLER *can, const PROGMEM char *msg, uint8_t tickTimeout = DIS_TEXT_TIMEOUT_TICKS, bool request = true)
	{		
		astraHDISSetTextBase(strlen_P(msg), tickTimeout);

		#if USE_STATIC_MEM == 0
			if (astraHDISText != NULL)
				free(astraHDISText);

			astraHDISText = (char*)malloc(astraHDISTextLength);
		#endif 

		memcpy_P(astraHDISText, msg, astraHDISTextTotalLength);

		if (request)
			astraHDISRequestText(can);
	}

	void astraHDISSetText(CAN_CONTROLLER *can, String *msg, uint8_t tickTimeout = DIS_TEXT_TIMEOUT_TICKS, bool request = true)
	{      
		return astraHDISSetText(can, (char*)msg->c_str(), tickTimeout, request);
	}

	void astraHDISSetDefaultText(CAN_CONTROLLER *can = &DEFAULT_CAN)
	{
		#if COMPILE_SETTINGS_MODE && COMPILE_CAR_STATUS_MONITOR
			if (ASTRA_H_SETTINGS_MODE_ENABLED)
				astraHPrintStatus(can);
			else
		#endif


		#if DIS_MESSAGE_USES_PROGMEM
			astraHDISSetText_P(can, DIS_MESSAGE, DIS_TEXT_TIMEOUT_TICKS);	
		#else
			astraHDISSetText(can, DIS_MESSAGE, DIS_TEXT_TIMEOUT_TICKS);
		#endif
	}

	bool astraHDISRequestText(CAN_CONTROLLER *can)
	{    
		uint8_t remainingLen = (astraHDISTextTotalLength - astraHDISTextPos);
		astraHDISTextRequestedLength = (remainingLen > DIS_MAX_CHARACTERS ? DIS_MAX_CHARACTERS : remainingLen);

		CAN_COMMAND req = CAN_COMMAND(	ASTRA_H_DIS_SET_ID, 
										0x10, 
										(astraHDISTextRequestedLength * 2) + 3 + 3, //MAIN LENGTH (Chars x2 (Unicode16) + 6 (headers))
										0x40, 0x00, //COMMAND (0x4000, 0x5000, 0xA000, 0xC000)
										(astraHDISTextRequestedLength * 2) + 3, //SUBFUNCTION LENGTH (Chars x2 (Unicode16) + 3 (header))
										0x03, //DISPLAY (Only Works 0x03 WITH BID)
										0x10, //LINE (Only Works 0x10 WITH BID), Maybe 0x90?
										astraHDISTextRequestedLength); //CHARS LENGTH
		bool res = can->sendCmd(&req);

		#if DIS_SIMULATE_BC_RESPONSE
			DEFAULT_CAN.pushRXCmd((CAN_COMMAND*)&CMD_DIS_RESPONSE_SET_TEXT);
		#endif     

		return res;
	}

	bool astraHDISShowText(CAN_CONTROLLER *can)
	{
		return astraHDISShowText(can, astraHDISText);
	}

	//PRIVATE FUNCTIONS
	static void astraHDISPerformTick()
	{
		astraHDISTicks++;
	
		if (astraHDISMarqueeCheck()) //STEP
			astraHDISRequestText(&DEFAULT_CAN);
	}

	static void astraHDISSetTextBase(uint8_t len, uint8_t tickTimeout)
	{
		astraHDISTextPos = 0;
		astraHDISTextTotalLength = (len > DIS_MAX_TEXT_LENGTH ? DIS_MAX_TEXT_LENGTH : len);
		astraHDISTicks = 0;
		astraHDISTextTimeoutTicksCount = 0;
		astraHDISTextTimeoutTicksLimit = tickTimeout;
	}

	static uint8_t astraHDISGetRandomTextsCount()
	{
		#if COMPILE_EEPROM_CONFIG 
			return EEPROM_ENTRY_COUNT;
		#else  
			return sizeof(DIS_MESSAGES) / sizeof(const char*);
		#endif
	}

	static bool astraHDISShowText(CAN_CONTROLLER *can, const char *inMsg)
	{
		if (inMsg == NULL || astraHDISTextRequestedLength <= 0)
			return false;		

		const char *msg = (inMsg + astraHDISTextPos);
		uint8_t msgLen = astraHDISTextRequestedLength;

		uint8_t rem = ((astraHDISTextRequestedLength * 2) % 7);
		uint8_t div = ((astraHDISTextRequestedLength * 2) / 7);
		uint8_t numCmds = (rem ? div + 1 : div);
		
		astraHDISTextRequestedLength = 0;
		
		#if CAN_ENABLE_TX_QUEUE && CAN_TX_QUEUE_LENGTH > 0 && !DIS_SEND_COMMANDS_IMMEDIATELY
			if (can->getTXQueueAvailableSlots() < numCmds)
				return false;
		#endif			


		CAN_COMMAND cmd = CAN_COMMAND(ASTRA_H_DIS_SET_ID);
		cmd.can_dlc = 8;

		for (uint8_t i = 0, c = 0; i < numCmds; i++)
		{
			bool cmdEven = !(i & B00000001);

			cmd.data[0] = 0x21 + i;

			for (uint8_t j = 1; j <= 7; j++)
			{
				bool charEven = !(j & B00000001);

				if (charEven == !cmdEven)
					cmd.data[j] = 0x00;
				else if(c >= msgLen)
					cmd.data[j] = 0x00;
				else
					cmd.data[j] = msg[c++];		
			}

			if (!can->sendCmd(&cmd, (DIS_SEND_COMMANDS_IMMEDIATELY ? true : false)))
				return false;
		}

		if (CONFIG.get(PRINT_DISPLAY_INFO))
		{
			utilsPrint_P(sSent);
			utilsPrint_P(sSpace);
			utilsPrint_P(sDIS);
			utilsPrint_P(sTwoDots);
			utilsPrint_P(sSpace);
			cmdSerial.print(millis());
			utilsPrint_P(sSpaceCommaSpace);

			for (uint8_t i = 0; i < msgLen; i++)
				cmdSerial.write(msg[i]);

			cmdSerial.println();
		}
		
		return true;
	}

	static uint8_t astraHDISRandom(uint8_t min, uint8_t max, boolean notRepeated) //[min,max) || [min,max-1]
	{ 
		uint8_t r;
		do
		{
			#ifdef ARDUINO_RANDOM_PIN
				astraHDISRandomSeed += analogRead(ARDUINO_RANDOM_PIN);       
			#endif
    
			astraHDISRandomSeed += micros();     
    
			randomSeed(astraHDISRandomSeed);
			r = random(min, max);
		}
		while((max - min) > 1 && notRepeated && r == astraHDISLastRandomNumber);
  
		astraHDISLastRandomNumber = r;        
		return r;   
	}

	static bool astraHDISMarqueeIsFirstChar(){ return (astraHDISTextPos == 0); }
	static bool astraHDISMarqueeIsLastChar(){ return (astraHDISTextPos == (astraHDISTextTotalLength - DIS_MAX_CHARACTERS)); }
	static bool astraHDISMarqueeHasRemainingText(){ return (astraHDISTextPos < (astraHDISTextTotalLength - DIS_MAX_CHARACTERS)); }
	static bool astraHTextRequiresMarquee(){ return (astraHDISTextTotalLength > DIS_MAX_CHARACTERS); }

	/*
		Se comprueba si ha ocurrido el delay correspondiente. Delays:
		- DIS_REFRESH_TICKS -> Refresco del texto. Para textos que quepan en la pantalla y deban refrescarse con un mínimo establecido por la pantalla.
		- DIS_TEXT_START_END_TICKS -> Retardo que ocurre cuando se visualiza el primer o el último carácter en pantalla.
		- DIS_TEXT_STEP_TICKS -> Retardo entre caracteres.

		Si ocurre el retardo almacenamos el total de ticks ocurridos con el texto actual y reiniciamos el contador de ticks. De esta forma podemos saber cuando cambiar los textos
		y además controlar los retardos para cada caracter.

		Entonces si queda texto por mostrar (un texto largo), simplemente aumentamos la posición del texto en uno para realizar el efecto de marquesina.
		Si no queda texto por mostrar (Esto es porque hemos llegado al final o porque el texto no requiere de marquesina) entonces:
			- Si el texto ya se ha mostrado el tiempo que le corresponde:
				- Reiniciamos el contador de tiempo del texto actual y establecerá el texto que corresponda según las opciones (random o por defecto).
			- Si el texto no se ha mostrado el tiempo que le corresponde
				- Volvemos al caracter 0 (Si el texto cabe sin marquesina se quedará igual y si no cabe volverá a empezar la marquesina).

		Este método devuelve true si el texto debe mandarse al módulo de la pantalla o false en caso contrario.
	*/
	static bool astraHDISMarqueeCheck()
	{      
		uint8_t del;    
		if(!astraHTextRequiresMarquee())del = DIS_REFRESH_TICKS;
		else if(astraHDISMarqueeIsFirstChar() || astraHDISMarqueeIsLastChar()) del = DIS_TEXT_START_END_TICKS;
		else del = DIS_TEXT_STEP_TICKS;
  
		if(astraHDISTicks > del)
		{ 
			astraHDISTextTimeoutTicksCount += astraHDISTicks;
			astraHDISTicks = 0;          
    
			if(!astraHDISMarqueeHasRemainingText())     
			{
				if (astraHDISTextTimeoutTicksCount >= astraHDISTextTimeoutTicksLimit)
				{
					astraHDISTextTimeoutTicksCount = 0;

					if (CONFIG.get(ENABLE_RANDOM_TEXTS) && astraHDISGetRandomTextsCount() > 0)
						astraHDISSetRandomText(&DEFAULT_CAN, DIS_TEXT_TIMEOUT_TICKS, false);
					else
						astraHDISSetDefaultText(&DEFAULT_CAN);
				}
				else astraHDISTextPos = 0;
			}
			else astraHDISTextPos++;               
		
			return true;
		}     
		else return false;
	}
#endif