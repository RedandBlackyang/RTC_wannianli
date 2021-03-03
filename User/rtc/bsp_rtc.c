#include "./usart/bsp_usart.h"
#include "./rtc/bsp_rtc.h"
#include "OLED_I2C.h"
#include "./dht11/bsp_dht11.h"
#include "stdio.h"
extern uint8_t str_day[15];
extern uint8_t alarm_flag,stopwatch_flag;
extern uint8_t str_time[15];
extern uint8_t str_canderday[50];
extern uint8_t str_wday[15];
extern uint8_t str_zodiac_sign[15];
extern uint8_t str_time_set[15];
extern uint8_t str_day_set[15];
extern uint8_t str_alarm_time[15];
extern uint8_t str_stop_time[15];
extern uint8_t str_tempandhumidity[20];
extern struct rtc_time time_set;
extern struct rtc_time systmtime;
extern struct rtc_time alarm_time;
extern struct rtc_time stopwatch_time;
extern DHT11_Data_TypeDef dht11_data;

/*���ڣ���Ф������ASCII��*/
char const *WEEK_STR[] = {"��", "һ", "��", "��", "��", "��", "��"};
char const *zodiac_sign[] = {"��", "��", "ţ", "��", "��", "��", "��", "��", "��", "��", "��", "��"};

///*Ӣ�ģ����ڣ���Ф������ASCII��*/
//char const *en_WEEK_STR[] = { "Sunday","Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//char const *en_zodiac_sign[] = {"Pig", "Rat", "Ox", "Tiger", "Rabbit", "Dragon", "Snake", "Horse", "Goat", "Monkey", "Rooster", "Dog"};
void RTC_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



void RTC_CheckAndConfig(struct rtc_time *tm)
{
   	/*������ʱ��鱸�ݼĴ���BKP_DR1��������ݲ���0xA5A5,
	  ������������ʱ�䲢ѯ���û�����ʱ��*/
	if (BKP_ReadBackupRegister(RTC_BKP_DRX) != RTC_BKP_DATA)
	{
		/* ʹ��tm��ʱ������RTC�Ĵ��� */
		Time_Adjust(tm);
		
		/*��BKP_DR1�Ĵ���д���־��˵��RTC��������*/
		BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
	}
	else
	{
		
		/* ʹ�� PWR �� Backup ʱ�� */
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
		/* ������� Backup ���� */
	  PWR_BackupAccessCmd(ENABLE);
		
	  /*LSE��������������ʱ��*/
		
#ifdef RTC_CLOCK_SOURCE_LSI		
			/* ʹ�� LSI */
			RCC_LSICmd(ENABLE);

			/* �ȴ� LSI ׼���� */
			while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
			{}
#endif

		/*����Ƿ��������*/
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
		    printf("\r\n\r\n Power On Reset occurred....");
		}
		/*����Ƿ�Reset��λ*/
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
			printf("\r\n\r\n External Reset occurred....");
		}
	
		printf("\r\n No need to configure RTC....");
		
		/*�ȴ��Ĵ���ͬ��*/
		RTC_WaitForSynchro();
		
		/*����RTC���ж�*/
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		
		/*�ȴ��ϴ�RTC�Ĵ���д�������*/
		RTC_WaitForLastTask();
	}
	   /*������ʱ������꣬������У��ʱ�������PC13*/
	#ifdef RTCClockOutput_Enable
	/* ʹ�� PWR �� Backup ʱ�� */
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	/* ������� Backup ���� */
	  PWR_BackupAccessCmd(ENABLE);
	
	  /* ��ֹ Tamper ���� */
	  /* Ҫ��� RTCCLK/64 �� Tamper ����,  tamper ���ܱ����ֹ */	
	  BKP_TamperPinCmd(DISABLE); 
	
	  /* ʹ�� RTC ʱ������� Tamper ���� */
	  BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
	#endif
	
	  /* �����λ��־ flags */
	  RCC_ClearFlag();

}


