#include <REGX52.H>
#include "Steering_engine.H"
#include "Timer0.H"
#include "Key.H"

unsigned char Rad=4;
unsigned char KeyNum;

void main()
{
	Timer0_Init();
	while(1)
	{
		KeyNum=Key();
		if(KeyNum)
		{
			if(KeyNum==1){Rad++;Rad%=5;}//大于4时，清零
			if(KeyNum==2){Rad--;if(Rad==255){Rad=4;}}//小于0时，回4
		}
	}
}

void Timer0_Routine() interrupt 1
{	
	static unsigned char T=1;
	if(T==1)
	{
    TH0=SteeringEngine_HEPT[Rad]/256;//500us
	  TL0=SteeringEngine_HEPT[Rad]%256+1;
		P0_0=1;
		T=0;
	}
	else if(T==0)
	{
		TH0=SteeringEngine_LEPT[Rad]/256;//19500us
		TL0=SteeringEngine_LEPT[Rad]%256+1;
		P0_0=0;
		T=1;
	}
}