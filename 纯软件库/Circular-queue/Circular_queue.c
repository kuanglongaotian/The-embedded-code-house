#include "Circular_queue.h"

/**
* @brief 写一个字节到环形缓冲区
* @param data：待写入的数据
* @return none
*/
void RecBuf_Write(unsigned char data)
{
	CirQueue.RecBuf[CirQueue.tail]=data;     //从尾部追加
    if(++CirQueue.tail>=MAX_SIZE)             //尾节点偏移
		CirQueue.tail=0;                           		 //大于数组最大长度归零,形成环形队列
    if(CirQueue.tail == CirQueue.head)         //接受的时候如果头和尾巴相等，则队列为满，则修改头节点偏移位置丢弃早期数据
	{
		if(++CirQueue.head >= MAX_SIZE)CirQueue.head=0;//头节点+1，然后判断是否超出队列长度
    }
}
/**
* @brief 读取环形缓冲区的一个字节的数据
* @param *pData:指针，用于保存读取到的数据
* @return 1表示缓冲区是空的，0表示读取数据成功
*/
unsigned char RecBuf_Read(unsigned char *pData)
{
	if(CirQueue.tail == CirQueue.head)                     //如果头尾接触表示队列为空
		return 1;                                                       //返回1，环形缓冲区是空的
    else
    {
		*pData=CirQueue.RecBuf[CirQueue.head];      //如果缓冲区非空则取头节点值并偏移头节点
		if(++CirQueue.head >= MAX_SIZE)CirQueue.head=0;
		return 0;     //返回0，表示读取数据成功
    }
}
