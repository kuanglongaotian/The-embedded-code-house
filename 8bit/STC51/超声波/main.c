#include <REGX52.H>
#include "LCD1602.H"
#include "Timer0.H"
#include "Ultrasonic.H"

void main()
{
	Timer0_Init();
	LCD_Init();
	while(1)
	{
		Transmit();//发送
    while(P1_0==0);
		TR0=1;//打开定时器1
		while(P1_0==1);
		TR0=0;//关闭定时器1	
    Calc();//计算
	}
}

//void Timer0_Routine() interrupt 1
//{	
//  TH0=0x28;//60毫秒发一次超声波
//	TL0=0x00;
//}