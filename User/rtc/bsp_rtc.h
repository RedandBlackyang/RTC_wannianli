#ifndef __RTC_H
#define	__RTC_H


#include "stm32f10x.h"
#include "./rtc/bsp_calendar.h"
#include "./rtc/bsp_date.h"

//使用LSE外部时钟 或 LSI内部时钟
#define RTC_CLOCK_SOURCE_LSE      
//#define RTC_CLOCK_SOURCE_LSI



#define RTC_BKP_DRX          BKP_DR1
// 写入到备份寄存器的数据宏定义
#define RTC_BKP_DATA         0xA5A5

//北京时间的时区秒数差
#define TIME_ZOOM						(8*60*60)


/* 如果定义了下面这个宏的话,PC13就会输出频率为RTC Clock/64的时钟 */   
//#define RTCClockOutput_Enable  /* RTC Clock/64 is output on tamper pin(PC.13) */

void RTC_NVIC_Config(void);
void RTC_Configuration(void);
void Time_Regulate_Get(struct rtc_time *tm);
void Time_Adjust(struct rtc_time *tm);
void Time_Display(uint32_t TimeVar,struct rtc_time *tm);
static uint8_t USART_Scanf(uint32_t value);
void RTC_CheckAndConfig(struct rtc_time *tm);
void oled_page_display(uint8_t page);
void Alarm_bkp_Write(void);				//将闹钟的相关信息分别写入备份寄存器BKP_DR2 BKP_DR3
void Alarm_bkp_Read(void);				//从备份寄存器读取闹钟的相关信息

#endif /* __XXX_H */
