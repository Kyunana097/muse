#ifndef _EEPROM_H_
#define _EEPROM_H_

#include <reg52.h>
#include <intrins.h>

// 函数声明
void EEPROM_Init(void);
unsigned char EEPROM_ReadByte(unsigned int addr);
void EEPROM_WriteByte(unsigned int addr, unsigned char dat);
void EEPROM_WriteData(unsigned int addr, unsigned char* data, unsigned int len);
void EEPROM_ReadData(unsigned int addr, unsigned char* data, unsigned int len);

// 常用地址定义（根据实际需求调整）
#define EEPROM_ADDR_TRACK1_SCORE  0x0000
#define EEPROM_ADDR_TRACK2_SCORE  0x0004

#endif