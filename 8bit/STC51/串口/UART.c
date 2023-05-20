#include <REGX52.H>
//串口初始化
void UART_Init()//9600波特，11.0592MHZ
{
	SCON =0x50;//串行控制寄存器0100 0000,设置串口工作方式为8位UART，波特率可变
	PCON &=0x7F;//电源寄存器,波特率不倍速
	TMOD &=0x0F;//该型号串口只适配定时器Timer1
	TMOD |=0x20;//设定计时器模式为双8位自动重装模式
	TL1 = 0xFD;		//设置定时初始值
	TH1 = 0xFD;		//设置定时重载值
	ET1 = 0;		//禁止定时器%d中断
	TR1 = 1;		//定时器1开始计时
	EA = 1;
	ES = 1;
}
//向电脑发送一个字节数据
void UART_SendByte(unsigned char Byte)
{
	SBUF=Byte;
	while(TI==0);
	TI=0;
}
/*串口中断模板,电脑向单片机发送数据
void UART_Routine() interrupt 4 //中断号位4
{
	if (RI==1)
	{

		RI=0;
	}
}
*/
