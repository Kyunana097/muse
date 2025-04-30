#include "AT24C02.h"
#include "IIC.h"

unsigned char AckFlag = 0; // 应答标志

// 全局变量定义
unsigned char AT24C02_WriteData[4] = { 0 }; // 默认初始化为0
unsigned char AT24C02_ReadData[4] = { 0 };  // 默认初始化为0


// 将一个16位整数拆分为4个字节并存储到EEPROM
void AT24C02_WriteValue(unsigned char addr, unsigned short value) {
    // 将16位整数拆分为4个字节
    AT24C02_WriteData[0] = (value >> 24) & 0xFF; // 高字节
    AT24C02_WriteData[1] = (value >> 16) & 0xFF;
    AT24C02_WriteData[2] = (value >> 8) & 0xFF;
    AT24C02_WriteData[3] = value & 0xFF;         // 低字节

    // 调用原写入函数
    AT24C02WriteBytes(addr);
}

// 从EEPROM读取4个字节并还原为一个16位整数
unsigned short AT24C02_ReadValue(unsigned char addr) {
    unsigned short value = 0;

    // 调用原读取函数
    AT24C02ReadBytes(addr);

    // 将4个字节还原为16位整数
    value = (AT24C02_ReadData[0] << 24) | (AT24C02_ReadData[1] << 16) |
        (AT24C02_ReadData[2] << 8) | AT24C02_ReadData[3];

    return value;
}

void AT24C02WriteBytes(unsigned char addr) {
    unsigned char i;
    I2cStart();
    I2cSendByte(AT24C02ADDR + 0); // 写命令
    if (I2cReadACK() == 0) {
        AckFlag = 0; // 应答成功
    }
    else {
        AckFlag = 1; // 应答失败
        I2cStop();
        return;
    }

    I2cSendByte(addr); // 发送起始地址
    if (I2cReadACK() == 0) {
        AckFlag = 0; // 应答成功
    }
    else {
        AckFlag = 1; // 应答失败
        I2cStop();
        return;
    }

    for (i = 0; i < 4; i++) {
        I2cSendByte(AT24C02_WriteData[i]); // 发送数据
        if (I2cReadACK() == 0) {
            AckFlag = 0; // 应答成功
        }
        else {
            AckFlag = 1; // 应答失败
            break;
        }
    }
    I2cStop();
}

void AT24C02ReadBytes(unsigned char addr)
{
    unsigned char i;
    I2cStart();
    I2cSendByte(AT24C02ADDR + 0); // 写命令
    if (I2cReadACK() == 0) {
        AckFlag = 0; // 应答成功
    }
    else {
        AckFlag = 1; // 应答失败
        I2cStop();
        return;
    }

    I2cSendByte(addr); // 发送起始地址
    if (I2cReadACK() == 0) {
        AckFlag = 0; // 应答成功
    }
    else {
        AckFlag = 1; // 应答失败
        I2cStop();
        return;
    }

    I2cStart();
    I2cSendByte(AT24C02ADDR + 1); // 读命令
    if (I2cReadACK() == 0) {
        AckFlag = 0; // 应答成功
    }
    else {
        AckFlag = 1; // 应答失败
        I2cStop();
        return;
    }

    for (i = 0; i < 4; i++) {
        AT24C02_ReadData[i] = I2cReadByte(); // 读取数据
        if (i < 3) {
            I2cSendAck(0); // 发送应答
        }
        else {
            I2cSendAck(1); // 发送非应答
        }
    }
    I2cStop();
}