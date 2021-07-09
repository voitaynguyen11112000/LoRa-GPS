#ifndef _BUZZER_H_
#define _BUZZER_H_

#include "stm32f10x.h"

void init_buzzer(void);
void BuzzerOn(uint8_t index);
void BuzzerOff(uint8_t index);

#endif 
