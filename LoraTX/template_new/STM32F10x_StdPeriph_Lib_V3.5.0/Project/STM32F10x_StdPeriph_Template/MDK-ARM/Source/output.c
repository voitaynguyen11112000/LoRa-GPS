#include "output.h"

void init_output(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RELAY_OUTPUT_GPIO_CLK, ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = RELAY_OUTPUT_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RELAY_OUTPUT_GPIO, &GPIO_InitStructure);
    
    RCC_APB2PeriphClockCmd(MOSFET_OUTPUT_GPIO_CLK, ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = MOSFET_OUTPUT_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MOSFET_OUTPUT_GPIO, &GPIO_InitStructure);
}

void RelayOutputOn(void)
{
	GPIO_SetBits(RELAY_OUTPUT_GPIO, RELAY_OUTPUT_Pin);
}

void RelayOutputOff(void)
{
	GPIO_ResetBits(RELAY_OUTPUT_GPIO, RELAY_OUTPUT_Pin);
}

void MosfetOutputOn(void)
{
    GPIO_ResetBits(MOSFET_OUTPUT_GPIO, MOSFET_OUTPUT_Pin);
}

void MosfetOutputOff(void)
{
	GPIO_SetBits(MOSFET_OUTPUT_GPIO, MOSFET_OUTPUT_Pin);
}



