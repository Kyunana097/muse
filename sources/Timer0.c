#include<REGX52.H>

void Timer0_Init(void)		//1毫秒@12.000MHz
{
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |= 0x01;			//设置定时器模式
	TL0 = 0x66;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;				//允许中断
	EA = 1;					//开启总中断
	PT0 = 0;				//中断优先级=0
}

//void Timer0_Routine() interrupt 1
//{
//	static unsigned int T0Count;
//	TL0 = 0x18;				//设置定时初始值
//	TH0 = 0xFC;				//设置定时初始值
//	if (T0Count >= 1000)	//1s
//	{
//		T0Count = 0;
//							//代码内容
//	}
//}
