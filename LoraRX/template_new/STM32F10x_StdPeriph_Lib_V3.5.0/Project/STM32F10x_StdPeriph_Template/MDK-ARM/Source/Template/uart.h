#ifndef _UART_H_
#define _UART_H_

#include "stm32f10x.h"

void USART1_Init(uint32_t baudrate);
void USART1_SendChar(uint8_t data);
void USART1_SendString(char* str);
void USART1_SendNum(long num);

void USART2_Init(uint32_t baudrate);
void USART2_SendChar(uint8_t data);
void USART2_SendString(char* str);
void USART2_SendNum(long num);

#endif 
