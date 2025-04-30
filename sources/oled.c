#include "OLED.h"
#include "IIC.h"
#include "oledfont.h" // ������������

// д��һ���ֽڵ�OLED
void Write_IIC_Byte(unsigned char IIC_Byte) {
    unsigned char i, m, da;
    da = IIC_Byte;
    OLED_SCL = 0;
    for (i = 0; i < 8; i++) {
        m = da & 0x80;
        if (m == 0x80) {
            OLED_SDIN = 1;
        }
        else {
            OLED_SDIN = 0;
        }
        da = da << 1;
        OLED_SCL = 1;
        OLED_SCL = 0;
    }
}

// д�����OLED
void Write_IIC_Command(unsigned char IIC_Command) {
    I2cStart();
    Write_IIC_Byte(0x78); // �ӻ���ַ
    I2cReadACK();
    Write_IIC_Byte(0x00); // д����
    I2cReadACK();
    Write_IIC_Byte(IIC_Command);
    I2cReadACK();
    I2cStop();
}

// д�����ݵ�OLED
void Write_IIC_Data(unsigned char IIC_Data) {
    I2cStart();
    Write_IIC_Byte(0x78); // �ӻ���ַ
    I2cReadACK();
    Write_IIC_Byte(0x40); // д����
    I2cReadACK();
    Write_IIC_Byte(IIC_Data);
    I2cReadACK();
    I2cStop();
}

// д���ֽڵ�OLED
void OLED_WR_Byte(unsigned dat, unsigned cmd) {
    if (cmd) {
        Write_IIC_Data(dat);
    }
    else {
        Write_IIC_Command(dat);
    }
}

// ��ʼ��OLED
void OLED_Init(void) {
    OLED_WR_Byte(0xAE, OLED_CMD); // �ر���ʾ
    OLED_WR_Byte(0x00, OLED_CMD); // ���õ��е�ַ
    OLED_WR_Byte(0x10, OLED_CMD); // ���ø��е�ַ
    OLED_WR_Byte(0x40, OLED_CMD); // ������ʼ�е�ַ
    OLED_WR_Byte(0xB0, OLED_CMD); // ����ҳ��ַ
    OLED_WR_Byte(0x81, OLED_CMD); // ���öԱȶ�
    OLED_WR_Byte(0xFF, OLED_CMD); // �Աȶ�ֵ
    OLED_WR_Byte(0xA1, OLED_CMD); // ���ö���ӳ��
    OLED_WR_Byte(0xA6, OLED_CMD); // ����������ʾ
    OLED_WR_Byte(0xA8, OLED_CMD); // ���ö�·���ñ�
    OLED_WR_Byte(0x3F, OLED_CMD); // ����1/32ռ�ձ�
    OLED_WR_Byte(0xC8, OLED_CMD); // ����COMɨ�跽��
    OLED_WR_Byte(0xD3, OLED_CMD); // ������ʾƫ��
    OLED_WR_Byte(0x00, OLED_CMD); // ƫ��ֵ
    OLED_WR_Byte(0xD5, OLED_CMD); // ����������Ƶ
    OLED_WR_Byte(0x80, OLED_CMD); // ��Ƶֵ
    OLED_WR_Byte(0xD8, OLED_CMD); // �ر�������ɫģʽ
    OLED_WR_Byte(0x05, OLED_CMD); // ����Ԥ�������
    OLED_WR_Byte(0xD9, OLED_CMD); // ����Ԥ�������
    OLED_WR_Byte(0xF1, OLED_CMD); // Ԥ�������ֵ
    OLED_WR_Byte(0xDA, OLED_CMD); // ����COM��������
    OLED_WR_Byte(0x12, OLED_CMD); // ��������ֵ
    OLED_WR_Byte(0xDB, OLED_CMD); // ����Vcomh
    OLED_WR_Byte(0x30, OLED_CMD); // Vcomhֵ
    OLED_WR_Byte(0x8D, OLED_CMD); // ���õ�ɱ�
    OLED_WR_Byte(0x14, OLED_CMD); // ��ɱ�ֵ
    OLED_WR_Byte(0xAF, OLED_CMD); // ����ʾ
}

