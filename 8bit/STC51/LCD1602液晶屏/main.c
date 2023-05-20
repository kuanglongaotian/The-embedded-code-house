#include <REGX52.H>
#include "LCD1602.H"
#include "Delay1ms.H"

void main()
{ 
	int result;
	LCD_Init();
	LCD_ShowString(1,1,"Hello world");
  while(1)
	{
	  result+=1;
	  Delay1ms(1000);
	  LCD_ShowNum(2,1,result,3);
	}
}