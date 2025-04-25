// Music.c
#include "Music.h"
#include "Delay.h"

//播放速度，值为四分音符的时长(ms)   120 / 60 = 2 --> 1 / 2 * 1e3 = 500 ms
// 																 100 / 60     --> 60 / 100 * 1e3 = 600ms
#define SPEED	300

//音符与索引对应表，P：休止符，L：低音，M：中音，H：高音，下划线：升半音符号#
#define P	0
#define L1	1
#define L1_	2
#define L2	3
#define L2_	4
#define L3	5
#define L4	6
#define L4_	7
#define L5	8
#define L5_	9
#define L6	10
#define L6_	11
#define L7	12
#define M1	13
#define M1_	14
#define M2	15
#define M2_	16
#define M3	17
#define M4	18
#define M4_	19
#define M5	20
#define M5_	21
#define M6	22
#define M6_	23
#define M7	24
#define H1	25
#define H1_	26
#define H2	27
#define H2_	28
#define H3	29
#define H4	30
#define H4_	31
#define H5	32
#define H5_	33
#define H6	34
#define H6_	35
#define H7	36


//乐谱1
unsigned char code Track1[] =
{
    //音符,时值,track(0:无,1:sky,2:ground)

    H3,4,0,
    H2,2,0,
    H1,4,0,
    H2,2,0,
    H3,3,0,
    H4,1,0,
    H3,2,0,
    H2,4,0,
    P,0,0,

    H3,4,0,
    H2,2,0,
    H1,4,0,
    H2,2,0,
    H3,3,0,
    H4,1,0,
    H3,2,0,
    H2,4,0,
    P,0,0,

    0xFF	//终止标志
};

unsigned char code Track2[] =
{
    //音符,时值,track(0:无,1:sky,2:ground)

    H3,4,0,
    H2,2,0,
    H1,4,0,
    H2,2,0,
    H3,3,0,
    H4,1,0,
    H3,2,0,
    H2,4,0,
    P,0,0,

    H3,4,0,
    H2,2,0,
    H1,4,0,
    H2,2,0,
    H3,3,0,
    H4,1,0,
    H3,2,0,
    H2,4,0,
    P,0,0,

    0xFF	//终止标志
};

unsigned int FreqTable[] = {
    0,
    63628,63731,63835,63928,64021,64103,64185,64260,64331,64400,64463,64528,
    64580,64633,64684,64732,64777,64820,64860,64898,64934,64968,65000,65030,
    65058,65085,65110,65134,65157,65178,65198,65217,65235,65252,65268,65283,
};

// 播放状态变量

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
    if (FreqTable[FreqSelect])	//如果不是休止符
    {
        // 设置定时器初值
        TL1 = Timer1l;
        TH1 = Timer1h;
        Buzzer = !Buzzer;  // 翻转蜂鸣器IO口
    }
}



void note_Play()
{
    // 设置定时器初值
    TL1 = Timer1h;
    TH1 = Timer1l;
    TR1 = 1;
    Delay(duration);
}

void Music_play()
{
    // 获取音符和时值
    if (!currentMusic) 
    {
        Music_Stop();
        return;
    }

    // 检查终止标志
    if (currentMusic[noteIndex] == 0xFF) 
    {
        Music_Stop();
        return;
    }

    FreqSelect = currentMusic[noteIndex];
    noteIndex++;
    duration = SPEED  * currentMusic[noteIndex] / 4;
    noteIndex = noteIndex + 2;
    Timer1h = FreqTable[FreqSelect] / 256;
    Timer1l = FreqTable[FreqSelect] % 256;
    note_Play();
}

void Music_Stop(void) 
{
    TR1 = 0;
    Buzzer = 0;
}