
#ifndef _Rtc_H
#define _Rtc_H

//-----------------------------------------------------------------------------

#include "stm32f10x.h"

//#include "rtc_ST.h"
//-----------------------------------------------------------------------------
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct 
	{
	uint16_t year;
	uint16_t month;
	uint16_t day;
	uint16_t wday;	
	uint16_t hour;
	uint16_t min;
	uint16_t sec;
	uint8_t balast;//Этот байт нужен для выравнивания структуры в памяти. 
								 //Непонял как это работает, но с этим байтом работа с календарем производится нормально.	
}TimeSTR_Type;
#pragma pack(pop)//вернули предыдущую настройку.
//------------------------------------------
//#define JD0 2451911 // дней до 01 янв 2001 ПН
#define JD0 2440587 //   дней 1 января 1970 года (четверг) 
#define JD  ((JD0 << 1) + 1)
//-----------------------------------------------------------------------------
void     TimeInit(void);
void     TimeFromMBSave(uint32_t time);
uint32_t TimeFromMBGet (void);

TimeSTR_Type* DateAhdTimeGet(void);
void          CounterToTimeAndDate(uint32_t counter);
uint32_t 			TimeAndDateToCounter(TimeSTR_Type* timeDate);

//-----------------------------------------------------------------------------
#endif 



