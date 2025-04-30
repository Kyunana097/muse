#include "note.h"
#include "oled.h"


extern code unsigned char track[] = 
{ 
    29, 4, 1, 
    30, 4, 2,
    32, 4, 0, 
    34, 4, 2, 
    29, 4, 1,
    30, 4, 2,
    32, 4, 0, 
    34, 4, 2,
    29, 4, 1, 
    30, 4, 2,
    32, 4, 0,
    34, 4, 2, 
    29, 4, 1,
    30, 4, 2,
    32, 4, 0, 
    34, 4, 2, 
    0, 0, 0 };

//����1
extern unsigned char code Track1[] =
{
    //����,ʱֵ

    29,4,
    27,2,
    25,4,
    27,2,
    29,3,
    30,1,
    29,2,
    27,4,
    0,0,

    29,4,
    27,2,
    25,4,
    27,2,
    29,3,
    30,1,
    29,2,
    27,4,
    0,0,

    0xFF	//��ֹ��־
};

extern unsigned char code Track2[] =
{
    //����,ʱֵ,track(0:��,1:sky,2:ground)

    25,2,
    29,2,
    32,6,
    34,2,
    32,2,
    29,2,
    25,2,
    29,2,
    27,4,
    25,2,
    27,2,
    29,4,
    0,0,

    25,2,
    29,2,
    32,6,
    34,2,
    32,2,
    29,2,
    25,2,
    29,2,
    27,4,
    25,2,
    24,2,
    25,4,
    0,0,

    0xFF	//��ֹ��־
};
/* ȫ�ֱ������� */
 static unsigned char xdata x_position[2][MAX_NOTES_PER_TRACK]; // ʹ��xdata��չ�ڴ�
 static unsigned char data is_active[2] = { 0 }; // ʹ���ڲ�RAM�洢�״̬
 unsigned char data note_count[2] = { 0 }; // ʹ���ڲ�RAM
 unsigned char data hit_result_counts[4] = { 0 }; // �ж����ͳ������
 unsigned char combo = 0;

/* ����λ�ø��º��� */
void update_note_positions(unsigned char speed)
{
    /* �������������ں�����ͷ */
    unsigned char track_idx;          // ��ǰ����Ĺ������
    unsigned char note_idx;           // ��ǰ�������������
    unsigned char active_notes;       // ��ǰ����м��������״̬��λ�ֶΣ�
    unsigned char count;              // ��ǰ���������������
    unsigned char* pos;               // ָ��ǰ����λ�õ�ָ��

    // �����������
    for (track_idx = 0; track_idx < 2; track_idx++)
    {
        active_notes = is_active[track_idx];  // ��ȡ��ǰ����ļ���״̬
        count = note_count[track_idx];        // ��ȡ��ǰ�������������

        // ������ǰ����е���������
        for (note_idx = 0; note_idx < count; note_idx++)
        {
            // ��鵱ǰ�����Ƿ񼤻�
            if (active_notes & (1 << note_idx))
            {
                pos = &x_position[track_idx][note_idx];  // ��ȡ��ǰ������λ��ָ��
                // �� OLED �������ǰ������λ��
                OLED_ShowString(x_position[track_idx][note_idx], (track_idx == 0) ? 0 : 3, " ", 16);
                
                // �ȼ��������һ���Ƿ񳬳���Ļ��Χ��������Ļ���Ϊ 128 ���أ�
                if (*pos <= speed)
                {
                    // �������������Ļ��Χ�����Ϊ�Ǽ���
                    active_notes &= ~(1 << note_idx);
                    combo = 0;  // ����������
                    hit_result_counts[0]++;  // ���� MISS �ļ���
                }
                else
                {
                    // �������������λ��
                    *pos -= speed;
                    // �� OLED ����ʾ���º������λ��
                    OLED_ShowString(x_position[track_idx][note_idx], (track_idx == 0) ? 0 : 3, "Q", 16);
                }
            }
        }
        // ���µ�ǰ����ļ���״̬
        is_active[track_idx] = active_notes;
    }

    /* �ӳټ��ټ��� */
    // �����������������Ǽ������������
    for (track_idx = 0; track_idx < 2; track_idx++)
    {
        // �����ǰ����������������� 0���������һ������δ����������������
        while (note_count[track_idx] > 0 &&
            !(is_active[track_idx] & (1 << (note_count[track_idx] - 1))))
        {
            note_count[track_idx]--;  // ������������
        }
    }
}


