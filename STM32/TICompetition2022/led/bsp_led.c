/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   led应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "bsp_led.h"   


static void Delay ( __IO uint32_t nCount )
{
  for ( ; nCount != 0; nCount -- );
	
}

 /**
  * @brief  初始化控制LED的IO
  * @param  无
  * @retval 无
  */
void LED_GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启LED相关的GPIO外设时钟*/
		RCC_APB2PeriphClockCmd( LED1_GPIO_CLK | LED2_GPIO_CLK | LED3_GPIO_CLK, ENABLE);
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIO*/
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	
		
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;

		/*调用库函数，初始化GPIO*/
		GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);
		
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;

		/*调用库函数，初始化GPIOF*/
		GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);

		/* 关闭所有led灯	*/
		GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
		
		/* 关闭所有led灯	*/
		GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);	 
    
    /* 关闭所有led灯	*/
		GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
}

/*********************************************END OF FILE**********************/

//反馈情况的闪灯 随便做就用最笨的方法了
void LED_flash(LedColor_t color){
	
	switch(color){
		case _RED: 
			LED_RGBOFF
			Delay(0xFFFFF);
			LED_RED
			Delay(0xFFFFF);
			LED_RGBOFF
			Delay(0xFFFFF);
			LED_RED
			Delay(0xFFFFF);
			LED_RGBOFF
			Delay(0xFFFFF);
			LED_RED
			Delay(0xFFFFF);
			LED_RGBOFF
			break;
		case _GREEN: 
			LED_RGBOFF
			Delay(0xFFFFF);
			LED_GREEN
			Delay(0xFFFFF);
			LED_RGBOFF
			Delay(0xFFFFF);
			LED_GREEN
			Delay(0xFFFFF);
			LED_RGBOFF
			Delay(0xFFFFF);
			LED_GREEN
			Delay(0xFFFFF);
			LED_RGBOFF
			break;
		case _BLUE: 
			LED_RGBOFF
			Delay(0xFFFFF);
			LED_BLUE
			Delay(0xFFFFF);
			LED_RGBOFF
			Delay(0xFFFFF);
			LED_BLUE
			Delay(0xFFFFF);
			LED_RGBOFF
			Delay(0xFFFFF);
			LED_BLUE
			Delay(0xFFFFF);
			LED_RGBOFF
			break;
		case _YELLOW: 
			LED_RGBOFF
			Delay(0xFFFFF);
			LED_YELLOW
			Delay(0xFFFFF);
			LED_RGBOFF
			Delay(0xFFFFF);
			LED_YELLOW
			Delay(0xFFFFF);
			LED_RGBOFF
			Delay(0xFFFFF);
			LED_YELLOW
			Delay(0xFFFFF);
			LED_RGBOFF
			break;
		case _PURPLE: 
			LED_RGBOFF
			Delay(0xFFFFF);
			LED_PURPLE
			Delay(0xFFFFF);
			LED_RGBOFF
			Delay(0xFFFFF);
			LED_PURPLE
			Delay(0xFFFFF);
			LED_RGBOFF
			Delay(0xFFFFF);
			LED_PURPLE
			Delay(0xFFFFF);
			LED_RGBOFF
			break;
		case _CYAN: 
			LED_RGBOFF
			Delay(0xFFFFF);
			LED_CYAN
			Delay(0xFFFFF);
			LED_RGBOFF
			Delay(0xFFFFF);
			LED_CYAN
			Delay(0xFFFFF);
			LED_RGBOFF
			Delay(0xFFFFF);
			LED_CYAN
			Delay(0xFFFFF);
			LED_RGBOFF
			break;
		case _WHITE: 
			LED_RGBOFF
			Delay(0xFFFFF);
			LED_WHITE
			Delay(0xFFFFF);
			LED_RGBOFF
			Delay(0xFFFFF);
			LED_WHITE
			Delay(0xFFFFF);
			LED_RGBOFF
			Delay(0xFFFFF);
			LED_WHITE
			Delay(0xFFFFF);
			LED_RGBOFF
			break;
	}

}








