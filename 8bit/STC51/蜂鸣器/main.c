#include <REGX52.H>
//#include "Buzzer_time.H"
#include "Key.H"
#include "Timer0.H"
#include "Delay1ms.H"

#define Speed 500

sbit Buzzer=P1^5;
unsigned char KeyNum;
unsigned char Select,Count;
unsigned char Music[]={
12,4,
12,4,
19,4,
19,4,
21,4,
21,4,
19,8,
17,4,
17,4,
16,4,
16,4,
14,4,
14,4,
12,8,
0xFF
};
unsigned int Reload[]={
63777,63872,63969,64054,64140,64216,64291,64360,64426,64489,64547,64607,
64655,64704,64751,64795,64837,64867,64913,64948,64981,65012,65042,65070,
65095,65120,65144,65166,65186,65206,65225,65242,65259,65274,65289,65303
};


void main()
{
	Timer0_Init();
  while(1)
  {
  	if(Music[Count]!=0xFF)
		{
			Select=Music[Count++];
			Delay1ms(Speed/4*Music[Count++]);
			TR0=0;//关闭计时器
			Delay1ms(5);
			TR0=1;//开启计时器
		}
		else{TR0=0;while(1);}
  }
}

void Timer0_Routine() interrupt 1
{	
  TH0=Reload[Select]/256;//设定定时初值
	TL0=Reload[Select]%256;//设定定时初值
	Buzzer=!Buzzer;
}