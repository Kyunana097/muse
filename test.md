```
#include <REGX52.H>
#include "oled.h"
#include "oledfont.h"
#include "Delay.h"
#include "note.h"
#include "music.h"
#include "key.h"

// 新增全局变量记录按键状态
static bit key1_pressed = 0;
static bit key2_pressed = 0;

void main()
{
    unsigned char result;
    unsigned char speed = 2;
    unsigned char key_state;
    combo = 0;

    OLED_Init();
    OLED_Clear();
    
    // 初始化显示固定元素
    OLED_ShowString(3, 6, "ready ", 8);  // 初始状态显示
    
    while (1)
    {
        // 1. 音符生成（每5次循环生成一次）
        static unsigned char spawn_counter = 0;
        OLED_ShowString(16, 0, "$", 16);
        OLED_ShowString(16, 3, "$", 16);
        if (++spawn_counter >= 5) {
            note_spawn(speed, track);
            spawn_counter = 0;
        }
    
        // 2. 按键检测（独立于循环计数）
        key_state = Key();
        if (key_state != 0)
        {
            // 轨道1按键
            if (key_state & 0x01 && !key1_pressed) {
                key1_pressed = 1;  // 标记按键已按下
                result = check_key_hit(speed, 1);
                if (result != NOTE_MISS) {
                    switch (result) {
                    case NOTE_BAD:    OLED_ShowString(3, 6, "bad    ", 8); break;
                    case NOTE_GOOD:   OLED_ShowString(3, 6, "good   ", 8); break;
                    case NOTE_PERFECT:OLED_ShowString(3, 6, "perfect", 8); break;
                    }
                }
                key1_pressed = 0;
            }
            // 轨道2按键
            else if (key_state & 0x02 && !key2_pressed) {
                key2_pressed = 1;  // 标记按键已按下
                result = check_key_hit(speed, 2);
                if (result != NOTE_MISS) {
                    switch (result) {
                    case NOTE_BAD:    OLED_ShowString(3, 6, "bad    ", 8); break;
                    case NOTE_GOOD:   OLED_ShowString(3, 6, "good   ", 8); break;
                    case NOTE_PERFECT:OLED_ShowString(3, 6, "perfect", 8); break;
                    }
                }
                key2_pressed = 0;
            }
            Delay(20);  // 按键消抖
        }
    
        // 3. 游戏状态更新
        update_note_positions(speed);
    
        // 4. Miss判定（放在最后保证优先处理按键）
        if (check_miss_notes(speed)) {
            OLED_ShowString(3, 6, "miss   ", 8);
        }
    
        // 5. 更新连击显示
        OLED_ShowValue(80, 6, combo, 8);
    
        // 6. 帧率控制（调整为更合理的延迟）
        Delay(50);  // 约20FPS
    }

}
```

