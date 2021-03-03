/**
  ******************************************************************************
  * @file    OLED_I2C.c
  * @author  fire
  * @version V1.0
  * @date    2014-xx-xx
  * @brief   128*64�����OLED��ʾ�������ļ�����������SD1306����IICͨ�ŷ�ʽ��ʾ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-ָ���� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
	
	* Function List:
	*	1. void I2C_Configuration(void) -- ����CPU��Ӳ��I2C
	* 2. void I2C_WriteByte(uint8_t addr,uint8_t data) -- ��Ĵ�����ַдһ��byte������
	* 3. void WriteCmd(unsigned char I2C_Command) -- д����
	* 4. void WriteDat(unsigned char I2C_Data) -- д����
	* 5. void OLED_Init(void) -- OLED����ʼ��
	* 6. void OLED_SetPos(unsigned char x, unsigned char y) -- ������ʼ������
	* 7. void OLED_Fill(unsigned char fill_Data) -- ȫ�����
	* 8. void OLED_CLS(void) -- ����
	* 9. void OLED_ON(void) -- ����
	* 10. void OLED_OFF(void) -- ˯��
	* 11. void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize) -- ��ʾ�ַ���(�����С��6*8��8*16����)
	* 12. void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N) -- ��ʾ����(������Ҫ��ȡģ��Ȼ��ŵ�codetab.h��)
	* 13. void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]) -- BMPͼƬ
	*
  *
  ******************************************************************************
  */ 




#include "OLED_I2C.h"
#include "codetab.h"
#include "./systick/bsp_SysTick.h"
#include "./font/fonts.h"
#include <math.h>
#include <string.h>

#define SCREEN_PAGE_NUM		8
#define SCREEN_ROW				64
#define SCREEN_COLUMN			128
#define pi 3.1415926535

uint8_t ScreenBuffer[SCREEN_PAGE_NUM][SCREEN_COLUMN]={0};



 /**
  * @brief  I2C_Configuration����ʼ��Ӳ��IIC����
  * @param  ��
  * @retval ��
  */
void I2C_Configuration(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure; 

	/*I2C1����ʱ��ʹ�� */
	OLED_I2C_CLK_INIT(OLED_I2C_CLK,ENABLE);
	
	/*I2C1����GPIOʱ��ʹ�� */
	RCC_APB2PeriphClockCmd(OLED_I2C_SCL_GPIO_CLK | OLED_I2C_SDA_GPIO_CLK,ENABLE);

	 /* I2C_SCL��I2C_SDA*/
  GPIO_InitStructure.GPIO_Pin = OLED_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // ��©���
  GPIO_Init(OLED_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = OLED_I2C_SDA_PIN;
  GPIO_Init(OLED_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);	
	
	
	/* I2C ���� */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;	
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;	/* �ߵ�ƽ�����ȶ����͵�ƽ���ݱ仯 SCL ʱ���ߵ�ռ�ձ� */
  I2C_InitStructure.I2C_OwnAddress1 =OLED_ADDRESS;    //������I2C��ַ
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;	
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	/* I2C��Ѱַģʽ */
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;	                            /* ͨ������ */
  
  I2C_Init(OLED_I2C, &I2C_InitStructure);	                                      /* I2C1 ��ʼ�� */
	I2C_Cmd(OLED_I2C, ENABLE);  	                                                /* ʹ�� I2C1 */
	
}


 /**
  * @brief  I2C_WriteByte����OLED�Ĵ�����ַдһ��byte������
  * @param  addr���Ĵ�����ַ
	*					data��Ҫд�������
  * @retval ��
  */
void I2C_WriteByte(uint8_t addr,uint8_t data)
{
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(I2C1, ENABLE);//����I2C1
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

	I2C_Send7bitAddress(I2C1, OLED_ADDRESS, I2C_Direction_Transmitter);//������ַ -- Ĭ��0x78
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C1, addr);//�Ĵ�����ַ
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_SendData(I2C1, data);//��������
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTOP(I2C1, ENABLE);//�ر�I2C1����
}


 /**
  * @brief  WriteCmd����OLEDд������
  * @param  I2C_Command���������
  * @retval ��
  */
void WriteCmd(unsigned char I2C_Command)//д����
{
	I2C_WriteByte(0x00, I2C_Command);
}


 /**
  * @brief  WriteDat����OLEDд������
  * @param  I2C_Data������
  * @retval ��
  */
void WriteDat(unsigned char I2C_Data)//д����
{
	I2C_WriteByte(0x40, I2C_Data);
}


 /**
  * @brief  OLED_Init����ʼ��OLED
  * @param  ��
  * @retval ��
  */
void OLED_Init(void)
{
	Delay_s(1);		// 1s,�������ʱ����Ҫ,�ϵ����ʱ��û�д�����������
	
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //���ȵ��� 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
}


 /**
  * @brief  OLED_SetPos�����ù��
  * @param  x,���xλ��
	*					y�����yλ��
  * @retval ��
  */
