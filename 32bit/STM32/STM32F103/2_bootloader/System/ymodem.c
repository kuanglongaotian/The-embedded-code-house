#include "ymodem.h"

u8 Eot_count=0;//结束传输符计数
int32_t file_size;//文件大小
u32 packets_received=0;//正常数据包
u8 error;
/*
return 1   :接收方取消传输
       0   :该帧数据包正常
			 -1  :该帧数据包不正常
*/
static int8_t Ymodem_Rievece_packet(u8* Ymodem_data,int16_t *length)
{
	uint16_t packet_size;
  *length = 0;
	switch(Ymodem_data[0])
	{
	  case SOH:
      packet_size = PACKET_SIZE;//128字节的数据包
      break;
    case STX://
      packet_size = PACKET_1K_SIZE;//1024字节的数据包
      break;
    case EOT://传输完毕结束,length=0
			Eot_count++;
		  //if(Eot_count==3)Eot_count=1;
      return 0;
    case CA://发送方中途取消传输
        *length = -1;
        return 0;
    case ABORT1://length=0
			return 1;
    case ABORT2://length=0
			return 1;
		default://length=0
      return -1;
	}
	//反码校验,校验不合格终止传输
	if (Ymodem_data[PACKET_SEQNO_INDEX] != ((Ymodem_data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff))
	{
		return -1;
	}
	*length = packet_size;
  return 0;
}
/*
return 
        2    :正常结束文件传输
        1    :用户取消接受数据
        0    :数据帧处理正常
       -1    :数据帧异常，重发本次数据帧
Continue
				0    :只一个文件
				1    :接受多个文件
*/

int16_t Ymodem_data_prasing(u8* Ymodem_Original_Data,u8 Continue,int16_t (* callback)(u8* Ymodem_Data,u16 Vaild_data))
{
	char file_name[FILE_NAME_LENGTH];//缓存文件名
	char file_size_buffer[FILE_SIZE_LENGTH];//缓存文件大小
	
	u8 Receive_data[PACKET_1K_SIZE];//对应Ymodem的最多传输数据量
	u8 i=0;Continue=0;
	
	int8_t head;
	u16 Crc_value;
	int16_t Data_Size,value;
	
	if(error>=MAX_ERRORS)
	{
//		packets_received=0;
//		Eot_count=0;
//		error=0;
//		file_size=0;
	  Send_CA();
    Send_CA();
		//printf("没看黄历吧，算了别搞了\r\n");
    return 1;
	}
	head = Ymodem_Rievece_packet(Ymodem_Original_Data,&Data_Size);
	switch(head)
	{
		case  0://数据包正常
			switch(Data_Size)
			{
			  case -1://发送方中途取消传输
					Send_ACK();
					return 1;
				case 0://发送方整一个文件数据包发送完毕
					if(Eot_count==1)Send_NAK();
					else 
					{
						Send_ACK();
						if(Continue)//继续接收下一个文件
						{
							Eot_count=0;
							packets_received=0;
							error=0;
							file_size=0;
							Send_CRC();
						}
					}
					return 0;
				default:
						if(packets_received==0)//处理起始数据帧
						{
							//CRC校验
							Crc_value=Ymodem_Original_Data[PACKET_HEADER+Data_Size]<<8|//高8位
							Ymodem_Original_Data[PACKET_HEADER+Data_Size+1];//低8位
							memcpy(Receive_data,Ymodem_Original_Data+PACKET_HEADER,Data_Size);
							//Crc_value2=CRC16_XMODEM(Receive_data,Data_Size);
							if(CRC16_XMODEM(Receive_data,Data_Size)!=Crc_value)
							{
								error++;
			          Send_NAK();
							  return -1;//重发！！！
							}
							if(Ymodem_Original_Data[PACKET_HEADER] != 0)//路径名非空
							{
								//获取起始帧传输的文件名
								for (i=0;i<=FILE_NAME_LENGTH;i++)
								{
									if(Ymodem_Original_Data[PACKET_HEADER+i]==0x00)break;//遇到0x00表示文件名结束
									file_name[i] = Ymodem_Original_Data[PACKET_HEADER+i];
								}
								//将数组中的字符转为字符串获取文件名
								for (u8 j=0;i<=FILE_SIZE_LENGTH;i++,j++)
								{
									if(Ymodem_Original_Data[PACKET_HEADER+i+1]==0x20)break;//遇到空格表示文件大小结束
									file_size_buffer[j] = Ymodem_Original_Data[PACKET_HEADER+i+1];
								}
								file_size = atoi(file_size_buffer);//降数组中的字符转为int获取文件大小
              //保险起见防止数据丢失，应答放在DMA串口中断开启的最后							
//							Send_ACK();//发送应答ACK
//							Send_CRC();//发送"C"，等待接收下一包数据包
								packets_received++;//成功接受到一包正常数据
							}
							else{Send_ACK();return 0;}//路径为空，也确认
							return 0;
						}
						else if(packets_received!=0 && Eot_count==0)//开始正文
						{
							Crc_value=Ymodem_Original_Data[PACKET_HEADER+Data_Size]<<8|//高8位
							Ymodem_Original_Data[PACKET_HEADER+Data_Size+1];//低8位
							memcpy(Receive_data,Ymodem_Original_Data+PACKET_HEADER,Data_Size);
							//Crc_value2=CRC16_XMODEM(Receive_data,Data_Size);
							if(CRC16_XMODEM(Receive_data,Data_Size)!=Crc_value)
							{
								error++;
			          Send_NAK();
							  return -1;//重发！！！
							}
							else value=callback(Receive_data,Data_Size);
							if(value==0)
							{
								packets_received++;//成功接收到一包正常数据
	  						//Send_ACK();//保险起见防止数据丢失，应答放在DMA串口中断开启的最后
								return 0;
							}
							else if(value==-1)//回调有问题，直接取消传输
							{
								Eot_count=0;
							  packets_received=0;
							  error=0;
							  file_size=0;
								Send_CA();
								Send_CA();
								return 1;
							}
						}
						else if(Eot_count==2 && packets_received>1 && Ymodem_Original_Data[PACKET_SEQNO_INDEX]==0x00)//判断结束帧
						{
							Eot_count=0;
							packets_received=0;
							error=0;
   						file_size=0;
							Send_ACK();
							return 2;//结束文件传输						
						}
			}
		case -1://整一个数据包就不正常,重发！！！！！
			error++;
			Send_NAK();
			return -1;
	  case  1://接收方取消传输数据
			packets_received=0;
		  Eot_count=0;
		  error=0;
		  file_size=0;
			Send_CA();
      Send_CA();
			return 1;
	}
	return 0;
}