void RTC_Configuration(void)
{
	/* ʹ�� PWR �� Backup ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	/* ������� Backup ���� */
	PWR_BackupAccessCmd(ENABLE);
	
	/* ��λ Backup ���� */
	BKP_DeInit();
	
//ʹ���ⲿʱ�ӻ����ڲ�ʱ�ӣ���bsp_rtc.h�ļ����壩	
//ʹ���ⲿʱ��ʱ������Щ����¾�������
//������Ʒ��ʱ�򣬺����׳����ⲿ����������������̫�ɿ�	
#ifdef 	RTC_CLOCK_SOURCE_LSE
	/* ʹ�� LSE */
	RCC_LSEConfig(RCC_LSE_ON);
	
	/* �ȴ� LSE ׼���� */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{}
	
	/* ѡ�� LSE ��Ϊ RTC ʱ��Դ */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	/* ʹ�� RTC ʱ�� */
	RCC_RTCCLKCmd(ENABLE);
	
	/* �ȴ� RTC �Ĵ��� ͬ��
	 * ��ΪRTCʱ���ǵ��ٵģ��ڻ�ʱ���Ǹ��ٵģ�����Ҫͬ��
	 */
	RTC_WaitForSynchro();
	
	/* ȷ����һ�� RTC �Ĳ������ */
	RTC_WaitForLastTask();
	
	/* ʹ�� RTC ���ж� */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	/* ȷ����һ�� RTC �Ĳ������ */
	RTC_WaitForLastTask();
	
	/* ���� RTC ��Ƶ: ʹ RTC ����Ϊ1s  */
	/* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) = 1HZ */
	RTC_SetPrescaler(32767); 
	
	/* ȷ����һ�� RTC �Ĳ������ */
	RTC_WaitForLastTask();
	
#else

	/* ʹ�� LSI */
	RCC_LSICmd(ENABLE);

	/* �ȴ� LSI ׼���� */
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{}
	
	/* ѡ�� LSI ��Ϊ RTC ʱ��Դ */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	
	/* ʹ�� RTC ʱ�� */
	RCC_RTCCLKCmd(ENABLE);
	
	/* �ȴ� RTC �Ĵ��� ͬ��
	 * ��ΪRTCʱ���ǵ��ٵģ��ڻ�ʱ���Ǹ��ٵģ�����Ҫͬ��
	 */
	RTC_WaitForSynchro();
	
	/* ȷ����һ�� RTC �Ĳ������ */
	RTC_WaitForLastTask();
	
	/* ʹ�� RTC ���ж� */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	/* ȷ����һ�� RTC �Ĳ������ */
	RTC_WaitForLastTask();
	
	/* ���� RTC ��Ƶ: ʹ RTC ����Ϊ1s ,LSIԼΪ40KHz */
	/* RTC period = RTCCLK/RTC_PR = (40 KHz)/(40000-1+1) = 1HZ */	
	RTC_SetPrescaler(40000-1); 
	
	/* ȷ����һ�� RTC �Ĳ������ */
	RTC_WaitForLastTask();
#endif
	
}


void Time_Regulate_Get(struct rtc_time *tm)
{
	  uint32_t temp_num = 0;
		uint8_t day_max=0 ;

	  printf("\r\n=========================����ʱ��==================");
		
	  do 
	  {
			printf("\r\n  ���������(Please Set Years),��Χ[1970~2038]�������ַ�����ӻس�:");
			scanf("%d",&temp_num);
			if(temp_num <1970 || temp_num >2038)
			{
				printf("\r\n �������������:%d��������Ҫ��",temp_num);
					  
			}
			else
			{	  
				printf("\n\r  ��ݱ�����Ϊ: %d\n\r", temp_num);

				tm->tm_year = temp_num;
				break;
			}
	  }while(1);


	 do 
	  {
			printf("\r\n  �������·�(Please Set Months):��Χ[1~12]�������ַ�����ӻس�:");
			scanf("%d",&temp_num);
			if(temp_num <1 || temp_num >12)
			{
				printf("\r\n �������������:%d��������Ҫ��",temp_num);
					  
			}
			else
			{	  
				printf("\n\r  �·ݱ�����Ϊ: %d\n\r", temp_num);

				tm->tm_mon = temp_num;
				break;
			}
	  }while(1);
		
		/*�����·ݼ����������*/
		switch(tm->tm_mon)
			{
				case 1:
				case 3:
				case 5:
				case 7:
				case 8:
				case 10:
				case 12:					
						day_max = 31;
					break;
				
				case 4:
				case 6:
				case 9:
				case 11:
						day_max = 30;
					break;
				
				case 2:					
				     /*��������*/
						if((tm->tm_year%4==0) &&
							 ((tm->tm_year%100!=0) || (tm->tm_year%400==0)) &&
							 (tm->tm_mon>2)) 
								{
									day_max = 29;
								} else 
								{
									day_max = 28;
								}
					break;			
			}

		do 
	  {				
			printf("\r\n  ����������(Please Set Months),��Χ[1~%d]�������ַ�����ӻس�:",day_max);
			scanf("%d",&temp_num);
			
			if(temp_num <1 || temp_num >day_max)
			{
				printf("\r\n �������������:%d��������Ҫ��",temp_num);
			}
			else
			{
				printf("\n\r  ���ڱ�����Ϊ: %d\n\r", temp_num);

				tm->tm_mday = temp_num;
				break;
			}
	  }while(1);
		
		do 
	  {				
			printf("\r\n  ������ʱ��(Please Set Hours),��Χ[0~23]�������ַ�����ӻس�:");
			scanf("%d",&temp_num);
			
			if( temp_num >23)
			{
				printf("\r\n �������������:%d��������Ҫ��",temp_num);
			}
			else
			{
				printf("\n\r  ʱ�ӱ�����Ϊ: %d\n\r", temp_num);

				tm->tm_hour = temp_num;
				break;
			}
	  }while(1);

		do 
	  {				
			printf("\r\n  ���������(Please Set Minutes),��Χ[0~59]�������ַ�����ӻس�:");
			scanf("%d",&temp_num);
			
			if( temp_num >59)
			{
				printf("\r\n �������������:%d��������Ҫ��",temp_num);
			}
			else
			{
				printf("\n\r  ���ӱ�����Ϊ: %d\n\r", temp_num);

				tm->tm_min = temp_num;
				break;
			}
	  }while(1);

		do 
	  {				
			printf("\r\n  ����������(Please Set Seconds),��Χ[0~59]�������ַ�����ӻس�:");
			scanf("%d",&temp_num);
			
			if( temp_num >59)
			{
				printf("\r\n �������������:%d��������Ҫ��",temp_num);
			}
			else
			{
				printf("\n\r  ���ӱ�����Ϊ: %d\n\r", temp_num);

				tm->tm_sec = temp_num;
				break;
			}
	  }while(1);

}

