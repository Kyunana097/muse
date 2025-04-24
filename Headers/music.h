// Music.h
#ifndef MUSIC_H
#define MUSIC_H

#include <REGX52.H>

// �������˿ڶ���
sbit Buzzer = P2 ^ 5;

// ���ſ��ƽӿ�
void Music_Init(void);
void Music_Play(unsigned char* musicData);
void Music_Stop(void);
bit Music_IsPlaying(void);

#endif