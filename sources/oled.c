#include "OLED.h"
#include "IIC.h"
#include "oledfont.h" // 包含字体数据

// 写入一个字节到OLED
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

// 写入命令到OLED
void Write_IIC_Command(unsigned char IIC_Command) {
    I2cStart();
    Write_IIC_Byte(0x78); // 从机地址
    I2cReadACK();
    Write_IIC_Byte(0x00); // 写命令
    I2cReadACK();
    Write_IIC_Byte(IIC_Command);
    I2cReadACK();
    I2cStop();
}

// 写入数据到OLED
void Write_IIC_Data(unsigned char IIC_Data) {
    I2cStart();
    Write_IIC_Byte(0x78); // 从机地址
    I2cReadACK();
    Write_IIC_Byte(0x40); // 写数据
    I2cReadACK();
    Write_IIC_Byte(IIC_Data);
    I2cReadACK();
    I2cStop();
}

// 写入字节到OLED
void OLED_WR_Byte(unsigned dat, unsigned cmd) {
    if (cmd) {
        Write_IIC_Data(dat);
    }
    else {
        Write_IIC_Command(dat);
    }
}

// 初始化OLED
void OLED_Init(void) {
    OLED_WR_Byte(0xAE, OLED_CMD); // 关闭显示
    OLED_WR_Byte(0x00, OLED_CMD); // 设置低列地址
    OLED_WR_Byte(0x10, OLED_CMD); // 设置高列地址
    OLED_WR_Byte(0x40, OLED_CMD); // 设置起始行地址
    OLED_WR_Byte(0xB0, OLED_CMD); // 设置页地址
    OLED_WR_Byte(0x81, OLED_CMD); // 设置对比度
    OLED_WR_Byte(0xFF, OLED_CMD); // 对比度值
    OLED_WR_Byte(0xA1, OLED_CMD); // 设置段重映射
    OLED_WR_Byte(0xA6, OLED_CMD); // 设置正常显示
    OLED_WR_Byte(0xA8, OLED_CMD); // 设置多路复用比
    OLED_WR_Byte(0x3F, OLED_CMD); // 设置1/32占空比
    OLED_WR_Byte(0xC8, OLED_CMD); // 设置COM扫描方向
    OLED_WR_Byte(0xD3, OLED_CMD); // 设置显示偏移
    OLED_WR_Byte(0x00, OLED_CMD); // 偏移值
    OLED_WR_Byte(0xD5, OLED_CMD); // 设置振荡器分频
    OLED_WR_Byte(0x80, OLED_CMD); // 分频值
    OLED_WR_Byte(0xD8, OLED_CMD); // 关闭区域颜色模式
    OLED_WR_Byte(0x05, OLED_CMD); // 设置预充电周期
    OLED_WR_Byte(0xD9, OLED_CMD); // 设置预充电周期
    OLED_WR_Byte(0xF1, OLED_CMD); // 预充电周期值
    OLED_WR_Byte(0xDA, OLED_CMD); // 设置COM引脚配置
    OLED_WR_Byte(0x12, OLED_CMD); // 引脚配置值
    OLED_WR_Byte(0xDB, OLED_CMD); // 设置Vcomh
    OLED_WR_Byte(0x30, OLED_CMD); // Vcomh值
    OLED_WR_Byte(0x8D, OLED_CMD); // 设置电荷泵
    OLED_WR_Byte(0x14, OLED_CMD); // 电荷泵值
    OLED_WR_Byte(0xAF, OLED_CMD); // 打开显示
}

// 清屏
void OLED_Clear(void) {
    u8 i, n;
    for (i = 0; i < 8; i++) {
        OLED_WR_Byte(0xB0 + i, OLED_CMD); // 设置页地址
        OLED_WR_Byte(0x00, OLED_CMD); // 设置低列地址
        OLED_WR_Byte(0x10, OLED_CMD); // 设置高列地址
        for (n = 0; n < 128; n++) {
            OLED_WR_Byte(0x00, OLED_DATA); // 写入空白数据
        }
    }
}

// 设置显示位置
void OLED_Set_Pos(u8 x, u8 y) {
    OLED_WR_Byte(0xB0 + y, OLED_CMD); // 设置页地址
    OLED_WR_Byte(((x & 0xF0) >> 4) | 0x10, OLED_CMD); // 设置高列地址
    OLED_WR_Byte((x & 0x0F), OLED_CMD); // 设置低列地址
}

// 显示一个字符
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 Char_Size) {
    u8 c = chr - ' '; // 获取偏移后的字符
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

// 显示字符串
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

// 显示数值
void OLED_ShowValue(u8 x, u8 y, u32 value, u8 size) {
    u8 i = 0;
    char buffer[5]; // 存储数值的字符串表示
    buffer[0] = (value / 1000) + '0'; // 千位
    buffer[1] = ((value / 100) % 10) + '0'; // 百位
    buffer[2] = ((value / 10) % 10) + '0'; // 十位
    buffer[3] = (value % 10) + '0'; // 个位
    buffer[4] = '\0'; // 字符串结束符

    // 跳过前导零
    while (i < 3 && buffer[i] == '0') {
        i++;
    }

    // 显示字符串
    OLED_ShowString(x, y, (u8*)&buffer[i], size);
}

// 滚动屏幕
void OLED_roll(void) {
    OLED_WR_Byte(0x2E, OLED_CMD); // 关闭滚动
    OLED_WR_Byte(0x27, OLED_CMD); // 水平向左滚动
    OLED_WR_Byte(0x00, OLED_CMD); // 虚拟字节
    OLED_WR_Byte(0x00, OLED_CMD); // 起始页
    OLED_WR_Byte(0x05, OLED_CMD); // 滚动时间间隔
    OLED_WR_Byte(0x01, OLED_CMD); // 终止页
    OLED_WR_Byte(0x00, OLED_CMD); // 虚拟字节
    OLED_WR_Byte(0x7F, OLED_CMD); // 虚拟字节
    OLED_WR_Byte(0x2F, OLED_CMD); // 开启滚动
}