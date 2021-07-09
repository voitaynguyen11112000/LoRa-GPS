#include "uart.h"
#include <stdio.h>

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


  
void USART1_Init(uint32_t baudrate)
{
    
}

void USART1_SendChar(uint8_t data)
{

}

void USART1_SendString(char* str)
{
	
}

void USART1_SendNum(long num)
{
	
}

void USART2_Init(uint32_t baudrate)
{
    
}

void USART2_SendChar(uint8_t data)
{

}

void USART2_SendString(char* str)
{
	
}

void USART2_SendNum(long num)
{
	
}

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}  
