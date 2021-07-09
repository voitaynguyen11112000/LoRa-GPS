#ifndef _LED_H_
#define _LED_H_

#include "stm32f10x.h"

void init_led(void);
void LedTurnOn(uint8_t index);
void LedTurnOff(uint8_t index);

#endif 
