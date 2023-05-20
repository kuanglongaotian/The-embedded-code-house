#include <REGX52.H>
#include "DS18B20.H"
#include "LCD1602.H"
#include "AT24C02.H"
#include "Key.H"
#include "Delay1ms.H"

float T,Tshow;
char Thigh,Tlow;
unsigned char KeyNum;

void main()
{
	DS18B20_ConvertT();
	Delay1ms(1000);
	Thigh=AT24C02_ReadByte(0);	//读取温度阈值数据
	Tlow=AT24C02_ReadByte(1);
	if(Thigh>125 || Tlow<-55 || Thigh<=Tlow)//如果阈值非法，则设为默认值
	{
		Thigh=25;			
		Tlow=18;
	}
	
	LCD_Init();
	LCD_ShowString(1,1,"T:");
	LCD_ShowString(2,1,"TH:");
	LCD_ShowString(2,9,"TL:");
	LCD_ShowSignedNum(2,4,Thigh,3);
	LCD_ShowSignedNum(2,12,Tlow,3);
	
  while(1)
  {
		KeyNum=Key();
		DS18B20_ConvertT();
		T=DS18B20_ReadT();
		if(T<0)
		{
			LCD_ShowChar(1,3,'-');
			Tshow=-T;
		}
		else
		{
			LCD_ShowChar(1,3,'+');
			Tshow=T;
		}
		LCD_ShowNum(1,4,Tshow,3);
		LCD_ShowChar(1,7,'.');
		LCD_ShowNum(1,8,(unsigned long)(Tshow*10000)%10000,4);
		
		if(KeyNum)
		{
			if(KeyNum==1)	//K1按键，THigh自增
			{
				Thigh++;
				if(Thigh>125){Thigh=125;}
			}
			if(KeyNum==2)	//K2按键，THigh自减
			{
				Thigh--;
				if(Thigh<=Tlow){Thigh++;}
			}
			if(KeyNum==3)	//K3按键，TLow自增
			{
				Tlow++;
				if(Tlow>=Thigh){Tlow--;}
			}
			if(KeyNum==4)	//K4按键，TLow自减
			{
				Tlow--;
				if(Tlow<-55){Tlow=-55;}
			}
			LCD_ShowSignedNum(2,4,Thigh,3);
			LCD_ShowSignedNum(2,12,Tlow,3);
			AT24C02_WriteByte(0,Thigh);//写入到At24C02中保存,并延时5ms
			Delay1ms(5);
			AT24C02_WriteByte(1,Tlow);
			Delay1ms(5);
		}
		if(T>Thigh)			//越界判断
		{
			LCD_ShowString(1,13,"OV:H");
		}
		else if(T<Tlow)
		{
			LCD_ShowString(1,13,"OV:L");
		}
		else
		{
			LCD_ShowString(1,13,"    ");
		}
  }
}