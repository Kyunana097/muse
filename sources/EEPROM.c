#include "EEPROM.h"

// IAP空闲函数
static void IAP_Idle(void) {
    IAP_CONTR = 0;      // 关闭IAP功能
    IAP_CMD = 0;        // 清除命令寄存器
    IAP_TRIG = 0;       // 清除触发寄存器
    IAP_ADDRH = 0x80;   // 地址恢复为默认
    IAP_ADDRL = 0;
}

// EEPROM初始化
void EEPROM_Init(void) {
    // 可以添加必要的初始化代码
}

// 从EEPROM读取一个字节
unsigned char EEPROM_ReadByte(unsigned int addr) {
    unsigned char dat;
    IAP_CONTR = 0x80;   // 使能IAP
    IAP_CMD = 1;        // 设置读命令
    IAP_ADDRL = addr;
    IAP_ADDRH = addr >> 8;
    IAP_TRIG = 0x5a;
    IAP_TRIG = 0xa5;
    _nop_();
    dat = IAP_DATA;
    IAP_Idle();
    return dat;
}

// 向EEPROM写入一个字节
void EEPROM_WriteByte(unsigned int addr, unsigned char dat) {
    IAP_CONTR = 0x80;   // 使能IAP
    IAP_CMD = 2;        // 设置写命令
    IAP_ADDRL = addr;
    IAP_ADDRH = addr >> 8;
    IAP_DATA = dat;
    IAP_TRIG = 0x5a;
    IAP_TRIG = 0xa5;
    _nop_();
    IAP_Idle();
}

// 向EEPROM写入数据块
void EEPROM_WriteData(unsigned int addr, unsigned char* data, unsigned int len) {
    unsigned char* p = (unsigned char*)data;
    while (len--) {
        EEPROM_WriteByte(addr++, *p++);
    }
}

// 从EEPROM读取数据块
void EEPROM_ReadData(unsigned int addr, unsigned char* data, unsigned int len) {
    unsigned char* p = (unsigned char*)data;
    while (len--) {
        *p++ = EEPROM_ReadByte(addr++);
    }
}