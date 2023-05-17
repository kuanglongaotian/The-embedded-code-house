#include "Circular_queue.h"

/**
* @brief дһ���ֽڵ����λ�����
* @param data����д�������
* @return none
*/
void RecBuf_Write(unsigned char data)
{
	CirQueue.RecBuf[CirQueue.tail]=data;     //��β��׷��
    if(++CirQueue.tail>=MAX_SIZE)             //β�ڵ�ƫ��
		CirQueue.tail=0;                           		 //����������󳤶ȹ���,�γɻ��ζ���
    if(CirQueue.tail == CirQueue.head)         //���ܵ�ʱ�����ͷ��β����ȣ������Ϊ�������޸�ͷ�ڵ�ƫ��λ�ö�����������
	{
		if(++CirQueue.head >= MAX_SIZE)CirQueue.head=0;//ͷ�ڵ�+1��Ȼ���ж��Ƿ񳬳����г���
    }
}
/**
* @brief ��ȡ���λ�������һ���ֽڵ�����
* @param *pData:ָ�룬���ڱ����ȡ��������
* @return 1��ʾ�������ǿյģ�0��ʾ��ȡ���ݳɹ�
*/
unsigned char RecBuf_Read(unsigned char *pData)
{
	if(CirQueue.tail == CirQueue.head)                     //���ͷβ�Ӵ���ʾ����Ϊ��
		return 1;                                                       //����1�����λ������ǿյ�
    else
    {
		*pData=CirQueue.RecBuf[CirQueue.head];      //����������ǿ���ȡͷ�ڵ�ֵ��ƫ��ͷ�ڵ�
		if(++CirQueue.head >= MAX_SIZE)CirQueue.head=0;
		return 0;     //����0����ʾ��ȡ���ݳɹ�
    }
}
