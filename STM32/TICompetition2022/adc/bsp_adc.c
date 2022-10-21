#include "bsp_adc.h"
#include "TIM_Capture.h"
#include <stdio.h>

__IO uint16_t ADC_ConvertedValue;

uint8_t presetValue = 55;
uint8_t Adc_convert_over = 0;



/**
  * @brief  ADC GPIO ��ʼ��
  * @param  ��
  * @retval ��
  */
void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// �� ADC IO�˿�ʱ��
	ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE );
	
	// ���� ADC IO ����ģʽ
	// ����Ϊģ������
	GPIO_InitStructure.GPIO_Pin = ADCAM_PORT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	// ��ʼ�� ADC IO
	GPIO_Init(ADCAM_PORT, &GPIO_InitStructure);		//AM		
}

void ADC2_GPIO_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// �� ADC IO�˿�ʱ��
	ADC_GPIO_APBxClock_FUN ( RCC_APB2Periph_GPIOB, ENABLE ); // B0
	
	// ���� ADC IO ����ģʽ
	// ����Ϊģ������
	GPIO_InitStructure.GPIO_Pin = ADCFM_PORT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// ��ʼ�� ADC IO
	GPIO_Init(ADCFM_PORT, &GPIO_InitStructure);	   //FM
}

void ADC3_GPIO_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// �� ADC IO�˿�ʱ��
	ADC_GPIO_APBxClock_FUN ( RCC_APB2Periph_GPIOA, ENABLE );
	
	// ���� ADC IO ����ģʽ
	// ����Ϊģ������
	GPIO_InitStructure.GPIO_Pin = ADCBASE_PORT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// ��ʼ�� ADC IO
	GPIO_Init(ADCBASE_PORT, &GPIO_InitStructure);	   // �����ɼ�
}

#if 0
//DMA1����
void DMA1_Init()
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	  			//ʹ��DMAͨ��ʱ��

	//DMA1��ʼ��
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADCx->DR));				//ADC1��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)y_input; 		//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 				//����(�����赽�ڴ�)
	DMA_InitStructure.DMA_BufferSize = NPT; 						//�������ݵĴ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 		//�����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 			//�ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord ; //�������ݵ�λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord ;    //�ڴ����ݵ�λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular  ; 		//DMAģʽ��ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High ; 		//���ȼ�����
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;   		//��ֹ�ڴ浽�ڴ�Ĵ���
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //����DMA1

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC, ENABLE);		//ʹ�ܴ�������ж�
	
	DMA_Cmd(DMA1_Channel1,DISABLE); // �ȹر�
}
#endif
/**
  * @brief  ����ADC����ģʽ
  * @param  ��
  * @retval ��
  */
static void ADCx_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;	
	// ��ADCʱ��
	ADC_APBxClock_FUN ( ADC_CLK | RCC_APB2Periph_AFIO, ENABLE );
	
	
	
	ADC_ExternalTrigConvCmd(ADCx, DISABLE);
	// ����ADC ������ʼת��
	ADC_Cmd(ADCx, DISABLE);
	ADC_DMACmd(ADCx, DISABLE);
	
	ADC_DeInit(ADCx);
	
	//DMA1_Init();
	
	// ADC ģʽ����
	// ֻʹ��һ��ADC�����ڶ���ģʽ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// ��ֹɨ��ģʽ����ͨ����Ҫ����ͨ������Ҫ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 

	// ����ת��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;

	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // �������

	// ת������Ҷ���
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// ת��ͨ��1��
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
		
	// ��ʼ��ADC
	ADC_Init(ADCx, &ADC_InitStructure);
	
	// ����ADCʱ��ΪPCLK2��6��Ƶ����12MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
	
	// ���� ADC ͨ��ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADCx, ADC_Channel_14, 1, 
	                         ADC_SampleTime_7Cycles5);
					

	
	//ADC_ExternalTrigConvCmd(ADCx, ENABLE);
	// ����ADC ������ʼת��
	ADC_Cmd(ADCx, ENABLE);
	//ADC_DMACmd(ADCx,ENABLE);
	// ��ʼ��ADC У׼�Ĵ���  
	ADC_ResetCalibration(ADCx);
	// �ȴ�У׼�Ĵ�����ʼ�����
	while(ADC_GetResetCalibrationStatus(ADCx));
	
	// ADC��ʼУ׼
	ADC_StartCalibration(ADCx);
	// �ȴ�У׼���
	while(ADC_GetCalibrationStatus(ADCx));
	

}

