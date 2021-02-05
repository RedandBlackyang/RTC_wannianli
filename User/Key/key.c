#include "key.h"
static void NVIC_config(void)
{
	NVIC_InitTypeDef   NVIC_InitStruct;                        //NVIC���ýṹ��
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);             //����Ϊ��1
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;               //ѡ���ж���
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;      //�����ȼ�Ϊ0
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;             //�����ȼ�Ϊ1
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;                //ʹ��NVIC
	
	NVIC_Init(&NVIC_InitStruct); 
	
}

void KEY_GPIO_Config(void)
{		
		//����ṹ��
		GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef  EXTI_InitStruct;
		
		NVIC_config();
		
		
		//������ص�GPIOC����ʱ��
		RCC_APB2PeriphClockCmd( KEY_GPIO_CLK, ENABLE); 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		
		GPIO_InitStructure.GPIO_Pin = C1_GPIO_PIN;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
		GPIO_Init(C1_GPIO_PORT, &GPIO_InitStructure);	
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource6);   //ѡ���ⲿ�ж�Դ
	  EXTI_InitStruct.EXTI_Line    = EXTI_Line6;                //
    EXTI_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt;       //����Ϊ�ж�
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;     
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	  EXTI_Init(&EXTI_InitStruct);                              
		GPIO_InitStructure.GPIO_Pin = C2_GPIO_PIN;
		GPIO_Init(C2_GPIO_PORT, &GPIO_InitStructure);		
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource7);   //ѡ���ⲿ�ж�Դ
	  EXTI_InitStruct.EXTI_Line    = EXTI_Line7; 
		EXTI_Init(&EXTI_InitStruct);                             
		GPIO_InitStructure.GPIO_Pin = C3_GPIO_PIN;
		GPIO_Init(C3_GPIO_PORT, &GPIO_InitStructure);	
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource8);   //ѡ���ⲿ�ж�Դ
	  EXTI_InitStruct.EXTI_Line    = EXTI_Line8; 
		EXTI_Init(&EXTI_InitStruct);                              //��ʼ������
		GPIO_InitStructure.GPIO_Pin = C4_GPIO_PIN;
		GPIO_Init(C4_GPIO_PORT, &GPIO_InitStructure);
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);   //ѡ���ⲿ�ж�Դ
	  EXTI_InitStruct.EXTI_Line    = EXTI_Line9; 
		EXTI_Init(&EXTI_InitStruct);                              //��ʼ������


		//��������ģʽΪͨ���������
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		//��ʼ����1
		GPIO_InitStructure.GPIO_Pin = R1_GPIO_PIN;
		GPIO_Init(R1_GPIO_PORT, &GPIO_InitStructure);
		GPIO_ResetBits ( R1_GPIO_PORT, R1_GPIO_PIN );



}

