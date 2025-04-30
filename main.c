/* MAIN.C */
#include <REGX52.H>
#include "OLED.h"
#include "RTX51TNY.H"
#include "Music.h"
#include "Note.h"
#include "Key.h"
#include "AT24C02.h"

/* 硬件引脚定义 */
sbit BUZZER = P2 ^ 5;

/* 全局状态变量（使用bit节省内存） */
bit game_state = 0;     // 0:菜单 1:游戏
bit setting_mode = 0;   // 0:正常 1:设置
bit show_info = 0;      // 信息显示标志
bit music_playing = 0;  // 音乐播放标志

/* 游戏数据存储优化 */
idata struct {
    unsigned char track;      // 当前轨道
    unsigned char speed;      // 游戏速度
    unsigned int score;       // 当前分数
    unsigned int hi_score[2]; // 历史最高分
} game_data;

/* 显示缓冲区（减少OLED刷新次数） */
xdata char disp_buf[16];

/********************* 任务定义 *********************/
/* 初始化任务 */
void init_task(void) _task_ 0 {
    // 硬件初始化
    P2 = 0xFF;
    OLED_Init();
    Music_Init(Track1); // 默认初始化

    // 从EEPROM加载最高分
    game_data.hi_score[0] = AT24C02_ReadValue(0x00);
    game_data.hi_score[1] = AT24C02_ReadValue(0x04);

    // 创建其他任务
    os_create_task(1); // 输入处理
    os_create_task(2); // 游戏逻辑
    os_create_task(3); // 显示更新

    os_delete_task(0); // 删除自身
}

/* 输入处理任务 */
void input_task(void) _task_ 1 {
    static bit key_lock = 0;

    while (1) {
        unsigned char key = Key();

        if (!key_lock && key) {
            key_lock = 1;

            /* 菜单导航 */
            if (!game_state && !setting_mode) {
                switch (key) {
                case 1: // 上
                    if (game_data.track > 1) game_data.track--;
                    break;
                case 2: // 下
                    if (game_data.track < 2) game_data.track++;
                    break;
                case 3: // 确认
                    if (game_data.track == 3) {
                        setting_mode = 1;
                    }
                    else {
                        game_state = 1;
                    }
                    break;
                }
            }

            /* 游戏控制 */
            if (game_state) {
                // 音符命中检测
                if (key == 1 || key == 2) {
                    unsigned char res = check_key_hit(game_data.speed, key);
                    update_score(res);
                }
            }

            /* 设置模式 */
            if (setting_mode) {
                switch (key) {
                case 1: // 加速
                    if (game_data.speed < 8) game_data.speed++;
                    break;
                case 2: // 减速
                    if (game_data.speed > 1) game_data.speed--;
                    break;
                case 3: // 退出设置
                    setting_mode = 0;
                    break;
                }
            }
        }

        if (!key) key_lock = 0;
        os_wait(K_IVL, 10, 0);
    }
}

/* 游戏逻辑任务 */
void game_task(void) _task_ 2 {
    static unsigned char timer = 0;

    while (1) {
        if (game_state) {
            // 音符生成（每5次循环生成）
            if (++timer >= 5) {
                note_spawn(game_data.speed, track);
                timer = 0;
            }

            // 更新音符位置
            update_note_positions(game_data.speed);

            // 音乐播放控制
            if (!music_playing) {
                music_playing = 1;
                os_create_task(4); // 启动音乐任务
            }
        }
        os_wait(K_IVL, 50, 0);
    }
}

/* 显示更新任务 */
void display_task(void) _task_ 3 {
    while (1) {
        OLED_Clear();

        if (setting_mode) {
            // 设置界面显示
            sprintf(disp_buf, "SPEED:%2d", game_data.speed);
            OLED_ShowString(16, 2, disp_buf, 16);
            OLED_ShowString(16, 5, "3 TO EXIT", 8);
        }
        else if (game_state) {
            // 游戏界面显示
            sprintf(disp_buf, "SCORE:%04u", game_data.score);
            OLED_ShowString(0, 0, disp_buf, 16);
            sprintf(disp_buf, "COMBO:%03u", combo);
            OLED_ShowString(0, 4, disp_buf, 16);
        }
        else {
            // 主菜单显示
            OLED_ShowString(16, 0, "1.TRACK1", 8);
            OLED_ShowString(16, 2, "2.TRACK2", 8);
            OLED_ShowString(16, 4, "3.SETTING", 8);
            // 显示最高分
            sprintf(disp_buf, "HI:%04u", game_data.hi_score[game_data.track - 1]);
            OLED_ShowString(80, 6, disp_buf, 8);
        }

        os_wait(K_IVL, 200, 0);
    }
}

/* 音乐播放任务 */
void music_task(void) _task_ 4 {
    while (music_playing) {
        Music_PlayFullTrack();
        os_wait(K_IVL, 10, 0);
    }
    os_delete_task(4);
}

/********************* 工具函数 *********************/
void update_score(unsigned char result) {
    // 得分计算逻辑
    static const unsigned char score_table[4] = { 0, 10, 30, 50 };

    if (result) {
        game_data.score += score_table[result];
        if (game_data.score > 1000) game_data.score = 1000;
        combo++;
    }
    else {
        combo = 0;
    }
}

void save_hi_score(void) {
    if (game_data.score > game_data.hi_score[game_data.track - 1]) {
        game_data.hi_score[game_data.track - 1] = game_data.score;
        AT24C02_WriteValue(0x00 + (game_data.track - 1) * 4, game_data.hi_score[game_data.track - 1]);
    }
}