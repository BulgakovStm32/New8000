
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
void	    TimeUTCSet(uint32_t timeUtc);
uint32_t	TimeUTCGet(void);

uint32_t   TimeToRtc(RtcTime_t *time);
void       RtcToTime(uint32_t cnt, RtcTime_t *time);
RtcTime_t* TimeGet(void);
	
//-----------------------------------------------------------------------------
#endif 

