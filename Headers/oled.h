#ifndef __OLED_H__
#define __OLED_H__

#include <REGX52.H>
#include "IIC.h"

#define u8 unsigned char
#define u32 unsigned int

#define OLED_CMD 0 // 写命令
#define OLED_DATA 1 // 写数据

// OLED IIC管脚定义
sbit OLED_SCL = P2 ^ 1; // SCLK
sbit OLED_SDIN = P2 ^ 0; // SDIN

// OLED操作函数声明
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 Char_Size);
void OLED_ShowString(u8 x, u8 y, u8* p, u8 Char_Size);
void OLED_Set_Pos(u8 x, u8 y);
void OLED_ShowValue(u8 x, u8 y, u32 value, u8 size);
void OLED_roll(void);

#endif