#ifndef _BSP_BEEP_H
#define _BSP_BEEP_H
#include "stm32f10x.h"
#include "bsp_systick.h"


#define BEEP_GPIO_PORT    	GPIOA			              /* GPIO�˿� */
#define BEEP_GPIO_CLK 	    RCC_APB2Periph_GPIOA	/* GPIO�˿�ʱ�� */
#define BEEP_GPIO_PIN				GPIO_Pin_8

#define BEEP_OFF							GPIO_ResetBits(BEEP_GPIO_PORT, GPIO_Pin_8)

#define BEEP_ON								GPIO_SetBits(BEEP_GPIO_PORT, GPIO_Pin_8)
#define BEEP_DELAY						while(1){GPIO_SetBits(BEEP_GPIO_PORT, GPIO_Pin_8);Delay(10000);GPIO_ResetBits(BEEP_GPIO_PORT, GPIO_Pin_8);Delay(10000);}


/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //���Ϊ�ߵ�ƽ		
#define digitalLo(p,i)		 {p->BRR=i;}	 //����͵�ƽ
#define digitalToggle(p,i) {p->ODR ^=i;} //�����ת״̬


/* �������IO�ĺ� */
#define BEEP_TOGGLE		 digitalToggle(BEEP_GPIO_PORT,BEEP_GPIO_PIN)

void Beep_Config(void);
void Sound(u16 frq);
void Sound2(u16 time);
void play_successful(void);
void play_failed(void);
#endif 	/*BSP_BEEP_H*/
