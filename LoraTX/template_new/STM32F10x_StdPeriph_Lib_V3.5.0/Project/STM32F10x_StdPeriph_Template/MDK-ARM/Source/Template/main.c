#include "main.h"
#include <stdio.h>

int main(void)
{
    init_timer();
	SetTimer_ms(50);
	init_led();
	init_output();
	init_input();
	USART1_Init(115200);
	init_buzzer();
	init_adc();
	
    printf("Testing\r\n");
	
    while (1)
    {
		while (!flag_timer);
		flag_timer = 0;
		
		scan_input();
		
		DisplayLcd();

    }

}
