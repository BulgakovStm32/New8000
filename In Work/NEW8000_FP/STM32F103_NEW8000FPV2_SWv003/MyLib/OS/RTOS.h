
#ifndef _RTOS_H
#define _RTOS_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"
#include "sys_tick_ST.h"

//-----------------------------------------------------------------------------
//Очередь задач.
#define TaskQueueSize 16                        //Макс. число задач в очереди задач.

typedef   void(*TPTR)(void);                    //Указатель-пустышка.
volatile static TPTR TaskQueue[TaskQueueSize+1];//Очередь указателей
//--------------------
//Очередь таймеров.
#define MainTimerQueueSize TaskQueueSize //

volatile static struct{
    //---------
    TPTR     Task;//Указатель перехода
    uint16_t Time;//Выдержка в мс
    //---------
}TaskWithTime[MainTimerQueueSize+1];// Очередь таймеров
//-----------------------------------------------------------------------------
void     RTOS_TaskManager (void);
void     RTOS_TimerService(void);
uint32_t RTOS_GetTickCount(void);

void RTOS_SetTask     (TPTR task);
void RTOS_SetTimerTask(TPTR task, uint16_t newTime);
void RTOS_Init        (void);
//void RTOS_Run         (void);

//Прототипы задач 
void Idle              (void);
//-----------------------------------------------------------------------------
#endif /*_RTOS_H*/


