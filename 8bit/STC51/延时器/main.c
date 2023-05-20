#include <REGX52.H>
#include "Delay1ms.H"

void main()
{
  while(1)
	{
		P2_2=0;
    Delay1ms(500);	
		P2_2=1;
    Delay1ms(500);
	}
}