static void ADC2_Config(void){
	ADC_InitTypeDef ADC_InitStructure;	
	// ��ADCʱ��
	ADC_APBxClock_FUN ( RCC_APB2Periph_ADC2 | RCC_APB2Periph_AFIO, ENABLE );
	
	
	
	ADC_ExternalTrigConvCmd(ADC2, DISABLE);
	// ����ADC ������ʼת��
	ADC_Cmd(ADC2, DISABLE);
	ADC_DMACmd(ADC2, DISABLE);
	
	ADC_DeInit(ADC2);
	
	//DMA1_Init();
	
	// ADC ģʽ����
	// ֻʹ��һ��ADC�����ڶ���ģʽ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// ��ֹɨ��ģʽ����ͨ����Ҫ����ͨ������Ҫ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 

	// ����ת��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;

	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // �������

	// ת������Ҷ���
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// ת��ͨ��1��
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
		
	// ��ʼ��ADC
	ADC_Init(ADC2, &ADC_InitStructure);
	
	// ����ADCʱ��ΪPCLK2��6��Ƶ����12MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
	
	// ���� ADC ͨ��ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC2, ADC_Channel_3, 1, 
	                         ADC_SampleTime_7Cycles5);
					

	
	//ADC_ExternalTrigConvCmd(ADC2, ENABLE);
	// ����ADC ������ʼת��
	ADC_Cmd(ADC2, ENABLE);
	//ADC_DMACmd(ADC2,ENABLE);
	// ��ʼ��ADC У׼�Ĵ���  
	ADC_ResetCalibration(ADC2);
	// �ȴ�У׼�Ĵ�����ʼ�����
	while(ADC_GetResetCalibrationStatus(ADC2));
	
	// ADC��ʼУ׼
	ADC_StartCalibration(ADC2);
	// �ȴ�У׼���
	while(ADC_GetCalibrationStatus(ADC2));
}

static void ADC3_Config(void){
	
	ADC_InitTypeDef ADC_InitStructure;	
	// ��ADCʱ��
	ADC_APBxClock_FUN ( RCC_APB2Periph_ADC3 | RCC_APB2Periph_AFIO, ENABLE );
	
	
	
	ADC_ExternalTrigConvCmd(ADC3, DISABLE);
	// ����ADC ������ʼת��
	ADC_Cmd(ADC3, DISABLE);
	ADC_DMACmd(ADC3, DISABLE);
	
	ADC_DeInit(ADC3);
	
	//DMA1_Init();
	
	// ADC ģʽ����
	// ֻʹ��һ��ADC�����ڶ���ģʽ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// ��ֹɨ��ģʽ����ͨ����Ҫ����ͨ������Ҫ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 

	// ����ת��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;

	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // �������

	// ת������Ҷ���
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// ת��ͨ��1��
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
		
	// ��ʼ��ADC
	ADC_Init(ADC3, &ADC_InitStructure);
	
	// ����ADCʱ��ΪPCLK2��6��Ƶ����12MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
	
	// ���� ADC ͨ��ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC3, ADC_Channel_2, 1, 
	                         ADC_SampleTime_7Cycles5);
					

	
	//ADC_ExternalTrigConvCmd(ADC3, ENABLE);
	// ����ADC ������ʼת��
	ADC_Cmd(ADC3, ENABLE);
	//ADC_DMACmd(ADC3,ENABLE);
	// ��ʼ��ADC У׼�Ĵ���  
	ADC_ResetCalibration(ADC3);
	// �ȴ�У׼�Ĵ�����ʼ�����
	while(ADC_GetResetCalibrationStatus(ADC3));
	
	// ADC��ʼУ׼
	ADC_StartCalibration(ADC3);
	// �ȴ�У׼���
	while(ADC_GetCalibrationStatus(ADC3));
}

