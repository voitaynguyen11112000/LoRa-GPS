#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "stm32f10x.h"

extern uint16_t key_code[8];

#define BUTTON_SCK_GPIO				GPIOB
#define BUTTON_SCK_GPIO_CLK			RCC_APB2Periph_GPIOB
#define BUTTON_SCK_Pin				GPIO_Pin_13							

#define BUTTON_LOAD_GPIO			GPIOB
#define BUTTON_LOAD_GPIO_CLK		RCC_APB2Periph_GPIOB
#define BUTTON_LOAD_Pin				GPIO_Pin_2	

#define BUTTON_DATA_GPIO		    GPIOB
#define BUTTON_DATA_GPIO_CLK		RCC_APB2Periph_GPIOB
#define BUTTON_DATA_Pin				GPIO_Pin_1	

#define	BUTTON_SCK(x)          		GPIO_WriteBit(BUTTON_SCK_GPIO,BUTTON_SCK_Pin,(x)?Bit_SET:Bit_RESET)
#define	BUTTON_LOAD(x)              GPIO_WriteBit(BUTTON_LOAD_GPIO,BUTTON_LOAD_Pin,(x)?Bit_SET:Bit_RESET)

void init_button(void);
void scan_button(void);

#endif 
