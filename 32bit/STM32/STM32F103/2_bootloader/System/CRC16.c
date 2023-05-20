#include "CRC16.h"

//CRC-16/XMODEM
uint16_t CRC16_XMODEM(u8 *data,int32_t datalen)
{
	static const unsigned short crc16tab[] = {
  0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
	};
	unsigned short crc = 0;
	unsigned char ch = 0;
 
	while (datalen-- != 0)
	{
		ch = crc >> 12;
		crc <<= 4;
		crc ^= crc16tab[ch ^ (*data / 16)];
 
		ch = crc >> 12;
		crc <<= 4;
		crc ^= crc16tab[ch ^ (*data & 0x0f)];
		data++;
	}
	return crc;
}
