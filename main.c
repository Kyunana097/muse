#include <REGX52.H>
#include "Headers/bmp.h"
#include "Headers/oled.h"
#include "Headers/oledfont.h"
#include "Headers/RTX51TNY.H"

sbit KEY1 = P3 ^ 1;
sbit KEY2 = P3 ^ 0;
sbit KEY3 = P3 ^ 2;
sbit KEY4 = P3 ^ 3;

// 游戏状态变量
bit game_state = 0;          // 0: 主菜单，1: 游戏进行中
bit setting_state = 0;       // 0: 主菜单，1: 设置界面
bit score_state = 0;         // 0: 主菜单，1: 最高分界面
volatile unsigned char board_num = 1;           // 1: start 2: setting 3: score
volatile unsigned char x_position = 119;        //音符起始位置 119+8=127
volatile unsigned char speed = 4;               //音符移动速度 1-8

// 将 speed 转换为字符串
char speed_str[2];  // 1位数字 + '\0'
unsigned char temp ;

unsigned char current_track = 1;//音符列位置 1-2

// 初始化任务
void task_init(void) _task_ 0
{
    P2 = 0xFF;
    OLED_Init();
    OLED_DrawBMP(0, 0, 128, 8, BMP1);//logo
    os_wait(K_IVL, 50, 0);

    os_create_task(1);  // 按键任务（优先级1）
    os_create_task(2);  // 菜单任务（优先级2）
    //os_create_task(3);  // 游戏任务（优先级3）
    //os_create_task(4);  // 积分任务（优先级4）
    //os_create_task(5);  // 设置任务（优先级5）
    os_create_task(6);  // 音乐任务（优先级6）

    os_delete_task(0);  // 删除自身
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
                if (KEY1 == 0)         //确认按钮按下
                {
                    os_wait(K_IVL, 10, 0); // 消抖
                    board_num--;       //向上
                    if(board_num < 1)
                        board_num = 3; //越界循环
                    while (KEY1 == 0) os_wait(K_IVL, 1, 0); // 等待释放
                }
            }
            
            if (KEY2 == 0)
            {
                os_wait(K_IVL, 3, 0); // 消抖
                if (KEY2 == 0)         //确认按钮按下
                {
                    os_wait(K_IVL, 10, 0); // 消抖
                    board_num++;        //向下
                    if(board_num > 3)
                        board_num = 1;//越界循环
                    while (KEY2 == 0) os_wait(K_IVL, 1, 0);// 等待释放
                }
               
            }
            
            if (KEY3 == 0)
            {
                os_wait(K_IVL, 3, 0); // 消抖
                if (KEY3 == 0)         //等待按键释放
                {
                    os_wait(K_IVL, 10, 0); // 消抖  
                    switch (board_num)
                    {
                    //game选项下按下确认按键
                    case 1:
                        game_state = 1;
                        os_delete_task(2);  //挂起菜单
                        os_create_task(3);  //启动游戏任务（优先级3）
                        break;
                
                    //setting选项下按下确认按键
                    case 2:
                        setting_state = 1;
                        os_delete_task(2);  //挂起菜单
                        os_create_task(5);  //启动设置任务（优先级5）
                        break;

                    //score选项下按下确认按钮
                    case 3:
                        score_state = 1;
                        os_delete_task(2);  //挂起菜单
                        os_create_task(4);  //启动积分任务（优先级4）
                        break;
                    }
                    while (KEY3 == 0) os_wait(K_IVL, 1, 0);
                }
                
            }
            os_wait(K_IVL, 10, 0);      //释放内存
        }

        /*游戏中
        * key1 = sky
        * key2 = ground
        * key3 = quit
        * key4 = pause
        */
        if (game_state == 1)
        {
            //退出游戏
            if (KEY3 == 0)
            {
                os_wait(K_IVL, 3, 0); // 消抖
                if (KEY3 == 0)
                {
                    os_wait(K_IVL, 10, 0); // 消抖  
                    while (KEY3 == 0) os_wait(K_IVL, 1, 0);
                }
                game_state = 0;
                os_wait(K_IVL, 3, 0);
            }
        }

        /*设置中
        * key1 = speed plus
        * key2 = speed less
        * key3 = comfirm & backward
        */
        if (setting_state == 1)
        {
            //speed减少
            if (KEY1 == 0)
            {
                os_wait(K_IVL, 3, 0); // 消抖
                if (KEY1 == 0)
                {
                    os_wait(K_IVL, 10, 0); // 消抖
                    speed--;       //speed减少
                    if (speed < 2)
                        speed = 1; //越界循环
                    while (KEY1 == 0) os_wait(K_IVL, 1, 0); // 等待释放

                }
                
            }

            //speed增加
            if (KEY2 == 0)
            {
                os_wait(K_IVL, 3, 0); // 消抖
                if (KEY2 == 0)         
                {
                    os_wait(K_IVL, 10, 0); // 消抖
                    speed++;        //speed增加
                    if (speed > 7)
                        speed = 8;//越界循环
                    while (KEY2 == 0) os_wait(K_IVL, 1, 0);

                }
            }

            //退出设置
            if (KEY3 == 0)
            {
                os_wait(K_IVL, 3, 0); // 消抖
                if (KEY3 == 0)         
                {
                    os_wait(K_IVL, 10, 0); // 消抖  
                    while (KEY3 == 0) os_wait(K_IVL, 1, 0);
                }
                setting_state = 0;
                os_wait(K_IVL, 3, 0);
            }

            os_wait(K_IVL, 10, 0);      //释放内存
            
        }
        /*计分板中
        * key3 = back
        */
        if (score_state == 1)
        {
            //退出计分板
            if (KEY3 == 0)
            {
                os_wait(K_IVL, 3, 0); // 消抖
                if (KEY3 == 0);        
                {
                    os_wait(K_IVL, 10, 0); // 消抖  
                    while (KEY3 == 0) os_wait(K_IVL, 1, 0);
                }
                score_state = 0;
                os_wait(K_IVL, 3, 0);
            }
        }
    }
}

