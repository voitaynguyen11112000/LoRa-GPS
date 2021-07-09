#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include "stm32f10x.h"

#define RELAY_OUTPUT_GPIO             		GPIOA
#define RELAY_OUTPUT_GPIO_CLK         		RCC_APB2Periph_GPIOA
#define RELAY_OUTPUT_Pin            		GPIO_Pin_8

#define MOSFET_OUTPUT_GPIO             		GPIOB
#define MOSFET_OUTPUT_GPIO_CLK         		RCC_APB2Periph_GPIOB
#define MOSFET_OUTPUT_Pin            		GPIO_Pin_14

void init_output(void);
void RelayOutputOn(void);
void RelayOutputOff(void);
void MosfetOutputOn(void);
void MosfetOutputOff(void);
#endif 
