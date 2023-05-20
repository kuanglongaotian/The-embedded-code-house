#include <REGX52.H>
#include "I2c.H"
#include "font.H"

#define Max_Column	128 //行
#define Max_Row		64 //列

void Write_IIC_Command(unsigned char IIC_Command)
{
	I2c_Start();
	I2c_SendByte(0x78);            //Slave address,SA0=0
	I2c_ReceiveAck();
	I2c_SendByte(0x00);			//write command
  I2c_ReceiveAck();
	I2c_SendByte(IIC_Command);
	I2c_ReceiveAck();
	I2c_Stop();
}

void Write_IIC_Data(unsigned char IIC_Data)
{
	I2c_Start();
	I2c_SendByte(0x78);			//D/C#=0; R/W#=0
	I2c_ReceiveAck();
	I2c_SendByte(0x40);			//write data
	I2c_ReceiveAck();
	I2c_SendByte(IIC_Data);
	I2c_ReceiveAck();
	I2c_Stop();
}
//光标位置 y为0-7行，x为0-127列
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
	Write_IIC_Command(0xb0+y);//page0-page7(行)
	Write_IIC_Command((((x)&0xf0)>>4)|0x10);//高四位
	Write_IIC_Command(((x)&0x0f));//低四位
}
//清屏
void OLED_Clear(void)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		Write_IIC_Command(0xb0+m);//page0-page7
		Write_IIC_Command(0x00);//low(四位)column列 start address
		Write_IIC_Command(0x10);//high(四位)column列 start address
		for(n=0;n<128;n++)
		{
			Write_IIC_Data(0x00);
		}
	}
}
//在指定位置显示一个字符，size提供1和2两种大小
//当size为1时，y为0-7行，x为0-127列
//当size为2时，y为0-3行，x为0-127列
void OLED_ShowChar(unsigned char y,unsigned char x,unsigned char chr,unsigned char sizes)
{
	unsigned char c=0,i=0;
	c=chr-' ';//得到偏移后的值 
	if(x>Max_Column-1)
	{
		x=0;
	  y=y+2;
	}
	if(sizes==2)
	{
		OLED_Set_Pos(x,y);//列，行
		for(i=0;i<8;i++)
			Write_IIC_Data(zf8x16[c*16+i]);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
			Write_IIC_Data(zf8x16[c*16+i+8]);
	}
	else if(sizes==1)
	{
		OLED_Set_Pos(x,y);//列，行
		for(i=0;i<6;i++)
		  Write_IIC_Data(zf6x8[c][i]);
	}
}
//m^n次方函数
unsigned long oled_pow(unsigned char m,unsigned char n)
{
	unsigned long result=1;
	while(n--)result*=m;
	return result;
}
//显示数字
//x,y :起点坐标
//len :数字的位数
//num:范围(0~4294967295)
//mode:显示格式，为0时，当要显示的最高位为0时显示空格；为1时，显示0
void OLED_ShowNum(unsigned char y,unsigned char x, unsigned long num, unsigned char len,unsigned char mode,unsigned char sizes)
{
	unsigned char t,temp;
	unsigned char enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode==0)
					OLED_ShowChar(y,x+8*t,' ',sizes);
				else
					OLED_ShowChar(y,x+8*t,'0',sizes);
				continue;
			}
			else enshow=1;
		}
		OLED_ShowChar(y,x+8*t,temp+'0',sizes);
	}
}
//SSD1306初始化。复位后，默认页寻址模式
void OLED_Init(void)
{
	Write_IIC_Command(0xAE); //--display off
	Write_IIC_Command(0x00); //---set low column address
	Write_IIC_Command(0x10); //---set high column address
	Write_IIC_Command(0x40); //--set start line address
	Write_IIC_Command(0xB0); //--set page address
	Write_IIC_Command(0x81); // contract control
	Write_IIC_Command(0xFF); //--128
	Write_IIC_Command(0xA1); //set segment remap
	Write_IIC_Command(0xA6); //--normal / reverse
	Write_IIC_Command(0xA8); //--set multiplex ratio(1 to 64)
	Write_IIC_Command(0x3F); //--1/32 duty
	Write_IIC_Command(0xC8); //Com scan direction
	Write_IIC_Command(0xD3); //-set display offset
	Write_IIC_Command(0x00); //

	Write_IIC_Command(0xD5); //set osc division
	Write_IIC_Command(0x80); //

	Write_IIC_Command(0xD8); //set area color mode off
	Write_IIC_Command(0x05); //

	Write_IIC_Command(0xD9); //Set Pre-Charge Period
	Write_IIC_Command(0xF1); //

	Write_IIC_Command(0xDA); //set com pin configuartion
	Write_IIC_Command(0x12); //

	Write_IIC_Command(0xDB); //set Vcomh
	Write_IIC_Command(0x30); //

	Write_IIC_Command(0x8D); //set charge pump enable
	Write_IIC_Command(0x14); //

	Write_IIC_Command(0xAF); //--turn on oled panel
}