
#include "Time.h"

//-----------------------------------------------------------------------------
static RtcTime_t RtcTimeSTR;
//-----------------------------------------------------------------------------
void Time_Init(void){

	RtcInit(); //Инициализация RTC.
}
//-----------------------------------------------------------------------------
void	TimeUTC_Set(uint32_t timeUtc){

	RtcSetCounter(timeUtc);//Установка времени.
}
//-----------------------------------------------------------------------------
uint32_t TimeUTC_Get(void){

	return RtcGetCounter();	
}
//-----------------------------------------------------------------------------  
//function преобразует  "Время" в секунды (если 0->00:00:00)
//argument указатель на струтуру, хранящую время для преобразования
//result   время в формате "секунды"
uint32_t Time_To_Rtc(RtcTime_t *time){
  
  uint32_t result;
  //--------------------
  result  = (uint32_t)time->Hour * 3600;//учесть часы
  result += (uint32_t)time->Min * 60;   //минуты
  result +=           time->Sec;        //секунды
  return result;
} 
//-----------------------------------------------------------------------------  
//function преобразует секунды в формат "Время" (если 0->00:00:00)
//argument значение счетчика, указатель на струтуру "Время"
//result   none
void Rtc_To_Time(uint32_t cnt, RtcTime_t *time){
  
  time->Sec = cnt % 60;               //получить секунды
  cnt /= 60;                          //количество минут
  time->Min = cnt % 60;               //получить минуты
  cnt /= 60;                          //количество дней
  time->Hour = cnt % 24;              //получить часы
} 
//-----------------------------------------------------------------------------
RtcTime_t* Time_Get(void){

	return &RtcTimeSTR;
}
//-----------------------------------------------------------------------------
