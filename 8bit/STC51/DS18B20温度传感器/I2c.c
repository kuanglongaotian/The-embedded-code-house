#include <REGX52.H>

sbit I2c_Scl=P2^1;//时钟线
sbit I2c_Sda=P2^0;//数据线

void I2c_Start()//开始条件
{
	I2c_Sda=1;
  I2c_Scl=1;
	I2c_Sda=0;
	I2c_Scl=0;
}

void I2c_Stop()//停止条件
{
	I2c_Sda=0;
	I2c_Scl=1;
	I2c_Sda=1;
}

void I2c_SendByte(unsigned char Byte)//发送字节
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		I2c_Sda=Byte&(0x80>>i);
		I2c_Scl=1;
		I2c_Scl=0;
	}
}

unsigned I2c_ReceiveByte()//接受字节
{
	unsigned char i,Byte=0x00;
	I2c_Sda=1;
	for(i=0;i<8;i++)
	{
		I2c_Scl=1;
		if(I2c_Sda){Byte|=(0x80>>i);}
		I2c_Scl=0;
	}
	return Byte;
}

void I2C_SendAck(unsigned char AckBit)//发送应答
{
	I2c_Sda=AckBit;
	I2c_Scl=1;
	I2c_Scl=0;
}

unsigned char I2C_ReceiveAck(void)//接收应答
{
	unsigned char AckBit;
	I2c_Sda=1;
	I2c_Scl=1;
	AckBit=I2c_Sda;
	I2c_Scl=0;
	return AckBit;
}