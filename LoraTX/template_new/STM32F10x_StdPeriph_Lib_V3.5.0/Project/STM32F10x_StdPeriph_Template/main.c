#include "main.h"
#include <stdio.h>
//#include "button.h"
#include "led.h"
#include "uart.h"
#include "adc.h"
//#include "ds1307.h"
#include "lora.h"
#define INIT 70
#define WAIT_SEND	71
#define	SEND	72
uint16_t cnt;
uint8_t send_data[256];
uint16_t receive_data[500];
unsigned char data;
uint8_t flagSend = 0;
uint8_t countLine = 0;
unsigned char status = INIT;
LoRa myLoRa;
void app();
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
	myLoRa.spredingFactor        = SF_7      ;
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
				cnt = (cnt+1)%1200;
				get_adc();
				if(flag_uart2==1 && cnt == 0){
					flag_uart2=0;					
					flagSend = LoRa_transmit(&myLoRa, dataOk, 128, 2000);
					USART1_SendChar(flagSend);
				}
				else{
					LoRa_gotoMode(SLEEP_MODE, &myLoRa);
				}
//				if(cnt==0){
//					get_adc();
//					USART1_SendChar(adc_value>>8);
//					USART1_SendChar(adc_value);
//				}
				//app();
				
    }


}

void init_system(void)
{

    init_timer();
    SetTimer_ms(50);   
    USART1_Init(115200);
		USART2_Init(9600);
    init_led();
    init_adc();
//    init_output();
//	I2C_INIT();
		Init_spi();
}
