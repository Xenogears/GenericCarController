#include "serial_handler.h"

SERIAL_HANDLER::SERIAL_HANDLER(const PROGMEM char *t, bool (*h)(char*, char**, uint8_t)) : type(t), handler(h)
{

}    
