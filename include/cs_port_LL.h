#ifndef GPIO_LL_H_
#define GPIO_LL_H_

#include <Arduino.h>

#define PORTB_BITMASK       (uint8_t)((1 << DDB5) | (1 << DDB5))
#define PORTC_BITMASK       (uint8_t)((1 << DDC6) | (1 << DDC7))


void CSportInit(void);
void CSportSet(uint8_t state);

#endif