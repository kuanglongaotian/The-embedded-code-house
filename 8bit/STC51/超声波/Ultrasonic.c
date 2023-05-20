#include <REGX52.H>
#include "Timer0.H"
#include "LCD1602.H"

void Delay10us()		//@11.0592MHz
{
	unsigned char i;
	i = 2;
	while (--i);
}

void Transmit()
{
	P1_1=1;
	Delay10us();
	P1_1=0;
}

void Calc()
{
  unsigned int Time,Distance;
	Time=TH0*256+TL0;
	Time*=12/11.0592;	
	TH0=0;
	TL0=0;
  Distance=Time*0.017;//340m/s=340mm/ms=0.340mm/us=0.017cm/us
	LCD_ShowNum(1,1,Distance,3);
}