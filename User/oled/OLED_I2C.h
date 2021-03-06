#ifndef __OLED_I2C_H
#define	__OLED_I2C_H

#include "stm32f10x.h"

#define OLED_ADDRESS	0x78 //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78

/* STM32 I2C 快速模式 */
#define I2C_Speed              400000

/*I2C接口*/
#define OLED_I2C                          I2C1
#define OLED_I2C_CLK                      RCC_APB1Periph_I2C1
#define OLED_I2C_CLK_INIT								  RCC_APB1PeriphClockCmd

#define OLED_I2C_SCL_PIN                  GPIO_Pin_6                 
#define OLED_I2C_SCL_GPIO_PORT            GPIOB                       
#define OLED_I2C_SCL_GPIO_CLK             RCC_APB2Periph_GPIOB
#define OLED_I2C_SCL_SOURCE               GPIO_PinSource6
#define OLED_I2C_SCL_AF                   GPIO_AF_I2C1

#define OLED_I2C_SDA_PIN                  GPIO_Pin_7                  
#define OLED_I2C_SDA_GPIO_PORT            GPIOB                       
#define OLED_I2C_SDA_GPIO_CLK             RCC_APB2Periph_GPIOB
#define OLED_I2C_SDA_SOURCE               GPIO_PinSource7
#define OLED_I2C_SDA_AF                   GPIO_AF_I2C1


void I2C_Configuration(void);
void I2C_WriteByte(uint8_t addr,uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);

void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void OLED_DispChar_CH ( uint16_t x, uint16_t y, uint16_t usCh);			//显示一个汉字,usCh为汉字的两个字节
void OLED_DisString_CH (uint16_t x, uint16_t y, char * pStr);				//显示 汉字字符串;

void OLED_Show_Numphoto(unsigned char x, unsigned char y, unsigned char N);
void OLED_Show_ClockPots(unsigned char x, unsigned char y);
void OLED_Clr_ClockPots(unsigned char x, unsigned char y);
void SetPointBuffer(int x,int y,int value);
void ClearScreenBuffer(unsigned char val);
void DrawLine(int x1,int y1,int x2,int y2);
void DrawCircle ( int usX_Center, int usY_Center, int usRadius);
void DrawRect1(int left,int top,int right,int bottom);
void OlED_Fill_Buffer(uint8_t BMP[]);
void UpdateScreenBuffer(void);
void Draw_CircleClock(int hour,int min ,int sec);

#endif
