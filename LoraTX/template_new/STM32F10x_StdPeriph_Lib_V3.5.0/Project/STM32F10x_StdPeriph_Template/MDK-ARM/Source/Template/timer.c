#include "timer.h"

unsigned int timer_value = 0;
unsigned char flag_timer = 0;
unsigned int timer_cnt = 0;
unsigned int time_MUL = 1;

void init_timer(void)
{
	if (SysTick_Config(SystemCoreClock / 1000)) //1ms
    { 
        /* Capture error */ 
        while (1);
    }
}

void SetTimer_ms(int time)
{
	time_MUL = time;
	timer_cnt = 0;
	flag_timer = 0;
}

void timer_isr(void)
{
	timer_cnt ++;
	if (timer_cnt >= time_MUL)
	{
		timer_cnt = 0;
		flag_timer = 1;
	}
}
