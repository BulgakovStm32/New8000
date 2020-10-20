
#ifndef _SwTimers_H
#define _SwTimers_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

//-----------------------------------------------------------------------------
#define MaxSwTimersConst 4


#define TIMER_Delay 		   0 
//#define TIMER_LCD 		   1
//#define TIMER_UART       2
//#define TIMER_CMD 		   3
//-----------------------------------------
#define TIMER_STOPPED		0
#define TIMER_RUNNING		1
#define TIMER_PAUSED		2

#define SEC 			1000
#define MIN 			60 * sec
#define HOUR 			60 * min
//-----------------------------------------------------------------------------
void     SwTimersLoop(void);
void     SwTimersInit(void);
uint16_t SwTimerGet(uint8_t timer);
void     SwTimerClr(uint8_t timer);
//Глобальные програмные таймеры.
uint16_t GlobSwTimerGet     (uint8_t timer);
void     GlobSwTimerStop    (uint8_t timer);
void     GlobSwTimerStart   (uint8_t timer);
void     GlobSwTimerPause   (uint8_t timer);
void     GlobSwTimerContinue(uint8_t timer);
//-----------------------------------------------------------------------------
#endif /*_SwTimers_H*/



