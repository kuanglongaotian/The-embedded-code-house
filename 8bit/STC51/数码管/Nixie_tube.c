#include <REGX52.H>

int Number[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
//0,1,2,3,4,5,6,7,8,9
void Nixie_tube(int led,number)
{
	switch(led)
	{
		case 1:P2_4=1;P2_3=1;P2_2=1;break;//LED1
		case 2:P2_4=1;P2_3=1;P2_2=0;break;//LED2
		case 3:P2_4=1;P2_3=0;P2_2=1;break;//LED3
		case 4:P2_4=1;P2_3=0;P2_2=0;break;//LED4
		case 5:P2_4=0;P2_3=1;P2_2=1;break;//LED5
		case 6:P2_4=0;P2_3=1;P2_2=0;break;//LED6
		case 7:P2_4=0;P2_3=0;P2_2=1;break;//LED7
		case 8:P2_4=0;P2_3=0;P2_2=0;break;//LED8
	}
	P0=Number[number];
  P0=0x00;  
}