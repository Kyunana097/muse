// Music.c
#include "Music.h"

// 音符与索引对应表（同前）
#define P 0

//播放速度，值为四分音符的时长(ms)   120 / 60 = 2 --> 1 / 2 * 1e3 = 500 ms
// 																 100 / 60     --> 60 / 100 * 1e3 = 600ms
#define SPEED	600

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

unsigned int FreqTable[] = {
    0,
    63628,63731,63835,63928,64021,64103,64185,64260,64331,64400,64463,64528,
    64580,64633,64684,64732,64777,64820,64860,64898,64934,64968,65000,65030,
    65058,65085,65110,65134,65157,65178,65198,65217,65235,65252,65268,65283,
};

// 播放状态变量
volatile bit isPlaying = 0;
volatile unsigned char* currentMusic = NULL;
volatile unsigned int noteIndex = 0;
volatile unsigned int noteDuration = 0;

void Timer1_Routine() interrupt 3 {
    TH1 = currentMusic ? FreqTable[currentMusic[noteIndex]] / 256 : 0;
    TL1 = currentMusic ? FreqTable[currentMusic[noteIndex]] % 256 : 0;

    if (currentMusic && currentMusic[noteIndex] != P) {
        Buzzer = !Buzzer;
    }
    else {
        Buzzer = 0;
    }

    // 音符时长计时
    if (noteDuration > 0) {
        noteDuration--;
        if (noteDuration == 0) {
            // 切换到下一个音符
            noteIndex += 2; // 跳过时值
            if (currentMusic[noteIndex - 1] == 0xFF) {
                MusicPlayer_Stop();
            }
        }
    }
}

void Music_Init(void) {
    TMOD &= 0x0F;
    TMOD |= 0x10;
    TH1 = 0xFC;
    TL1 = 0x18;
    TF1 = 0;
    TR1 = 0;
    ET1 = 1;
    EA = 1;
    isPlaying = 0;
}

void Music_Play(unsigned char* musicData) {
    if (isPlaying) return;

    currentMusic = musicData;
    noteIndex = 0;
    isPlaying = 1;
    TR1 = 1;

    // 加载第一个音符
    if (currentMusic[noteIndex] != 0xFF) {
        unsigned int freq = (currentMusic[noteIndex] == P) ? 0 : FreqTable[currentMusic[noteIndex]];
        noteDuration = SPEED * currentMusic[noteIndex + 1] / 4;
        TH1 = freq / 256;
        TL1 = freq % 256;
    }
}

void Music_Stop(void) {
    isPlaying = 0;
    TR1 = 0;
    Buzzer = 0;
    currentMusic = NULL;
}

bit Music_IsPlaying(void) {
    return isPlaying;
}