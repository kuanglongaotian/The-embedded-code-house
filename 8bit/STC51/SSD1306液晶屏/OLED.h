#ifndef _OLED_H__
#define _OLED_H__

void OLED_Init(void);
void OLED_Clear(void);
void OLED_Fill(unsigned char fill_Data);
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char sizes);
void OLED_ShowNum(unsigned char x,unsigned char y,unsigned long num,unsigned char len,unsigned char mode,unsigned char sizes);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);

void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);

#endif