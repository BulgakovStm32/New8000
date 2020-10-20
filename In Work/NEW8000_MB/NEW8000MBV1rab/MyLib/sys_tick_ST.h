
#ifndef _sys_tick_ST_H
#define _sys_tick_ST_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

//-----------------------------------------------------------------------------

#define F_CPU				24000000UL		//Тактовая частота 24МГЦ.
#define TimerTick		F_CPU/1000-1  	//

//-----------------------------------------------------------------------------

void SysTickInit(void);

//-----------------------------------------------------------------------------
#endif /*_sys_tick_ST_H*/
