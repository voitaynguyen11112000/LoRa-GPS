#include "ds1307.h"
#include "delay.h"
unsigned char BCD2Dec(unsigned char BCD){
	unsigned char L, H;
	L = BCD & 0x0F;
	H = (BCD>>4)*10;
	return (H+L);
}
unsigned char Dec2BCD(unsigned char Dec){
	unsigned char L, H;
	L=Dec % 10;
	H=(Dec/10)<<4;
	return (H+L);
}
void I2C_INIT(void){
	GPIO_InitTypeDef         GPIO_InitStructure;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin=I2C_SCL_PIN;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(I2C_SCL_PORT,&GPIO_InitStructure );
	
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin=I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(I2C_SDA_PORT,&GPIO_InitStructure );
}
void I2C_Start(void){
	SDA(1);
	SCL(1);
	delay_us(50);
	SDA(0);
	delay_us(50);
	SCL(0);
	delay_us(50);
}
void I2C_Stop(void){
	SDA(0);
	SCL(1);
	delay_us(50);
	SDA(1);
}
void I2C_WaitAck(void)
{
	SDA(0);
	delay_us(50);
	SCL(1);
	delay_us(50);
	SCL(0);
	delay_us(50);
}
void I2C_NWaitAck(void){
	SDA(1);
	delay_us(50);
	SCL(1);
	delay_us(50);
	SCL(0);
	delay_us(50);
}
char Send_Byte(char data){
	GPIO_InitTypeDef         GPIO_InitStructure;
	u8 i, nack;
	for(i=0;i<8;i++)
	{
		if(data&0x80)
		{
			SDA(1);
		}
		else
		{
			SDA(0);
		}
		delay_us(50);
		SCL(1);
		delay_us(50);
		SCL(0);
		data = data << 1;
		delay_us(50);
	}
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(I2C_SDA_PORT,&GPIO_InitStructure );
	SCL(1);
	delay_us(50);
	nack = GPIO_ReadInputDataBit(I2C_SDA_PORT, I2C_SDA_PIN);
	SCL(0);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin=I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(I2C_SDA_PORT,&GPIO_InitStructure );
	delay_us(50);
	return nack;
}
char I2C_Receive_Byte(char ack){
	uint8_t temp = 0x00;
	uint8_t i;
	GPIO_InitTypeDef         GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(I2C_SDA_PORT,&GPIO_InitStructure );
	for(i=0; i<8; i++){
		
		SCL(1);
		delay_us(50);
		temp = (temp<<1)|GPIO_ReadInputDataBit(I2C_SDA_PORT, I2C_SDA_PIN);		
		SCL(0);
		delay_us(50);
	}
	delay_us(50);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin=I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(I2C_SDA_PORT,&GPIO_InitStructure );
	if(ack==1){
		I2C_NWaitAck();
	}
	else
		I2C_WaitAck();
	return temp;
}
void I2C_Write_Byte(unsigned char data, unsigned char time){
	
	I2C_Start();
	Send_Byte(0xD0);
	Send_Byte(time);
	Send_Byte(Dec2BCD(data));
	I2C_Stop();
}
unsigned char I2C_Read_Byte(unsigned char time){
	unsigned char temp;
	I2C_Start();
	Send_Byte(0xD0);
	Send_Byte(time);
	
	I2C_Start();
	Send_Byte(0xD1);
	temp = BCD2Dec(I2C_Receive_Byte(1));
	I2C_Stop();
	return temp;
}