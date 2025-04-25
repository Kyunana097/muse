#include <REGX52.H>
#include "Headers/oled.h"
#include "Headers/oledfont.h"
#include "Headers/RTX51TNY.H"
#include "Headers/music.h"

sbit KEY1 = P3 ^ 1;
sbit KEY2 = P3 ^ 0;
sbit KEY3 = P3 ^ 2;
sbit KEY4 = P3 ^ 3;

// 游戏状态变量
bit game_state = 0;          // 0: 主菜单，1: 游戏进行中
bit setting_state = 0;       // 0: 主菜单，1: 设置界面
bit info_state = 0;          // 0: 主菜单，1: 制作信息界面
bit isPlaying = 0;           // 0: 没有正在游玩的游戏 1; 正在游戏中
bit musicPlaying = 0;        // 0: 不播放 1: 播放

volatile unsigned short track1_score = 0;
volatile unsigned short track2_score = 0;
volatile unsigned short temp_score = 0;
volatile unsigned char track_num = 1;           // 1: 第一关，2: 第二关
volatile unsigned char board_num = 1;           // 1: start 2: setting 3: info
volatile unsigned char x_position = 119;        //音符起始位置 119+8=127
volatile unsigned char speed = 4;               //音符移动速度 1-8

//即将转移至包装的数据
bit KEY4_pressed = 0;        // 判定状态
// 将 speed 转换为字符串
char speed_str[2];  // 1位数字 + '\0'
volatile unsigned char temp ;
volatile unsigned char hit_time = 0;
volatile unsigned char current_track = 1;//音符列位置 1-2

// 初始化任务
void task_init(void) _task_ 0
{
    P2 = 0xFF;
    OLED_Init();
    OLED_Clear();
    os_wait(K_IVL, 50, 0);

    //显示菜单教程
    OLED_ShowString(3, 0, "QG Dash v2.8", 8);
    OLED_ShowString(3, 2, "Key1:up", 8);
    OLED_ShowString(3, 3, "Key2:down", 8);
    OLED_ShowString(3, 4, "Key3:comfirm", 8);
    OLED_ShowString(3, 6, "Loading...", 8);
    
    os_wait(K_IVL, 1000, 0);
    OLED_ShowString(3, 6, "Welcome!  ", 8);
    os_wait(K_IVL, 100, 0);

    os_create_task(1);  // 按键任务（优先级1）
    os_create_task(2);  // 菜单任务（优先级2）
    //os_create_task(3);  // 游戏任务（优先级3）
    //os_create_task(4);  // 积分任务（优先级4）
    //os_create_task(5);  // 设置任务（优先级5）
    //os_create_task(6);  // 音乐任务（优先级6）

    os_delete_task(0);  // 删除自身
}

