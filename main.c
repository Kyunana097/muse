#include <REGX52.H>
#include "Headers/bmp.h"
#include "Headers/oled.h"
#include "Headers/oledfont.h"
#include "Headers/RTX51TNY.H"

sbit KEY1 = P3 ^ 1;
sbit KEY2 = P3 ^ 0;
sbit KEY3 = P3 ^ 2;
sbit KEY4 = P3 ^ 3;

// ��Ϸ״̬����
bit game_state = 0;          // 0: ���˵���1: ��Ϸ������
bit setting_state = 0;       // 0: ���˵���1: ���ý���
bit score_state = 0;         // 0: ���˵���1: ��߷ֽ���
volatile unsigned char board_num = 1;           // 1: start 2: setting 3: score

volatile unsigned char speed = 4;

// �� speed ת��Ϊ�ַ���
char speed_str[2];  // 1λ���� + '\0'
unsigned char temp ;  // �����޸�ԭ����
unsigned char current_track = 1;//������λ�� 1-2

// ��ʼ������
void task_init(void) _task_ 0
{
    P2 = 0xFF;
    OLED_Init();
    OLED_DrawBMP(0, 0, 128, 8, BMP1);
    os_wait(K_IVL, 50, 0);

    os_create_task(1);  // �����������ȼ�1��
    os_create_task(2);  // �˵��������ȼ�2��
    //os_create_task(3);  // ��Ϸ�������ȼ�3��
    //os_create_task(4);  // �����������ȼ�4��
    //os_create_task(5);  // �����������ȼ�5��
    os_create_task(6);  // �����������ȼ�6��

    os_delete_task(0);  // ɾ������
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
                if (KEY1 == 0)         //ȷ�ϰ�ť����
                {
                    os_wait(K_IVL, 10, 0); // ����
                    board_num--;       //����
                    if(board_num < 1)
                        board_num = 3; //Խ��ѭ��
                    while (KEY1 == 0) os_wait(K_IVL, 1, 0); // �ȴ��ͷ�
                }
            }
            
            if (KEY2 == 0)
            {
                os_wait(K_IVL, 3, 0); // ����
                if (KEY2 == 0)         //ȷ�ϰ�ť����
                {
                    os_wait(K_IVL, 10, 0); // ����
                    board_num++;        //����
                    if(board_num > 3)
                        board_num = 1;//Խ��ѭ��
                    while (KEY2 == 0) os_wait(K_IVL, 1, 0);// �ȴ��ͷ�
                }
               
            }
            
            if (KEY3 == 0)
            {
                os_wait(K_IVL, 3, 0); // ����
                if (KEY3 == 0)         //�ȴ������ͷ�
                {
                    os_wait(K_IVL, 10, 0); // ����  
                    switch (board_num)
                    {
                    //gameѡ���°���ȷ�ϰ���
                    case 1:
                        game_state = 1;
                        os_delete_task(2);
                        os_create_task(3);  // ��Ϸ�������ȼ�3��
                        break;
                
                    //settingѡ���°���ȷ�ϰ���
                    case 2:
                        setting_state = 1;
                        os_delete_task(2);
                        os_create_task(5);  // �����������ȼ�5��
                        break;

                    //scoreѡ���°���ȷ�ϰ�ť
                    case 3:
                        score_state = 1;
                        os_delete_task(2);
                        os_create_task(4);  // �����������ȼ�4��
                        break;
                    }
                    while (KEY3 == 0) os_wait(K_IVL, 1, 0);
                }
                
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
            //�˳���Ϸ
            if (KEY3 == 0)
            {
                os_wait(K_IVL, 3, 0); // ����
                if (KEY3 == 0)
                {
                    os_wait(K_IVL, 10, 0); // ����  
                    while (KEY3 == 0) os_wait(K_IVL, 1, 0);
                }
                game_state = 0;
                os_wait(K_IVL, 3, 0);
            }
        }

        /*������
        * key1 = speed plus
        * key2 = speed less
        * key3 = comfirm & backward
        */
        if (setting_state == 1)
        {
            //speed����
            if (KEY1 == 0)
            {
                os_wait(K_IVL, 3, 0); // ����
                if (KEY1 == 0)
                {
                    os_wait(K_IVL, 10, 0); // ����
                    speed--;       //speed����
                    if (speed < 2)
                        speed = 1; //Խ��ѭ��
                    while (KEY1 == 0) os_wait(K_IVL, 1, 0); // �ȴ��ͷ�

                }
                
            }

            //speed����
            if (KEY2 == 0)
            {
                os_wait(K_IVL, 3, 0); // ����
                if (KEY2 == 0)         
                {
                    os_wait(K_IVL, 10, 0); // ����
                    speed++;        //speed����
                    if (speed > 7)
                        speed = 8;//Խ��ѭ��
                    while (KEY2 == 0) os_wait(K_IVL, 1, 0);

                }
            }

            //�˳�����
            if (KEY3 == 0)
            {
                os_wait(K_IVL, 3, 0); // ����
                if (KEY3 == 0)         
                {
                    os_wait(K_IVL, 10, 0); // ����  
                    while (KEY3 == 0) os_wait(K_IVL, 1, 0);
                }
                setting_state = 0;
                os_wait(K_IVL, 3, 0);
            }

            os_wait(K_IVL, 10, 0);      //�ͷ��ڴ�
            
        }
        /*�Ʒְ���
        * key3 = back
        */
        if (score_state == 1)
        {
            //�˳��Ʒְ�
            if (KEY3 == 0)
            {
                os_wait(K_IVL, 3, 0); // ����
                if (KEY3 == 0);        
                {
                    os_wait(K_IVL, 10, 0); // ����  
                    while (KEY3 == 0) os_wait(K_IVL, 1, 0);
                }
                score_state = 0;
                os_wait(K_IVL, 3, 0);
            }
        }
    }
}

