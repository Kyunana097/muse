#include "note.h"
#include "oled.h"


extern code unsigned char track[] = 
{ 
    29, 4, 1, 
    30, 4, 2,
    32, 4, 0, 
    34, 4, 2, 
    29, 4, 1,
    30, 4, 2,
    32, 4, 0, 
    34, 4, 2,
    29, 4, 1, 
    30, 4, 2,
    32, 4, 0,
    34, 4, 2, 
    29, 4, 1,
    30, 4, 2,
    32, 4, 0, 
    34, 4, 2, 
    0, 0, 0 };

//乐谱1
extern unsigned char code Track1[] =
{
    //音符,时值

    29,4,
    27,2,
    25,4,
    27,2,
    29,3,
    30,1,
    29,2,
    27,4,
    0,0,

    29,4,
    27,2,
    25,4,
    27,2,
    29,3,
    30,1,
    29,2,
    27,4,
    0,0,

    0xFF	//终止标志
};

extern unsigned char code Track2[] =
{
    //音符,时值,track(0:无,1:sky,2:ground)

    25,2,
    29,2,
    32,6,
    34,2,
    32,2,
    29,2,
    25,2,
    29,2,
    27,4,
    25,2,
    27,2,
    29,4,
    0,0,

    25,2,
    29,2,
    32,6,
    34,2,
    32,2,
    29,2,
    25,2,
    29,2,
    27,4,
    25,2,
    24,2,
    25,4,
    0,0,

    0xFF	//终止标志
};
/* 全局变量声明 */
 static unsigned char xdata x_position[2][MAX_NOTES_PER_TRACK]; // 使用xdata扩展内存
 static unsigned char data is_active[2] = { 0 }; // 使用内部RAM存储活动状态
 unsigned char data note_count[2] = { 0 }; // 使用内部RAM
 unsigned char data hit_result_counts[4] = { 0 }; // 判定结果统计数组
 unsigned char combo = 0;

/* 音符位置更新函数 */
void update_note_positions(unsigned char speed)
{
    /* 变量声明必须在函数开头 */
    unsigned char track_idx;          // 当前处理的轨道索引
    unsigned char note_idx;           // 当前处理的音符索引
    unsigned char active_notes;       // 当前轨道中激活的音符状态（位字段）
    unsigned char count;              // 当前轨道中音符的数量
    unsigned char* pos;               // 指向当前音符位置的指针

    // 遍历两个轨道
    for (track_idx = 0; track_idx < 2; track_idx++)
    {
        active_notes = is_active[track_idx];  // 获取当前轨道的激活状态
        count = note_count[track_idx];        // 获取当前轨道的音符数量

        // 遍历当前轨道中的所有音符
        for (note_idx = 0; note_idx < count; note_idx++)
        {
            // 检查当前音符是否激活
            if (active_notes & (1 << note_idx))
            {
                pos = &x_position[track_idx][note_idx];  // 获取当前音符的位置指针
                // 在 OLED 上清除当前音符的位置
                OLED_ShowString(x_position[track_idx][note_idx], (track_idx == 0) ? 0 : 3, " ", 16);
                
                // 先检查音符下一步是否超出屏幕范围（假设屏幕宽度为 128 像素）
                if (*pos <= speed)
                {
                    // 如果音符超出屏幕范围，标记为非激活
                    active_notes &= ~(1 << note_idx);
                    combo = 0;  // 重置连击数
                    hit_result_counts[0]++;  // 增加 MISS 的计数
                }
                else
                {
                    // 否则更新音符的位置
                    *pos -= speed;
                    // 在 OLED 上显示更新后的音符位置
                    OLED_ShowString(x_position[track_idx][note_idx], (track_idx == 0) ? 0 : 3, "Q", 16);
                }
            }
        }
        // 更新当前轨道的激活状态
        is_active[track_idx] = active_notes;
    }

    /* 延迟减少计数 */
    // 遍历两个轨道，清理非激活的音符计数
    for (track_idx = 0; track_idx < 2; track_idx++)
    {
        // 如果当前轨道的音符数量大于 0，并且最后一个音符未激活，则减少音符计数
        while (note_count[track_idx] > 0 &&
            !(is_active[track_idx] & (1 << (note_count[track_idx] - 1))))
        {
            note_count[track_idx]--;  // 减少音符计数
        }
    }
}


