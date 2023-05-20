#include "system_flash.h"

#define FLASH_START_ADDR  ((uint32_t)0x08000000)
#define FLASH_END_ADDR    ((uint32_t)0x08010000)

uint16_t Flash_Buffer[512];//1KB缓存，对应一个页数容量

/**
*@ 读取内部FLASH数据
 *@ ReadAddress:数据起始地址(偏移地址)
 *@ *dest_Data: 数据读取缓存首地址
 *@ num:        读取字节个数
 */
void ReadFlashData_Byte(uint32_t ReadAddress, uint8_t *dest_Data, uint32_t num)
{
	for(uint32_t i=0;i<num;i++)
	{
		dest_Data[i]=*(uint8_t*)(FLASH_START_ADDR+ReadAddress+i);
	}
}
/**
*@ 读取内部FLASH数据
 *@ ReadAddr:数据起始地址(偏移地址)
 *@ *pBuffer: 数据读取缓存首地址
 *@ NumToRead:半字节个数（几个16位数据）
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
 *@ 写入内部Flash数据
 *@ WriteAddress:	数据起始地址(偏移地址)
 *@	*pBuffer: 数据写入缓存首地址
 *@	NumToWrite: 半字节个数（几个16位数据）
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
 *@ 写入内部Flash数据,最小写入2个字节。
 *@ WriteAddress:	数据起始地址(偏移地址)
 *@	*pBuffer: 数据写入缓存首地址
 *@	NumToWrite: 半字节个数（几个16位数据）
 *@ If_Projectdata:1,对当前写入地址的扇区数据有保护
                   0,没有保护
*/
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite,u8 If_Projectdata)
{
	u32 Page;	        //写入地址所在页数
	u16 Page_Surplus; //写入地址所在页数偏移的半字节数量(半字单位)
	u16 Page_Last;    //写入地址页数剩余空间大小(半字单位)
 	u16 i;

	if((WriteAddr|FLASH_START_ADDR)>=FLASH_END_ADDR)return;//非法地址
	Page =WriteAddr/FLASH_PAGE_SIZE;              //页数地址0~63 for STM32F103C8T6
	Page_Surplus =(WriteAddr%FLASH_PAGE_SIZE)/2;	//页数偏移地址(半字单位)
	Page_Last =FLASH_PAGE_SIZE/2-Page_Surplus;		//页数剩余空间大小
	if(NumToWrite<=Page_Last)Page_Last=NumToWrite;
	
	FLASH_Unlock();	//flash解锁
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
			ReadFlashData_Halfword(Page*FLASH_PAGE_SIZE,Flash_Buffer,FLASH_PAGE_SIZE/2);//读出当前页数放入缓存
//			for(i=0;i<Page_Last;i++)//
//			{
//				if(Flash_Buffer[Page_Surplus+i]!=0XFFFF)break;//找到擦除
//			}
//			if(i<Page_Last)//当前页数有数据需要缓存
//			{
			FLASH_ErasePage(Page*FLASH_PAGE_SIZE+FLASH_START_ADDR);//擦除写入地址所在页数
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
//			STMFLASH_Write_NoCheck(WriteAddr,pBuffer,Page_Last);//当前页数没有数据直接写入
//			printf("Write_NoProject\n");
//			}
			if(NumToWrite==Page_Last)break;//数据写完了
			else//
			{
				Page++;				        //页数地址+1
				Page_Surplus=0;				//偏移地址归0
				pBuffer+=Page_Last;  	//数组指针偏移
				WriteAddr+=Page_Last;	//写地址偏移
				NumToWrite-=Page_Last;
				if(NumToWrite>(FLASH_PAGE_SIZE/2))Page_Last=FLASH_PAGE_SIZE/2;//下一个页数写不完
				else Page_Last=NumToWrite;//下一个页数可以写完
			}
		}
	}
	FLASH_Lock();//flash上锁
}

