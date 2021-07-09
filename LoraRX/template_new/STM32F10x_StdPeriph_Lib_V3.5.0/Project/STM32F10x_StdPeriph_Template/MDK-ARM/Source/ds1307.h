#ifndef __DS1307_H
#define __DS1307_H
#include "stm32f10x.h"
#include "stm32_eval.h"

#define Seconds	0x00
#define Minutes	0x01
#define Hours	0x02
#define Day		0x03
#define Date	0x04
#define Month	0x05
#define Year	0x06
#define	I2C_SCL_PIN		GPIO_Pin_6
#define	I2C_SDA_PIN		GPIO_Pin_7
#define	I2C_SCL_PORT	GPIOB
#define	I2C_SDA_PORT	GPIOB
#define SCL(x) ((x) ? (GPIO_SetBits(I2C_SCL_PORT, I2C_SCL_PIN)) : (GPIO_ResetBits(I2C_SCL_PORT, I2C_SCL_PIN)))
#define SDA(x) ((x) ? (GPIO_SetBits(I2C_SDA_PORT, I2C_SDA_PIN)) : (GPIO_ResetBits(I2C_SDA_PORT, I2C_SDA_PIN)))
void I2C_INIT(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_WaitAck(void);
char Send_Byte(char data);
char I2C_Receive_Byte(char ack);
void I2C_Write_Byte(unsigned char data, unsigned char time);
unsigned char I2C_Read_Byte(unsigned char time);
#endif 
