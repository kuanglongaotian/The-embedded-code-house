#ifndef __AT24C02_H__
#define __AT24C02_H__

void I2c_Start(void);
void I2c_Stop(void);
void I2c_SendByte(unsigned char Byte);
//unsigned char I2c_ReceiveByte();
//void I2c_SendAck(unsigned char AckBit);
unsigned char I2c_ReceiveAck();    							   		    

#endif