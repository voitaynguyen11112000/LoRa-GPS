#ifndef _LCD_H_
#define _LCD_H_

#include "stm32f10x.h"

void init_lcd(void);
void LcdPrintCharS(unsigned char row, unsigned char column, char c);
void LcdPrintNumS(unsigned char row, unsigned char column, long num);
void LcdPrintStringS(unsigned char row, unsigned char column, char *string);
void DisplayLcd(void);

#endif 
