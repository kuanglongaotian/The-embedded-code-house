#include <REGX52.H>
#include "I2c.H"

#define AT24C02_ADDRESS		0xA0

void AT24C02_WriteByte(unsigned char WordAddress,Data)//写入数据
{
	I2c_Start();
	I2c_SendByte(AT24C02_ADDRESS);
	I2c_ReceiveAck();
	I2c_SendByte(WordAddress);
	I2c_ReceiveAck();
	I2c_SendByte(Data);
	I2c_ReceiveAck();
	I2c_Stop();
}

unsigned char AT24C02_ReadByte(unsigned char WordAddress)//读取数据
{
	unsigned char Data;
	I2c_Start();
	I2c_SendByte(AT24C02_ADDRESS);
	I2c_ReceiveAck();
	I2c_SendByte(WordAddress);
	I2c_ReceiveAck();
	I2c_Start();
	I2c_SendByte(AT24C02_ADDRESS|0x01);
	I2c_ReceiveAck();
	Data=I2c_ReceiveByte();
	I2c_SendAck(1);
	I2c_Stop();
	return Data;
}