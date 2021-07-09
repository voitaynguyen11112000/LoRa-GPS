#include "delay.h"

void delay_ms(uint16_t value)
{
    volatile uint16_t i,j;
    for (i = 0; i < value; i++)
        for (j = 0; j < 5000; j++);
}

void delay_us(uint16_t value)
{
    volatile uint16_t i,j;
    for (i = 0; i < value; i++)
        for (j = 0; j < 5; j++);
}
