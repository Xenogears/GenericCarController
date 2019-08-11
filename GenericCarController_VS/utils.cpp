//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#include "utils.h"
#include "cmd_serial.h"

int utilsFreeRAM() 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

bool utilsResetSystem()
{    
	cmdSerialFlush();

	#if POW_ENABLE_WATCHDOG
		while (1);
	#elif ARDUINO_RESET_PIN				
		FastGPIO::Pin<ARDUINO_RESET_PIN>::setOutputLow();
		return true;
	#endif	

	return false;
}

bool utilsEndsWith(const char *str1, const char *str2)
{
	int8_t len1 = strlen(str1);
	int8_t len2 = strlen(str2);

	return utilsEquals(str1 + len1 - len2, str2);
}

bool utilsStartWith(const char *str1, const char *str2)
{
	return (strncmp(str1, str2, strlen(str2)) == 0);
}

bool utilsEquals(const char *str1, const char *str2)
{
	return (strcmp(str1, str2) == 0);
}

bool utilsEndsWith_P(const char *str1, const PROGMEM char *str2)
{
	int8_t len1 = strlen(str1);
	int8_t len2 = strlen_P(str2);

	return utilsEquals_P(str1 + len1 - len2, str2);
}

bool utilsStartWith_P(const char *str1, const PROGMEM char *str2)
{
	return (strncmp_P(str1, str2, strlen_P(str2)) == 0);
}

bool utilsEquals_P(const char *str1, const PROGMEM char *str2)
{
	return (strcmp_P(str1, str2) == 0);
}


uint8_t utilsFirstIndexOf(const char *str1, const char *str2)
{
	char *res = strstr(str1, str2);
	if (res == NULL)
		return -1;
	else
		return (res - str1);
}

uint8_t utilsFirstIndexOf_P(const char *str1, const PROGMEM char *str2)
{
	char *res = strstr_P(str1, str2);
	if (res == NULL)
		return -1;
	else
		return (res - str1);
}




uint8_t hexStrToByte(char* string, uint32_t *out)
{
	if (string == NULL)
		return NULL;

	size_t slength = strlen(string);
	if ((slength % 2) != 0) // must be even
		return NULL;

	size_t index = 0;
	while (index < slength)
	{
		char c = string[index];
		int value = 0;
		if (c >= '0' && c <= '9')
			value = (c - '0');
		else if (c >= 'A' && c <= 'F')
			value = (10 + (c - 'A'));
		else if (c >= 'a' && c <= 'f')
			value = (10 + (c - 'a'));
		else
			return NULL;

		*out |= (value << ((slength - 1 - index) * 4));

		index++;
	}

	return slength;
}