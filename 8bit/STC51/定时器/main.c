#include <REGX52.H>
#include "Timer0.H"
#include <INTRINS.H>
#include "Key.H"

unsigned char KeyNum,LedMode;

void main()
{
	P2=0xFE;
	Timer0_Init();
	while(1)
  {
		KeyNum=Key();
		if(KeyNum)
		{
			if(KeyNum==1)
			{
				LedMode++;
				if(LedMode==2)LedMode=0;
			}
		}
  }
}

void Timer0_Routine() interrupt 1
{	
	static unsigned int Count=0;//设置静态局部变量
  TH0=0xFC;
	TL0=0x66;
	Count++;
	if(Count==500)
	{
		Count=0;
		if(LedMode==0)P2=_crol_(P2,1);
		if(LedMode==1)P2=_cror_(P2,1);
	}
}