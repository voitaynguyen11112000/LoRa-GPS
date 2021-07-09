#ifndef __LCD1602_H_
#define __LCD1602_H_

#include "stm32f10x.h"

#define LCD_SCREEN_0		0
#define LCD_SCREEN_1		1
#define LCD_SCREEN_2        2
#define LCD_SCREEN_3		3

#define DELAY_2N     4

#define DB4_PIN                         GPIO_Pin_5
#define DB4_GPIO_PORT                   GPIOB
#define DB4_GPIO_CLK                    RCC_APB2Periph_GPIOB

#define DB5_PIN                         GPIO_Pin_4
#define DB5_GPIO_PORT                   GPIOB
#define DB5_GPIO_CLK                    RCC_APB2Periph_GPIOB

#define DB6_PIN                         GPIO_Pin_3
#define DB6_GPIO_PORT                   GPIOB
#define DB6_GPIO_CLK                    RCC_APB2Periph_GPIOB

#define DB7_PIN                         GPIO_Pin_15
#define DB7_GPIO_PORT                   GPIOA
#define DB7_GPIO_CLK                    RCC_APB2Periph_GPIOA

#define DB4								GPIO_ReadInputDataBit(DB4_GPIO_PORT, DB4_PIN)
#define DB5								GPIO_ReadInputDataBit(DB5_GPIO_PORT, DB5_PIN)
#define DB6								GPIO_ReadInputDataBit(DB6_GPIO_PORT, DB6_PIN)
#define DB7								GPIO_ReadInputDataBit(DB7_GPIO_PORT, DB7_PIN)


#define EN_PIN                         	GPIO_Pin_8
#define EN_GPIO_PORT                   	GPIOB
#define EN_GPIO_CLK                    	RCC_APB2Periph_GPIOB

#define RS_PIN                         	GPIO_Pin_9
#define RS_GPIO_PORT                   	GPIOB
#define RS_GPIO_CLK                    	RCC_APB2Periph_GPIOB

#define LCD_EN(x)              ((x) ? (EN_GPIO_PORT->BSRR = EN_PIN)  : (EN_GPIO_PORT->BRR = EN_PIN) )
#define LCD_RW(x)             ((x) ? (RW_GPIO_PORT->BSRR = RW_PIN)  : (RW_GPIO_PORT->BRR = RW_PIN))
#define LCD_RS(x)             ((x) ? (RS_GPIO_PORT->BSRR = RS_PIN)  : (RS_GPIO_PORT->BRR = RS_PIN))

// Main Functions
void init_lcd(void);
void LcdClear(void);
void LcdPrintNum(unsigned char row, unsigned char column, long num);
void LcdPrintString(unsigned char row, unsigned char column, char * string);

void LcdClearS(void);
void LcdPrintCharS(unsigned char row, unsigned char column, char c);
void LcdPrintNumS(unsigned char row, unsigned char column, long num);
void LcdPrintStringS(unsigned char row, unsigned char column, char* string);

// Sub Functions
void LCD_PutChar(char c);
void LCD_PrintNum(long num);
void LCD_SetCursorPos(unsigned char row, unsigned char column);
void LCD_Print(char *string);
void LCD_PrintCharS(unsigned char c);
void LCD_WriteData(unsigned char c);
void LCD_Write4bit(unsigned char c);
void LCD_WriteCMD(unsigned char c);
void LCD_Delay (int cnt);

unsigned char PINS_DATA(void);
unsigned char LCD_DATA_IN(void);
void LCD_DATA_OUT(unsigned char data);


void DisplayLcdScreen(void);
extern unsigned int lcdHeadPointer;
extern unsigned char LcdScreen[16][16];
void ScreenProcess(void);
void LcdClearScreen(void);

#endif
