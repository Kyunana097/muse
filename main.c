#include <REGX52.H>
#include "Headers/oled.h"
#include "Headers/oledfont.h"
#include "Headers/RTX51TNY.H"
#include "Headers/music.h"

sbit KEY1 = P3 ^ 1;
sbit KEY2 = P3 ^ 0;
sbit KEY3 = P3 ^ 2;
sbit KEY4 = P3 ^ 3;

// ��Ϸ״̬����
bit game_state = 0;          // 0: ���˵���1: ��Ϸ������
bit setting_state = 0;       // 0: ���˵���1: ���ý���
bit info_state = 0;          // 0: ���˵���1: ������Ϣ����
bit isPlaying = 0;           // 0: û�������������Ϸ 1; ������Ϸ��
bit musicPlaying = 0;        // 0: ������ 1: ����

volatile unsigned short track1_score = 0;
volatile unsigned short track2_score = 0;
volatile unsigned short temp_score = 0;
volatile unsigned char track_num = 1;           // 1: ��һ�أ�2: �ڶ���
volatile unsigned char board_num = 1;           // 1: start 2: setting 3: info
volatile unsigned char x_position = 119;        //������ʼλ�� 119+8=127
volatile unsigned char speed = 4;               //�����ƶ��ٶ� 1-8

//����ת������װ������
bit KEY4_pressed = 0;        // �ж�״̬
// �� speed ת��Ϊ�ַ���
char speed_str[2];  // 1λ���� + '\0'
volatile unsigned char temp ;
volatile unsigned char hit_time = 0;
volatile unsigned char current_track = 1;//������λ�� 1-2

// ��ʼ������
void task_init(void) _task_ 0
{
    P2 = 0xFF;
    OLED_Init();
    OLED_Clear();
    os_wait(K_IVL, 50, 0);

    //��ʾ�˵��̳�
    OLED_ShowString(3, 0, "QG Dash v2.8", 8);
    OLED_ShowString(3, 2, "Key1:up", 8);
    OLED_ShowString(3, 3, "Key2:down", 8);
    OLED_ShowString(3, 4, "Key3:comfirm", 8);
    OLED_ShowString(3, 6, "Loading...", 8);
    
    os_wait(K_IVL, 1000, 0);
    OLED_ShowString(3, 6, "Welcome!  ", 8);
    os_wait(K_IVL, 100, 0);

    os_create_task(1);  // �����������ȼ�1��
    os_create_task(2);  // �˵��������ȼ�2��
    //os_create_task(3);  // ��Ϸ�������ȼ�3��
    //os_create_task(4);  // �����������ȼ�4��
    //os_create_task(5);  // �����������ȼ�5��
    //os_create_task(6);  // �����������ȼ�6��

    os_delete_task(0);  // ɾ������
}

