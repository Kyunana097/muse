#include<REGX52.H>

void Timer0_Init(void)		//1����@12.000MHz
{
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TMOD |= 0x01;			//���ö�ʱ��ģʽ
	TL0 = 0x66;				//���ö�ʱ��ʼֵ
	TH0 = 0xFC;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
	ET0 = 1;				//�����ж�
	EA = 1;					//�������ж�
	PT0 = 0;				//�ж����ȼ�=0
}

//void Timer0_Routine() interrupt 1
//{
//	static unsigned int T0Count;
//	TL0 = 0x18;				//���ö�ʱ��ʼֵ
//	TH0 = 0xFC;				//���ö�ʱ��ʼֵ
//	if (T0Count >= 1000)	//1s
//	{
//		T0Count = 0;
//							//��������
//	}
//}
