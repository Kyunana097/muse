// Music.h
#ifndef MUSIC_H
#define MUSIC_H

#include <REGX52.H>

// 蜂鸣器端口定义
sbit Buzzer = P2 ^ 5;

// 播放控制接口
void Music_Init(void);
void Music_Play(unsigned char* musicData);
void Music_Stop(void);
bit Music_IsPlaying(void);

#endif