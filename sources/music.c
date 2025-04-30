// Music.c
#include "Music.h"
#include "Delay.h"

//�����ٶȣ�ֵΪ�ķ�������ʱ��(ms)   120 / 60 = 2 --> 1 / 2 * 1e3 = 500 ms
// 																 100 / 60     --> 60 / 100 * 1e3 = 600ms
#define SPEED	300
/*


unsigned int FreqTable[] = {
    0,
    63628,63731,63835,63928,64021,64103,64185,64260,64331,64400,64463,64528,
    64580,64633,64684,64732,64777,64820,64860,64898,64934,64968,65000,65030,
    65058,65085,65110,65134,65157,65178,65198,65217,65235,65252,65268,65283,
};

// ����״̬����

unsigned char FreqSelect;
int duration;
unsigned char Timer1h,Timer1l;
volatile unsigned char* currentMusic = NULL;
volatile unsigned int noteIndex = 0;

void Music_Init(unsigned char* TrackData)
{
    TMOD &= 0x0F;
    TMOD |= 0x10;
    TH1 = 0xFC;
    TL1 = 0x18;
    TF1 = 0;
    TR1 = 0;
    ET1 = 1;
    EA = 1;
    noteIndex = 0;
    currentMusic = TrackData;
}

void Timer1_Routine() interrupt 3
{
    if (FreqTable[FreqSelect])	//���������ֹ��
    {
        // ���ö�ʱ����ֵ
        TL1 = Timer1l;
        TH1 = Timer1h;
        Buzzer = !Buzzer;  // ��ת������IO��
    }
}

void Music_PlayFullTrack()
{
    if (!currentMusic)
    {
        Music_Stop();
        return;
    }

    while (1)
    {
        // �����ֹ��־
        if (currentMusic[noteIndex] == 0xFF)
        {
            Music_Stop();
            return;
        }

        // ���ŵ�ǰ����
        FreqSelect = currentMusic[noteIndex];
        noteIndex++;
        duration = SPEED * currentMusic[noteIndex] / 4;
        noteIndex++;
        Timer1h = FreqTable[FreqSelect] / 256;
        Timer1l = FreqTable[FreqSelect] % 256;

        TR1 = 1;
        Delay(duration);
        TR1 = 0;

        // ����ͣ�ٷ�ֹ����ճ��
        Delay(10);
    }
}

void Music_Stop(void) 
{
    TR1 = 0;
    Buzzer = 0;
}