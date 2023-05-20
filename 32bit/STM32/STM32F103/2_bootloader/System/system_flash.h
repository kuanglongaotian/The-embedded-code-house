#ifndef __SYSTEM_FLASH_H
#define	__SYSTEM_FLASH_H

#include "stm32f10x.h"
#include <stdio.h>

#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
  #define FLASH_PAGE_SIZE    ((uint16_t)0x800)	//2048
#else
  #define FLASH_PAGE_SIZE    ((uint16_t)0x400)	//1024
#endif

void ReadFlashData_Byte(uint32_t ReadAddress, uint8_t *dest_Data, uint32_t num);
void ReadFlashData_Halfword(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite,u8 If_Projectdata);

#endif


