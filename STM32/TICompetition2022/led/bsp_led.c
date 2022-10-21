/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ledӦ�ú����ӿ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-ָ���� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "bsp_led.h"   


static void Delay ( __IO uint32_t nCount )
{
  for ( ; nCount != 0; nCount -- );
	
}

 /**
  * @brief  ��ʼ������LED��IO
  * @param  ��
  * @retval ��
  */
void LED_GPIO_Config(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����LED��ص�GPIO����ʱ��*/
		RCC_APB2PeriphClockCmd( LED1_GPIO_CLK | LED2_GPIO_CLK | LED3_GPIO_CLK, ENABLE);
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;	

		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	
		
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;

		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);
		
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;

		/*���ÿ⺯������ʼ��GPIOF*/
		GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);

		/* �ر�����led��	*/
		GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
		
		/* �ر�����led��	*/
		GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);	 
    
    /* �ر�����led��	*/
		GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
}

/*********************************************END OF FILE**********************/

//������������� �����������ķ�����
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