/* 音符生成函数 */
void note_spawn(unsigned char speed, const unsigned char code* track)
{
    /* 变量声明必须在函数开头 */
    static unsigned char track_idx = 0; // 静态变量，用于记录当前处理的音符索引
    unsigned char note_track;
    unsigned char idx;

    // 检查是否还有音符需要生成
    if (track[track_idx] == 0) // 如果遇到终止符（全零），表示音符生成结束
    {
        return; // 退出函数
    }

    // 获取当前音符的轨道编号
    note_track = track[track_idx + 2] - 1;

    // 检查轨道是否有效且音符池未满
    if (note_track < 2 && note_count[note_track] < MAX_NOTES_PER_TRACK)
    {
        idx = note_count[note_track]; // 获取当前轨道的音符索引
        x_position[note_track][idx] = 121 - speed * 2; // 设置音符的初始位置
        is_active[note_track] |= (1 << idx); // 标记为激活状态
        note_count[note_track]++; // 增加轨道的音符数量
        OLED_ShowString(x_position[note_track][idx], (note_track == 0) ? 0 : 3, "Q", 16); // 显示音符
    }

    // 移动到下一个音符定义
    track_idx += 3;
}

/**
 * @brief 检查按键是否命中音符（Keil兼容版本）
 * @param speed 音符移动速度
 * @param track 音轨编号(1~N)
 * @return 判定结果(NOTE_MISS表示未命中)
 */
unsigned char check_key_hit(unsigned char speed, unsigned char track)
{
    /* 所有变量声明必须在函数开头 */
    unsigned char track_idx;
    unsigned char result;
    unsigned char note_idx;
    unsigned char pos;
    unsigned char hit_time;

    track_idx = track - 1;
    result = NOTE_MISS;

    for (note_idx = 0; note_idx < note_count[track_idx]; note_idx++)
    {
        if ((is_active[track_idx] & (1 << note_idx)) &&
            (x_position[track_idx][note_idx] < 40)) /* 在判定区域内 */
        {
            pos = x_position[track_idx][note_idx];
            hit_time = (pos - 16) / speed;

            if (hit_time < 1)
                result = NOTE_PERFECT;
            else if (hit_time < 2)
                result = NOTE_GOOD;
            else
                result = NOTE_BAD;

            OLED_ShowString(x_position[track_idx][note_idx],
                (track_idx == 0) ? 0 : 3, " ", 16);
            is_active[track_idx] &= ~(1 << note_idx);
            hit_result_counts[result]++;
            combo++;
            return result;
        }
    }

    return NOTE_MISS;
}
/**
 * @brief 检查并立即销毁超时音符（紧急处理版）
 * @param speed 音符移动速度阈值
 * @return 1:存在并处理了超时音符 0:无超时音符
 */
unsigned char check_miss_notes(unsigned char speed)
{
    /* 所有变量声明必须在函数开头（Keil规范） */
    unsigned char track_idx;
    unsigned char note_idx;

    /* 优先检查最紧急的音符（x_position <= speed） */
    for (track_idx = 0; track_idx < 2; track_idx++)
    {

        for (note_idx = 0; note_idx < note_count[track_idx]; note_idx++)
        {
            if ((is_active[track_idx] & (1 << note_idx)) &&
                (x_position[track_idx][note_idx] <= speed))
            {
                /* 立即清除音符 */
                OLED_ShowString(x_position[track_idx][note_idx],
                    (track_idx == 0) ? 0 : 3, " ", 16);
                is_active[track_idx] &= ~(1 << note_idx);

                /* 更新状态 */
                hit_result_counts[NOTE_MISS]++;
                combo = 0;

                return 1; // 发现并处理了超时音符
            }
        }
    }

    return 0; // 无超时音符
}