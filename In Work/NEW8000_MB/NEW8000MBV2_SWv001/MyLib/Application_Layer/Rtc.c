
#include "Rtc.h"

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//function преобразует  "Время" в секунды (если 0->00:00:00)
//argument указатель на струтуру, хранящую время для преобразования
//result   время в формате "секунды"
uint32_t TimeToRtc(RtcTimeSTR *time){
  
  uint32_t result;
  //--------------------
  result  = (uint32_t)time->hour * 3600; //учесть часы
  result += (uint32_t)time->min * 60;    //минуты
  result +=           time->sec;         //секунды
 
  return result;
} 
//-----------------------------------------------------------------------------  
//function преобразует секунды в формат "Время" (если 0->00:00:00)
//argument значение счетчика, указатель на струтуру "Время"
//result   none
void RtcToTime(uint32_t cnt, RtcTimeSTR *time){
  
  time->sec = cnt % 60;               //получить секунды
  cnt /= 60;                          //количество минут
  time->min = cnt % 60;               //получить минуты
  cnt /= 60;                          //количество дней
  time->hour = cnt % 24;              //получить часы
} 
//-----------------------------------------------------------------------------
