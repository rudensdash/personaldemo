#include "TIM_Capture.h"



void TIM_EventGPIO_Config(){
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void TIM4_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


// 利用 TIM4 PWM 输出模式上升沿触发ADC
void EventTimer_Init(uint16_t arr, uint16_t psc){
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    TIM_EventGPIO_Config();
    TIM_DeInit(TIM4);

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision =  TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //TIM脉宽调制输出模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 比较输出使能
    TIM_OCInitStructure.TIM_Pulse       = (arr+1)/2;
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High; // 输出极性: TIM输出比较极性低
    TIM_OC4Init(TIM4,&TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	//TIM_CtrlPWMOutputs(TIM4, ENABLE); 
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
		
	TIM4_NVIC_Configuration();
		
	TIM_Cmd(TIM4, ENABLE);										
    //TIM_Cmd(TIM4, DISABLE); // 先关闭定时器
    //
}




