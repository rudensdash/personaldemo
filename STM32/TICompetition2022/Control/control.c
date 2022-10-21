#include "control.h"


// ��ʼ����������
void Control_gpio_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	ControlPortClockCmd( ControlPortClock | OUTPUTPortClock, ENABLE );  // ʹ��ʱ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = ControlPin1;
	GPIO_Init(ControlPort, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ControlPin2;
	GPIO_Init(ControlPort, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ControlPin3;
	GPIO_Init(ControlPort, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ControlPin4;
	GPIO_Init(ControlPort, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = OUTPUT_Control_Low;
	GPIO_Init(OUTPUTPort, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = OUTPUT_Control_High;
	GPIO_Init(OUTPUTPort, &GPIO_InitStructure);
	
}

int Shift_Amp_Status(uint8_t Status){
	
	if(Status < 4){
		switch(Status){
			case 0:
				GPIO_WriteBit(GPIOB, FM_Amp_Control_Low, Bit_RESET );
				GPIO_WriteBit(GPIOB, FM_Amp_Control_High, Bit_RESET );
				break;
			case 1:
				GPIO_WriteBit(GPIOB, FM_Amp_Control_Low, Bit_SET );
				GPIO_WriteBit(GPIOB, FM_Amp_Control_High, Bit_RESET );
				break;
			case 2:
				GPIO_WriteBit(GPIOB, FM_Amp_Control_Low, Bit_RESET );
				GPIO_WriteBit(GPIOB, FM_Amp_Control_High, Bit_SET );
				break;
				
		}
//		if(Status & 0x01){
//			//GPIO_WriteBit(GPIOB, AM_Amp_Control_Low, Bit_SET ); // ��λ
//			GPIO_WriteBit(GPIOB, FM_Amp_Control_Low, Bit_SET ); // ��λ
//		}else{
//			//GPIO_WriteBit(GPIOB, AM_Amp_Control_Low, Bit_RESET ); // ��λ
//			GPIO_WriteBit(GPIOB, FM_Amp_Control_Low, Bit_RESET ); // ��λ
//		}
//		
//		if(Status & 0x02){
//			//GPIO_WriteBit(GPIOB, AM_Amp_Control_High, Bit_SET); // ��λ
//			GPIO_WriteBit(GPIOB, FM_Amp_Control_High, Bit_SET); // ��λ
//		}else{
//			//GPIO_WriteBit(GPIOB, AM_Amp_Control_High, Bit_RESET); // ��λ
//			GPIO_WriteBit(GPIOB, FM_Amp_Control_High, Bit_RESET); // ��λ
//		}
//		
//		
		
		return 1;
	}
	
	return 0;
}
	// ����ź�Դ 0 ΪAM����ź� 1 Ϊ FM ����ź� 2 Ϊԭ�ź�
int Set_OutPut(int Source){
	
	switch(Source){
		case 0:
			GPIO_WriteBit(OUTPUTPort, OUTPUT_Control_Low, Bit_RESET );  // 00
			GPIO_WriteBit(OUTPUTPort, OUTPUT_Control_High, Bit_RESET ); 
			break;
		case 1:
			GPIO_WriteBit(OUTPUTPort, OUTPUT_Control_Low, Bit_SET ); 
			GPIO_WriteBit(OUTPUTPort, OUTPUT_Control_High, Bit_RESET );  // 01
			break;
		case 2:
			GPIO_WriteBit(OUTPUTPort, OUTPUT_Control_Low, Bit_RESET );   // 10
			GPIO_WriteBit(OUTPUTPort, OUTPUT_Control_High, Bit_SET ); 
			break;
	}
	
	return 0;
}



// ------- end line ---------

