#include <REGX52.H>

sbit OneWire_DQ=P3^7;

unsigned char OneWire_Init()
{
	unsigned char AckBit,i;
	OneWire_DQ=1;
	OneWire_DQ=0;
	i = 227;while (--i);//delay 500us
	OneWire_DQ=1;
  i = 25;while (--i);//delay 60us
	AckBit=OneWire_DQ;
	i = 227;while (--i);//delay 500us
	return AckBit;
}

void OneWire_SendBit(unsigned char Bit)
{
	unsigned char i;
	OneWire_DQ=0;
	i = 3;while (--i);//delay 10us
	OneWire_DQ=Bit;
	i = 22;while (--i);//delay 50us
	OneWire_DQ=1;
}

unsigned char OneWire_ReceiveBit()
{
	unsigned char Bit,i;
	OneWire_DQ=0;
	i = 1;while (--i);//delay 5us
	OneWire_DQ=1;
	i = 1;while (--i);//delay 5us
	Bit=OneWire_DQ;
	i = 22;while (--i);//delay 50us
	return Bit;
}

void OneWire_SendByte(unsigned char Byte)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		OneWire_SendBit(Byte&(0x01<<i));
	}
}

unsigned char OneWire_ReceiveByte(void)
{
	unsigned char i;
	unsigned char Byte=0x00;
	for(i=0;i<8;i++)
	{
		if(OneWire_ReceiveBit()){Byte|=(0x01<<i);}
	}
	return Byte;
}