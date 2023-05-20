#include <REGX52.H>
/*给新的变量赋值，并未改变任何寄存器地址，只是增加可读性
sbit位将寄存器地址赋给变量*/
sbit rclk=P3^5;//输出锁存时钟线，下降沿有效
sbit srclk=P3^6;//数据输入时钟，上升沿有效
sbit ser=P3^4;

void _74HC595_WriteByte(unsigned char Byte)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		ser=Byte &(0x80>>i);
		srclk=1;
		srclk=0;
	}
	rclk=1;
	rclk=0;
}
//该led矩阵屏为共阳极，按列扫描
void MatrixLED_ShowColum(unsigned char Column,Data)
{
	_74HC595_WriteByte(Data);
	P0=~(0x80>>Column);
	P0=0xFF;//消影
}
//初始化
void MatrixLED_Init()
{
	srclk=0;
	rclk=0;
}