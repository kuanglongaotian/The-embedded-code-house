#include <REGX52.H>
#include "LCD1602.H"
#include "DS1302.H"
#include "Key.H"
#include "Timer0.H"

//sbit Buzzer=P1^5;

unsigned char KeyNum,TimeSetSelect,cursor,symbol;
//unsigned char Timedata[]={0,0,0,0,0,0,0};

void Time_Show()
{
	DS1302_ReadTime();
	LCD_ShowNum(1,1,DS1302_Time[0],2);//年
	LCD_ShowNum(1,4,DS1302_Time[1],2);//月
	LCD_ShowNum(1,7,DS1302_Time[2],2);//日
	LCD_ShowNum(2,1,DS1302_Time[3],2);//时
	LCD_ShowNum(2,4,DS1302_Time[4],2);//分
	LCD_ShowNum(2,7,DS1302_Time[5],2);//秒
	LCD_ShowNum(1,10,DS1302_Time[6],1);//星期
}

void Time_Set()
{
	if(KeyNum==2)//设置修改位置
  {
		TimeSetSelect++;
		TimeSetSelect%=7;//超过6自动变为0
	}
	if(KeyNum==3)//修改当前位置的时钟数据(自增)
	{
		DS1302_Time[TimeSetSelect]++;
		if(DS1302_Time[0]>99){DS1302_Time[0]=0;}
		if(DS1302_Time[1]>12){DS1302_Time[1]=1;}
		if( DS1302_Time[1]==1 || DS1302_Time[1]==3 || DS1302_Time[1]==5 || DS1302_Time[1]==7 || 
			DS1302_Time[1]==8 || DS1302_Time[1]==10 || DS1302_Time[1]==12)//日越界判断
		{if(DS1302_Time[2]>31){DS1302_Time[2]=1;}}//大月
		else if(DS1302_Time[1]==4 || DS1302_Time[1]==6 || DS1302_Time[1]==9 || DS1302_Time[1]==11)
		{if(DS1302_Time[2]>30){DS1302_Time[2]=1;}}//小月
		else if(DS1302_Time[1]==2)
		{
			if(DS1302_Time[0]%4==0)
			{if(DS1302_Time[2]>29){DS1302_Time[2]=1;}}//闰年2月
			else
			{if(DS1302_Time[2]>28){DS1302_Time[2]=1;}}//平年2月}
		}
		if(DS1302_Time[3]>23){DS1302_Time[3]=0;}
		if(DS1302_Time[4]>59){DS1302_Time[4]=0;}
		if(DS1302_Time[5]>59){DS1302_Time[5]=0;}
		if(DS1302_Time[6]>7){DS1302_Time[6]=1;}
	}
	if(KeyNum==4)//修改当前位置的时钟数据(自减)
	{
		DS1302_Time[TimeSetSelect]--;
		if(DS1302_Time[0]<0){DS1302_Time[0]=99;}//年越界判断
		if(DS1302_Time[1]<1){DS1302_Time[1]=12;}//月越界判断
		if( DS1302_Time[1]==1 || DS1302_Time[1]==3 || DS1302_Time[1]==5 || DS1302_Time[1]==7 || 
			DS1302_Time[1]==8 || DS1302_Time[1]==10 || DS1302_Time[1]==12)//日越界判断
		{
			if(DS1302_Time[2]<1){DS1302_Time[2]=31;}//大月
			if(DS1302_Time[2]>31){DS1302_Time[2]=1;}
		}
		else if(DS1302_Time[1]==4 || DS1302_Time[1]==6 || DS1302_Time[1]==9 || DS1302_Time[1]==11)
		{
			if(DS1302_Time[2]<1){DS1302_Time[2]=30;}//小月
			if(DS1302_Time[2]>30){DS1302_Time[2]=1;}
		}
		else if(DS1302_Time[1]==2)
		{
			if(DS1302_Time[0]%4==0)
			{
				if(DS1302_Time[2]<1){DS1302_Time[2]=29;}//闰年2月
				if(DS1302_Time[2]>29){DS1302_Time[2]=1;}
			}
			else
			{
				if(DS1302_Time[2]<1){DS1302_Time[2]=28;}//平年2月
				if(DS1302_Time[2]>28){DS1302_Time[2]=1;}
			}
		}
		if(DS1302_Time[3]<0){DS1302_Time[3]=23;}//时越界判断
		if(DS1302_Time[4]<0){DS1302_Time[4]=59;}//分越界判断
		if(DS1302_Time[5]<0){DS1302_Time[5]=59;}//秒越界判断
		if(DS1302_Time[6]<1){DS1302_Time[6]=7;}
	}
	if(cursor==1 && TimeSetSelect==0){LCD_ShowString(1,1," _");}
	else{LCD_ShowNum(1,1,DS1302_Time[0],2);}
	if(cursor==1 && TimeSetSelect==1){LCD_ShowString(1,4," _");}
	else{LCD_ShowNum(1,4,DS1302_Time[1],2);}
	if(cursor==1 && TimeSetSelect==2){LCD_ShowString(1,7," _");}
	else{LCD_ShowNum(1,7,DS1302_Time[2],2);}
	if(cursor==1 && TimeSetSelect==3){LCD_ShowString(2,1," _");}
	else{LCD_ShowNum(2,1,DS1302_Time[3],2);}
	if(cursor==1 && TimeSetSelect==4){LCD_ShowString(2,4," _");}
	else{LCD_ShowNum(2,4,DS1302_Time[4],2);}
	if(cursor==1 && TimeSetSelect==5){LCD_ShowString(2,7," _");}
	else{LCD_ShowNum(2,7,DS1302_Time[5],2);}
	if(cursor==1 && TimeSetSelect==6){LCD_ShowString(1,10,"_");}
	else{LCD_ShowNum(1,10,DS1302_Time[6],1);}
}

void main()
{
	unsigned char Mode;
	LCD_Init();//LCD初始化
	DS1302_Init();//时钟芯片初始化	
	Timer0_Init();//定时器初始化
//	Timer1_Init();
  DS1302_SetTime();//写入初始时间
	LCD_ShowString(1,1,"  -  -  - ");
	LCD_ShowString(2,1,"  :  :  ");
  while(1)
  {
		KeyNum=Key();//按键扫描
		if(KeyNum==1)//开启时钟编辑模式
		{
			Mode++;
//			if(Mode==0){Mode=1;}
//			else if(Mode==1){Mode=0;DS1302_SetTime();}//将数据写入时钟芯片内部
			if (Mode==2){Mode=0;DS1302_SetTime();}
		}
		switch(Mode)
		{
			case 0: Time_Show();break;//显示芯片数据
			case 1: Time_Set();break;//按键修改时钟数据
//			case 2: 
			//case 2:
		}
		if(DS1302_Time[4]==0 && DS1302_Time[5]==0)//整点报时
		{
//			TR1=1;
			symbol=8;
		}
  }
}

void Timer0_Routine() interrupt 1
{	
	static unsigned int Count=0;//设置静态局部变量
  TH0=0xFC;
	TL0=0x66;
	Count++;
	if(Count==500)//500ms定时
	{
		Count=0;
		cursor=!cursor;
		if(symbol)
		{
		  P2_0=!P2_0;
			symbol--;
		}
	}
}

//void Timer1_Routine() interrupt 3
//{	
//	unsigned int Time;
//  TH1=0xFF;//(65535-1000*11.0592/12)/256,设置定时器高8位
//	TL1=0xF7;	
//	Time++;
//  Buzzer=!Buzzer;
//	if(Time==10000){Time=0;TR1=0;}
//}