// 菜单任务
void task_board(void) _task_ 2
{
    OLED_Clear();
    while (1)
    {  
        os_wait(K_IVL, 10, 0);
        P2 = 0xAA;
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
            os_wait(K_IVL, 10, 0);  
        }
    }
}

// 游戏任务
void task_game(void) _task_ 3
{
    while (1)  
    {
        if (game_state == 1)
        {
            OLED_Clear();

            P2 = 0x01;
            //暂未添加游戏逻辑
            while (game_state == 1)
            {
                OLED_ShowString(3, 3, "gamestate now", 16); 
            }
            //game_state == 0后 进入退出步骤
            OLED_Clear();
            os_wait(K_IVL, 100, 0);
            game_state = 0;
            os_create_task(2);  //激活菜单
            os_delete_task(3);  //挂起游戏
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
            OLED_Init(); 
            OLED_Clear();
            P2 = 0x02;
            // 添加积分显示逻辑
            while (score_state == 1)
            {
                OLED_ShowString(3, 3, "track1 = 100000", 16); 
                OLED_ShowString(3, 6, "track2 = 100000", 16);
                
                os_wait(K_IVL, 20, 0);
            }
            //score_state == 0后 进入退出步骤
            os_wait(K_IVL, 100, 0);
            OLED_Clear();
            os_create_task(2);  //激活菜单
            os_delete_task(4);  //挂起积分
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
            OLED_Init(); 
            OLED_Clear();
            os_wait(K_IVL, 10, 0);
            P2 = 0x03;
            while(setting_state == 1)
            {
                temp = speed;
                os_wait(K_IVL, 1, 0);

                //showstring需要一个U8参数 而speed无法直接输出
                speed_str[0] = temp % 10 + '0';          // 得到speed的值
                speed_str[1] = '\0';                     // 字符串结束符

                os_wait(K_IVL, 1, 0);
                OLED_ShowString(3, 3, "speed:", 16);
                os_wait(K_IVL, 1, 0);
                
                // 传递字符串指针给 OLED_ShowString
                OLED_ShowString(60, 3, speed_str, 16);  // 强制转换类型 输出当前speed值

                //移动Q（避免使用for而无法识别speed输入）
                if (x_position > speed && x_position < 120)
                {
                    OLED_ShowString(x_position, 0, " ", 16);
                    x_position = x_position - speed;
                    OLED_ShowString(x_position, 0, "Q", 16);
                }
                else 
                {
                    OLED_ShowString(x_position, 0, " ", 16);
                    x_position = 119;
                }
                os_wait(K_IVL, 1, 0);
            }

            //setting_state == 0后 进入退出步骤
            os_wait(K_IVL, 100, 0);
            OLED_Clear();
            os_create_task(2);  //激活菜单
            os_delete_task(5);  //挂起设置
        }
        else
        {
            os_wait(K_IVL, 100, 0);
        }
    }
}

//蜂鸣器任务（未启用 需使用定时器1）
void task_beep(void) _task_ 6
{
    while (1)
    {
        if (game_state == 1)
        {
            os_wait(K_IVL, 100, 0);
        }
        else
        {
            os_wait(K_IVL, 100, 0);
        }
    }
}