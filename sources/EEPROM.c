#include "EEPROM.h"

// IAP���к���
static void IAP_Idle(void) {
    IAP_CONTR = 0;      // �ر�IAP����
    IAP_CMD = 0;        // �������Ĵ���
    IAP_TRIG = 0;       // ��������Ĵ���
    IAP_ADDRH = 0x80;   // ��ַ�ָ�ΪĬ��
    IAP_ADDRL = 0;
}

// EEPROM��ʼ��
void EEPROM_Init(void) {
    // ������ӱ�Ҫ�ĳ�ʼ������
}

// ��EEPROM��ȡһ���ֽ�
unsigned char EEPROM_ReadByte(unsigned int addr) {
    unsigned char dat;
    IAP_CONTR = 0x80;   // ʹ��IAP
    IAP_CMD = 1;        // ���ö�����
    IAP_ADDRL = addr;
    IAP_ADDRH = addr >> 8;
    IAP_TRIG = 0x5a;
    IAP_TRIG = 0xa5;
    _nop_();
    dat = IAP_DATA;
    IAP_Idle();
    return dat;
}

// ��EEPROMд��һ���ֽ�
void EEPROM_WriteByte(unsigned int addr, unsigned char dat) {
    IAP_CONTR = 0x80;   // ʹ��IAP
    IAP_CMD = 2;        // ����д����
    IAP_ADDRL = addr;
    IAP_ADDRH = addr >> 8;
    IAP_DATA = dat;
    IAP_TRIG = 0x5a;
    IAP_TRIG = 0xa5;
    _nop_();
    IAP_Idle();
}

// ��EEPROMд�����ݿ�
void EEPROM_WriteData(unsigned int addr, unsigned char* data, unsigned int len) {
    unsigned char* p = (unsigned char*)data;
    while (len--) {
        EEPROM_WriteByte(addr++, *p++);
    }
}

// ��EEPROM��ȡ���ݿ�
void EEPROM_ReadData(unsigned int addr, unsigned char* data, unsigned int len) {
    unsigned char* p = (unsigned char*)data;
    while (len--) {
        *p++ = EEPROM_ReadByte(addr++);
    }
}