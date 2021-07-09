#include "lcd.h"
#include "delay.h"
#include "button.h"
#include <stdio.h>
#include <string.h>

#define LCD_MAX_ROW     16
#define LCD_MAX_COL     16

unsigned int lcdHeadPointer = 0;
int current_row, current_col;
char statusLCD = LCD_SCREEN_0;

/* 8 user defined characters to be loaded into CGRAM (used for bargraph)      */
const unsigned char UserFont[8][8] = {
  { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
  { 0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10 },
  { 0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18 },
  { 0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C },
  { 0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E },
  { 0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F },
  { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
  { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 }
};

unsigned char LcdScreen[16][16] = {
'S','T','A','R','T',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','E','N','D'
};

void LCD_DATA_DIR_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  
	/* LCD1602 Pins Configuration -> IN */
	// DB4
	RCC_APB2PeriphClockCmd(DB4_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = DB4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DB4_GPIO_PORT, &GPIO_InitStructure);
	// DB5
	RCC_APB2PeriphClockCmd(DB5_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = DB5_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DB5_GPIO_PORT, &GPIO_InitStructure);
	// DB6
	RCC_APB2PeriphClockCmd(DB6_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = DB6_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DB6_GPIO_PORT, &GPIO_InitStructure);
	// DB7
	RCC_APB2PeriphClockCmd(DB7_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = DB7_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DB7_GPIO_PORT, &GPIO_InitStructure);
}

void LCD_DATA_DIR_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* LCD1602 Pins Configuration -> OUT */
	// DB4
	RCC_APB2PeriphClockCmd(DB4_GPIO_CLK, ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = DB4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DB4_GPIO_PORT, &GPIO_InitStructure);
	// DB5
	RCC_APB2PeriphClockCmd(DB5_GPIO_CLK, ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = DB5_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DB5_GPIO_PORT, &GPIO_InitStructure);
	// DB6
	RCC_APB2PeriphClockCmd(DB6_GPIO_CLK, ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = DB6_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DB6_GPIO_PORT, &GPIO_InitStructure);
	// DB7
	RCC_APB2PeriphClockCmd(DB7_GPIO_CLK, ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = DB7_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DB7_GPIO_PORT, &GPIO_InitStructure);
}

void LCD_ALL_DIR_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* LCD1602 Data Pins Configuration -> OUT */
	LCD_DATA_DIR_OUT();
	
	/* LCD1602 Option Pins Configuration -> OUT */
	// EN
	RCC_APB2PeriphClockCmd(EN_GPIO_CLK, ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = EN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(EN_GPIO_PORT, &GPIO_InitStructure);
	// RS
	RCC_APB2PeriphClockCmd(RS_GPIO_CLK, ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = RS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RS_GPIO_PORT, &GPIO_InitStructure);	
}

unsigned char PINS_DATA(void)
{
	unsigned char pins_data = 0x00;
	
	if (DB7)
		pins_data = (pins_data | 0x08);
	if (DB6)
		pins_data = (pins_data | 0x04);
	if (DB5)
		pins_data = (pins_data | 0x02);
	if (DB4)
		pins_data = (pins_data | 0x01);
	
	return pins_data;
}

unsigned char LCD_DATA_IN(void)
{
	return PINS_DATA();
}

void LCD_DATA_OUT(unsigned char data)
{	
	if ((data & 0x08) == 0x08)
		GPIO_WriteBit(DB7_GPIO_PORT, DB7_PIN, Bit_SET);
	else
		GPIO_WriteBit(DB7_GPIO_PORT, DB7_PIN, Bit_RESET);
	
	if ((data & 0x04) == 0x04)
		GPIO_WriteBit(DB6_GPIO_PORT, DB6_PIN, Bit_SET);
	else
		GPIO_WriteBit(DB6_GPIO_PORT, DB6_PIN, Bit_RESET);
	
	if ((data & 0x02) == 0x02)
		GPIO_WriteBit(DB5_GPIO_PORT, DB5_PIN, Bit_SET);
	else
		GPIO_WriteBit(DB5_GPIO_PORT, DB5_PIN, Bit_RESET);
	
	if ((data & 0x01) == 0x01)
		GPIO_WriteBit(DB4_GPIO_PORT, DB4_PIN, Bit_SET);
	else
		GPIO_WriteBit(DB4_GPIO_PORT, DB4_PIN, Bit_RESET);

}

void LCD_Delay (int cnt)
{
  cnt <<= DELAY_2N;

  while (cnt--);
}

void LCD_Write4bit(unsigned char c)
{
    LCD_EN(1);
    LCD_DATA_OUT(c&0x0F);
    LCD_Delay(10);
    LCD_EN(0);
    LCD_Delay(10);
}

void LCD_WriteCMD(unsigned char c)
{
    delay_us(20);
    LCD_RS(0);
    LCD_Write4bit(c>>4);
    LCD_Write4bit(c);
}

void LCD_WriteData(unsigned char c)
{
    delay_us(20);
    LCD_RS(1);
    LCD_Write4bit(c>>4);
    LCD_Write4bit(c);
}

void LCD_PutChar(char c)
{ 
    LCD_WriteData(c);
}

