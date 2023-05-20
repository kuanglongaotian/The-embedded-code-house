#include <REGX52.H>
#include "Key.H"
#include "Nixie_tube.H"
#include "Timer0.H"

sbit Motor=P1^0;

unsigned char Count,Compare;
unsigned char KeyNum,Speed;

void main()
{
	Timer0_Iint();
  while(1)
  {
    KeyNum=Key();
		if(KeyNum==1)
		{
			Speed++;
			Speed%=4;
			if(Speed==0){Compare=0;}
			if(Speed==1){Compare=50;}
			if(Speed==2){Compare=75;}
			if(Speed==3){Compare=100;}
		}
		Nixie_tube(1,Speed);
  }
}

void Timer0_Routine() interrupt 1//100us
{	
  TH0=0xFF;
	TL0=0xA4;
	Count++;
	Count%=100;
	if(Count<Compare)
	{Motor=1;}
	else
	{Motor=0;}
}