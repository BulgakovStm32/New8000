
#ifndef _Time_H
#define _Time_H
//-----------------------------------------------------------------------------

#include "rtc_ST.h"

//-----------------------------------------------------------------------------
//Структура для хранения параметров времени.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
	uint16_t  Hour;//часы	       
	uint16_t  Min; //минуты    
	uint16_t  Sec; //секунды    
}RtcTime_t;
#pragma pack(pop)//вернули предыдущую настройку.
//-----------------------------------------------------------------------------
void		 Time_Init  (void);
void	   TimeUTC_Set(uint32_t timeUtc);
uint32_t TimeUTC_Get(void);

uint32_t   Time_To_Rtc(RtcTime_t *time);
void       Rtc_To_Time(uint32_t cnt, RtcTime_t *time);
RtcTime_t* Time_Get(void);
	
//-----------------------------------------------------------------------------
#endif 