void init_lcd(void)
{
	int i;
    unsigned char const *p;
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(DB4_GPIO_CLK | DB5_GPIO_CLK | DB6_GPIO_CLK | DB7_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(EN_GPIO_CLK | RS_GPIO_CLK, ENABLE);

	/* Config all pins for LCD as outputs */
	LCD_ALL_DIR_OUT();
	
	LCD_Delay(15000);
    LCD_RS(0);
    LCD_Write4bit(0x3);                  /* Select 4-bit interface             */
    LCD_Delay(15000);
    LCD_Write4bit(0x3);
    LCD_Delay(15000);
    LCD_Write4bit(0x3);
    LCD_Delay(15000);
    LCD_Write4bit(0x2);
    LCD_Delay(15000);

    LCD_WriteCMD(0x28);                  /* 2 lines, 5x8 character matrix      */
    LCD_Delay(15000);
    LCD_WriteCMD(0x0C);                  /* Display ctrl:Disp=ON,Curs/Blnk=OFF */
    LCD_Delay(15000);
    LCD_WriteCMD(0x06);                  /* Entry mode: Move right, no shift   */
    LCD_Delay(15000);

    /* Load user-specific characters into CGRAM */
    LCD_WriteCMD(0x40);                  /* Set CGRAM address counter to 0     */
    LCD_Delay(15000);
    p = &UserFont[0][0];
    for (i = 0; i < sizeof(UserFont); i++, p++)
    LCD_PutChar(*p);

    LCD_WriteCMD(0x80);                  /* Set DDRAM address counter to 0     */
    LCD_Delay(15000);
	
}

void LCD_SetCursorPos(unsigned char line, unsigned char column)
{
    uint8_t  address;
    switch(line)
    {
        case 0: address = 0x80; break; //Starting address of 1st line
        case 1: address = 0xC0; break; //Starting address of 2nd line
        case 2: address = 0x94; break; //Starting address of 3rd line
        case 3: address = 0xD4; break; //Starting address of 4th line
        default: ; 
    }
    address += column;
    LCD_WriteCMD(address);
}

void LcdClear(void)
{
    LCD_WriteCMD(0x01);
    LCD_SetCursorPos(0, 0);
}

void LCD_Print(char *string)
{
    while (*string)  
    {
        LCD_PutChar(*string++);
    }
}

void LCD_PrintNum(long num) 
{
	char _sNum[21];
    sprintf(_sNum, "%ld", num);
    
    LCD_Print(_sNum);
}

void LcdPrintNum(unsigned char row, unsigned char column, long num)
{
	LCD_SetCursorPos(row,column);
	LCD_PrintNum(num);
}
void LcdPrintString(unsigned char row, unsigned char column, char * string)
{
	LCD_SetCursorPos(row,column);
	LCD_Print(string);
}

void LCD_PrintCharS(unsigned char c)
{
    LcdScreen[current_row][ current_col] = c;
    current_col = (current_col + 1) % LCD_MAX_COL;
}

void LCD_PrintStringS(char* _str)
{
    while (*_str)
    {
        LCD_PrintCharS(*_str++);
    }
}

void LCD_PrintNumS(long num) 
{
    char _sNum[21];
    sprintf(_sNum, "%ld", num);
    
    LCD_PrintStringS(_sNum);
}

void LcdClearS()
{
    char i;
	lcdHeadPointer = 0;
    for (i = 0; i<16; i++)
    {
        LcdScreen[0][i] = ' ';
        LcdScreen[1][i] = ' ';
    }
}

void DisplayLcdScreen(void)
{
    unsigned char i;
    LCD_SetCursorPos (0,0);
    for (i = 0; i<16; i++)
        LCD_PutChar(LcdScreen[0][lcdHeadPointer + i]);
    LCD_SetCursorPos (1,0);
    for (i = 0; i<16; i++)
        LCD_PutChar(LcdScreen[1][lcdHeadPointer + i]);
}

void LcdPrintCharS(unsigned char row, unsigned char column, char c)
{
    current_row = row % LCD_MAX_ROW;
    current_col = column % LCD_MAX_COL;

    LcdScreen[current_row][ current_col] = c;
}

void LcdPrintNumS(unsigned char row, unsigned char column, long num)
{
	current_row = row % LCD_MAX_ROW;
	current_col = column % LCD_MAX_COL;
	LCD_PrintNumS(num);
}

void LcdPrintStringS(unsigned char row, unsigned char column, char *string)
{
	current_row = row % LCD_MAX_ROW;
	current_col = column % LCD_MAX_COL;
	while (*string)
    {
        LCD_PrintCharS (*string++);
    }
}

unsigned char isButtonIncLine(void)
{
	if(key_code[6] == 1)
		return 1;
	else
		return 0;
}
unsigned char isButtonDecLine(void)
{
	if(key_code[7] == 1)
		return 1;
	else
		return 0;
}

void ScreenProcess(void)
{       
    if (isButtonIncLine())
        lcdHeadPointer = (lcdHeadPointer + 32)%240;
    if (isButtonDecLine())
        lcdHeadPointer = (lcdHeadPointer + 240-32)%240;             
}

void LcdClearScreen(void)
{
    char i;
    for (i = 0; i<16; i++)
    {
        LcdScreen[0][i] = ' ';
        LcdScreen[1][i] = ' ';
    }
}
