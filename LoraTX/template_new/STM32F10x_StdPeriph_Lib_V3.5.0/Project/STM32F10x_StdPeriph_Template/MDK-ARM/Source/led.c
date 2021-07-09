#include "led.h"

uint8_t ledOutput = 0;

void init_led(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(DEBUG_LED_GPIO_CLK, ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = DEBUG_LED_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_LED_GPIO, &GPIO_InitStructure);
    
    RCC_APB2PeriphClockCmd(LED_CLK_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LED_CLK_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_Init(LED_CLK_GPIO, &GPIO_InitStructure);
    
    RCC_APB2PeriphClockCmd(LED_LAT_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LED_LAT_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_Init(LED_LAT_GPIO, &GPIO_InitStructure);
    
    RCC_APB2PeriphClockCmd(LED_SDO_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LED_SDO_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_Init(LED_SDO_GPIO, &GPIO_InitStructure);
}

void DebugLedOn(void)
{
    GPIO_ResetBits(DEBUG_LED_GPIO, DEBUG_LED_Pin);
}

void DebugLedOff(void)
{
    GPIO_SetBits(DEBUG_LED_GPIO, DEBUG_LED_Pin);
}

void BlinkDebugLed(void)
{
    static uint8_t cnt = 0;
    cnt = (cnt + 1)%20;
    if (cnt < 10)
        DebugLedOn();
    else
        DebugLedOff();
}

void LedTurnOn(uint8_t index)
{
    uint8_t i, data;
    
    if(index < 8)
	{
		ledOutput = ledOutput | (1<<index);
	}
    
    LED_LAT(0);
    LED_CLK(0);
    
    data = ledOutput;
	for (i = 0; i < 8; i++)
	{
		if ( (data & 0x01) == 0)
			LED_SDO(0);
		else
			LED_SDO(1);
	
		data  = data >> 1;
		
        LED_CLK(1);
		LED_CLK(0);
	}
    LED_LAT(1);
	LED_LAT(0);	
}

void LedTurnOff(uint8_t index)
{
    uint8_t i, data;
    
    if(index < 8)
	{
		ledOutput = ledOutput & ~(1<<index);
	}
    
    LED_LAT(0);
    LED_CLK(0);
    
    data = ledOutput;
	for (i = 0; i < 8; i++)
	{
		if ( (data & 0x01) == 0)
			LED_SDO(0);
		else
			LED_SDO(1);
	
		data  = data >> 1;
		
        LED_CLK(1);
		LED_CLK(0);
	}
    LED_LAT(1);
	LED_LAT(0);	
}
