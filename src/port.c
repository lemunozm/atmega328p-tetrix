#include "port.h"

#include <avr/io.h>

void port_mode(uint8_t port, uint8_t dir)
{
    if (port == PORT_0)
    {
        DDRD = dir;
    }
    else if(port == PORT_1)
    {
        DDRB = dir;
    }
}

void digital_port_out(uint8_t port, uint8_t value)
{
    if (port == PORT_0)
    {
        PORTD = value;
    }
    else if(port == PORT_1)
    {
        PORTB = value;
    }
}

uint8_t digital_port_in(uint8_t port)
{
    if (port == PORT_0)
    {
        return PIND;
    }
    else if(port == PORT_1)
    {
        return PINB;
    }

    return 0;
}
