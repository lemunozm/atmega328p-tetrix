#ifndef _SDK_PIN_H_
#define _SDK_PIN_H_

#include <stdint.h>

#define D_PIN_0 0
#define D_PIN_1 1
#define D_PIN_2 2
#define D_PIN_3 3
#define D_PIN_4 4
#define D_PIN_5 5
#define D_PIN_6 6
#define D_PIN_7 7
#define D_PIN_8 8
#define D_PIN_9 9
#define D_PIN_10 10
#define D_PIN_11 11
#define D_PIN_12 12
#define D_PIN_13 13
#define A_PIN_0 16
#define A_PIN_1 17
#define A_PIN_2 18
#define A_PIN_3 19
#define A_PIN_4 20
#define A_PIN_5 21
#define UNUSED_PIN 64

#define PIN_OUTPUT 1
#define PIN_INPUT 0
void pin_mode(uint8_t pin, uint8_t dir);

#define HIGH 1
#define LOW 0
void digital_pin_out(uint8_t pin, uint8_t value);
uint8_t digital_pin_in(uint8_t pin);

#endif // _SDK_PIN_H_