// ����
void OLED_Clear(void) {
    u8 i, n;
    for (i = 0; i < 8; i++) {
        OLED_WR_Byte(0xB0 + i, OLED_CMD); // ����ҳ��ַ
        OLED_WR_Byte(0x00, OLED_CMD); // ���õ��е�ַ
        OLED_WR_Byte(0x10, OLED_CMD); // ���ø��е�ַ
        for (n = 0; n < 128; n++) {
            OLED_WR_Byte(0x00, OLED_DATA); // д��հ�����
        }
    }
}

// ������ʾλ��
void OLED_Set_Pos(u8 x, u8 y) {
    OLED_WR_Byte(0xB0 + y, OLED_CMD); // ����ҳ��ַ
    OLED_WR_Byte(((x & 0xF0) >> 4) | 0x10, OLED_CMD); // ���ø��е�ַ
    OLED_WR_Byte((x & 0x0F), OLED_CMD); // ���õ��е�ַ
}

// ��ʾһ���ַ�
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 Char_Size) {
    u8 c = chr - ' '; // ��ȡƫ�ƺ���ַ�
    u8 i;
    if (x > 120) {
        x = 0;
        y += 2;
    }
    if (Char_Size == 16) {
        OLED_Set_Pos(x, y);
        for (i = 0; i < 8; i++) {
            OLED_WR_Byte(F8X16[c * 16 + i], OLED_DATA);
        }
        OLED_Set_Pos(x, y + 1);
        for (i = 0; i < 8; i++) {
            OLED_WR_Byte(F8X16[c * 16 + i + 8], OLED_DATA);
        }
    }
    else {
        OLED_Set_Pos(x, y);
        for (i = 0; i < 6; i++) {
            OLED_WR_Byte(F6x8[c][i], OLED_DATA);
        }
    }
}

// ��ʾ�ַ���
void OLED_ShowString(u8 x, u8 y, u8* p, u8 Char_Size) {
    u8 j = 0;
    while (p[j] != '\0') {
        OLED_ShowChar(x, y, p[j], Char_Size);
        x += 8;
        if (x > 120) {
            x = 0;
            y += 2;
        }
        j++;
    }
}

// ��ʾ��ֵ
void OLED_ShowValue(u8 x, u8 y, u32 value, u8 size) {
    u8 i = 0;
    char buffer[5]; // �洢��ֵ���ַ�����ʾ
    buffer[0] = (value / 1000) + '0'; // ǧλ
    buffer[1] = ((value / 100) % 10) + '0'; // ��λ
    buffer[2] = ((value / 10) % 10) + '0'; // ʮλ
    buffer[3] = (value % 10) + '0'; // ��λ
    buffer[4] = '\0'; // �ַ���������

    // ����ǰ����
    while (i < 3 && buffer[i] == '0') {
        i++;
    }

    // ��ʾ�ַ���
    OLED_ShowString(x, y, (u8*)&buffer[i], size);
}

// ������Ļ
void OLED_roll(void) {
    OLED_WR_Byte(0x2E, OLED_CMD); // �رչ���
    OLED_WR_Byte(0x27, OLED_CMD); // ˮƽ�������
    OLED_WR_Byte(0x00, OLED_CMD); // �����ֽ�
    OLED_WR_Byte(0x00, OLED_CMD); // ��ʼҳ
    OLED_WR_Byte(0x05, OLED_CMD); // ����ʱ����
    OLED_WR_Byte(0x01, OLED_CMD); // ��ֹҳ
    OLED_WR_Byte(0x00, OLED_CMD); // �����ֽ�
    OLED_WR_Byte(0x7F, OLED_CMD); // �����ֽ�
    OLED_WR_Byte(0x2F, OLED_CMD); // ��������
}