void Time_Adjust(struct rtc_time *tm)
{
	
			/* RTC ���� */
		RTC_Configuration();

	  /* �ȴ�ȷ����һ�β������ */
	  RTC_WaitForLastTask();
		  
	  /* �������� */
	  GregorianDay(tm);

	  /* �����ڼ���ʱ�����д�뵽RTC�����Ĵ��� */
	  RTC_SetCounter(mktimev(tm)-TIME_ZOOM);

	  /* �ȴ�ȷ����һ�β������ */
	  RTC_WaitForLastTask();
}
void Alarm_bkp_Read(void)
{
	uint16_t temp;
	temp=BKP_ReadBackupRegister(BKP_DR2);
	alarm_time.tm_hour=(temp)>>8;
	alarm_time.tm_min=(temp&0x0f);
	alarm_flag=BKP_ReadBackupRegister(BKP_DR3);
}

void Alarm_bkp_Write(void)
{	
	BKP_WriteBackupRegister(BKP_DR2,(alarm_time.tm_hour<<8)|(alarm_time.tm_min&0x0f));
//	BKP_WriteBackupRegister(BKP_DR2,(alarm_time.tm_hour<<8)|(alarm_time.tm_min&0x0f));
	BKP_WriteBackupRegister(BKP_DR3,alarm_flag);
}

