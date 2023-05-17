#ifndef  CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#define MAX_SIZE  10                        //��������С

typedef struct 
{
  unsigned char head;                      //������ͷ��λ��
  unsigned char tail;                         //������β��λ��
  unsigned char RecBuf[MAX_SIZE]; //����������
} Circular_Buffer;

Circular_Buffer CirQueue; 

void RecBuf_Write(unsigned char data);
unsigned char RecBuf_Read(unsigned char *pData);

#endif