// 按键与判定任务
void task_key(void) _task_ 1
{
    while(1)
    {
        /*主菜单中
        * key1 = up
        * key2 = down
        * key3 = comfirm
        */
        if (game_state == 0 && setting_state == 0 && info_state == 0)
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
                        //进入游戏二级菜单
                        game_state = 1;
                        break;
                
                    //setting选项下按下确认按键
                    case 2:
                        setting_state = 1;
                        os_delete_task(2);  //挂起菜单
                        os_create_task(5);  //启动设置任务（优先级5）
                        break;

                    //info选项下按下确认按钮
                    case 3:
                        //进入info二级菜单
                        info_state = 1;
                        break;
                    }
                    while (KEY3 == 0) os_wait(K_IVL, 1, 0);
                }
                
            }
            os_wait(K_IVL, 10, 0);      //释放内存
        }
        /**游戏二级菜单中
         * key 1 = up
         * key 2 = down
         * key 3 = comfirm
         * key 4 = back
         */
        if (game_state == 1 && isPlaying == 0)
        {
            if (KEY1 == 0)
            {
                os_wait(K_IVL, 3, 0); // 消抖
                if (KEY1 == 0)         //确认按钮按下
                {
                    os_wait(K_IVL, 10, 0); // 消抖
                    track_num--;       //向上
                    if (track_num < 1)
                        track_num = 2; //越界循环
                    while (KEY1 == 0) os_wait(K_IVL, 1, 0); // 等待释放
                }
            }

            if (KEY2 == 0)
            {
                os_wait(K_IVL, 3, 0); // 消抖
                if (KEY2 == 0)         //确认按钮按下
                {
                    os_wait(K_IVL, 10, 0); // 消抖
                    track_num++;        //向下
                    if (track_num > 2)
                        track_num = 1;//越界循环
                    while (KEY2 == 0) os_wait(K_IVL, 1, 0);// 等待释放
                }

            }

            if (KEY3 == 0)
            {
                os_wait(K_IVL, 3, 0); // 消抖
                if (KEY3 == 0)         //等待按键释放
                {
                    os_wait(K_IVL, 10, 0); // 消抖  
                    //激活当前停留的轨道
                    isPlaying = 1;
                    while (KEY3 == 0) os_wait(K_IVL, 1, 0);
                }
                os_delete_task(2);  //挂起菜单
                os_create_task(3);  //启动游戏任务（优先级3）
            }

            //返回上级菜单
            if (KEY4 == 0)
            {
                os_wait(K_IVL, 3, 0); // 消抖
                if (KEY4 == 0)
                {
                    os_wait(K_IVL, 10, 0); // 消抖  
                    while (KEY4 == 0) os_wait(K_IVL, 1, 0);
                }
                game_state = 0;
                os_wait(K_IVL, 3, 0);
            }
            os_wait(K_IVL, 10, 0);      //释放内存
        } 

        /**制作信息二级菜单中
         * key 3 = quit
         */
        if (info_state == 1)
        {
            //退出二级菜单
            if (KEY3 == 0)
            {
                os_wait(K_IVL, 3, 0); // 消抖
                if (KEY3 == 0);        
                {
                    os_wait(K_IVL, 10, 0); // 消抖  
                    while (KEY3 == 0) os_wait(K_IVL, 1, 0);
                }
                os_wait(K_IVL, 100, 0);
                info_state = 0;
            }
        }

        /*游戏中
        * key1 = sky
        * key2 = ground
        * key3 = quit
        * key4 = pause（暂时不写）
        */
        if (game_state == 1 && isPlaying == 1)
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
                OLED_Clear();
                isPlaying = 0;
                os_wait(K_IVL, 100, 0);
            }

        }

        /*设置中
        * key1 = speed plus
        * key2 = speed less
        * key3 = comfirm & quit
        * key4 = tap
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

            //判定
            if (KEY4 == 0)
            {
                os_wait(K_IVL, 3, 0); // 消抖
                if (KEY4 == 0)
                {
                    os_wait(K_IVL, 10, 0); // 消抖  
                    while (KEY4 == 0) os_wait(K_IVL, 1, 0);
                }
                KEY4_pressed = 1; 
                hit_time = (x_position - 16) / speed;
                os_wait(K_IVL, 3, 0);
                if (x_position < 40 && KEY4_pressed == 1)
                {
                    if (hit_time < 1)
                    {
                        OLED_ShowString(3, 6, "Perfect", 8);
                    }
                    else if (hit_time < 2)
                    {
                        OLED_ShowString(3, 6, "Good   ", 8);
                    }
                    else if (hit_time < 3)
                    {
                        OLED_ShowString(3, 6, "Bad    ", 8);
                    }
                    x_position = 121 - speed * 2;
                    KEY4_pressed = 0;
                }
                os_wait(K_IVL, 3, 0);
            }

            os_wait(K_IVL, 10, 0);      //释放内存
            
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
        //主菜单
        while (game_state == 0 && setting_state == 0 && info_state == 0)
        {
            OLED_ShowString(25, 0, "Start", 16);
            OLED_ShowString(25, 3, "Setting", 16);
            OLED_ShowString(25, 6, "Info", 16);
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

        //游戏二级菜单
        if (game_state == 1 && isPlaying == 0)
        {
            OLED_Clear();
            OLED_ShowString(3, 1, "Key1:up", 8);
            OLED_ShowString(3, 2, "Key2:down", 8);
            OLED_ShowString(3, 3, "Key3:comfirm", 8);
            OLED_ShowString(3, 4, "Key4:back", 8);
            OLED_ShowString(3, 6, "Loading...", 8);
            os_wait(K_IVL, 1000, 0);
            OLED_ShowString(3, 6, "          ", 8);
            OLED_Clear();
            while (game_state == 1 && isPlaying == 0 )
            {
                OLED_ShowString(25, 0, "Track 1", 16);
                OLED_ShowString(25, 3, "Track 2", 16);
                OLED_ShowString(3, 6, "top:", 8);
                switch (track_num)
                {
                case 1:
                    OLED_ShowString(3, 0, ">>", 16);
                    OLED_ShowString(3, 3, "  ", 16);
                    OLED_ShowValue(40, 6, track1_score, 8);
                    break;

                case 2:
                    OLED_ShowString(3, 0, "  ", 16);
                    OLED_ShowString(3, 3, ">>", 16);
                    OLED_ShowValue(40, 6, track2_score, 8);
                    break;
                }
            }
            OLED_Clear();
            os_wait(K_IVL, 100, 0);
        }
        
        //制作信息二级菜单
        if (info_state == 1)
        {
            OLED_Clear();
            while (info_state == 1)
            {
                OLED_ShowString(3, 0, "Auth:Kyunana", 16);
                OLED_ShowString(3, 3, "25/4/2025", 8);
                OLED_ShowString(3, 4, "in AT89C52RC", 8);
                OLED_ShowString(3, 5, "V2.8", 8);
                OLED_ShowString(3, 6, "press 3 to quit", 16);
                os_wait(K_IVL, 1000, 0);
            }
            OLED_Clear();
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
            isPlaying = 1;
            OLED_Clear();
            P2 = 0x01;
            OLED_ShowString(3, 3, "Loading...", 16);
            os_create_task(4);  //激活积分
            os_create_task(6);  //激活播放器
            //曲目选择
            if (track_num == 1)
                Music_Init(Track1);
            else Music_Init(Track2);

            //Track1
            while (game_state == 1 && track_num == 1 && isPlaying == 1)
            {
                OLED_Clear();
                OLED_ShowString(3, 3, "Track1 now", 16);
                OLED_ShowString(3, 6, "Loading...", 8);
                musicPlaying = 1;
                while(musicPlaying == 1)
                    os_wait(K_IVL, 100, 0);
                OLED_Clear();

                OLED_ShowString(3, 0, "game end", 16);
                OLED_ShowString(3, 3, "your score", 16);
                OLED_ShowValue(3, 6, track1_score, 16);
                os_wait(K_IVL, 2000, 0);  
                // 退出游戏状态
                isPlaying = 0;
                // 确保积分任务已完成处理
                os_wait(K_IVL, 100, 0);
            }

            //Track2
            while (game_state == 1 && track_num == 2 && isPlaying == 1)
            {
                OLED_Clear();
                OLED_ShowString(3, 3, "Track2 now", 16);
                OLED_ShowString(3, 6, "Loading...", 8);
                musicPlaying = 1; 
                while(musicPlaying == 1)
                    os_wait(K_IVL, 100, 0);
                OLED_Clear(); 

                OLED_ShowString(3, 0, "game end", 16);
                OLED_ShowString(3, 3, "your score", 16);
                OLED_ShowValue(3, 6, track2_score, 16);
                os_wait(K_IVL, 2000, 0);
                // 退出游戏状态
                isPlaying = 0;
                // 确保积分任务已完成处理
                os_wait(K_IVL, 100, 0);
            }

            //此处添加玩家游戏后结算选择界面（回到主菜单或继续选歌）

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
        if (game_state == 1 && isPlaying == 1)
        {
            temp_score = 1000;
            os_wait(K_IVL, 10, 0);  // 适当延时
		}
		else
		{
			// 游戏结束时保存分数
			if (game_state == 1 && isPlaying == 0 && temp_score > 0)
			{
				if (track_num == 1)
				{
						if (temp_score > track1_score)
								track1_score = temp_score;
				}
				else if (track_num == 2)
				{
						if (temp_score > track2_score)
								track2_score = temp_score;
				}

				temp_score = 0;  // 重置临时分数
				os_wait(K_IVL, 100, 0);
			}
			os_wait(K_IVL, 100, 0);  // 非游戏状态时降低CPU占用
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
            x_position = 121 - speed * 2;
            while(setting_state == 1)
            {
                os_wait(K_IVL, 1, 0);
                OLED_ShowString(3, 3, "speed:", 16);
                os_wait(K_IVL, 1, 0);
                
                // 传递字符串指针给 OLED_ShowString
                OLED_ShowValue(60, 3, speed, 16); 
                OLED_ShowString(16, 0, "$", 16);//判定点
               
                //移动Q（避免使用for而无法识别speed输入）
                if (x_position > speed && x_position < 120 && KEY4_pressed == 0)
                {
                    OLED_ShowString(x_position, 0, " ", 16);
                    x_position = x_position - speed;
                    OLED_ShowString(x_position, 0, "Q", 16);
                }
                else 
                {
                    OLED_ShowString(x_position, 0, " ", 16);
                    OLED_ShowString(3, 6, "Miss   ", 8);
                    x_position = 121 - speed * 2;
                    KEY4_pressed = 0;
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

void task_music(void) _task_ 6
{
    while (1)
    {
        if (musicPlaying)
        {
            Music_PlayFullTrack();
            musicPlaying = 0;
        }
        os_wait(K_IVL, 10, 0);
    }
}