/* �������ɺ��� */
void note_spawn(unsigned char speed, const unsigned char code* track)
{
    /* �������������ں�����ͷ */
    static unsigned char track_idx = 0; // ��̬���������ڼ�¼��ǰ�������������
    unsigned char note_track;
    unsigned char idx;

    // ����Ƿ���������Ҫ����
    if (track[track_idx] == 0) // ���������ֹ����ȫ�㣩����ʾ�������ɽ���
    {
        return; // �˳�����
    }

    // ��ȡ��ǰ�����Ĺ�����
    note_track = track[track_idx + 2] - 1;

    // ������Ƿ���Ч��������δ��
    if (note_track < 2 && note_count[note_track] < MAX_NOTES_PER_TRACK)
    {
        idx = note_count[note_track]; // ��ȡ��ǰ�������������
        x_position[note_track][idx] = 121 - speed * 2; // ���������ĳ�ʼλ��
        is_active[note_track] |= (1 << idx); // ���Ϊ����״̬
        note_count[note_track]++; // ���ӹ������������
        OLED_ShowString(x_position[note_track][idx], (note_track == 0) ? 0 : 3, "Q", 16); // ��ʾ����
    }

    // �ƶ�����һ����������
    track_idx += 3;
}

/**
 * @brief ��鰴���Ƿ�����������Keil���ݰ汾��
 * @param speed �����ƶ��ٶ�
 * @param track ������(1~N)
 * @return �ж����(NOTE_MISS��ʾδ����)
 */
unsigned char check_key_hit(unsigned char speed, unsigned char track)
{
    /* ���б������������ں�����ͷ */
    unsigned char track_idx;
    unsigned char result;
    unsigned char note_idx;
    unsigned char pos;
    unsigned char hit_time;

    track_idx = track - 1;
    result = NOTE_MISS;

    for (note_idx = 0; note_idx < note_count[track_idx]; note_idx++)
    {
        if ((is_active[track_idx] & (1 << note_idx)) &&
            (x_position[track_idx][note_idx] < 40)) /* ���ж������� */
        {
            pos = x_position[track_idx][note_idx];
            hit_time = (pos - 16) / speed;

            if (hit_time < 1)
                result = NOTE_PERFECT;
            else if (hit_time < 2)
                result = NOTE_GOOD;
            else
                result = NOTE_BAD;

            OLED_ShowString(x_position[track_idx][note_idx],
                (track_idx == 0) ? 0 : 3, " ", 16);
            is_active[track_idx] &= ~(1 << note_idx);
            hit_result_counts[result]++;
            combo++;
            return result;
        }
    }

    return NOTE_MISS;
}
/**
 * @brief ��鲢�������ٳ�ʱ��������������棩
 * @param speed �����ƶ��ٶ���ֵ
 * @return 1:���ڲ������˳�ʱ���� 0:�޳�ʱ����
 */
unsigned char check_miss_notes(unsigned char speed)
{
    /* ���б������������ں�����ͷ��Keil�淶�� */
    unsigned char track_idx;
    unsigned char note_idx;

    /* ���ȼ���������������x_position <= speed�� */
    for (track_idx = 0; track_idx < 2; track_idx++)
    {

        for (note_idx = 0; note_idx < note_count[track_idx]; note_idx++)
        {
            if ((is_active[track_idx] & (1 << note_idx)) &&
                (x_position[track_idx][note_idx] <= speed))
            {
                /* ����������� */
                OLED_ShowString(x_position[track_idx][note_idx],
                    (track_idx == 0) ? 0 : 3, " ", 16);
                is_active[track_idx] &= ~(1 << note_idx);

                /* ����״̬ */
                hit_result_counts[NOTE_MISS]++;
                combo = 0;

                return 1; // ���ֲ������˳�ʱ����
            }
        }
    }

    return 0; // �޳�ʱ����
}