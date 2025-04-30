#ifndef __AT24C02_H__
#define __AT24C02_H__

#define AT24C02ADDR 0xA0

// ȫ�ֱ�������
extern unsigned char AT24C02_WriteData[4]; // ����д�������
extern unsigned char AT24C02_ReadData[4];  // ���ڶ�ȡ������

// ��������
void AT24C02WriteBytes(unsigned char addr);
void AT24C02ReadBytes(unsigned char addr);
void AT24C02_WriteValue(unsigned char addr, unsigned short value);
unsigned short AT24C02_ReadValue(unsigned char addr);

#endif