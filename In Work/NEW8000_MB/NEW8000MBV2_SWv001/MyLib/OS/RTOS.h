
#ifndef _RTOS_H
#define _RTOS_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"
#include "sys_tick_ST.h"

//-----------------------------------------------------------------------------
//Очередь задач.
#define TaskQueueSize 20                        //Макс. число задач в очереди задач.

typedef   void(*TPTR)(void);                    //Указатель-пустышка.
volatile static TPTR TaskQueue[TaskQueueSize+1];//Очередь указателей
//--------------------
//Очередь таймеров.
#define MainTimerQueueSize TaskQueueSize //

volatile static struct{
    //---------
    TPTR     GoToTask;// Указатель перехода
    uint16_t Time;		// Выдержка в мс
    //---------
}MainTimer[MainTimerQueueSize+1];// Очередь таймеров
//-----------------------------------------------------------------------------
void     RTOS_TaskManager (void);
void     RTOS_TimerService(void);
uint32_t RTOS_GetTickCount(void);

void RTOS_SetTask     (TPTR Task);
void RTOS_SetTimerTask(TPTR TS, uint16_t NewTime);
void RTOS_Init        (void);
void RTOS_Run         (void);

//--------------------------------
// Прототипы задач 
void Idle                  (void);
//-----------------------------------------------------------------------------
#endif /*_RTOS_H*/


