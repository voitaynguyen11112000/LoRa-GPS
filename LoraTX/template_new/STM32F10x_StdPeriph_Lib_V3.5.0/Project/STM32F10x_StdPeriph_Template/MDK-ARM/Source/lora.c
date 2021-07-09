#include"lora.h"
void Init_spi(){

	GPIO_InitTypeDef GPIO_InitStructure;
//	SPI_InitTypeDef spi2Init;

	RCC_APB2PeriphClockCmd(LORA_CLK_GPIO_CLK, ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	/*CLK - PB13*/
	GPIO_InitStructure.GPIO_Pin = LORA_CLK_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_Init(LORA_CLK_GPIO, &GPIO_InitStructure);
	
	/*NSS - PB12*/

	GPIO_InitStructure.GPIO_Pin = LORA_NSS_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_Init(LORA_NSS_GPIO, &GPIO_InitStructure);
	
	/*MISO - PB14*/

	GPIO_InitStructure.GPIO_Pin = LORA_MISO_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
    GPIO_Init(LORA_MISO_GPIO, &GPIO_InitStructure);
	
	/*MOSI - PB15*/

	GPIO_InitStructure.GPIO_Pin = LORA_MOSI_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_Init(LORA_MOSI_GPIO, &GPIO_InitStructure);
	
	/*RST - PB1*/
	GPIO_InitStructure.GPIO_Pin = LORA_RST_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;   
    GPIO_Init(LORA_RST_GPIO, &GPIO_InitStructure);
	
	/*DIO0 - PB0*/
	GPIO_InitStructure.GPIO_Pin = LORA_DIO0_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_Init(LORA_DIO0_GPIO, &GPIO_InitStructure);
	LORA_CLK(0);
	/*SPI2*/

//	spi2Init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
//	spi2Init.SPI_CPHA = SPI_CPHA_1Edge;
//	spi2Init.SPI_CPOL = SPI_CPOL_Low;
//	spi2Init.SPI_DataSize = SPI_DataSize_8b;
//	spi2Init.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//	spi2Init.SPI_FirstBit = SPI_FirstBit_MSB;
//	spi2Init.SPI_Mode = SPI_Mode_Master;
//	spi2Init.SPI_NSS = SPI_NSS_Soft;
//	spi2Init.SPI_CRCPolynomial = 7;
//	
//	SPI_Init(SPI2, &spi2Init);
//	
//	SPI_Cmd(SPI2, ENABLE);
}
LoRa newLoRa(){
	LoRa new_LoRa;

	new_LoRa.frequency             = 433       ;
	new_LoRa.spredingFactor        = SF_7      ;
	new_LoRa.bandWidth			   = BW_125KHz ;
	new_LoRa.crcRate               = CR_4_5    ;
	new_LoRa.power				   = POWER_20db;
	new_LoRa.overCurrentProtection = 100       ;
	new_LoRa.preamble			   = 8         ;

	return new_LoRa;
}
void LoRa_reset(LoRa* _LoRa){
	GPIO_WriteBit(_LoRa->reset_port, _LoRa->reset_pin, Bit_RESET);
	delay_us(200);
	GPIO_WriteBit(_LoRa->reset_port, _LoRa->reset_pin, Bit_SET);
	delay_ms(10);
}

uint16_t LoRa_init(LoRa* _LoRa){
	uint8_t    data;
	uint8_t    read;

	if(LoRa_isvalid(_LoRa)){
		// goto sleep mode:
			LoRa_gotoMode(SLEEP_MODE, _LoRa);
			delay_us(10);

		// turn on lora mode:
			read = LoRa_readReg(RegOpMode, _LoRa);
			delay_us(10);
			data = read | 0x80;
			LoRa_writeReg(RegOpMode, data, _LoRa);
			delay_us(100);

		// set frequency:
			LoRa_setFrequency(_LoRa->frequency, _LoRa);

		// set output power gain:
			LoRa_setPower(_LoRa->power, _LoRa);

		// set over current protection:
			LoRa_setOCP(_LoRa->overCurrentProtection, _LoRa);

		// set LNA gain:
			LoRa_writeReg(RegLna, 0x23, _LoRa);

		// set spreading factor, CRC on, and Timeout Msb:
			LoRa_setTOMsb_setCRCon(_LoRa);
			LoRa_setSpreadingFactor(_LoRa->spredingFactor, _LoRa);

		// set Timeout Lsb:
			LoRa_writeReg(RegSymbTimeoutL, 0xFF, _LoRa);

		// set bandwidth, coding rate and expilicit mode:
			// 8 bit RegModemConfig --> | X | X | X | X | X | X | X | X |
			//       bits represent --> |   bandwidth   |     CR    |I/E|
			data = 0;
			data = (_LoRa->bandWidth << 4) + (_LoRa->crcRate << 1);
			LoRa_writeReg(RegModemConfig1, data, _LoRa);

		// set preamble:
			LoRa_writeReg(RegPreambleMsb, _LoRa->preamble >> 8, _LoRa);
			LoRa_writeReg(RegPreambleLsb, _LoRa->preamble >> 0, _LoRa);

		// DIO mapping:   --> DIO: RxDone
			read = LoRa_readReg(RegDioMapping1, _LoRa);
			data = read | 0x3F;
			LoRa_writeReg(RegDioMapping1, data, _LoRa);

		// goto standby mode:
			LoRa_gotoMode(STNBY_MODE, _LoRa);
			_LoRa->current_mode = STNBY_MODE;
			delay_us(10);

			read = LoRa_readReg(RegVersion, _LoRa);
			if(read == 0x12)
				return LORA_OK;
			else
				return LORA_NOT_FOUND;
	}
	else {
		return LORA_UNAVAILABLE;
	}
}
void Send_byte(unsigned char out)   
{   
    unsigned char i = 0;   
    for (i = 0; i < 8; i++)   
    {   
        if ((out & 0x80) == 0x80)           /* check if MSB is high */   
            LORA_MOSI(1);  
        else   
            LORA_MOSI(0);                  /* if not, set to low */   
        
        LORA_CLK(1); 		/* toggle clock high */
		delay_us(10);
        out = (out << 1);                   /* shift 1 place for next bit */   
        LORA_CLK(0);                       /* toggle clock low */
		delay_us(10);
    }   
}   
unsigned char Get_Byte(void)   
{   
    unsigned char i = 0, in = 0, temp = 0;
    
    for (i = 0; i < 8; i++)   
    {   
        in = (in << 1);                     /* shift 1 place to the left or shift in 0 */   
        temp = GPIO_ReadInputDataBit(LORA_MISO_GPIO, LORA_MISO_Pin);                  /* save input */   
        
        LORA_CLK(1);                     /* toggle clock high */
		delay_us(10);
        if (temp == 1)			            /* check to see if bit is high */
            in = in | 0x01;		            /* if high, make bit high */
    
        LORA_CLK(0);                      /* toggle clock low */
		delay_us(10);
    }
    
    return in;   
} 
void LoRa_writeReg(unsigned char address, unsigned char data, LoRa* _LoRa){
	unsigned char add = (0x80 | address);
	GPIO_WriteBit(_LoRa->CS_port, _LoRa->CS_pin, Bit_RESET);
	Send_byte(add);
	Send_byte(data);
	GPIO_WriteBit(_LoRa->CS_port, _LoRa->CS_pin, Bit_SET);
	
}

unsigned char LoRa_readReg(unsigned char address, LoRa* _LoRa){
	unsigned char add = (0x7f & address);
	unsigned char data;
	GPIO_WriteBit(_LoRa->CS_port, _LoRa->CS_pin, Bit_RESET);
	Send_byte(add);
	data = Get_Byte();
	GPIO_WriteBit(_LoRa->CS_port, _LoRa->CS_pin, Bit_SET);
	return data;
}

void LoRa_setFrequency(int freq, LoRa * _LoRa){
	uint8_t  data;
	uint32_t F;
	F = (freq * 524288)>>5;

	// write Msb:
	data = F >> 16;
	LoRa_writeReg(RegFrMsb, data, _LoRa);
	delay_us(10);

	// write Mid:
	data = F >> 8;
	LoRa_writeReg(RegFrMid, data, _LoRa);
	delay_us(10);

	// write Lsb:
	data = F >> 0;
	LoRa_writeReg(RegFrLsb, data, _LoRa);
	delay_us(10);
}
void LoRa_setSpreadingFactor(int SF, LoRa* _LoRa){
	uint8_t	data;
	uint8_t	read;

	if(SF>12)
		SF = 12;
	if(SF<6)
		SF = 6;

	if (SF == 6) {
    LoRa_writeReg(RegDetectionOptimize, 0xc5, _LoRa);
    LoRa_writeReg(RegDetectionThreshold, 0x0c, _LoRa);
	}
	else {
    LoRa_writeReg(RegDetectionOptimize, 0xc3, _LoRa);
    LoRa_writeReg(RegDetectionThreshold, 0x0a, _LoRa);
	}
	
	read = LoRa_readReg(RegModemConfig2, _LoRa);
	delay_us(10);

	data = (read & 0x0F) | ((SF << 4) & 0xf0);
	LoRa_writeReg(RegModemConfig2, data, _LoRa);
	delay_us(10);
}

void LoRa_setPower(uint8_t power, LoRa* _LoRa){
	LoRa_writeReg(RegPaConfig, power, _LoRa);
	delay_us(10);
}

void LoRa_setOCP(uint8_t current, LoRa* _LoRa){
	uint8_t	OcpTrim = 0;

	if(current<45)
		current = 45;
	if(current>240)
		current = 240;

	if(current <= 120)
		OcpTrim = (current - 45)/5;
	else if(current <= 240)
		OcpTrim = (current + 30)/10;

	OcpTrim = OcpTrim + (1 << 5);
	LoRa_writeReg(RegOcp, OcpTrim, _LoRa);
	delay_us(10);
}
void LoRa_gotoMode(int mode, LoRa* _LoRa){
	uint8_t    read;
	uint8_t    data;

	read = LoRa_readReg(RegOpMode, _LoRa);
	data = read;

	if(mode == SLEEP_MODE){
		data = (read & 0xF8) | 0x00;
		_LoRa->current_mode = SLEEP_MODE;
	}else if (mode == STNBY_MODE){
		data = (read & 0xF8) | 0x01;
		_LoRa->current_mode = STNBY_MODE;
	}else if (mode == TRANSMIT_MODE){
		data = (read & 0xF8) | 0x03;
		_LoRa->current_mode = TRANSMIT_MODE;
	}else if (mode == RXCONTIN_MODE){
		data = (read & 0xF8) | 0x05;
		_LoRa->current_mode = RXCONTIN_MODE;
	}else if (mode == RXSINGLE_MODE){
		data = (read & 0xF8) | 0x06;
		_LoRa->current_mode = RXSINGLE_MODE;
	}

	LoRa_writeReg(RegOpMode, data,_LoRa);
}	
void LoRa_BurstWrite(LoRa* _LoRa, uint8_t address, uint8_t *value, uint8_t length){

}	
uint8_t LoRa_transmit(LoRa* _LoRa, uint8_t* data, uint8_t length, uint16_t timeout){
	uint8_t read;
	uint8_t i;
	int mode = _LoRa->current_mode;
	LoRa_gotoMode(STNBY_MODE, _LoRa);
	read = LoRa_readReg(RegFiFoTxBaseAddr, _LoRa);
	LoRa_writeReg(RegFiFoAddPtr, read, _LoRa);
	LoRa_writeReg(RegPayloadLength, length, _LoRa);
	for(i=0; i<length; i++){
		LoRa_writeReg(RegFiFo, data[i], _LoRa);
	}
	LoRa_gotoMode(TRANSMIT_MODE, _LoRa);
	while(1){
		read = LoRa_readReg(RegIrqFlags, _LoRa);
		if((read & 0x08)!=0){
			LoRa_writeReg(RegIrqFlags, 0xFF, _LoRa);
			LoRa_gotoMode(mode, _LoRa);
			return 1;
		}
		else{
			if(--timeout==0){
				LoRa_gotoMode(mode, _LoRa);
				return 0;
			}
		}
		delay_us(10);
	}

}
void LoRa_setTOMsb_setCRCon(LoRa* _LoRa){
	uint8_t read, data;

	read = LoRa_readReg(RegModemConfig2, _LoRa);

	data = read | 0x07;
	LoRa_writeReg(RegModemConfig2, data, _LoRa);
	delay_us(10);
}
uint8_t LoRa_isvalid(LoRa* _LoRa){
	return 1;
}

void LoRa_startReceiving(LoRa* _LoRa){
	LoRa_gotoMode(RXCONTIN_MODE, _LoRa);
}

void LoRa_receive(LoRa* _LoRa, uint8_t* data, uint8_t length){
	uint8_t read;
	uint8_t number_of_bytes;
	uint8_t min;
	uint8_t i;
	uint8_t j;
	for(i=0; i<length; i++)
		data[i]=0;

	LoRa_gotoMode(STNBY_MODE, _LoRa);
	read = LoRa_readReg(RegIrqFlags, _LoRa);
	if((read & 0x40) != 0){
		LoRa_writeReg(RegIrqFlags, 0xFF, _LoRa);
		number_of_bytes = LoRa_readReg(RegRxNbBytes, _LoRa);
		read = LoRa_readReg(RegFiFoRxCurrentAddr, _LoRa);
		LoRa_writeReg(RegFiFoAddPtr, read, _LoRa);
		min = length >= number_of_bytes ? number_of_bytes : length;
		for(j=0; j<min; j++)
			data[j] = LoRa_readReg(RegFiFo, _LoRa);
	}
	LoRa_gotoMode(RXCONTIN_MODE, _LoRa);
}

int LoRa_getRSSI(LoRa* _LoRa){
	uint8_t read;
	read = LoRa_readReg(RegPktRssiValue, _LoRa);
	return -164 + read;
}