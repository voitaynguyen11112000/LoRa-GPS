#ifndef _TIMER_H_
#define _TIMER_H_

#include "stm32f10x.h"

extern unsigned char flag_timer;
    
void init_timer(void);
void timer_isr(void);
void SetTimer_ms(int time);

#endif 
