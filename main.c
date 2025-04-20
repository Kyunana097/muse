#include <REGX52.H>
#include "Headers/bmp.h"
#include "Headers/oled.h"
#include "Headers/oledfont.h"
#include "Headers/RTX51TNY.H>

// �޸�1��ɾ���ظ�����
sbit KEY1 = P3 ^ 1;
sbit KEY2 = P3 ^ 0;
sbit KEY3 = P3 ^ 2;
sbit KEY4 = P3 ^ 3;

// ��Ϸ״̬����
volatile unsigned char game_state = 0;    // ʹ��volatile
volatile unsigned char setting_state = 0;
volatile unsigned char score_state = 0;
volatile unsigned char board_num = 0;

// ��ʼ������
void task_init(void) _task_ 0
{
    P2 = 0xFF;
    OLED_Init();
    OLED_DrawBMP(0, 0, 128, 8, BMP1);
    os_wait(K_IVL, 50, 0);

    os_create_task(1);  // �����������ȼ�1��
    os_create_task(2);  // �˵��������ȼ�2��
    os_create_task(3);  // ��Ϸ�������ȼ�3��
    os_create_task(4);  // �����������ȼ�4��

    os_delete_task(0);  // ɾ������
    while (1);  // �޸�2����ֹ��������
}

// ��������
void task_key(void) _task_ 1
{
    unsigned char key1_pressed = 0;
    unsigned char key2_pressed = 0;
    unsigned char key3_pressed = 0;
    unsigned char key4_pressed = 0;

    while (1)
    {
        // �޸�3������ƴд����
        if (!KEY1 && !key1_pressed)
        {
            os_wait(K_IVL, 20, 0); // ����
            if (!KEY1)
            {
                os_disable_isr();
                board_num = (board_num == 0) ? 2 : board_num - 1;
                key1_pressed = 1;
                os_enable_isr();
            }
        }
        else if (KEY1)
        {
            key1_pressed = 0;
        }

        if (!KEY2 && !key2_pressed)
        {
            os_wait(K_IVL, 20, 0);
            if (!KEY2)
            {
                os_disable_isr();
                board_num = (board_num + 1) % 3;
                key2_pressed = 1;
                os_enable_isr();
            }
        }
        else if (KEY2)
        {
            key2_pressed = 0;
        }

        if (!KEY3 && !key3_pressed)
        {
            os_wait(K_IVL, 20, 0);
            if (!KEY3)
            {
                os_disable_isr();
                if (board_num == 0) game_state = 1;
                if (board_num == 1) setting_state = 1;
                if (board_num == 2) score_state = 1;
                key3_pressed = 1;
                os_enable_isr();
            }
        }
        else if (KEY3)
        {
            key3_pressed = 0;
        }

        os_wait(K_IVL, 50, 0);  // 50msɨ������
    }
}

// �˵�����
void task_board(void) _task_ 2
{
    P2 = 0xAA;
    OLED_Clear();
    while (1)
    {
        OLED_ShowString(25, 0, "Start", 16);
        OLED_ShowString(25, 3, "Setting", 16);
        OLED_ShowString(25, 6, "Score", 16);
        os_wait(K_IVL, 200, 0);  // �Ż�ˢ����
    }
}

// ��Ϸ����
void task_game(void) _task_ 3
{
    while (1)  // �޸�5��ʹ������ѭ��
    {
        if (game_state == 1)
        {
            P2 = 0x01;
            // �����Ϸ�߼�
            os_wait(K_TMO, 30, 0);
        }
        else
        {
            os_wait(K_IVL, 100, 0);
        }
    }
}

// ��������
void task_score(void) _task_ 4
{
    while (1)
    {
        if (score_state == 1)
        {
            P2 = 0x02;
            // ��ӻ�����ʾ�߼�
            os_wait(K_TMO, 50, 0);
        }
        else
        {
            os_wait(K_IVL, 100, 0);
        }
    }
}