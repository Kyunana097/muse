#ifndef __NOTE_H__
#define __NOTE_H__

#include <REGX52.H>
#include "OLED.h"
#include "RTX51TNY.h"

// 使用标准C51兼容的类型定义
typedef unsigned char NoteHitResult;

// 判定结果常量定义
#define NOTE_MISS     0
#define NOTE_BAD      1
#define NOTE_GOOD     2
#define NOTE_PERFECT  3

// 音符池大小
#define MAX_NOTES_PER_TRACK 5

extern code unsigned char track[];

// 判定结果统计数组
extern unsigned char data hit_result_counts[];

// 全局变量声明
extern unsigned char combo;

// 函数声明
unsigned char check_key_hit(unsigned char speed, unsigned char track);
void note_spawn(unsigned char speed, const unsigned char code* track);
void update_note_positions(unsigned char speed);
unsigned char check_miss_notes(unsigned char speed);

#endif // __NOTE_H__	