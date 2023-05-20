#include <REGX52.H>
#include <string.H>
#include "LCD1602.H"
#include "MatrixKey.H"

unsigned char KeyNum;
//unsigned int i=1;啊啊啊啊啊啊，全局变量nmd，为什么液晶驱动也有变量i！！！
unsigned char key_save[]="00000";//存放输入的密码
unsigned char key[]="10086";//密码
unsigned int symbol=1;
unsigned int confirm=0;

void main()
{   
	unsigned int i=1;
	
	LCD_Init();
	LCD_ShowString(1,1,"Password:");
	LCD_ShowNum(2,1,0,1);//初始化密码全为零
	LCD_ShowNum(2,2,0,1);
	LCD_ShowNum(2,3,0,1);
	LCD_ShowNum(2,4,0,1);
	LCD_ShowNum(2,5,0,1);
	
	while(1)
	{
		KeyNum=Matrixkey();
		
		if (symbol)
		{
			switch(KeyNum)
			{
				case 1: {LCD_ShowNum(2,i,1,1);key_save[i-1]='1';break;}//密码输出1
				case 2: {LCD_ShowNum(2,i,2,1);key_save[i-1]='2';break;}//密码输出2
				case 3: {LCD_ShowNum(2,i,3,1);key_save[i-1]='3';break;}//密码输出3
				case 4: {i+=1;break;}//密码左移一位
				case 5: {LCD_ShowNum(2,i,4,1);key_save[i-1]='4';break;}//密码输出4
				case 6: {LCD_ShowNum(2,i,5,1);key_save[i-1]='5';break;}//密码输出5
				case 7: {LCD_ShowNum(2,i,6,1);key_save[i-1]='6';break;}//密码输出6
				case 8: {i-=1;break;}//密码右移一位
				case 9: {LCD_ShowNum(2,i,7,1);key_save[i-1]='7';break;}//密码输出7
				case 10: {LCD_ShowNum(2,i,8,1);key_save[i-1]='8';break;}//密码输出8
				case 11: {LCD_ShowNum(2,i,9,1);key_save[i-1]='9';break;}//密码输出9
				case 12: {LCD_ShowNum(2,i,0,1);key_save[i-1]='0';break;}//密码输出0
				case 16: {confirm=1;break;}//确认密码
			}
			
			if(i==6 || i==0){i=1;}//限制可输入密码长度
			
			if(confirm){if(strcmp(key,key_save)==0){symbol=0;}}
		}
		else
		{ 
			LCD_Init();
			while(1)
			{
				LCD_ShowString(1,1,"OK");
			}
		}
	}
}