// �˵�����
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
            os_wait(K_IVL, 10, 0);  // �Ż�ˢ����
        }
    }
}

// ��Ϸ����
void task_game(void) _task_ 3
{
    while (1)  
    {
        if (game_state == 1)
        {
            OLED_Clear();

            P2 = 0x01;
            //��δ�����Ϸ�߼�
            while (game_state == 1)
            {
                OLED_ShowString(3, 3, "gamestate now", 16); 
            }
            OLED_Clear();
            os_wait(K_IVL, 100, 0);
            game_state = 0;
            os_create_task(2);  // �˵��������ȼ�2��
            os_delete_task(3);
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
            OLED_Init(); 
            OLED_Clear();
            P2 = 0x02;
            // ��ӻ�����ʾ�߼�
            while (score_state == 1)
            {
                OLED_ShowString(3, 3, "track1 = 100000", 16); 
                OLED_ShowString(3, 6, "track2 = 100000", 16);
                
                os_wait(K_IVL, 20, 0);
            }

            os_wait(K_IVL, 100, 0);
            OLED_Clear();
            os_create_task(2);  // �˵��������ȼ�2��
            os_delete_task(4);
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
            OLED_Init(); 
            OLED_Clear();
            os_wait(K_IVL, 10, 0);
            P2 = 0x03;
            OLED_ShowString(120, 0, "Q", 16);
            while(setting_state == 1)
            {
                temp = speed;
                os_wait(K_IVL, 10, 0);
                speed_str[0] = temp % 10 + '0';          // ��λ
                speed_str[1] = '\0';                     // �ַ���������
                os_wait(K_IVL, 10, 0);
                OLED_ShowString(3, 3, "speed:", 16);
                os_wait(K_IVL, 10, 0);
                
                // �����ַ���ָ��� OLED_ShowString
                OLED_ShowString(60, 3, speed_str, 16);  // ǿ��ת������
                os_wait(K_IVL, 20, 0);
            }
            os_wait(K_IVL, 100, 0);
            OLED_Clear();
            os_create_task(2);  // �˵��������ȼ�2��
            os_delete_task(5);
            
        }
        else
        {
            os_wait(K_IVL, 100, 0);
        }
    }
}

//����������
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