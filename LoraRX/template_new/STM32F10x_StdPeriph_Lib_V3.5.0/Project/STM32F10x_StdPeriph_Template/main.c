#include "main.h"
#include <stdio.h>
//#include "button.h"
#include "led.h"
#include "uart.h"
#include "adc.h"
//#include "ds1307.h"
#include "lora.h"
#define INIT 70
#define SEND	71
#define	END_SEND	72
uint16_t cnt;
uint8_t send_data[256];
uint8_t receive_data[128];
unsigned char data;
uint8_t flagSend = 0;
uint8_t countLine = 0;
unsigned char status = INIT;
LoRa myLoRa;
char buffer[128];
uint8_t i;
void app();
void emptyData(uint8_t* str);
int main(void)
{
	
	
	
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	init_system();
	
	myLoRa = newLoRa();
	myLoRa.CS_port = LORA_NSS_GPIO;
	myLoRa.CS_pin = LORA_NSS_Pin;
	myLoRa.reset_port = LORA_RST_GPIO;
	myLoRa.reset_pin = LORA_RST_Pin;
	myLoRa.DIO0_port = LORA_DIO0_GPIO;
	myLoRa.DIO0_pin = LORA_DIO0_Pin;
	
	myLoRa.frequency             = 433       ;
	myLoRa.spredingFactor        = SF_7     ;
	myLoRa.bandWidth			   = BW_125KHz ;
	myLoRa.crcRate               = CR_4_5    ;
	myLoRa.power				   = POWER_20db;
	myLoRa.overCurrentProtection = 100       ;
	myLoRa.preamble			   = 8         ;
	LoRa_init(&myLoRa);
  LoRa_reset(&myLoRa);
    while (1)
    {		
        while (!flag_timer);
        flag_timer = 0;
				BlinkDebugLed();
				cnt = (cnt+1)%300;
				emptyData(receive_data);
				if(cnt==0){
					LoRa_startReceiving(&myLoRa);
					LoRa_receive(&myLoRa, receive_data, 128);
					USART1_SendString(receive_data);
				}
				app();
				
    }


}
void emptyData(uint8_t* str){
	int i = 0;
	while(i<128){
		*str = 0;
		str++;
		i++;
	}
}
void app(){
	switch (status){
		case INIT:
				if(cnt == 299){
					USART2_SendChar(0xfe);
					status = SEND;
				}
		break;		
		case SEND:
				if(cnt == 0){
					USART2_SendString(receive_data);
					status = END_SEND;
				}
				
		break;
		
		case END_SEND:
			USART2_SendChar(0xff);
			status = INIT;
				
		break;
	}
		
		
}
void init_system(void)
{

    init_timer();
    SetTimer_ms(50);   
    USART1_Init(115200);
		USART1_SendChar(0x00);
		USART2_Init(115200);
    init_led();
    init_adc();
//    init_output();
//	I2C_INIT();
		Init_spi();
}
