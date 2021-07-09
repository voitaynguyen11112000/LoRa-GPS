#include "stm32f10x_it.h"
#include "timer.h"
#include "uart.h"


void SysTick_Handler(void)
{
    timer_isr();
}
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART1_IRQ_Handler();
	}
}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART2_IRQ_Handler();
	}
}