/*
	* ������ʼ��
*/

void Key_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC,ENABLE);
	// K1											
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	// K2
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	  GPIO_Init(GPIOC, &GPIO_InitStructure);

}



// ����ADC1 ת������ʱ��
/*
	* @param Sampler �������ṹ��
	* @param value ������ʱ����װ��ֵ
	* @param DIV  ������ʱ��Ԥ��Ƶֵ
*/
void ADC1_TriggertimeSet(SampleParameter_t* Sampler,uint16_t value, uint16_t DIV){
	Sampler->VALUE_TIM4 = value;
	Sampler->DIV_TIM4 = DIV;
}

/*
	* ����ADC�Դ��Ĳ�������
	* ���ݴ�������ڲ��� cycles ���Զ��л�����ʱ��
	* ������������������ṹ�巽�����������ʱ��
	
	* @param Sampler �������ṹ��
	* @param cycles  ��Ҫ���õ�������
*/
void ADC1_SetSampleCycles(SampleParameter_t* Sampler ,uint8_t cycles){
	
	Sampler->SampleCycles = cycles; 
	
	switch(Sampler->SampleCycles){
		case 1:
			ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_1Cycles5); 
			break;
		case 7:
			ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_7Cycles5); 
			break;
		case 13:
			ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_13Cycles5); 
			break;
		case 28:
			ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_28Cycles5); 
			break;
		case 41:
			ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_41Cycles5); 
			break;
		case 55:
			ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5); 
			break;
		case 71:
			ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_71Cycles5); 
			break;
		case 239:
			ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_239Cycles5); 
			break;
	}
}

