
#ifndef _Blink_H
#define _Blink_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"
//#include "stm32f0xx.h"

//-----------------------------------------------------------------------------
enum BlinkIntervalEnum
{
	Interval50mS = 0,
	Interval100mS,
	Interval250mS,
	Interval500mS,
	Interval1S,
};
//-----------------------------------------------------------------------------
void    BlinkLoop(void);
uint8_t Blink(enum BlinkIntervalEnum interval);
//-----------------------------------------------------------------------------
#endif /*_Blink_H*/


