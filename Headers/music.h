// Music.h
#ifndef MUSIC_H
#define MUSIC_H
#ifndef NULL
#define NULL ((void *)0)
#endif
#include <REGX52.H>

// 蜂鸣器端口定义
sbit Buzzer = P2 ^ 5;
extern unsigned char code track[];
extern unsigned char code Track1[];
extern unsigned char code Track2[];

// 播放控制接口
void Music_Init(unsigned char* musicData);
void Music_PlayFullTrack();
void Music_Stop(void);

#endif