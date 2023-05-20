#include "common.h"

u8 Key=0x00,mode=0;
int16_t value;
u16 Flash_Data_halfword[FLASH_SECTOR_SIZE/2];//1KB,对应c8t6的一个扇区大小

extern u8 Data_Buffer[Ymodem_data_Size];//1029,缓冲一帧Ymodem数据,最坏打算为1KB数据帧
extern u8 Eot_count;//结束传输符计数
extern int32_t file_size;//文件大小
extern u32 packets_received;//正常数据包

//app区回到boot不再重新设置pc指针，直接软件复位，从头开始。
void System_Rest(void)
{
	int Time;
	
	Time=0xFFFFFF;
	while(Time--)
	{
		__nop();
	}
	//这个寄存器也是只有1bit，如果设置为1，除了NMI中断被允许，其他全部中断包括Hard Fault中断也被屏蔽，默认为0；
	__set_FAULTMASK(1);//FAULTMASK（断层屏蔽寄存器）
	//系统复位,复位后默认从MCU的起始地址开始。
	//这样程序就从APP回到了BOOT,再从BOOT跳转至APP的时候
	//如果复位，那么在app区跳转到boot可以正常跳转，但是第二次跳转到app区
	//会因为MCU堆栈不够,发生溢出,导致进入了硬件错误。所以跳转回boot直接采用软件系统复位
	//这样堆栈已经清除了,也就不会发生溢出
	NVIC_SystemReset();
}

void JumpToApplication(void)
{
  uint32_t StackAddr;
  uint32_t ResetVector;         

  pFunction JumpToApp;
  //这个寄存器只有1bit，如果设置为1，除了NMI和Hard Fault中断被允许外，其他中断全部被屏蔽，默认为0    
  __set_PRIMASK(1);//PRIMASK（优先级屏蔽寄存器）
  StackAddr = *(__IO uint32_t*)ApplicationAddress;              //0x08003000;
  ResetVector = *(__IO uint32_t*)(ApplicationAddress + 4);      //0x08003004;
  
  if((StackAddr&0x2FFC0000)==0x20000000)
  {
    JumpToApp = (pFunction)ResetVector; 
		__set_MSP(StackAddr);                               
    JumpToApp();
  }
}

int16_t App_Data_Write(u8* Flash_Data,u16 Receive_data_size)
{
	static u16 k=0,Flash_Page=0;
  static u32 Byte_Count=0;
	u16 i; 
	
	if(file_size > (0x10000))//大于c8t6容量就取消传输
  {
		Send_CA();
		Send_CA();
		//printf("Writing Data exceed Flash\r\n");
		return -1;
  }
	for(i=0;i<(Receive_data_size/2);i++)//
	{
		Flash_Data_halfword[k+i]=(Flash_Data[i*2+1]<<8)|Flash_Data[i*2];
	}
	k+=(Receive_data_size/2);
	Byte_Count+=Receive_data_size;
	if(k==512 || Byte_Count>=file_size)//数据存满1KB写入一次。以及无论满不满最后都写入一次
	{
	  k=0;	
		//一次写入一页数据
	  STMFLASH_Write(OffsetAddress+Flash_Page*1024,Flash_Data_halfword,sizeof(Flash_Data_halfword)/sizeof(u16),0);
		Flash_Page++;
		for(u16 k=0;k<512;k++)
			Flash_Data_halfword[k]=0xffff;
		if(Byte_Count>=file_size)
		{
		  Flash_Page=0;
			Byte_Count=0;
		}
	}
	
  return 0;//数据正常	
}

void Main_Menu()
{
	printf("\r\n============================ Main Menu ===============================\r\n\n");
	printf("  Download Image To the STM32F103C8T6 Internal Flash ------- 1\r\n\n");
	printf("  Upload Image From the STM32F103C8T6 Internal Flash ------- 2\r\n\n");
	printf("  Execute The New Program ----------====-------------------- 3\r\n\n");
	printf("======================================================================\r\n\n");
	
	for(u16 i=0;i<FLASH_SECTOR_SIZE/2;i++)
		Flash_Data_halfword[i]=0xffff;
	  
  while(1)
	{
		if(Key == 0x31)//模拟按键'1'
    {
			Key=0x00;
			mode=1;
			printf("Waiting for download\r\n");
			while(1)
			{
				if(packets_received==0)
				{
					delay_ms(1500);
					Send_CRC();
				}
				if(value==2)
				{
					value=0;
					break;//CRC16校验,完善等待下一个文件,第二次小文件无法覆盖
				}
//				else if(value==1){}
//				else if(value==-1){}
		  }
      mode=0;		
			printf("\r\n============================ Main Menu ===============================\r\n\n");
			printf("  Download Image To the STM32F103C8T6 Internal Flash ------- 1\r\n\n");
			printf("  Upload Image From the STM32F103C8T6 Internal Flash ------- 2\r\n\n");
			printf("  Execute The New Program ----------====-------------------- 3\r\n\n");
			printf("======================================================================\r\n\n");
		}
		if(Key == 0x32)//模拟按键'2'
		{
			Key = 0x00;//
		  printf("Sorry ,to be continued....\r\r\r\r\n");
			
			printf("\r\n============================ Main Menu ===============================\r\n\n");
			printf("  Download Image To the STM32F103C8T6 Internal Flash ------- 1\r\n\n");
			printf("  Upload Image From the STM32F103C8T6 Internal Flash ------- 2\r\n\n");
			printf("  Execute The New Program ---------------------------------- 3\r\n\n");
			printf("======================================================================\r\n\n");
		}
		if(Key == 0x33)//模拟按键'3'
		{
			printf("Start jump to App....\r\n");
			delay_ms(1500);
			JumpToApplication();
			printf("App jump flase\r\n");
		}
	}
}
//u8 l;
void DEBUG_USART_IRQHandler(void)
{ 
  if(USART_GetITStatus(DEBUG_USARTx,USART_IT_IDLE )!=RESET)
	{		
		//l++;
		USART_ReceiveData(DEBUG_USARTx);//
		USART_ClearFlag(DEBUG_USARTx,USART_FLAG_IDLE);//
	  DMA_Cmd(DMA1_Channel5,DISABLE);//
		if(mode)
		{
			value=Ymodem_data_prasing(Data_Buffer,0,App_Data_Write);//处理函数尽量不放中断里。写的时候忘记了懒得改了
			for (u16 i=0;i<1030;i++)//DMA数据清空,防止影响下一帧数据判断处理
			{
				Data_Buffer[i]=0;
			}
			DMA_SetCurrDataCounter(DMA1_Channel5,Ymodem_data_Size);//
			DMA_Cmd(DMA1_Channel5,ENABLE);//
			if(packets_received==0)
			{
				Send_ACK();//发送应答ACK
				//delay_ms(5);//不能延时
				Send_CRC();//发送"C"，等待接收下一包数据包
			}
			else Send_ACK();//发送应答
		}
		else
		{
			Key=Data_Buffer[0];
		  DMA_SetCurrDataCounter(DMA1_Channel5,Ymodem_data_Size);//
			DMA_Cmd(DMA1_Channel5,ENABLE);//
		}
	}
}

