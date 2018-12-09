#include "configuration.h"

CONFIG_BASE_TYPE CONFIGURATION::getAll()
{
	return params;
}

bool CONFIGURATION::get(CONFIG_BASE_TYPE i)
{
	return params & i;
}

void CONFIGURATION::clear()
{
	params = 0;
}

void CONFIGURATION::setDefaults()
{
	params = ~0; //All enabled
}

void CONFIGURATION::set(CONFIG_BASE_TYPE value)
{
	params = value;
}

void CONFIGURATION::set(uint8_t shift, uint8_t value)
{
	shift <<= 3; //*8
	params &= ~(0xFF << shift);
	params |= (value << shift);
}

void CONFIGURATION::set(CONFIG_BASE_TYPE mask, bool value)
{
	params &= ~mask; //params[bit] = 0;
	if (value)
		params |= mask; //params[bit] = 1;
}
