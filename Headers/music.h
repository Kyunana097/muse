// Music.h
#ifndef MUSIC_H
#define MUSIC_H
#ifndef NULL
#define NULL ((void *)0)
#endif
#include <REGX52.H>

// �������˿ڶ���
sbit Buzzer = P2 ^ 5;
unsigned char code Track1[];
unsigned char code Track2[];

// ���ſ��ƽӿ�
void Music_Init(unsigned char* musicData);
void note_Play(void);
void Music_play(void);
void Music_Stop(void);

#endif