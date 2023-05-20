#include <REGX52.H>

/*TMOD默认为0000 0000
TMOD只能整体操作，为了不影响Timer0和Timer1，所以采用与和或操作
TMOD=TMOD&0xF0,只对低四位清零，保留高四位
TMOD=TMOD&0x0F,只对高四位清零，保留低四位
TMOD=TMOD|0x0X,只对高四位操作，保留低四位
TMOD=TMOD|0xX0,只对高四位操作，保留低四位
清零采用与，赋值采用或
*/
/*TH TL分别为8位，既00000000 00000000，总共可计时65535μs*/

void Timer0_Init()//11.0592晶振，所以定时器+1的频率比12晶振慢
{
	TMOD=TMOD&0xF0;//高位保留，地位清零
	TMOD=TMOD|0x01;//0000 0001，只启动定时器0，并设置为16位定时器/计数器模式
	TF0=0;
	TR0=1;
  TH0=0xFC;//(65535-1000*11.0592/12)/256,设置定时器高8位
	TL0=0x66;//(65535-1000*11.0592/12)%256+1,设置定时器低8位，总共定时1ms
	ET0=1;//设置中断
	EA=1;
	PT0=0;//设置第一优先级
}

/*中断函数模板
void Timer0_Routine() interrupt 1
{	
	static unsigned int Count=0;//设置静态局部变量
  TH0=0xFC;
	TL0=0x18;
	Count++;
	if(Count==500)
	{
		Count=0;
		if(LedMode==0)P2=_crol_(P2,1);
		if(LedMode==1)P2=_cror_(P2,1);
	}
}
*/