/*
	* �л�ADC������λ 
	* ������Ϊ��������Ĳ���Ƶ�ʣ������ж�Ȼ��ֶε���������

	@param Sampler �������ṹ��
	@param freq �ɼ�����Ƶ��
	@return �����Ƿ��л���λ Ϊ1 ���л� 0 ���л� 
*/
uint8_t ADC1_SampleShift(SampleParameter_t* Sampler, float freq ){
	
	
	uint8_t Change_Flag; // �Ƿ���Ĳ�����
	Change_Flag = 0;
	
	switch(Sampler->current_freq){
			
		case FREQ496KHZ:
			if(!((freq >= 8000) && (freq <= 124000))){  // ���ݵ�ǰ�Ĳ���Ƶ�ʺͲɼ�����Ƶ�ʷ�Χ�ж� �Ƿ񽵵�
				// ���������²ɼ�  ��λ����ѭ�������л�
				Change_Flag = 1;
				ADC1_TriggertimeSet(&(*Sampler), 24, 9); 
				Sampler->current_freq_ = 320000.0;
				Sampler->current_freq = FREQ320KHZ;   
				printf("Change to FREQ320KHZ\r\n\r");
			}
			break;
			
		case FREQ320KHZ:
			if(!((freq > 5000) && (freq <= 9000))){  // ���ݵ�ǰ�Ĳ���Ƶ�ʺͲɼ�����Ƶ�ʷ�Χ�ж� �Ƿ񽵵�
				if(freq > 9000){
					printf("Change to FREQ496KHZ\r\n\r");
					ADC1_TriggertimeSet(&(*Sampler), 144, 1);  
					Sampler->current_freq_ = 496551.72413793;
					Sampler->current_freq = FREQ496KHZ;
					Change_Flag = 1;
				}else{
					// ���������²ɼ�  ��λ����ѭ�������л�
					
					if(freq == 0){ // ԶС�� �� 80KHZ �л�
						Sampler->current_freq = FREQ80KHZ; // ����
						ADC1_TriggertimeSet(&(*Sampler), 24, 36); 
						Sampler->current_freq_ = 80000.0;
						printf("Change to FREQ80KHZ\r\n\r");
						Change_Flag = 2;
					}else{
						ADC1_TriggertimeSet(&(*Sampler), 25, 12); 
						Sampler->current_freq_ = 230769.230769;
						Sampler->current_freq = FREQ240KHZ;   
						printf("Change to FREQ240KHZ\r\n\r");
						Change_Flag = 1;
					}
					
				}
				
				
			}
			break;
			
			
		case FREQ240KHZ:  
			if(!((freq >= 3750) && (freq <= 8000))){  // ���ݵ�ǰ�Ĳ���Ƶ�ʺͲɼ�����Ƶ�ʷ�Χ�ж� �Ƿ񽵵�
				// ���������²ɼ�  ��λ����ѭ�������л�
				if(freq > 8000){
					ADC1_TriggertimeSet(&(*Sampler), 24, 9); 
					Sampler->current_freq_ = 320000.0;
					Sampler->current_freq = FREQ320KHZ;   
					printf("Change to FREQ320KHZ\r\n\r");
				}else{
					ADC1_TriggertimeSet(&(*Sampler), 225, 2); 
					Sampler->current_freq_ = 159292.035398;
					Sampler->current_freq = FREQ160KHZ;   
					printf("Change to FREQ160KHZ\r\n\r");
				}
				Change_Flag = 1;
				
			}
			break;
			

		case FREQ160KHZ:
			if(!((freq >= 2000) && (freq <= 5000))){
				if(freq > 5000){
					ADC1_TriggertimeSet(&(*Sampler), 25, 12); 
					Sampler->current_freq_ = 230769.23076900;
					Sampler->current_freq = FREQ240KHZ;   
					printf("Change to FREQ240KHZ\r\n\r");
				}else{
					Sampler->current_freq = FREQ80KHZ; // ����
					ADC1_TriggertimeSet(&(*Sampler), 24, 36); 
					Sampler->current_freq_ = 80000.0;
					printf("Change to FREQ80KHZ\r\n\r");
				}
				Change_Flag = 1;
			}
			break;
			
		case FREQ80KHZ:
			if(!((freq >= 1250) && (freq <= 2500))){
				if(freq > 2500){
					ADC1_TriggertimeSet(&(*Sampler), 225, 2); 
					Sampler->current_freq_ = 159292.03539800;
					Sampler->current_freq = FREQ160KHZ;   
					printf("Change to FREQ160KHZ\r\n\r");
					Change_Flag = 1;
				}else{
					if(freq == 0){ // ԶС�ھ͵�2560HZ����
						Sampler->current_freq = FREQ25600HZ; // ����
						ADC1_TriggertimeSet(&(*Sampler), 79, 36); 
						Sampler->current_freq_ = 25600.0;
						printf("Change to FREQ25.6KHZ\r\n\r");
						Change_Flag = 2;
					}else{
						Sampler->current_freq = FREQ64KHZ; // ����
						ADC1_TriggertimeSet(&(*Sampler), 124, 9); 
						Sampler->current_freq_ = 64000;;
						printf("Change to FREQ64KHZ\r\n\r");
						Change_Flag = 1;
					}
				}
				
				
				
				
			}
			break;
			
		case FREQ64KHZ:
			if(!((freq > 750) && (freq <= 2000))){
				if(freq > 2000){
					Sampler->current_freq = FREQ80KHZ; // ����
					ADC1_TriggertimeSet(&(*Sampler), 24, 36); 
					Sampler->current_freq_ = 80000.0;
					printf("Change to FREQ80KHZ\r\n\r");
				}else{
					Sampler->current_freq = FREQ25600HZ; // ����
					ADC1_TriggertimeSet(&(*Sampler), 77, 36); 
					Sampler->current_freq_ = 25600.0;
					printf("Change to FREQ25.6KHZ\r\n\r");
				}
				Change_Flag = 1;
			}
			break;
			
		case FREQ25600HZ:
			if(!((freq > 350) && (freq <= 800))){
				if(freq > 800){
					Sampler->current_freq = FREQ64KHZ; // ����
					ADC1_TriggertimeSet(&(*Sampler), 124, 9); 
					Sampler->current_freq_ = 64000;
					printf("Change to FREQ64KHZ\r\n\r");
					Change_Flag = 1;
				}else{
					if(freq == 0){
						Sampler->current_freq = FREQ2560HZ; // ����
						ADC1_TriggertimeSet(&(*Sampler), 3124, 9);  // 2560HZ
						Sampler->current_freq_ = 2560.0;
						printf("Change to FREQ2560HZ\r\n\r");
						Change_Flag = 2;
					}else{
						Sampler->current_freq = FREQ12800HZ; // ����
						ADC1_TriggertimeSet(&(*Sampler), 624, 9); 
						Sampler->current_freq_ = 12800.0;
						printf("Change to FREQ12800HZ\r\n\r");
						Change_Flag = 1;
					}
					
				}
				
			}
			break;
			
		case FREQ12800HZ:
			if(!((freq > 80) && (freq <= 400))){
				if(freq > 400){
					Sampler->current_freq = FREQ25600HZ; // ����
					ADC1_TriggertimeSet(&(*Sampler), 77, 36); 
					Sampler->current_freq_ = 25600.0;
					printf("Change to FREQ25.6KHZ\r\n\r");
				}else{ 
					Sampler->current_freq = FREQ2560HZ; // ����
					ADC1_TriggertimeSet(&(*Sampler), 3124, 9);  // 2560HZ
					Sampler->current_freq_ = 2560.0;
					printf("Change to FREQ2560HZ\r\n\r");
				}
				Change_Flag = 1;

			}
			break;
		
		case FREQ2560HZ: // 2.5KHZ
			if(!((freq >= 39) && (freq <= 80))){
				if(freq > 80){
					Sampler->current_freq = FREQ12800HZ; // ����
					ADC1_TriggertimeSet(&(*Sampler), 624, 9); 
					Sampler->current_freq_ = 12800.0;
					printf("Change to FREQ12800HZ\r\n\r");
					Change_Flag = 1;
				}else{
					if(freq == 0){
						Sampler->current_freq = FREQ512HZ;
						ADC1_TriggertimeSet(&(*Sampler), 15624, 9);   // 512HZ
						printf("Change to FREQ512HZ\r\n\r");
						Sampler->current_freq_ = 512.0;
						Change_Flag = 2;
					}else{
						Sampler->current_freq = FREQ1280HZ;
						ADC1_TriggertimeSet(&(*Sampler), 6250, 9);  // 1280HZ
						printf("Change to FREQ1280HZ\r\n\r");
						Sampler->current_freq_ = 1280.0;
						Change_Flag = 1;
					}
					
				}
				
			}	
			break;
			
		case FREQ1280HZ: 
			if(!((freq >= 5) && (freq <= 40))){
				if(freq > 40){
					Sampler->current_freq = FREQ2560HZ; // ����
					ADC1_TriggertimeSet(&(*Sampler), 3124, 9);  // 2560HZ
					Sampler->current_freq_ = 2560.0;
					printf("Change to FREQ2560HZ\r\n\r");
				}else{
					Sampler->current_freq = FREQ512HZ;
					ADC1_TriggertimeSet(&(*Sampler), 15624, 9);   // 512HZ
					printf("Change to FREQ512HZ\r\n\r");
					Sampler->current_freq_ = 512.0;
				}
				Change_Flag = 1;
			}	
			break;
			
		case FREQ512HZ:
			if(!((freq >= 0.9) && (freq <= 4))){
				
				printf("Change to FREQ496KHZ\r\n\r");
				ADC1_TriggertimeSet(&(*Sampler), 144, 1);  
				Sampler->current_freq_ = 496551.72413793;
				Sampler->current_freq = FREQ496KHZ;
				
				Change_Flag = 1;
			}
			break;
	}
	
	return Change_Flag;
}


#if 0
// DMA�ж� ת����ɴ���
void DMA1_Channel1_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_FLAG_TC1) != RESET){
        Adc_convert_over = 1;
    }
    DMA_ClearITPendingBit(DMA1_FLAG_TC1);
}	
#endif
/**
  * @brief  ADC��ʼ��
  * @param  ��
  * @retval ��
  */
void ADCx_Init(void)
{
	ADCx_GPIO_Config();

	ADCx_Mode_Config();
	
	Key_Config();
}

void ADC2_Init(void){
	ADC2_GPIO_Config();
	ADC2_Config();
}

void ADC3_Init(void){
	ADC3_GPIO_Config();
	ADC3_Config();
}

/*********************************************END OF FILE**********************/
