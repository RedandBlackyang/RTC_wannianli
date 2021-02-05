#include "./systick/bsp_SysTick.h"

void Delay_s(unsigned int ms)
{ 
	unsigned char i;
	for(i=0;i<ms;ms--)
		{
		Delay_ms(1000);
		}
}

void Delay_us (uint32_t us)
{
	uint32_t i;
	SysTick_Config(72);  //һ�μ���ѭ����ʱ��
	for(i=0;i<us;i++)
	{
		while( !((SysTick->CTRL) & (1<<16)));						//��ȡcounter���������1����ȡ����0  ���Ƽ�״̬�Ĵ���
		
	}
	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;//������ɺ�ֹͣ����

}

void Delay_ms (uint32_t ms)
{
	uint32_t i;
	SysTick_Config(72000);  //һ�μ���ѭ����ʱ��
	for(i=0;i<ms;i++)
	{
		while( !((SysTick->CTRL) & (1<<16)));						//��ȡcounter���������1����ȡ����0  ���Ƽ�״̬�Ĵ���
		
	}
	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;//������ɺ�ֹͣ����

}

