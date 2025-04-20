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



// ��Ϸ״̬����
unsigned char game_state = 0;   // 0: ���˵���1: ��Ϸ������
unsigned char setting_state = 0;   // 0: ���˵���1: ��Ϸ������
unsigned char score_state = 0;   // 0: ���˵���1: ��Ϸ������
unsigned char board_num = 0;    // 0: start 1: setting 2: score

//��ʼ��
void task_init(void) _task_ 0
{
    P2 = 0xFF;
    OLED_Init();
    OLED_DrawBMP(0, 0, 128, 8, BMP1);   //ͼƬ��ʾ
    os_wait(K_IVL, 50, 0);
    P2 = 0x7F;
    os_wait(K_IVL, 100, 0);
    // ��������
    os_create_task(1);      // ������������
    os_create_task(2);      // �����˵�����
    os_create_task(3);      // ������Ϸ����
    os_create_task(4);      // ������������
    P2 = 0x3F; 
    os_wait(K_IVL, 500, 0);
    // ��ɾ������0
    os_delete_task(0);
}

/**
 * �˵��������
 */
void task_key(void) _task_ 1
{
unsigned char key1_pressed = 0; // ��ť1״̬��־
unsigned char key2_pressed = 0; // ��ť2״̬��־
unsigned char key3_pressed = 0; // ��ť3״̬��־
unsigned char key4_pressed = 0; // ��ť4״̬��־
    while(1)
    {
        /*���˵���
        * key1 = up
        * key2 = down
        * key3 = comfirm
        */
        if (game_state == 0)
        {
            if (!key1_pressed)
            {
                os_wait(K_IVL, 3, 0); // ����
                if(key1_perssed == 0)
                {
                    board_num--;       //����
                    if(board_num<0)
                        board_num = 2; //Խ��ѭ��
                }
            }
            os_wait(K_IVL, 1, 0);
            
            if (!key2_pressed)
            {
                os_wait(K_IVL, 3, 0); // ����
                if(key2_perssed == 0)
                {
                    board_num++;        //����
                    if(board_num>2)
                        board_num = 0;//Խ��ѭ��
                }
            }
            os_wait(K_IVL, 1, 0);
            
            if (!key3_pressed)
            {
                os_wait(K_IVL, 3, 0); // ����
                if(key3_perssed == 0)
                {
                    if (board_num == 0)//gameѡ���°���ȷ�ϰ���
                        game_state = 1;
                    if (board_num == 1)//settingѡ���°���ȷ�ϰ���
                        setting_state = 1;
                    if (board_num == 2)//scoreѡ���°���ȷ�ϰ�ť
                        score_state = 1;
                }
            }
            os_wait(K_IVL, 1, 0);
            
            //���˵��²���Ҫ����4
        }
        /*��Ϸ��
        * key1, key2 = sky
        * key3, key4 = ground
        */
        if (game_state == 1)
        {
    
        }
    }
}
    
/**
 * �˵�
 */
void task_board(void) _task_ 2
{ 
    P2 = 0xAA;  // ��LEDָʾ�������
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
 * ��Ϸ����
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
* �Ʒ�ϵͳ
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