#include "IIC.h"

bit ack = 0;
// IIC起始信号
void I2cStart(void) {
    SCL = 1;
    SDA = 1;
    SDA = 0;
    Delay6us();
}

// IIC停止信号
void I2cStop(void) {
    SCL = 0;
    SDA = 0;
    SCL = 1;
    Delay6us();
    SDA = 1;
    Delay6us();
}

// 读取IIC应答信号
bit I2cReadACK(void) {
    SCL = 0;
    SDA = 1; // 释放SDA，准备读取应答信号
    SCL = 1;
    Delay6us();
    ack = SDA;
    SCL = 0;
    return ack;
}

// 发送IIC应答信号
void I2cSendAck(bit n) {
    SCL = 0;
    SDA = n; // n=1表示不发送应答，n=0表示发送应答
    SCL = 1;
    Delay6us();
    SCL = 0;
    SDA = 1; // 释放SDA
}

// 发送一个字节数据
void I2cSendByte(unsigned char dat) {
    unsigned char i;
    for (i = 0; i < 8; i++) {
        SCL = 0;
        if (dat & 0x80) {
            SDA = 1;
        }
        else {
            SDA = 0;
        }
        SCL = 1;
        dat = dat << 1;
    }
    SCL = 0;
    SDA = 1;
}

// 读取一个字节数据
unsigned char I2cReadByte(void) {
    unsigned char i, dat = 0;
    SDA = 1; // 释放SDA，准备读取数据
    for (i = 0; i < 8; i++) {
        dat = dat << 1;
        SCL = 0;
        SCL = 1;
        if (SDA == 1) {
            dat |= 0x01;
        }
    }
    return dat;
}

// 延时函数
void Delay6us(void) {
    _nop_();
}