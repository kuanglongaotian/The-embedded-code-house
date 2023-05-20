#include <REGX52.H>

sbit DS1302_SCLK=P3^6;
sbit DS1302_IO=P3^4;
sbit DS1302_CE=P3^5;
//定义写入地址
#define DS1302_Seconds  0x80
#define DS1302_Minutes  0x82
#define DS1302_Hours    0x84
#define DS1302_Dates    0x86
#define DS1302_Months   0x88
#define DS1302_Weeks    0x8A
#define DS1302_Years    0x8C
#define DS1302_WP       0x8E
/*时钟芯片内部数据为BCD码。如35秒，为两个四位2进制数分别表示各位和十位0010 0101，
十进制转BCD，BCD=十进制/10*16+十进制%10，
BCD转十进制，十进制=BCD/16*10+BCD%16，然而BCD码实际按16进制数规则既0x25=37*/
char DS1302_Time[]={21,12,17,8,59,55,5};//年，月，日，时，分，秒，星期

void DS1302_Init()
{
  DS1302_SCLK=0;
	DS1302_CE=0;
}

void DS1302_WriteByte(unsigned char Command,Data)
{ 
	unsigned char i;
	DS1302_CE=1;
	for(i=0;i<8;i++)//从低到高读取
	{
		DS1302_IO=Command &(0x01<<i);//0000 0001,0000 0010
		DS1302_SCLK=1;
		DS1302_SCLK=0;
	}
	
	for(i=0;i<8;i++)//从低到高读取
	{
		DS1302_IO=Data &(0x01<<i);//0000 0001,0000 0010
		DS1302_SCLK=1;
		DS1302_SCLK=0;
	}
	DS1302_CE=0;
}

unsigned char DS1302_ReadByte(unsigned char Command)
{
	unsigned char i,Data=0x00;
	Command=Command |0x01;//写入的地址改为读出地址，既最低为变为1
	DS1302_CE=1;
	for(i=0;i<8;i++)//从低到高读取
	{
		DS1302_IO=Command &(0x01<<i);//0000 0001,0000 0010
		DS1302_SCLK=0;
		DS1302_SCLK=1;
	}
	for(i=0;i<8;i++)
	{
		DS1302_SCLK=1;
		DS1302_SCLK=0;
		if(DS1302_IO){Data=Data|(0x01<<i);}
	}
	
	DS1302_CE=0;
	DS1302_IO=0;
	return Data;
}
//写入年月日时分秒星期
void DS1302_SetTime()
{
	DS1302_WriteByte(DS1302_WP,0x00);//关闭写入保护
	DS1302_WriteByte(DS1302_Years,DS1302_Time[0]/10*16+DS1302_Time[0]%10);
	DS1302_WriteByte(DS1302_Months,DS1302_Time[1]/10*16+DS1302_Time[1]%10);
	DS1302_WriteByte(DS1302_Dates,DS1302_Time[2]/10*16+DS1302_Time[2]%10);
	DS1302_WriteByte(DS1302_Hours,DS1302_Time[3]/10*16+DS1302_Time[3]%10);
	DS1302_WriteByte(DS1302_Minutes,DS1302_Time[4]/10*16+DS1302_Time[4]%10);
	DS1302_WriteByte(DS1302_Seconds,DS1302_Time[5]/10*16+DS1302_Time[5]%10);
	DS1302_WriteByte(DS1302_Weeks,DS1302_Time[6]/10*16+DS1302_Time[6]%10);
	DS1302_WriteByte(DS1302_WP,0x80);//开启写入保护
}
//获取年月日时分秒星期
void DS1302_ReadTime()
{
	unsigned char Time_BCD;
	Time_BCD=DS1302_ReadByte(DS1302_Years);//年
	DS1302_Time[0]=Time_BCD/16*10+Time_BCD%16;
	Time_BCD=DS1302_ReadByte(DS1302_Months);//月
	DS1302_Time[1]=Time_BCD/16*10+Time_BCD%16;
	Time_BCD=DS1302_ReadByte(DS1302_Dates);//日
	DS1302_Time[2]=Time_BCD/16*10+Time_BCD%16;
	Time_BCD=DS1302_ReadByte(DS1302_Hours);//时
	DS1302_Time[3]=Time_BCD/16*10+Time_BCD%16;
	Time_BCD=DS1302_ReadByte(DS1302_Minutes);//分
	DS1302_Time[4]=Time_BCD/16*10+Time_BCD%16;
	Time_BCD=DS1302_ReadByte(DS1302_Seconds);//秒
	DS1302_Time[5]=Time_BCD/16*10+Time_BCD%16;
	Time_BCD=DS1302_ReadByte(DS1302_Weeks);//星期
	DS1302_Time[6]=Time_BCD/16*10+Time_BCD%16;
}