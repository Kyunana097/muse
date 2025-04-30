#include "AT24C02.h"
#include "IIC.h"

unsigned char AckFlag = 0; // Ӧ���־

// ȫ�ֱ�������
unsigned char AT24C02_WriteData[4] = { 0 }; // Ĭ�ϳ�ʼ��Ϊ0
unsigned char AT24C02_ReadData[4] = { 0 };  // Ĭ�ϳ�ʼ��Ϊ0


// ��һ��16λ�������Ϊ4���ֽڲ��洢��EEPROM
void AT24C02_WriteValue(unsigned char addr, unsigned short value) {
    // ��16λ�������Ϊ4���ֽ�
    AT24C02_WriteData[0] = (value >> 24) & 0xFF; // ���ֽ�
    AT24C02_WriteData[1] = (value >> 16) & 0xFF;
    AT24C02_WriteData[2] = (value >> 8) & 0xFF;
    AT24C02_WriteData[3] = value & 0xFF;         // ���ֽ�

    // ����ԭд�뺯��
    AT24C02WriteBytes(addr);
}

// ��EEPROM��ȡ4���ֽڲ���ԭΪһ��16λ����
unsigned short AT24C02_ReadValue(unsigned char addr) {
    unsigned short value = 0;

    // ����ԭ��ȡ����
    AT24C02ReadBytes(addr);

    // ��4���ֽڻ�ԭΪ16λ����
    value = (AT24C02_ReadData[0] << 24) | (AT24C02_ReadData[1] << 16) |
        (AT24C02_ReadData[2] << 8) | AT24C02_ReadData[3];

    return value;
}

void AT24C02WriteBytes(unsigned char addr) {
    unsigned char i;
    I2cStart();
    I2cSendByte(AT24C02ADDR + 0); // д����
    if (I2cReadACK() == 0) {
        AckFlag = 0; // Ӧ��ɹ�
    }
    else {
        AckFlag = 1; // Ӧ��ʧ��
        I2cStop();
        return;
    }

    I2cSendByte(addr); // ������ʼ��ַ
    if (I2cReadACK() == 0) {
        AckFlag = 0; // Ӧ��ɹ�
    }
    else {
        AckFlag = 1; // Ӧ��ʧ��
        I2cStop();
        return;
    }

    for (i = 0; i < 4; i++) {
        I2cSendByte(AT24C02_WriteData[i]); // ��������
        if (I2cReadACK() == 0) {
            AckFlag = 0; // Ӧ��ɹ�
        }
        else {
            AckFlag = 1; // Ӧ��ʧ��
            break;
        }
    }
    I2cStop();
}

void AT24C02ReadBytes(unsigned char addr)
{
    unsigned char i;
    I2cStart();
    I2cSendByte(AT24C02ADDR + 0); // д����
    if (I2cReadACK() == 0) {
        AckFlag = 0; // Ӧ��ɹ�
    }
    else {
        AckFlag = 1; // Ӧ��ʧ��
        I2cStop();
        return;
    }

    I2cSendByte(addr); // ������ʼ��ַ
    if (I2cReadACK() == 0) {
        AckFlag = 0; // Ӧ��ɹ�
    }
    else {
        AckFlag = 1; // Ӧ��ʧ��
        I2cStop();
        return;
    }

    I2cStart();
    I2cSendByte(AT24C02ADDR + 1); // ������
    if (I2cReadACK() == 0) {
        AckFlag = 0; // Ӧ��ɹ�
    }
    else {
        AckFlag = 1; // Ӧ��ʧ��
        I2cStop();
        return;
    }

    for (i = 0; i < 4; i++) {
        AT24C02_ReadData[i] = I2cReadByte(); // ��ȡ����
        if (i < 3) {
            I2cSendAck(0); // ����Ӧ��
        }
        else {
            I2cSendAck(1); // ���ͷ�Ӧ��
        }
    }
    I2cStop();
}