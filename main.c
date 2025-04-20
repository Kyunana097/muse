#include <REGX52.H>
#include "Headers/bmp.h"
#include "Headers/oled.h"
#include "Headers/oledfont.h"
#include "Headers/RTX51TNY.H>

sbit KEY1 = P3 ^ 1;
sbit KEY2 = P3 ^ 0;
sbit KEY3 = P3 ^ 2;
sbit KEY4 = P3 ^ 3;

// 游戏状态变量
volatile unsigned char game_state = 0;          // 0: 主菜单，1: 游戏进行中
volatile unsigned char setting_state = 0;       // 0: 主菜单，1: 设置界面
volatile unsigned char score_state = 0;         // 0: 主菜单，1: 最高分界面
volatile unsigned char board_num = 1;           // 1: start 2: setting 3: score

// 初始化任务
void task_init(void) _task_ 0
{
    P2 = 0xFF;
    OLED_Init();
    OLED_DrawBMP(0, 0, 128, 8, BMP1);
    os_wait(K_IVL, 50, 0);

    os_create_task(1);  // 按键任务（优先级1）
    os_create_task(2);  // 菜单任务（优先级2）
    os_create_task(3);  // 游戏任务（优先级3）
    os_create_task(4);  // 积分任务（优先级4）
    os_create_task(5);  // 设置任务（优先级5）

    os_delete_task(0);  // 删除自身
    while (1);  // 修复2：阻止任务重生
}

// 按键任务
void task_key(void) _task_ 1
{
    while(1)
    {
        /*主菜单中
        * key1 = up
        * key2 = down
        * key3 = comfirm
        */
        if (game_state == 0 && setting_state == 0 && score_state == 0)
        {
            if (KEY1 == 0)
            {
                os_wait(K_IVL, 3, 0); // 消抖
                if (KEY1 == 0)         //等待按键释放
                {
                    os_wait(K_IVL, 10, 0); // 消抖
                    EA = 0;
                    board_num--;       //向上
                    EA = 1;
                    if(board_num < 1)
                        board_num = 3; //越界循环
                    while (KEY1 == 0) os_wait(K_IVL, 1, 0); // 等待释放
                }
                
            }
            
            if (KEY2 == 0)
            {
                os_wait(K_IVL, 10, 0); // 消抖
                while(KEY2 == 0);         //等待按键释放
                os_wait(K_IVL, 10, 0); // 消抖
                EA = 0;
                board_num++;        //向下
                EA = 1;
                if(board_num > 3)
                    board_num = 1;//越界循环
                while (KEY2 == 0) os_wait(K_IVL, 1, 0);
            }
            
            if (KEY3 == 0)
            {
                os_wait(K_IVL, 10, 0); // 消抖
                while (KEY3 == 0);         //等待按键释放
                os_wait(K_IVL, 10, 0); // 消抖  
                switch (board_num)
                {
                //game选项下按下确认按键
                case 1:
                    game_state = 1;
                    break;
                
                //setting选项下按下确认按键
                case 2:
                    setting_state = 1;
                    break;

                //score选项下按下确认按钮
                case 3:
                    score_state = 1;
                    break;
                }
                while (KEY3 == 0) os_wait(K_IVL, 1, 0);
            }
            os_wait(K_IVL, 10, 0);      //释放内存
            
            //主菜单下不需要按键4
        }
        /*游戏中
        * key1, key2 = sky
        * key3, key4 = ground
        */
        if (game_state == 1)
        {
    
        }
        /*设置中
        * key1 = plus
        * key2 = less
        * key3 = comfirm
        * key4 = back
        */
        if (setting_state == 1)
        {

        }
        /*计分板中
        * key4 = back
        */
        if (score_state == 1)
        {

        }
    }
}

// 菜单任务
void task_board(void) _task_ 2
{
    P2 = 0xAA;
    OLED_Clear();
    while (1)
    {
        while (game_state == 0 && score_state == 0 && setting_state == 0)
        { 
            OLED_ShowString(25, 0, "Start", 16);
            OLED_ShowString(25, 3, "Setting", 16);
            OLED_ShowString(25, 6, "Score", 16);
            switch (board_num)
            {
            case 1: 
                OLED_ShowString(3, 0, ">>", 16);
                OLED_ShowString(3, 3, "  ", 16);
                OLED_ShowString(3, 6, "  ", 16);
                break;

            case 2: 
                OLED_ShowString(3, 0, "  ", 16);
                OLED_ShowString(3, 3, ">>", 16);
                OLED_ShowString(3, 6, "  ", 16);
                break;

            case 3: 
                OLED_ShowString(3, 0, "  ", 16);
                OLED_ShowString(3, 3, "  ", 16);
                OLED_ShowString(3, 6, ">>", 16);
                break;
            }
            os_wait(K_IVL, 10, 0);  // 优化刷新率
        }
    }
}

// 游戏任务
void task_game(void) _task_ 3
{
    while (1)  // 修复5：使用永久循环
    {
        if (game_state == 1)
        {
            OLED_Clear();
            P2 = 0x01;
            // 添加游戏逻辑
            OLED_ShowString(3, 3, "gamestate now", 16); 
            os_wait(K_TMO, 300, 0);
            OLED_Clear();
            game_state = 0;
        }
        else
        {
            os_wait(K_IVL, 100, 0);
        }
    }
}

// 积分任务
void task_score(void) _task_ 4
{
    while (1)
    {
        if (score_state == 1)
        {
            OLED_Clear();
            P2 = 0x02;
            // 添加积分显示逻辑
            OLED_ShowString(3, 3, "scorestate now", 16); 
            os_wait(K_TMO, 300, 0);
            OLED_Clear();
            score_state = 0;
        }
        else
        {
            os_wait(K_IVL, 100, 0);
        }
    }
}

// 设置任务
void task_setting(void) _task_ 5
{
    while (1)
    {
        if (setting_state == 1)
        {
            OLED_Clear();
            P2 = 0x03;
            // 添加设置显示逻辑
            OLED_ShowString(3, 3, "settingstate now", 16);
            os_wait(K_TMO, 300, 0);
            OLED_Clear();
            setting_state = 0;
        }
        else
        {
            os_wait(K_IVL, 100, 0);
        }
    }
}