#include "../../configuration.h"

#if COMPILE_ALARM_LED && CAR_MODEL == OPEL_ASTRA_H_2004_CAR
	#include "astra_h_alarm_led.h"

	volatile bool astraHAlarmLEDIsOn = false;
	static volatile uint8_t astraHAlarmLEDTicks = 0;

	static void astraHAlarmLED_ISR();

	#if ALARM_LED_USE_TIMER_ONE == 0
		static bool astraHAlarmLEDEnabled = false;
		static unsigned long astraHAlarmLEDLastTickMillis = 0;

		void astraHAlarmLEDCheck()
		{
			if (astraHAlarmLEDEnabled)
			{
				unsigned long m = millis();
				if ((m - astraHAlarmLEDLastTickMillis) >= ALARM_LED_TICK_INTERVAL)
				{
					astraHAlarmLEDLastTickMillis = m;
					astraHAlarmLED_ISR();
				}
			}
		}
	#endif

	void astraHAlarmLEDSetOff()
	{
		FastGPIO::Pin<ALARM_LED_PIN>::setOutputHigh();
		astraHAlarmLEDIsOn = false;
	}

	void astraHAlarmLEDSetOn()
	{
		FastGPIO::Pin<ALARM_LED_PIN>::setOutputLow();
		astraHAlarmLEDIsOn = true;
	}

	bool astraHAlarmLEDSetup()
	{		
		astraHAlarmLEDIsOn = false;
		FastGPIO::Pin<ALARM_LED_PIN>::setOutput(HIGH);

		astraHAlarmLEDTicks = 0; 

		#if ALARM_LED_USE_TIMER_ONE
			Timer1.initialize(ALARM_LED_TICK_INTERVAL * 1000L);
			Timer1.attachInterrupt(astraHAlarmLED_ISR);		
		#endif
			
		astraHAlarmLEDEnable();

		return true;
	}

	void astraHAlarmLEDEnable()
	{
		#if ALARM_LED_USE_TIMER_ONE
			Timer1.start();
		#else
			astraHAlarmLEDEnabled = true;
		#endif	  
	}

	void astraHAlarmLEDDisable()
	{
		#if ALARM_LED_USE_TIMER_ONE
			Timer1.stop();
		#else
			astraHAlarmLEDEnabled = false;
		#endif	  
	}

	static void astraHAlarmLED_ISR()
	{
		if(!CONFIG.get(ENABLE_ALARM_LED))
			return;

		astraHAlarmLEDTicks++;      
		if(astraHAlarmLEDIsOn && astraHAlarmLEDTicks >= ALARM_LED_ON_TICKS)
		{
			astraHAlarmLEDSetOff();
			astraHAlarmLEDTicks = 0;
		}
		else if(astraHAlarmLEDTicks >= ALARM_LED_OFF_TICKS)
		{
			astraHAlarmLEDSetOn();    
			astraHAlarmLEDTicks = 0;
		}
	}
#endif