#include <REGX52.H>
#include "Headers/bmp.h"
#include "Headers/oled.h"
#include "Headers/oledfont.h"
#include "Headers/RTX51TNY.H>

sbit KEY1 = P3 ^ 1;
sbit KEY2 = P3 ^ 0;
sbit KEY3 = P3 ^ 2;
sbit KEY4 = P3 ^ 3;

// ��Ϸ״̬����
volatile unsigned char game_state = 0;          // 0: ���˵���1: ��Ϸ������
volatile unsigned char setting_state = 0;       // 0: ���˵���1: ���ý���
volatile unsigned char score_state = 0;         // 0: ���˵���1: ��߷ֽ���
volatile unsigned char board_num = 1;           // 1: start 2: setting 3: score

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
    os_create_task(5);  // �����������ȼ�5��

    os_delete_task(0);  // ɾ������
    while (1);  // �޸�2����ֹ��������
}

// ��������
void task_key(void) _task_ 1
{
    while(1)
    {
        /*���˵���
        * key1 = up
        * key2 = down
        * key3 = comfirm
        */
        if (game_state == 0 && setting_state == 0 && score_state == 0)
        {
            if (KEY1 == 0)
            {
                os_wait(K_IVL, 3, 0); // ����
                if (KEY1 == 0)         //�ȴ������ͷ�
                {
                    os_wait(K_IVL, 10, 0); // ����
                    EA = 0;
                    board_num--;       //����
                    EA = 1;
                    if(board_num < 1)
                        board_num = 3; //Խ��ѭ��
                    while (KEY1 == 0) os_wait(K_IVL, 1, 0); // �ȴ��ͷ�
                }
                
            }
            
            if (KEY2 == 0)
            {
                os_wait(K_IVL, 10, 0); // ����
                while(KEY2 == 0);         //�ȴ������ͷ�
                os_wait(K_IVL, 10, 0); // ����
                EA = 0;
                board_num++;        //����
                EA = 1;
                if(board_num > 3)
                    board_num = 1;//Խ��ѭ��
                while (KEY2 == 0) os_wait(K_IVL, 1, 0);
            }
            
            if (KEY3 == 0)
            {
                os_wait(K_IVL, 10, 0); // ����
                while (KEY3 == 0);         //�ȴ������ͷ�
                os_wait(K_IVL, 10, 0); // ����  
                switch (board_num)
                {
                //gameѡ���°���ȷ�ϰ���
                case 1:
                    game_state = 1;
                    break;
                
                //settingѡ���°���ȷ�ϰ���
                case 2:
                    setting_state = 1;
                    break;

                //scoreѡ���°���ȷ�ϰ�ť
                case 3:
                    score_state = 1;
                    break;
                }
                while (KEY3 == 0) os_wait(K_IVL, 1, 0);
            }
            os_wait(K_IVL, 10, 0);      //�ͷ��ڴ�
            
            //���˵��²���Ҫ����4
        }
        /*��Ϸ��
        * key1, key2 = sky
        * key3, key4 = ground
        */
        if (game_state == 1)
        {
    
        }
        /*������
        * key1 = plus
        * key2 = less
        * key3 = comfirm
        * key4 = back
        */
        if (setting_state == 1)
        {

        }
        /*�Ʒְ���
        * key4 = back
        */
        if (score_state == 1)
        {

        }
    }
}

// �˵�����
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
            os_wait(K_IVL, 10, 0);  // �Ż�ˢ����
        }
    }
}

// ��Ϸ����
void task_game(void) _task_ 3
{
    while (1)  // �޸�5��ʹ������ѭ��
    {
        if (game_state == 1)
        {
            OLED_Clear();
            P2 = 0x01;
            // �����Ϸ�߼�
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

// ��������
void task_score(void) _task_ 4
{
    while (1)
    {
        if (score_state == 1)
        {
            OLED_Clear();
            P2 = 0x02;
            // ��ӻ�����ʾ�߼�
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

// ��������
void task_setting(void) _task_ 5
{
    while (1)
    {
        if (setting_state == 1)
        {
            OLED_Clear();
            P2 = 0x03;
            // ���������ʾ�߼�
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