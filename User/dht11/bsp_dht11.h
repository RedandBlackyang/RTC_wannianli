#ifndef __DHT11_H
#define	__DHT11_H



#include "stm32f10x.h"



/************************** DHT11 数据类型定义********************************/
typedef struct
{
	uint8_t  humi_int;		//湿度的整数部分
	uint8_t  humi_deci;	 	//湿度的小数部分
	uint8_t  temp_int;	 	//温度的整数部分
	uint8_t  temp_deci;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和11
		                 
} DHT11_Data_TypeDef;

#define      DHT11_Dout_SCK_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define      DHT11_Dout_GPIO_CLK                       RCC_APB2Periph_GPIOE

#define      DHT11_Dout_GPIO_PORT                      GPIOE
#define      DHT11_Dout_GPIO_PIN                       GPIO_Pin_6

#define      DHT11_Dout_0	                            GPIO_ResetBits ( DHT11_Dout_GPIO_PORT, DHT11_Dout_GPIO_PIN ) 
#define      DHT11_Dout_1	                            GPIO_SetBits ( DHT11_Dout_GPIO_PORT, DHT11_Dout_GPIO_PIN ) 

#define      DHT11_Dout_IN()	                        GPIO_ReadInputDataBit ( DHT11_Dout_GPIO_PORT, DHT11_Dout_GPIO_PIN ) 


void DHT11_Init( void );
uint8_t DHT11_Read_TempAndHumidity (DHT11_Data_TypeDef * DHT11_Data );

#endif /* __DHT11_H */







