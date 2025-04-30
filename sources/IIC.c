#include "IIC.h"

bit ack = 0;
// IIC��ʼ�ź�
void I2cStart(void) {
    SCL = 1;
    SDA = 1;
    SDA = 0;
    Delay6us();
}

// IICֹͣ�ź�
void I2cStop(void) {
    SCL = 0;
    SDA = 0;
    SCL = 1;
    Delay6us();
    SDA = 1;
    Delay6us();
}

// ��ȡIICӦ���ź�
bit I2cReadACK(void) {
    SCL = 0;
    SDA = 1; // �ͷ�SDA��׼����ȡӦ���ź�
    SCL = 1;
    Delay6us();
    ack = SDA;
    SCL = 0;
    return ack;
}

// ����IICӦ���ź�
void I2cSendAck(bit n) {
    SCL = 0;
    SDA = n; // n=1��ʾ������Ӧ��n=0��ʾ����Ӧ��
    SCL = 1;
    Delay6us();
    SCL = 0;
    SDA = 1; // �ͷ�SDA
}

// ����һ���ֽ�����
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

// ��ȡһ���ֽ�����
unsigned char I2cReadByte(void) {
    unsigned char i, dat = 0;
    SDA = 1; // �ͷ�SDA��׼����ȡ����
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

// ��ʱ����
void Delay6us(void) {
    _nop_();
}