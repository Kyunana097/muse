#include <REGX52.H>
#include "Headers/bmp.h"
#include "Headers/oled.h"
#include "Headers/oledfont.h"
#include "Headers/RTX51TNY.H"
extern void init_system(void);

sbit KEY1 = P3^1;
sbit KEY2 = P3^0;
sbit KEY3 = P3^2;
sbit KEY4 = P3^3;



// 游戏状态变量
unsigned char game_state = 0;   // 0: 主菜单，1: 游戏进行中
unsigned char setting_state = 0;   // 0: 主菜单，1: 游戏进行中
unsigned char score_state = 0;   // 0: 主菜单，1: 游戏进行中
unsigned char board_num = 0;    // 0: start 1: setting 2: score

//初始化
void task_init(void) _task_ 0
{
    P2 = 0xFF;
    OLED_Init();
    OLED_DrawBMP(0, 0, 128, 8, BMP1);   //图片显示
    os_wait(K_IVL, 50, 0);
    P2 = 0x7F;
    os_wait(K_IVL, 100, 0);
    // 创建任务
    os_create_task(1);      // 启动按键任务
    os_create_task(2);      // 启动菜单任务
    os_create_task(3);      // 启动游戏任务
    os_create_task(4);      // 启动积分任务
    P2 = 0x3F; 
    os_wait(K_IVL, 500, 0);
    // 自删除任务0
    os_delete_task(0);
}

/**
 * 菜单按键检测
 */
void task_key(void) _task_ 1
{
unsigned char key1_pressed = 0; // 按钮1状态标志
unsigned char key2_pressed = 0; // 按钮2状态标志
unsigned char key3_pressed = 0; // 按钮3状态标志
unsigned char key4_pressed = 0; // 按钮4状态标志
    while(1)
    {
        /*主菜单中
        * key1 = up
        * key2 = down
        * key3 = comfirm
        */
        if (game_state == 0)
        {
            if (!key1_pressed)
            {
                os_wait(K_IVL, 3, 0); // 消抖
                if(key1_perssed == 0)
                {
                    board_num--;       //向上
                    if(board_num<0)
                        board_num = 2; //越界循环
                }
            }
            os_wait(K_IVL, 1, 0);
            
            if (!key2_pressed)
            {
                os_wait(K_IVL, 3, 0); // 消抖
                if(key2_perssed == 0)
                {
                    board_num++;        //向下
                    if(board_num>2)
                        board_num = 0;//越界循环
                }
            }
            os_wait(K_IVL, 1, 0);
            
            if (!key3_pressed)
            {
                os_wait(K_IVL, 3, 0); // 消抖
                if(key3_perssed == 0)
                {
                    if (board_num == 0)//game选项下按下确认按键
                        game_state = 1;
                    if (board_num == 1)//setting选项下按下确认按键
                        setting_state = 1;
                    if (board_num == 2)//score选项下按下确认按钮
                        score_state = 1;
                }
            }
            os_wait(K_IVL, 1, 0);
            
            //主菜单下不需要按键4
        }
        /*游戏中
        * key1, key2 = sky
        * key3, key4 = ground
        */
        if (game_state == 1)
        {
    
        }
    }
}
    
/**
 * 菜单
 */
void task_board(void) _task_ 2
{ 
    P2 = 0xAA;  // 用LED指示任务进入
    while (1)
    {       
            OLED_Clear();
            P2 = 0x00;
            OLED_ShowString(25, 0, "Start", 16);
            OLED_ShowString(25, 3, "Setting", 16);
            OLED_ShowString(25, 6, "Score", 16);
            os_wait(K_IVL, 5000, 0);
        if(os_delete_task(0)==1)
            P2 = 0x06;
    }
}

/**
 * 游戏界面
 */
void task_game(void) _task_ 3
{
    while(game_state = 1)
    {
        os_wait(K_IVL, 30, 0);
        P2 = 0x01;
        os_wait(K_IVL, 500, 0);
        game_state = 0;
    }
}

/*
* 计分系统
*/
void task_score(void) _task_ 4
{
    while(score_state = 1)
    {
        os_wait(K_IVL, 20, 0);
        P2 = 0x02;
        os_wait(K_IVL, 500, 0);
        score_state = 0;
    }
}