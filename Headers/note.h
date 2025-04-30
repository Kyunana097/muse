#ifndef __NOTE_H__
#define __NOTE_H__

#include <REGX52.H>
#include "OLED.h"
#include "RTX51TNY.h"

// ʹ�ñ�׼C51���ݵ����Ͷ���
typedef unsigned char NoteHitResult;

// �ж������������
#define NOTE_MISS     0
#define NOTE_BAD      1
#define NOTE_GOOD     2
#define NOTE_PERFECT  3

// �����ش�С
#define MAX_NOTES_PER_TRACK 5

extern code unsigned char track[];

// �ж����ͳ������
extern unsigned char data hit_result_counts[];

// ȫ�ֱ�������
extern unsigned char combo;

// ��������
unsigned char check_key_hit(unsigned char speed, unsigned char track);
void note_spawn(unsigned char speed, const unsigned char code* track);
void update_note_positions(unsigned char speed);
unsigned char check_miss_notes(unsigned char speed);

#endif // __NOTE_H__	