void Time_Display(uint32_t TimeVar,struct rtc_time *tm)
{
	   static uint32_t FirstDisplay = 1;
	   uint32_t BJ_TimeVar;
	   uint8_t str[200]; // �ַ����ݴ�  	
	   /*  �ѱ�׼ʱ��ת��Ϊ����ʱ��*/
	   BJ_TimeVar =TimeVar + TIME_ZOOM;

	   to_tm(BJ_TimeVar, tm);/*�Ѷ�ʱ����ֵת��Ϊ����ʱ��*/	
	
	  if((!tm->tm_hour && !tm->tm_min && !tm->tm_sec)  || (FirstDisplay))
	  {
	      
	      GetChinaCalendar((u16)tm->tm_year, (u8)tm->tm_mon, (u8)tm->tm_mday, str);	
	      FirstDisplay = 0;
	  }	 	  	
		sprintf((char*)str_day,"%0.4d-%0.2d-%0.2d", tm->tm_year, tm->tm_mon, tm->tm_mday);
		str_day[12]=0;
		sprintf((char*)str_time,"%0.2d:%0.2d:%0.2d",tm->tm_hour, tm->tm_min, tm->tm_sec);
}
void oled_page_display(uint8_t page)
{
	if(page==0)
	{
		OLED_ShowStr(30, 0, str_day, 1);
		OLED_Show_Numphoto(0,2,systmtime.tm_hour/10);
		OLED_Show_Numphoto(26,2,systmtime.tm_hour%10);
		OLED_Show_ClockPots(56, 2);
		OLED_Show_Numphoto(74,2,systmtime.tm_min/10);
		OLED_Show_Numphoto(100,2,systmtime.tm_min%10);
		OLED_DisString_CH(0, 6, "�˵�");OLED_DisString_CH(90, 6, "ũ��");
	}
	if(page==1)
	{
		GetChinaCalendarStr((u16)systmtime.tm_year,(u8)systmtime.tm_mon,(u8)systmtime.tm_mday,str_canderday);
		OLED_DisString_CH(0,2,(char *)str_canderday);
		
		sprintf((char*)str_wday,"����%s",WEEK_STR[systmtime.tm_wday]);								//����
		OLED_DisString_CH(0,4,(char *)str_wday);

		sprintf((char*)str_zodiac_sign,"%s��",zodiac_sign[systmtime.tm_year%12-3]);		//����Ϊ��Ӧ��ũ�����
		OLED_DisString_CH(80,4,(char *)str_zodiac_sign);
		
		OLED_DisString_CH(0,0,"ʪ");
		sprintf((char*)str_tempandhumidity,":%0.2d.%0.2d",dht11_data.humi_int,dht11_data.humi_deci);
		OLED_ShowStr(16, 0, str_tempandhumidity, 1);
		
		OLED_DisString_CH(60,0,"��");
		sprintf((char*)str_tempandhumidity,":%0.2d.%0.2d",dht11_data.temp_int,dht11_data.temp_deci);
		OLED_ShowStr(76, 0, str_tempandhumidity, 1);
		
		OLED_DisString_CH(0, 6, "�˵�");OLED_DisString_CH(90, 6, "����");
	}
	if(page==2)
	{
		OLED_DisString_CH(0, 0, "У׼");
		OLED_DisString_CH(0 , 2,"����");
		OLED_DisString_CH(0 , 4,"��ʱ��");
		OLED_DisString_CH(0 , 6,"ͼ��ʱ��");
	}
	if(page==3)
	{
		OLED_DisString_CH(0, 0, "ʱ��У׼");
		OLED_DisString_CH(0 , 2,"����У׼");
	
	}
	if(page==4)
	{
		sprintf((char*)str_time_set,"%0.2d:%0.2d:%0.2d",time_set.tm_hour, time_set.tm_min, time_set.tm_sec);
		OLED_ShowStr(35 , 3, str_time_set, 1);
		OLED_ShowStr(0, 7,"+", 1);OLED_ShowStr(40, 7, "-", 1);OLED_DisString_CH(70, 6, "��һ��");
	}
	if(page==5)
	{
		sprintf((char*)str_day_set,"%0.4d-%0.2d-%0.2d", time_set.tm_year, time_set.tm_mon, time_set.tm_mday);
		OLED_ShowStr(30, 0, str_day_set, 1);
		OLED_ShowStr(0, 7,"+", 1);OLED_ShowStr(40, 7, "-", 1);OLED_DisString_CH(70, 6, "��һ��");
	}
	if(page==6)
	{
		sprintf((char*)str_alarm_time,"%0.2d:%0.2d:%0.2d",alarm_time.tm_hour, alarm_time.tm_min, alarm_time.tm_sec);
		OLED_ShowStr(35 , 3, str_alarm_time, 1);
		OLED_DisString_CH(70 , 6, "����");
		if(alarm_flag==1)		OLED_DisString_CH(0 , 6, "��");
		else		OLED_DisString_CH(0 , 6, "��");
	}
	if(page==7)
	{
		sprintf((char*)str_alarm_time,"%0.2d:%0.2d:%0.2d",alarm_time.tm_hour, alarm_time.tm_min, alarm_time.tm_sec);
		OLED_ShowStr(35 , 3, str_alarm_time, 1);
		OLED_ShowStr(0, 7,"+", 1);OLED_ShowStr(40, 7, "-", 1);OLED_DisString_CH(70, 6, "��һ��");
	}
	if(page==8)
	{
		sprintf((char*)str_stop_time,"%0.2d:%0.2d:%0.2d",stopwatch_time.tm_hour, stopwatch_time.tm_min, stopwatch_time.tm_sec);
		OLED_ShowStr(35 , 3, str_stop_time, 1);
		if(stopwatch_flag==0)OLED_DisString_CH(0, 6,"��");
		else	OLED_DisString_CH(0, 6, "ͣ");
		OLED_DisString_CH(70, 6, "��λ");
	}
	if(page==9)
	{
		Draw_CircleClock(systmtime.tm_hour,systmtime.tm_min ,systmtime.tm_sec);
//		OLED_ShowStr(35 , 3, "2019214145", 1);
//		OLED_DisString_CH(35 , 5, "�");
		
	}


}

