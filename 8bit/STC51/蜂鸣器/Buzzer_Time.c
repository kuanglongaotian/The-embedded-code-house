#include <REGX52.H>
//#include <INTRINS.H>

sbit Buzzer=P1^5;//自定义端口地址

void Delay500us()		//@11.0592MHz
{
	unsigned char i;

	//_nop_();
	i = 227;
	while (--i);
}

void Buzzer_Time(unsigned int ms)
{
	unsigned int i;
  for(i=0;i<ms*2;i++)
	{
		Buzzer=!Buzzer;
		Delay500us();
	}
}