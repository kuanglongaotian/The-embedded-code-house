#include <REGX52.H>
#include "OLED.H"

unsigned p=0;

void main()
{
  OLED_Init();//1306初始化
  OLED_Clear();//清屏幕
	OLED_ShowChar(0,0,'b',1);
	OLED_ShowChar(0,8,'i',1);
	OLED_ShowChar(0,16,'l',1);
	OLED_ShowChar(0,24,'i',1);
	OLED_ShowChar(0,32,'b',1);
	OLED_ShowChar(0,40,'i',1);
	OLED_ShowChar(0,48,'l',1);
	OLED_ShowChar(0,56,'i',1);	
//	Write_IIC_Command(0x2E); //关闭滚动
//  Write_IIC_Command(0x26); //水平向左或者右滚动 26/27
//  Write_IIC_Command(0x00); //A 虚拟字节
//  Write_IIC_Command(0x00); //B 起始页 0
//  Write_IIC_Command(0x07); //C 滚动时间间隔
//  Write_IIC_Command(0x07); //D 终止页 1
//  Write_IIC_Command(0x00); //虚拟字节
//  Write_IIC_Command(0xFF); //虚拟字节
//	Write_IIC_Command(0x2F); //开启滚动
	while(1)
	{
		OLED_ShowNum(4,40,p,8,1,1);
		p++;
	}
}