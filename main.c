/* MAIN.C */
#include <REGX52.H>
#include "OLED.h"
#include "RTX51TNY.H"
#include "Music.h"
#include "Note.h"
#include "Key.h"
#include "AT24C02.h"

/* Ӳ�����Ŷ��� */
sbit BUZZER = P2 ^ 5;

/* ȫ��״̬������ʹ��bit��ʡ�ڴ棩 */
bit game_state = 0;     // 0:�˵� 1:��Ϸ
bit setting_mode = 0;   // 0:���� 1:����
bit show_info = 0;      // ��Ϣ��ʾ��־
bit music_playing = 0;  // ���ֲ��ű�־

/* ��Ϸ���ݴ洢�Ż� */
idata struct {
    unsigned char track;      // ��ǰ���
    unsigned char speed;      // ��Ϸ�ٶ�
    unsigned int score;       // ��ǰ����
    unsigned int hi_score[2]; // ��ʷ��߷�
} game_data;

/* ��ʾ������������OLEDˢ�´����� */
xdata char disp_buf[16];

/********************* ������ *********************/
/* ��ʼ������ */
void init_task(void) _task_ 0 {
    // Ӳ����ʼ��
    P2 = 0xFF;
    OLED_Init();
    Music_Init(Track1); // Ĭ�ϳ�ʼ��

    // ��EEPROM������߷�
    game_data.hi_score[0] = AT24C02_ReadValue(0x00);
    game_data.hi_score[1] = AT24C02_ReadValue(0x04);

    // ������������
    os_create_task(1); // ���봦��
    os_create_task(2); // ��Ϸ�߼�
    os_create_task(3); // ��ʾ����

    os_delete_task(0); // ɾ������
}

/* ���봦������ */
void input_task(void) _task_ 1 {
    static bit key_lock = 0;

    while (1) {
        unsigned char key = Key();

        if (!key_lock && key) {
            key_lock = 1;

            /* �˵����� */
            if (!game_state && !setting_mode) {
                switch (key) {
                case 1: // ��
                    if (game_data.track > 1) game_data.track--;
                    break;
                case 2: // ��
                    if (game_data.track < 2) game_data.track++;
                    break;
                case 3: // ȷ��
                    if (game_data.track == 3) {
                        setting_mode = 1;
                    }
                    else {
                        game_state = 1;
                    }
                    break;
                }
            }

            /* ��Ϸ���� */
            if (game_state) {
                // �������м��
                if (key == 1 || key == 2) {
                    unsigned char res = check_key_hit(game_data.speed, key);
                    update_score(res);
                }
            }

            /* ����ģʽ */
            if (setting_mode) {
                switch (key) {
                case 1: // ����
                    if (game_data.speed < 8) game_data.speed++;
                    break;
                case 2: // ����
                    if (game_data.speed > 1) game_data.speed--;
                    break;
                case 3: // �˳�����
                    setting_mode = 0;
                    break;
                }
            }
        }

        if (!key) key_lock = 0;
        os_wait(K_IVL, 10, 0);
    }
}

/* ��Ϸ�߼����� */
void game_task(void) _task_ 2 {
    static unsigned char timer = 0;

    while (1) {
        if (game_state) {
            // �������ɣ�ÿ5��ѭ�����ɣ�
            if (++timer >= 5) {
                note_spawn(game_data.speed, track);
                timer = 0;
            }

            // ��������λ��
            update_note_positions(game_data.speed);

            // ���ֲ��ſ���
            if (!music_playing) {
                music_playing = 1;
                os_create_task(4); // ������������
            }
        }
        os_wait(K_IVL, 50, 0);
    }
}

/* ��ʾ�������� */
void display_task(void) _task_ 3 {
    while (1) {
        OLED_Clear();

        if (setting_mode) {
            // ���ý�����ʾ
            sprintf(disp_buf, "SPEED:%2d", game_data.speed);
            OLED_ShowString(16, 2, disp_buf, 16);
            OLED_ShowString(16, 5, "3 TO EXIT", 8);
        }
        else if (game_state) {
            // ��Ϸ������ʾ
            sprintf(disp_buf, "SCORE:%04u", game_data.score);
            OLED_ShowString(0, 0, disp_buf, 16);
            sprintf(disp_buf, "COMBO:%03u", combo);
            OLED_ShowString(0, 4, disp_buf, 16);
        }
        else {
            // ���˵���ʾ
            OLED_ShowString(16, 0, "1.TRACK1", 8);
            OLED_ShowString(16, 2, "2.TRACK2", 8);
            OLED_ShowString(16, 4, "3.SETTING", 8);
            // ��ʾ��߷�
            sprintf(disp_buf, "HI:%04u", game_data.hi_score[game_data.track - 1]);
            OLED_ShowString(80, 6, disp_buf, 8);
        }

        os_wait(K_IVL, 200, 0);
    }
}

/* ���ֲ������� */
void music_task(void) _task_ 4 {
    while (music_playing) {
        Music_PlayFullTrack();
        os_wait(K_IVL, 10, 0);
    }
    os_delete_task(4);
}

/********************* ���ߺ��� *********************/
void update_score(unsigned char result) {
    // �÷ּ����߼�
    static const unsigned char score_table[4] = { 0, 10, 30, 50 };

    if (result) {
        game_data.score += score_table[result];
        if (game_data.score > 1000) game_data.score = 1000;
        combo++;
    }
    else {
        combo = 0;
    }
}

void save_hi_score(void) {
    if (game_data.score > game_data.hi_score[game_data.track - 1]) {
        game_data.hi_score[game_data.track - 1] = game_data.score;
        AT24C02_WriteValue(0x00 + (game_data.track - 1) * 4, game_data.hi_score[game_data.track - 1]);
    }
}