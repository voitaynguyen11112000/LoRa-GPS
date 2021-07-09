#include "button.h"

uint16_t temp_code[8] = {0,0,0,0,0,0,0,0};
uint8_t arrayMaskOfKeyCode[8] = {7,6,5,4,3,2,1,0};
uint16_t key_code[8] = {0,0,0,0,0,0,0,0};


void init_button(void){
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(BUTTON_SCK_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = BUTTON_SCK_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_Init(BUTTON_SCK_GPIO, &GPIO_InitStructure);
    
    RCC_APB2PeriphClockCmd(BUTTON_LOAD_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = BUTTON_LOAD_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_Init(BUTTON_LOAD_GPIO, &GPIO_InitStructure);
    
    RCC_APB2PeriphClockCmd(BUTTON_DATA_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = BUTTON_DATA_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
    GPIO_Init(BUTTON_DATA_GPIO, &GPIO_InitStructure);
}

void scan_button(void)
{
    unsigned char i;
    BUTTON_SCK(0);
    BUTTON_LOAD(0);
    BUTTON_LOAD(1);

    for (i = 0; i < 8; i++)
    {
        if (GPIO_ReadInputDataBit(BUTTON_DATA_GPIO, BUTTON_DATA_Pin) == 0)
            temp_code[i] = temp_code[i] + 1;
        else
            temp_code[i] = 0;
        BUTTON_SCK(1);
        BUTTON_SCK(0);
    }
		
    for (i = 0; i < 8; i++)
    {
        key_code[i] = temp_code[arrayMaskOfKeyCode[i]];
    }
}