// �������ж�����
void task_key(void) _task_ 1
{
    while(1)
    {
        /*���˵���
        * key1 = up
        * key2 = down
        * key3 = comfirm
        */
        if (game_state == 0 && setting_state == 0 && info_state == 0)
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
                        //������Ϸ�����˵�
                        game_state = 1;
                        break;
                
                    //settingѡ���°���ȷ�ϰ���
                    case 2:
                        setting_state = 1;
                        os_delete_task(2);  //����˵�
                        os_create_task(5);  //���������������ȼ�5��
                        break;

                    //infoѡ���°���ȷ�ϰ�ť
                    case 3:
                        //����info�����˵�
                        info_state = 1;
                        break;
                    }
                    while (KEY3 == 0) os_wait(K_IVL, 1, 0);
                }
                
            }
            os_wait(K_IVL, 10, 0);      //�ͷ��ڴ�
        }
        /**��Ϸ�����˵���
         * key 1 = up
         * key 2 = down
         * key 3 = comfirm
         * key 4 = back
         */
        if (game_state == 1 && isPlaying == 0)
        {
            if (KEY1 == 0)
            {
                os_wait(K_IVL, 3, 0); // ����
                if (KEY1 == 0)         //ȷ�ϰ�ť����
                {
                    os_wait(K_IVL, 10, 0); // ����
                    track_num--;       //����
                    if (track_num < 1)
                        track_num = 2; //Խ��ѭ��
                    while (KEY1 == 0) os_wait(K_IVL, 1, 0); // �ȴ��ͷ�
                }
            }

            if (KEY2 == 0)
            {
                os_wait(K_IVL, 3, 0); // ����
                if (KEY2 == 0)         //ȷ�ϰ�ť����
                {
                    os_wait(K_IVL, 10, 0); // ����
                    track_num++;        //����
                    if (track_num > 2)
                        track_num = 1;//Խ��ѭ��
                    while (KEY2 == 0) os_wait(K_IVL, 1, 0);// �ȴ��ͷ�
                }

            }

            if (KEY3 == 0)
            {
                os_wait(K_IVL, 3, 0); // ����
                if (KEY3 == 0)         //�ȴ������ͷ�
                {
                    os_wait(K_IVL, 10, 0); // ����  
                    //���ǰͣ���Ĺ��
                    isPlaying = 1;
                    while (KEY3 == 0) os_wait(K_IVL, 1, 0);
                }
                os_delete_task(2);  //����˵�
                os_create_task(3);  //������Ϸ�������ȼ�3��
            }

            //�����ϼ��˵�
            if (KEY4 == 0)
            {
                os_wait(K_IVL, 3, 0); // ����
                if (KEY4 == 0)
                {
                    os_wait(K_IVL, 10, 0); // ����  
                    while (KEY4 == 0) os_wait(K_IVL, 1, 0);
                }
                game_state = 0;
                os_wait(K_IVL, 3, 0);
            }
            os_wait(K_IVL, 10, 0);      //�ͷ��ڴ�
        } 

        /**������Ϣ�����˵���
         * key 3 = quit
         */
        if (info_state == 1)
        {
            //�˳������˵�
            if (KEY3 == 0)
            {
                os_wait(K_IVL, 3, 0); // ����
                if (KEY3 == 0);        
                {
                    os_wait(K_IVL, 10, 0); // ����  
                    while (KEY3 == 0) os_wait(K_IVL, 1, 0);
                }
                os_wait(K_IVL, 100, 0);
                info_state = 0;
            }
        }

        /*��Ϸ��
        * key1 = sky
        * key2 = ground
        * key3 = quit
        * key4 = pause����ʱ��д��
        */
        if (game_state == 1 && isPlaying == 1)
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
                OLED_Clear();
                isPlaying = 0;
                os_wait(K_IVL, 100, 0);
            }

        }

        /*������
        * key1 = speed plus
        * key2 = speed less
        * key3 = comfirm & quit
        * key4 = tap
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

            //�ж�
            if (KEY4 == 0)
            {
                os_wait(K_IVL, 3, 0); // ����
                if (KEY4 == 0)
                {
                    os_wait(K_IVL, 10, 0); // ����  
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

            os_wait(K_IVL, 10, 0);      //�ͷ��ڴ�
            
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
        //���˵�
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

        //��Ϸ�����˵�
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
        
        //������Ϣ�����˵�
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

// ��Ϸ����
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
            os_create_task(4);  //�������
            os_create_task(6);  //�������
            //��Ŀѡ��
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
                // �˳���Ϸ״̬
                isPlaying = 0;
                // ȷ��������������ɴ���
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
                // �˳���Ϸ״̬
                isPlaying = 0;
                // ȷ��������������ɴ���
                os_wait(K_IVL, 100, 0);
            }

            //�˴���������Ϸ�����ѡ����棨�ص����˵������ѡ�裩

            //game_state == 0�� �����˳�����
            OLED_Clear();
            os_wait(K_IVL, 100, 0);
            game_state = 0;

            os_create_task(2);  //����˵�
            os_delete_task(3);  //������Ϸ
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
        if (game_state == 1 && isPlaying == 1)
        {
            temp_score = 1000;
            os_wait(K_IVL, 10, 0);  // �ʵ���ʱ
		}
		else
		{
			// ��Ϸ����ʱ�������
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

				temp_score = 0;  // ������ʱ����
				os_wait(K_IVL, 100, 0);
			}
			os_wait(K_IVL, 100, 0);  // ����Ϸ״̬ʱ����CPUռ��
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
            x_position = 121 - speed * 2;
            while(setting_state == 1)
            {
                os_wait(K_IVL, 1, 0);
                OLED_ShowString(3, 3, "speed:", 16);
                os_wait(K_IVL, 1, 0);
                
                // �����ַ���ָ��� OLED_ShowString
                OLED_ShowValue(60, 3, speed, 16); 
                OLED_ShowString(16, 0, "$", 16);//�ж���
               
                //�ƶ�Q������ʹ��for���޷�ʶ��speed���룩
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

            //setting_state == 0�� �����˳�����
            os_wait(K_IVL, 100, 0);
            OLED_Clear();
            os_create_task(2);  //����˵�
            os_delete_task(5);  //��������
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