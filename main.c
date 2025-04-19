#include <REGX52.H>
#include "Headers/LCD1602.h"
#include "Headers/bmp.h"
#include "Headers/Delay.h"
#include "Headers/oled.h"
#include "Headers/oledfont.h"
#include "Headers/RTX51TNY.H"
extern void init_system(void);

sbit KEY1 = P3_1;
sbit KEY2 = P3_0;
sbit KEY3 = P3_2;
sbit KEY4 = P3_3;

// 游戏状态变量
unsigned char game_state = 0;   // 0: 主菜单，1: 游戏进行中
unsigned char board_num = 0;    // 0: 

//初始化
void task_init(void) _task_ 0
{
    OLED_Init();
    LCD_Init();

    // 创建任务
    os_create_task(1);      // 启动按键任务
    os_create_task(2);      // 启动菜单任务
    os_create_task(3);      // 启动游戏任务
    os_create_task(4);      // 启动积分任务

    // 自删除任务0
    os_delete_task(0);
}

/**
 * 菜单按键检测
 */
void task_key(void) _task_ 1
{
    bit key1_pressed = 0; // 按钮1状态标志
    bit key2_pressed = 0; // 按钮2状态标志
    bit key3_pressed = 0; // 按钮3状态标志
    bit key4_pressed = 0; // 按钮4状态标志
    
    /*主菜单中
    * key1 = up
    * key2 = down
    * key3 = forward
    * key4 = back
    */
    if (game_state == 0)
    {
        if (!key1_pressed)
        {
            os_wait(K_IVL, 10, 0); // 消抖
        }
    }
    /*游戏中
    * key1, key2 = sky
    * key3, key4 = ground
    */
    if (game_state == 1)
    {

    }
}

/**
 * 菜单
 */
void task_board(void) _task_ 2
{
    //显示图片
    OLED_DrawBMP(0, 0, 128, 8, BMP1);   //图片显示
    os_wait(K_IVL, 100, 0);             //等待1000ms

    //显示可选项
    OLED_ShowString(2, 0, "Start", 16);
    OLED_ShowString(2, 3, "Setting", 16);
    OLED_ShowString(2, 6, "Score", 16);

}

/**
 * 游戏界面
 */
void task_game(void) _task_ 3
{

}

/*
* 计分系统
*/
void task_score(void) _task_ 4
{

}

/*******************************
  主函数 - 硬件初始化与任务启动
*******************************/
void init_system(void)
{


    // 创建任务（RTX51自动启动任务0）
    os_create_task(1);      // 启动按键任务
    os_create_task(2);      // 启动OLED任务
    os_create_task(3);      // 启动菜单任务
    os_create_task(4);      // 启动积分任务

    os_delete_task(0);    // 删除初始任务
}