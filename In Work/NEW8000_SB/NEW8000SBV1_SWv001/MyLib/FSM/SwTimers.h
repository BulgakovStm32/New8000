
#ifndef _SwTimers_H
#define _SwTimers_H
//-----------------------------------------------------------------------------

//#include "stm32f10x.h"
#include "stm32f0xx.h"

//-----------------------------------------------------------------------------
#define MaxSwTimersConst      6
//Програмные таймеры
#define SwTimerForSiren1 			0
#define SwTimerForSiren2 			1
#define SwTimerForSiren3 			2

//#define SwTimerForSirenFaultOffset 3

#define SwTimerForSiren1Fault 3
#define SwTimerForSiren2Fault 4
#define SwTimerForSiren3Fault 5
//-----------------------------------------
#define SwTimerSTOP 	0
#define SwTimerRUN  	1
#define SwTimerPAUSE  2

#define SEC 			1000
#define MIN 			60 * sec
#define HOUR 			60 * min
//-----------------------------------------------------------------------------
void     SwTimers_Loop(void);
void     SwTimers_Init(void);
uint16_t SwTimers_Get(uint8_t timer);
void     SwTimers_Clr(uint8_t timer);
//Глобальные програмные таймеры.
uint16_t GlobSwTimer_Get     (uint8_t timer);
void     GlobSwTimer_Stop    (uint8_t timer);
void     GlobSwTimer_Start   (uint8_t timer);
void     GlobSwTimer_Pause   (uint8_t timer);
void     GlobSwTimer_Continue(uint8_t timer);
//-----------------------------------------------------------------------------
#endif /*_SwTimers_H*/



