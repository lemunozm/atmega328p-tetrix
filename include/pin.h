#ifndef _SDK_PIN_H_
#define _SDK_PIN_H_

#include <stdint.h>

#define DIGITAL_0 0
#define DIGITAL_1 1
#define DIGITAL_2 2
#define DIGITAL_3 3
#define DIGITAL_4 4
#define DIGITAL_5 5
#define DIGITAL_6 6
#define DIGITAL_7 7
#define DIGITAL_8 8
#define DIGITAL_9 9
#define DIGITAL_10 10
#define DIGITAL_11 11
#define DIGITAL_12 12
#define DIGITAL_13 13
#define ANALOG_0 16
#define ANALOG_1 17
#define ANALOG_2 18
#define ANALOG_3 19
#define ANALOG_4 20
#define ANALOG_5 21
#define UNUSED_PIN 64

#define PIN_OUTPUT 1
#define PIN_INPUT 0
void pin_mode(uint8_t pin, uint8_t dir);

#define HIGH 1
#define LOW 0
void digital_pin_out(uint8_t pin, uint8_t value);
uint8_t digital_pin_in(uint8_t pin);

#define MSB 1
#define LSB 0
void shift_out(uint8_t data_pin, uint8_t clock_pin, uint8_t bit_order, uint8_t value);

#endif // _SDK_PIN_H_