void OLED_SetPos(unsigned char x, unsigned char y) //������ʼ������
{ 
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x00);
}

 /**
  * @brief  OLED_Fill�����������Ļ
  * @param  fill_Data:Ҫ��������
	* @retval ��
  */
void OLED_Fill(unsigned char fill_Data)//ȫ�����
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
			{
				WriteDat(fill_Data);
			}
	}
}

 /**
  * @brief  OLED_CLS������
  * @param  ��
	* @retval ��
  */
void OLED_CLS(void)//����
{
	OLED_Fill(0x00);
}


 /**
  * @brief  OLED_ON����OLED�������л���
  * @param  ��
	* @retval ��
  */
void OLED_ON(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X14);  //������ɱ�
	WriteCmd(0XAF);  //OLED����
}


 /**
  * @brief  OLED_OFF����OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
  * @param  ��
	* @retval ��
  */
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X10);  //�رյ�ɱ�
	WriteCmd(0XAE);  //OLED����
}


 /**
  * @brief  OLED_ShowStr����ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
  * @param  x,y : ��ʼ������(x:0~127, y:0~7);
	*					ch[] :- Ҫ��ʾ���ַ���; 
	*					TextSize : �ַ���С(1:6*8 ; 2:8*16)
	* @retval ��
  */
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}

 /**
  * @brief  OLED_ShowCN����ʾcodetab.h�еĺ���,16*16����
  * @param  x,y: ��ʼ������(x:0~127, y:0~7); 
	*					N:������codetab.h�е�����
	* @retval ��
  */
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int  adder=32*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
}
void OLED_Show_Numphoto(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int  adder=72*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 24;wm++)
	{
		WriteDat(Nun_photo[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 24;wm++)
	{
		WriteDat(Nun_photo[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 2);
	for(wm = 0;wm < 24;wm++)
	{
		WriteDat(Nun_photo[adder]);
		adder += 1;
	}

}

void OLED_Show_ClockPots(unsigned char x, unsigned char y)
{
	unsigned char wm=0;
	unsigned int  adder=0;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 12;wm++)
	{
		WriteDat(ClockPots[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 12;wm++)
	{
		WriteDat(ClockPots[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 2);
	for(wm = 0;wm < 12;wm++)
	{
		WriteDat(ClockPots[adder]);
		adder += 1;
	}
}
void OLED_Clr_ClockPots(unsigned char x, unsigned char y)
{
	unsigned char wm=0;
	unsigned int  adder=0;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 12;wm++)
	{
		WriteDat(0x00);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 12;wm++)
	{
		WriteDat(0x00);
		adder += 1;
	}
	OLED_SetPos(x,y + 2);
	for(wm = 0;wm < 12;wm++)
	{
		WriteDat(0x00);
		adder += 1;
	}
}
void OLED_DispChar_CH ( uint16_t x, uint16_t y, uint16_t usCh)
{
	unsigned char wm=0;
	uint8_t ucBuffer[32];		
	//ȡ��ģ����  
  GetGBKCode ( ucBuffer, usCh );	
	
	OLED_SetPos(x , y);
	for(wm = 0;wm <16;wm++)
	{
		WriteDat(ucBuffer[wm*2]);
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(ucBuffer[wm*2+1]);
	}	
}

void OLED_DisString_CH (uint16_t x, uint16_t y, char * pStr)
{
	uint16_t usCh;
	while(*pStr != '\0')
	{
		usCh=*(uint16_t *)pStr;
		usCh=(usCh<<8)+(usCh>>8);		//��С��ת��
		OLED_DispChar_CH(x,y,usCh);
		x+=16;
		pStr+=2;
	}
}

 /**
  * @brief  OLED_DrawBMP����ʾBMPλͼ
  * @param  x0,y0 :��ʼ������(x0:0~127, y0:0~7);
	*					x1,y1 : ���Խ���(������)������(x1:1~128,y1:1~8)
	* @retval ��
  */
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
		{
			WriteDat(BMP[j++]);
		}
	}
}


void SetPointBuffer(int x,int y,int value)
{
	if(x>SCREEN_COLUMN-1 ||y>SCREEN_ROW-1)   //������Χ
		return;
	
	if(value)
		ScreenBuffer[y/SCREEN_PAGE_NUM][x] |= 1<<(y%SCREEN_PAGE_NUM);
	else
		ScreenBuffer[y/SCREEN_PAGE_NUM][x] &= ~(1<<(y%SCREEN_PAGE_NUM));	

}

void ClearScreenBuffer(unsigned char val)
{
	memset(ScreenBuffer,val,sizeof(ScreenBuffer));
}

void DrawLine(int x1,int y1,int x2,int y2)
{
	unsigned short us; 
	unsigned short usX_Current, usY_Current;
	
	int lError_X = 0, lError_Y = 0, lDelta_X, lDelta_Y, lDistance; 
	int lIncrease_X, lIncrease_Y; 	
	
	lDelta_X = x2 - x1; //������������ 
	lDelta_Y = y2 - y1; 
	
	usX_Current = x1; 
	usY_Current = y1; 
	
	
	if ( lDelta_X > 0 ) 
		lIncrease_X = 1; //���õ������� 
	
	else if ( lDelta_X == 0 ) 
		lIncrease_X = 0;//��ֱ�� 
	
	else 
  { 
    lIncrease_X = -1;
    lDelta_X = - lDelta_X;
  } 

	
	if ( lDelta_Y > 0 )
		lIncrease_Y = 1; 
	
	else if ( lDelta_Y == 0 )
		lIncrease_Y = 0;//ˮƽ�� 
	
	else 
  {
    lIncrease_Y = -1;
    lDelta_Y = - lDelta_Y;
  } 

	
	if (  lDelta_X > lDelta_Y )
		lDistance = lDelta_X; //ѡȡ�������������� 
	
	else 
		lDistance = lDelta_Y; 
	
	for ( us = 0; us <= lDistance + 1; us ++ )//������� 
	{  	
		SetPointBuffer(usX_Current,usY_Current,1);//���� 
		lError_X += lDelta_X ; 
		lError_Y += lDelta_Y ; 
		
		if ( lError_X > lDistance ) 
		{ 
			lError_X -= lDistance; 
			usX_Current += lIncrease_X; 
		}  
		
		if ( lError_Y > lDistance ) 
		{ 
			lError_Y -= lDistance; 
			usY_Current += lIncrease_Y; 
		} 		
	}  
}

void DrawCircle ( int usX_Center, int usY_Center, int usRadius)
{
	short sCurrentX, sCurrentY;
	short sError;
	sCurrentX = 0; sCurrentY = usRadius;	  
	sError = 3 - ( usRadius << 1 );     //�ж��¸���λ�õı�־
	while ( sCurrentX <= sCurrentY )
	{      
			SetPointBuffer ( usX_Center + sCurrentX, usY_Center + sCurrentY	,1);             //1���о�����
			SetPointBuffer ( usX_Center - sCurrentX, usY_Center + sCurrentY ,1);             //2      
			SetPointBuffer ( usX_Center - sCurrentY, usY_Center + sCurrentX ,1);             //3
			SetPointBuffer ( usX_Center - sCurrentY, usY_Center - sCurrentX ,1);             //4
			SetPointBuffer ( usX_Center - sCurrentX, usY_Center - sCurrentY ,1);             //5       
			SetPointBuffer ( usX_Center + sCurrentX, usY_Center - sCurrentY ,1);             //6
			SetPointBuffer ( usX_Center + sCurrentY, usY_Center - sCurrentX ,1);             //7 
			SetPointBuffer ( usX_Center + sCurrentY, usY_Center + sCurrentX ,1);             //0
			sCurrentX ++;		
			if ( sError < 0 ) 
				sError += 4 * sCurrentX + 6;	  
			else
			{
				sError += 10 + 4 * ( sCurrentX - sCurrentY );   
				sCurrentY --;
			} 		
	}
}

void DrawRect1(int left,int top,int right,int bottom)
{
		DrawLine ( left, top, right, top );
		DrawLine ( left, bottom , right , bottom );
		DrawLine ( left, top, left, bottom );
		DrawLine ( right , top, right , bottom );	
}

void OlED_Fill_Buffer(uint8_t BMP[])
{
		unsigned int n;
		unsigned char *p;
		p=BMP;
		WriteCmd(0xb0);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		while(I2C_GetFlagStatus(OLED_I2C, I2C_FLAG_BUSY));	
		I2C_GenerateSTART(I2C1, ENABLE);//����I2C1
		while(!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/
		I2C_Send7bitAddress(OLED_I2C, OLED_ADDRESS, I2C_Direction_Transmitter);//������ַ -- Ĭ��0x78
		while(!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
		I2C_SendData(OLED_I2C, 0x40);//�Ĵ�����ַ
		while (!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));	
		for(n=0;n<128*8;n++)
		{
			I2C_SendData(OLED_I2C, *p++);//��������
			while (!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		}	
		I2C_GenerateSTOP(OLED_I2C, ENABLE);//�ر�I2C1����
	
}

void UpdateScreenBuffer(void)
{
	OlED_Fill_Buffer(ScreenBuffer[0]);
}





void Draw_CircleClock(int hour,int min ,int sec)
{
	int x,y,r;
	x=63,y=36,r=27;	
	DrawCircle( x,y,r);
	//ʱ��
	DrawLine(x,y,x+(r-10)*sin(pi/6*(hour%12)),y-(r-10)*cos(pi/6*(hour%12)));
	//����
	DrawLine(x,y,x+(r-5)*sin(pi/30*(min%60)),y-(r-5)*cos(pi/30*(min%60)));
	//����
	DrawLine(x,y,x+r*sin(pi/30*(sec%60)),y-r*cos(pi/30*(sec%60)));
	UpdateScreenBuffer();
	ClearScreenBuffer(0);
}


