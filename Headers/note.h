#ifndef __NOTE_H__
#define __NOTE_H__

extern unsigned char track_position;	//�������λ�� 1-2
extern unsigned char speed;				//���������ٶ� 1-8
extern unsigned char x_position;		//����ˮƽλ�� 0-128

void note_spawn(track);

#endif // !__NOTE_H__
