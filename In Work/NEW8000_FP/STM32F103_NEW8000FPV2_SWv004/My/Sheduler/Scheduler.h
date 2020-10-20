
#ifndef _Scheduler_H
#define _Scheduler_H
//-----------------------------------------------------------------------------

#include "sys_tick_ST.h"

//-----------------------------------------------------------------------------
//Очередь задач.
#define TaskQueueSize 20U  //Макс. число задач в очереди задач.
typedef void(*pTask)(void);//Указатель-пустышка.

volatile static pTask TaskQueue[TaskQueueSize + 1];//Очередь указателей
//--------------------
//Очередь таймеров.
#define MainTimerQueueSize TaskQueueSize 

volatile static struct{
	//---------
	pTask    Task;//Указатель перехода
	uint16_t Time;//Выдержка в мс
	//---------
}TaskWithTime[MainTimerQueueSize + 1];// Очередь таймеров
//-----------------------------------------------------------------------------
void Scheduler_Init 			 (void);
void Scheduler_Start			 (void);
void Scheduler						 (void);
void Scheduler_TimerService(void);

uint32_t Scheduler_GetTickCount(void);
void 		 Scheduler_SetTask     (pTask task);
void     Scheduler_SetTimerTask(pTask task, uint16_t newTime);
//--------------------------------
// Прототипы задач 
void IdleTask(void);
//-----------------------------------------------------------------------------
#endif /*_Scheduler_H*/


