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

// ��Ϸ״̬����
unsigned char game_state = 0;   // 0: ���˵���1: ��Ϸ������
unsigned char board_num = 0;    // 0: 

//��ʼ��
void task_init(void) _task_ 0
{
    OLED_Init();
    LCD_Init();

    // ��������
    os_create_task(1);      // ������������
    os_create_task(2);      // �����˵�����
    os_create_task(3);      // ������Ϸ����
    os_create_task(4);      // ������������

    // ��ɾ������0
    os_delete_task(0);
}

/**
 * �˵��������
 */
void task_key(void) _task_ 1
{
    bit key1_pressed = 0; // ��ť1״̬��־
    bit key2_pressed = 0; // ��ť2״̬��־
    bit key3_pressed = 0; // ��ť3״̬��־
    bit key4_pressed = 0; // ��ť4״̬��־
    
    /*���˵���
    * key1 = up
    * key2 = down
    * key3 = forward
    * key4 = back
    */
    if (game_state == 0)
    {
        if (!key1_pressed)
        {
            os_wait(K_IVL, 10, 0); // ����
        }
    }
    /*��Ϸ��
    * key1, key2 = sky
    * key3, key4 = ground
    */
    if (game_state == 1)
    {

    }
}

/**
 * �˵�
 */
void task_board(void) _task_ 2
{
    //��ʾͼƬ
    OLED_DrawBMP(0, 0, 128, 8, BMP1);   //ͼƬ��ʾ
    os_wait(K_IVL, 100, 0);             //�ȴ�1000ms

    //��ʾ��ѡ��
    OLED_ShowString(2, 0, "Start", 16);
    OLED_ShowString(2, 3, "Setting", 16);
    OLED_ShowString(2, 6, "Score", 16);

}

/**
 * ��Ϸ����
 */
void task_game(void) _task_ 3
{

}

/*
* �Ʒ�ϵͳ
*/
void task_score(void) _task_ 4
{

}

/*******************************
  ������ - Ӳ����ʼ������������
*******************************/
void init_system(void)
{


    // ��������RTX51�Զ���������0��
    os_create_task(1);      // ������������
    os_create_task(2);      // ����OLED����
    os_create_task(3);      // �����˵�����
    os_create_task(4);      // ������������

    os_delete_task(0);    // ɾ����ʼ����
}