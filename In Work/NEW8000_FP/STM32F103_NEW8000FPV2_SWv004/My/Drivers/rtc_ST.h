
#ifndef _rtc_ST_H
#define _rtc_ST_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "gpio_ST.h"
//-----------------------------------------------------------------------------
void      RtcInit(void);
uint32_t  GetCounterRTC(void);
void      SetCounterRTC(uint32_t value);
//-----------------------------------------------------------------------------
#endif 


