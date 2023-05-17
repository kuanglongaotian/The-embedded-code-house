#ifndef  CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#define MAX_SIZE  10                        //缓冲区大小

typedef struct 
{
  unsigned char head;                      //缓冲区头部位置
  unsigned char tail;                         //缓冲区尾部位置
  unsigned char RecBuf[MAX_SIZE]; //缓冲区数组
} Circular_Buffer;

Circular_Buffer CirQueue; 

void RecBuf_Write(unsigned char data);
unsigned char RecBuf_Read(unsigned char *pData);

#endif