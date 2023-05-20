#include <REGX52.H>
#include "Matrix_Led.H"

unsigned char code List[]=
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0xC0,0x3C,0x03,0x3C,0x3C,0x03,0x3C,0xC0,
 0x00,0xFF,0x81,0x81,0x42,0x3C,0x00,0xFF,
 0x80,0x60,0x1E,0x01,0xFF,0x00,0xFF,0xC0,
 0x30,0x0C,0x02,0x0C,0x30,0xC0,0xFF,0x00,
 0xFF,0x81,0x81,0x42,0x3C,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};//将数据储存在闪存内

void main()
{ 
	unsigned char i=0,offset=0,count=0;
	MatrixLED_Init();
	while(1)
  {
		for(i=0;i<8;i++)
		{
			MatrixLED_ShowColum(i,List[i+offset]);
		}
		count++;
		if(count==50)
		{
			count=0;
			offset++;
			if(offset==48){offset=0;}
		}
  }
}