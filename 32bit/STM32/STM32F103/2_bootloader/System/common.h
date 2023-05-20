#ifndef _COMMON_H
#define _COMMON_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "ymodem.h"
#include "system_flash.h"
#include "usart1.h"
#include "delay.h"

#define ApplicationAddress      (uint32_t)0x8003000 //
#define OffsetAddress           (uint32_t)0x3000    // 
#define FLASH_SECTOR_SIZE       (uint16_t)0x0400

typedef void (*pFunction)(void); 

void JumpToApplication(void);
void Main_Menu(void);
void System_Rest(void);

#endif

