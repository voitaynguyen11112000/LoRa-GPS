#ifndef _UART_H_
#define _UART_H_

#include "stm32f10x.h"
#define USART1_GPIO             GPIOA
#define USART1_GPIO_CLK         RCC_APB2Periph_GPIOA
#define USART1_RxPin            GPIO_Pin_10
#define USART1_TxPin            GPIO_Pin_9

#define USART2_GPIO             GPIOA
#define USART2_GPIO_CLK         RCC_APB2Periph_GPIOA
#define USART2_RxPin            GPIO_Pin_3
#define USART2_TxPin            GPIO_Pin_2
extern uint8_t uart_receive1;
extern uint8_t flag_uart1;

extern uint8_t uart_receive2;
extern uint8_t flag_uart2;

extern uint8_t dataOk[128];
extern uint16_t dem;

void USART2_Init(uint32_t baudrate);
void USART2_SendChar(uint8_t data);
void USART2_IRQ_Handler(void);

void USART1_Init(uint32_t baudrate);
void USART1_SendChar(uint8_t data);
void USART1_IRQ_Handler(void);
void USART1_SendFloat(float data);
#endif 
