#include "stm32f10x.h"                  // Device header
#include "multi_button.h"
#include "usart1.h"
#include "timer4.h"

unsigned char btn1_id = 0;

/*
PRESS_DOWN | 按键按下，每次按下都触发
PRESS_UP | 按键弹起，每次松开都触发
PRESS_REPEAT | 重复按下触发，变量repeat计数连击次数
SINGLE_CLICK | 单击按键事件
DOUBLE_CLICK | 双击按键事件
LONG_PRESS_START | 达到长按时间阈值时触发一次
LONG_PRESS_HOLD | 长按期间一直触发

*/

uint8_t read_button_GPIO(uint8_t button_id)
{
	unsigned char button_value;
  // you can share the GPIO read function with multiple Buttons
	switch(button_id)
	{
		case 0://对应btn1_id
			button_value = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12);
			break;
		default:
			button_value = 1;
			break;
	}
	return button_value;
}

void BTN1_PRESS_DOWN_Handler(void* btn)
{
  Usart_SendString(DEBUG_USARTx,"button down\n");
}

void BTN1_PRESS_UP_Handler(void* btn)
{
  Usart_SendString(DEBUG_USARTx,"button up\n");
}

void BTN1_SINGLE_CLICK_Handler(void* btn)
{
  Usart_SendString(DEBUG_USARTx,"button click\n");
}

void BTN1_SINGLE_DOUBLE_Handler(void* btn)
{
  Usart_SendString(DEBUG_USARTx,"button double\n");
}

void BTN1_LONG_PRESS_START_Handler(void* btn)
{
  Usart_SendString(DEBUG_USARTx,"button long press\n");
}

void BTN1_LONG_PRESS_HOLD_Handler(void* btn)
{
  Usart_SendString(DEBUG_USARTx,"button press continue\n");
}


int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//GPIO_Mode_IPU
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	
	USART_Config();
	Usart_SendString(DEBUG_USARTx,"button down\n");
	TIM4_Int_Init(199,7199);//50ms
	
	struct Button button1;
	button_init(&button1, read_button_GPIO, 0, btn1_id);
	button_attach(&button1, SINGLE_CLICK, BTN1_SINGLE_CLICK_Handler);
  button_attach(&button1, DOUBLE_CLICK, BTN1_SINGLE_DOUBLE_Handler);
	button_attach(&button1, PRESS_DOWN, BTN1_PRESS_DOWN_Handler);
  button_attach(&button1, PRESS_UP, BTN1_PRESS_UP_Handler);
	button_attach(&button1, LONG_PRESS_START,BTN1_LONG_PRESS_START_Handler);
	button_attach(&button1, LONG_PRESS_HOLD,BTN1_LONG_PRESS_HOLD_Handler);
	button_start(&button1);
	
	while(1)
	{
		
	}
}

void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	  button_ticks();
	}
}



