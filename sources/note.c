#include "note.h"
// 修改为unsigned char数组
static int x_position[2] = { 0 };
static unsigned char is_active[2] = { 0 };  // 0=非激活, 1=激活
int hit_time ;
unsigned char track_idx;
unsigned char y_pos;

code const char* HitResults[] = 
{
    "Miss   ", "Perfect", "Good   ", "Bad    "
};


NoteHitResult check_note_hit(unsigned char speed, unsigned char track)
{
    track_idx = track - 1;
    y_pos = (track == 1) ? 0 : 3;
    if (is_active[track_idx] && x_position[track_idx] < 40)
    {
        OLED_ShowString(x_position[track_idx], y_pos, " ", 16);
        is_active[track_idx] = 0; // 设置为非激活

        hit_time = (x_position[track_idx] - 16) / speed;

        if (hit_time < 1) return NOTE_PERFECT;
        else if (hit_time < 2) return NOTE_GOOD;
        else return NOTE_BAD;
    }
    return NOTE_MISS;
}

void note_spawn(unsigned char speed, unsigned char track)
{
    track_idx = track - 1;
    y_pos = (track == 1) ? 0 : 3;
    if (!is_active[track_idx]) 
    {
        x_position[track_idx] = 121 - speed * 2;
        is_active[track_idx] = 1;  // 设置为激活
    }

    if (x_position[track_idx] > speed && x_position[track_idx] < 120)
    {
        OLED_ShowString(x_position[track_idx], y_pos, " ", 16);
        x_position[track_idx] -= speed;
        OLED_ShowString(x_position[track_idx], y_pos, "Q", 16);
    }
    else 
    {
        OLED_ShowString(x_position[track_idx], y_pos, " ", 16);
        OLED_ShowString(3, 6, "Miss   ", 8);
        is_active[track_idx] = 0;  // 设置为非激活
    }
}