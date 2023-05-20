#include "stm32f10x.h"                  // Device header
#include "common.h"

ErrorStatus HSEStartUpStatus;

void RCC_Configuration(void)
{
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();
  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  if(HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 
    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);
    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);//
    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);
    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() !=0x08);
  }
}

int main(void)
{
	RCC_Configuration();//72MHZ
	delay_init();//
	USART1_DMA_Recieve_Config(); 
	USART1_Config();//115200
	USART_DMACmd(DEBUG_USARTx,USART_DMAReq_Rx,ENABLE);//USART1 DMA 初始化
	delay_ms(2000);
	printf("\r\n======================================================================");
	printf("\r\n=              (C) COPYRIGHT 2022 STMicroelectronics                 =");
	printf("\r\n=                                                                    =");
	printf("\r\n=     In-Application Programming Application  (Version 4.0.1)        =");
	printf("\r\n=                                                                    =");
	printf("\r\n=                                             By 电子爱好者兼CV大师  =");
	printf("\r\n======================================================================");
	printf("\r\n\r\n");
	Main_Menu();
  while(1)
	{}
}




