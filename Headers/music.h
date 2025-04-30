// Music.h
#ifndef MUSIC_H
#define MUSIC_H
#ifndef NULL
#define NULL ((void *)0)
#endif
#include <REGX52.H>

// �������˿ڶ���
sbit Buzzer = P2 ^ 5;
extern unsigned char code track[];
extern unsigned char code Track1[];
extern unsigned char code Track2[];

// ���ſ��ƽӿ�
void Music_Init(unsigned char* musicData);
void Music_PlayFullTrack();
void Music_Stop(void);

#endif