#include "system_flash.h"

#define FLASH_START_ADDR  ((uint32_t)0x08000000)
#define FLASH_END_ADDR    ((uint32_t)0x08010000)

uint16_t Flash_Buffer[512];//1KB���棬��Ӧһ��ҳ������

/**
*@ ��ȡ�ڲ�FLASH����
 *@ ReadAddress:������ʼ��ַ(ƫ�Ƶ�ַ)
 *@ *dest_Data: ���ݶ�ȡ�����׵�ַ
 *@ num:        ��ȡ�ֽڸ���
 */
void ReadFlashData_Byte(uint32_t ReadAddress, uint8_t *dest_Data, uint32_t num)
{
	for(uint32_t i=0;i<num;i++)
	{
		dest_Data[i]=*(uint8_t*)(FLASH_START_ADDR+ReadAddress+i);
	}
}
/**
*@ ��ȡ�ڲ�FLASH����
 *@ ReadAddr:������ʼ��ַ(ƫ�Ƶ�ַ)
 *@ *pBuffer: ���ݶ�ȡ�����׵�ַ
 *@ NumToRead:���ֽڸ���������16λ���ݣ�
*/
void ReadFlashData_Halfword(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=*(__IO uint16_t*)(FLASH_START_ADDR+ReadAddr);
		ReadAddr+=2;
	}
}
/**
 *@ д���ڲ�Flash����
 *@ WriteAddress:	������ʼ��ַ(ƫ�Ƶ�ַ)
 *@	*pBuffer: ����д�뻺���׵�ַ
 *@	NumToWrite: ���ֽڸ���������16λ���ݣ�
 */
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)
{
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		//printf("%d\n",pBuffer[i]);
		FLASH_ProgramHalfWord(WriteAddr+FLASH_START_ADDR,pBuffer[i]);
	  WriteAddr+=2;//
	}
}
/**
 *@ д���ڲ�Flash����,��Сд��2���ֽڡ�
 *@ WriteAddress:	������ʼ��ַ(ƫ�Ƶ�ַ)
 *@	*pBuffer: ����д�뻺���׵�ַ
 *@	NumToWrite: ���ֽڸ���������16λ���ݣ�
 *@ If_Projectdata:1,�Ե�ǰд���ַ�����������б���
                   0,û�б���
*/
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite,u8 If_Projectdata)
{
	u32 Page;	        //д���ַ����ҳ��
	u16 Page_Surplus; //д���ַ����ҳ��ƫ�Ƶİ��ֽ�����(���ֵ�λ)
	u16 Page_Last;    //д���ַҳ��ʣ��ռ��С(���ֵ�λ)
 	u16 i;

	if((WriteAddr|FLASH_START_ADDR)>=FLASH_END_ADDR)return;//�Ƿ���ַ
	Page =WriteAddr/FLASH_PAGE_SIZE;              //ҳ����ַ0~63 for STM32F103C8T6
	Page_Surplus =(WriteAddr%FLASH_PAGE_SIZE)/2;	//ҳ��ƫ�Ƶ�ַ(���ֵ�λ)
	Page_Last =FLASH_PAGE_SIZE/2-Page_Surplus;		//ҳ��ʣ��ռ��С
	if(NumToWrite<=Page_Last)Page_Last=NumToWrite;
	
	FLASH_Unlock();	//flash����
	if(If_Projectdata)
	{
		for(i=0;i<(NumToWrite*2/FLASH_PAGE_SIZE);i++)
    {
			FLASH_ErasePage(Page*FLASH_PAGE_SIZE);
			Page++;
			//printf("%d\n",Page);
		}
		STMFLASH_Write_NoCheck(WriteAddr,pBuffer,NumToWrite);
	}
	else
	{
		while(1)
		{
			ReadFlashData_Halfword(Page*FLASH_PAGE_SIZE,Flash_Buffer,FLASH_PAGE_SIZE/2);//������ǰҳ�����뻺��
//			for(i=0;i<Page_Last;i++)//
//			{
//				if(Flash_Buffer[Page_Surplus+i]!=0XFFFF)break;//�ҵ�����
//			}
//			if(i<Page_Last)//��ǰҳ����������Ҫ����
//			{
			FLASH_ErasePage(Page*FLASH_PAGE_SIZE+FLASH_START_ADDR);//����д���ַ����ҳ��
			for(i=0;i<Page_Last;i++)//
			{
				Flash_Buffer[i+Page_Surplus]=pBuffer[i];
			}
			STMFLASH_Write_NoCheck(Page*FLASH_PAGE_SIZE,Flash_Buffer,FLASH_PAGE_SIZE/2);
			//printf("Write_Project\n");
//			}
//			else 
//			{
//			FLASH_ErasePage(WriteAddr);
//			STMFLASH_Write_NoCheck(WriteAddr,pBuffer,Page_Last);//��ǰҳ��û������ֱ��д��
//			printf("Write_NoProject\n");
//			}
			if(NumToWrite==Page_Last)break;//����д����
			else//
			{
				Page++;				        //ҳ����ַ+1
				Page_Surplus=0;				//ƫ�Ƶ�ַ��0
				pBuffer+=Page_Last;  	//����ָ��ƫ��
				WriteAddr+=Page_Last;	//д��ַƫ��
				NumToWrite-=Page_Last;
				if(NumToWrite>(FLASH_PAGE_SIZE/2))Page_Last=FLASH_PAGE_SIZE/2;//��һ��ҳ��д����
				else Page_Last=NumToWrite;//��һ��ҳ������д��
			}
		}
	}
	FLASH_Lock();//flash����
}

