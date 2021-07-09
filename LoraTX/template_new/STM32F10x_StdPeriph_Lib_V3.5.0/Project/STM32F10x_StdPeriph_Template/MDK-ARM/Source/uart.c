#include "uart.h"
#include <stdio.h>
#include "adc.h"
#define WAIT1 12
#define	WAIT2 13
#define WAIT3 14
#define	WAIT4 15
#define	RECEIVE_DATA 16
uint8_t uart_receive1 = 0;
uint8_t flag_uart1 = 0;

uint8_t uart_receive2 = 0;
uint8_t flag_uart2 = 0;
uint8_t dataOk[128];
uint8_t countLine1 = 0;
uint8_t status1 = WAIT1;
uint16_t dem = 0;
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

  
void USART1_Init(uint32_t baudrate)
{		
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd(USART1_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);
	/* Configure USART1 Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = USART1_TxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(USART1_GPIO, &GPIO_InitStructure);
	
	/* Configure USART1 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = USART1_RxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(USART1_GPIO, &GPIO_InitStructure);

	/* Enable USART clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);
	
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	/* Enable USART1 Receive and Transmit interrupts */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	/* Configures the priority grouping: pre-emption priority and subpriority. */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* NVIC Init Structure definition */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
  
	/* Enable USART */
	USART_Cmd(USART1, ENABLE);
}
void USART1_IRQ_Handler(void)
{
    /* Read one byte from the receive data register */
    uart_receive1 = USART_ReceiveData(USART1);
		flag_uart1 = 1;
}
void USART1_SendChar(uint8_t data)
{
    USART_SendData(USART1, data);

    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
    {}
}
void USART1_SendFloat(float data){
	printf("%f\n", data);
}
void USART2_Init(uint32_t baudrate)
{		
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd  (USART2_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_AFIO, ENABLE);
	/* Configure USART2 Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = USART2_TxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(USART2_GPIO, &GPIO_InitStructure);
	
	/* Configure USART2 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = USART2_RxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(USART2_GPIO, &GPIO_InitStructure);

	/* Enable USART clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_USART2, ENABLE);
	
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	
	/* Enable USART1 Receive and Transmit interrupts */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	/* Configures the priority grouping: pre-emption priority and subpriority. */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* NVIC Init Structure definition */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
  
	/* Enable USART */
	USART_Cmd(USART2, ENABLE);
}
void USART2_IRQ_Handler(void)
{
    /* Read one byte from the receive data register */
	switch (status1){
		case WAIT1:
			if(USART_ReceiveData(USART2) == 0x24 && flag_uart2 == 0){
				status1 = WAIT2;
			}
		break;
		case WAIT2:
			if(USART_ReceiveData(USART2) == 0x47){
				status1 = WAIT3;
			}
			else{
				status1 = WAIT1;
			}
		break;
		case WAIT3:
			if(USART_ReceiveData(USART2) == 0x4e){
				status1 = WAIT4;
			}
			else{
				status1 = WAIT1;
			}
		break;
		case WAIT4:
			if(USART_ReceiveData(USART2) == 0x52){
				for(dem=4;dem<128;dem++){
					dataOk[dem] = 0;
				}
				dem = 4;
				dataOk[0] = 0x24;
				dataOk[1] = 0x47;
				dataOk[2] = 0x4e;
				dataOk[3] = 0x52;
				status1 = RECEIVE_DATA;
			}
			else{
				status1 = WAIT1;
			}
		break;
		case RECEIVE_DATA:	
			if(USART_ReceiveData(USART2) != 0x0d){
				dataOk[dem] = USART_ReceiveData(USART2);
				dem++;
			}
			else{
				
				dataOk[dem] = '/';
				sprintf(dataOk, "%s%02x%02x", dataOk, adc_value>>8, adc_value);
				dem+=4;
				flag_uart2=1;
				status1 = WAIT1;
			}
		break;
	}
}
void USART2_SendChar(uint8_t data)
{
    USART_SendData(USART2, data);

    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
    {}
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
