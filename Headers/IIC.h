#ifndef __IIC_H__
#define __IIC_H__

#include <REGX52.H>
#include <intrins.h> // 包含内联汇编函数

// IIC管脚定义
sbit SCL = P2 ^ 1;
sbit SDA = P2 ^ 0;

// IIC操作函数声明
void I2cStart(void);
void I2cStop(void);
bit I2cReadACK(void);
void I2cSendAck(bit n);
void I2cSendByte(unsigned char dat);
unsigned char I2cReadByte(void);
void Delay6us(void);

#endif