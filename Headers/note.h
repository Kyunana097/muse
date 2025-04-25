#ifndef __NOTE_H__
#define __NOTE_H__

#include <REGX52.H>
#include "OLED.h"
#include "RTX51TNY.h"

/* 使用标准C51兼容的类型定义 */
typedef unsigned char NoteHitResult;

/* 判定结果常量定义 */
#define NOTE_MISS     0
#define NOTE_PERFECT  1
#define NOTE_GOOD     2
#define NOTE_BAD      3

/* 判定结果描述表（存储在CODE区） */
extern code const char* HitResults[4];

/* 函数声明 */
NoteHitResult check_note_hit(unsigned char speed, unsigned char track);
void note_spawn(unsigned char speed, unsigned char current_track);

#endif