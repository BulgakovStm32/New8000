
#ifndef _SwTimers_H
#define _SwTimers_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

//-----------------------------------------------------------------------------
#define MaxSwTimersConst        8
//Програмные таймеры
#define SwTimerForPojLine1 			0
#define SwTimerForPojLine2 			1
#define SwTimerForPojLine3 			2
#define SwTimerForPojChS   			3

#define SwTimerForPojLineFaultOffset 4

#define SwTimerForPojLine1Fault 4
#define SwTimerForPojLine2Fault 5
#define SwTimerForPojLine3Fault 6
#define SwTimerForPojChSFault   7
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



