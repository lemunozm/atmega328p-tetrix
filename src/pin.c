#include "pin.h"

#include <avr/io.h>

void pin_mode(uint8_t pin, uint8_t dir)
{
    if(pin < 8)
    {
        DDRD = (dir == PIN_OUTPUT) ? DDRD | (1 << pin) : DDRD & ~(1 << pin);
    }
    else if(pin < 16)
    {
        DDRB = (dir == PIN_OUTPUT) ? DDRB | (1 << (pin - 8)) : DDRB & ~(1 << (pin - 8));
    }
    else if(pin < 24)
    {
        DDRC = (dir == PIN_OUTPUT) ? DDRC | (1 << (pin - 16)) : DDRC & ~(1 << (pin - 16));
    }
}

void digital_pin_out(uint8_t pin, uint8_t value)
{
    if(pin < 8)
    {
        PORTD = (value == HIGH) ? PORTD | (1 << pin) : PORTD & ~(1 << pin);
    }
    else if(pin < 16)
    {
        PORTB = (value == HIGH) ? PORTB | (1 << (pin - 8)) : PORTB & ~(1 << (pin - 8));
    }
}

uint8_t digital_pin_in(uint8_t pin)
{
    if(pin < 8)
    {
        return !!(PIND & (1 << pin));
    }
    else if(pin < 16)
    {
        return !!(PINB & (1 << (pin - 8)));
    }

    return LOW;
}


