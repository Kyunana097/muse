#ifndef __NOTE_H__
#define __NOTE_H__

#include <REGX52.H>
#include "OLED.h"
#include "RTX51TNY.h"

/* ʹ�ñ�׼C51���ݵ����Ͷ��� */
typedef unsigned char NoteHitResult;

/* �ж������������ */
#define NOTE_MISS     0
#define NOTE_PERFECT  1
#define NOTE_GOOD     2
#define NOTE_BAD      3

/* �ж�����������洢��CODE���� */
extern code const char* HitResults[4];

/* �������� */
NoteHitResult check_note_hit(unsigned char speed, unsigned char track);
void note_spawn(unsigned char speed, unsigned char current_track);

#endif