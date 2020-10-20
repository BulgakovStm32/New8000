
#include "Scheduler.h"

//-----------------------------------------------------------------------------

static volatile uint32_t RtosTickCount = 0;

//-----------------------------------------------------------------------------
//Диспетчер задач.
void Scheduler(void){

  uint8_t index = 0;
  TaskPTR goToTask = IdleTask;
  //--------------------
  // Как видишь, тут есть указатель Idle - ведущий на процедуру простоя ядра.
  // На нее можно повесить что нибудь совсем фоновое, например отладочные примочки =)  
  // И локальная переменная-указатель GoToTask куда мы будем жрать адреса переходов 

  __disable_irq();	//Глобальное запрещение прерываний.
  //Прерывания надо запрещать потому, что идет обращение к глобальной очереди диспетчера. 
  //Ее могут менять и прерывания, поэтому заботимся об атомарности операции. 

	//Хватаем первое значение из очереди.
	goToTask = TaskQueue[0];
	//Проверка значения на корректность.
	if(((uint32_t)goToTask & 0xFFFF0000) != 0x08000000)
		{
			TaskQueue[0] = IdleTask;//Помещаем 
			__enable_irq();         //Глобальное разрешение прерываний.		
			(IdleTask)();			 
		}
  //Если там пусто то переходим на обработку пустого цикла
  else if(goToTask == IdleTask)		
    {
       __enable_irq();//Глобальное разрешение прерываний.		
      (IdleTask)();		//пустой цикл
    }
  //В противном случае сдвигаем всю очередь
  else
   	{
      for(index = 0; index < TaskQueueSize; index++)	
        {
          TaskQueue[index] = TaskQueue[index+1];
        }	
      TaskQueue[TaskQueueSize] = IdleTask;//В последнюю запись пихаем затычку Idle
      __enable_irq();	                    //Глобальное разрешение рерываний.		
      (goToTask)();		                    //Переходим к задаче
   	}
}
//---------------------------------------------------------
//Постановщик в очередь.
void Scheduler_SetTask(TaskPTR task){

  uint8_t index = 0;
  //--------------------
  __disable_irq();	//Глобальное запрещение прерываний.
  //А вот и постановка задачи в очередь.
  //Прочесываем очередь задач на предмет 
  //свободной ячейки со значением Idle - конец очереди.
  while(TaskQueue[index] != IdleTask)	
    {			
      index++;
      if(index > TaskQueueSize)//Если очередь переполнена то выходим.	
        {
          __enable_irq();//Если мы не в прерывании, то разрешаем прерывания
          return;		     //Можно вернуть код ошибки...
        }
    }
  // Если нашли свободное место, то
  TaskQueue[index] = task;//Записываем в очередь задачу
  __enable_irq();         //И включаем прерывания если не в обработчике прерывания.
}
//---------------------------------------------------------
//Прерывание таймера
//Служба таймеров пихается в обработчик прерывания от таймера. 
//Служба таймера.
void Scheduler_TimerService(void){

  uint8_t index = 0;
  //--------------------
  RtosTickCount++;
  //--------------------
  //Прочесываем очередь таймеров
  for(index = 0; index < MainTimerQueueSize; index++) 
    {
      if(TaskWithTime[index].Task == IdleTask) continue;					 //Если нашли пустышку - щелкаем следующую итерацию
      if(TaskWithTime[index].Time != 1) TaskWithTime[index].Time--;//Если таймер не выщелкал, то щелкаем еще раз.
      else//if(--MainTimer[index].Time == 0)//else
        {
          Scheduler_SetTask(TaskWithTime[index].Task);//Дощелкали до нуля? Пихаем в очередь задачу
          TaskWithTime[index].Task = IdleTask;        //А в ячейку пишем затычку
        }
    }
}
//---------------------------------------------------------
uint32_t Scheduler_GetTickCount(void){

  return RtosTickCount;
}
//---------------------------------------------------------
//Постановщик таймеров
void Scheduler_SetTimerTask(TaskPTR task, uint16_t newTime){

  uint8_t  index=0;
  //-------------------- 
  __disable_irq();	//Глобальное запрещение прерываний.
  //Прочесываем очередь таймеров 
  for(index = 0; index < MainTimerQueueSize; index++)
    {
      if(TaskWithTime[index].Task == task)	  //Если уже есть запись с таким адресом
        {
          TaskWithTime[index].Time = newTime;//Перезаписываем ей выдержку
          __enable_irq();										 //Разрешаем прерывания
					return;          		               //Выходим. Раньше был код успешной операции. Пока убрал
        }
   }
  //Алгоритм, в данном случае не очень оптимален. В прошлом цикле можно было запомнить 
  //положение первого Idle и сейчас не искать его.
  for(index = 0; index < MainTimerQueueSize; index++)//Если не находим похожий таймер, то ищем любой пустой 
    {
      if(TaskWithTime[index].Task == IdleTask)  
        {
          TaskWithTime[index].Task = task;	 //Заполняем поле перехода задачи
          TaskWithTime[index].Time = newTime;//И поле выдержки времени
					__enable_irq();									   //Разрешаем прерывания
          return; 					                  
        }
    }            
  // тут можно сделать return c кодом ошибки - нет свободных таймеров
}
//-----------------------------------------------------------------------------
//Инициализация диспетчера:
//В отличии от флагового автомата, где только флаги установить, 
//тут требуется при старте сделать инициализацию очередей и очистку таймеров.
//Функция выполняется один раз.
void Scheduler_Init(void){

  uint8_t index = 0;
  //--------------------
  for(index = 0; index < TaskQueueSize; index++)  
    {
			//Во все позиции записываем Idle
      TaskQueue[index] = IdleTask;
			//Обнуляем все таймеры.
			TaskWithTime[index].Task = IdleTask;
			TaskWithTime[index].Time = 0;
    }
//  //Обнуляем все таймеры.
//  for(index=0; index < MainTimerQueueSize; index++) 
//    {
//      TaskWithTime[index].Task = IdleTask;
//      TaskWithTime[index].Time = 0;
//    }
}
//-----------------------------------------------------------------------------
//Запуск диспетчера. Собственно запускать то там надо таймер. 
//RTOS Запуск системного таймера
void Scheduler_Start(void){
  
  SysTick_Init();
}
//-----------------------------------------------------------------------------
//Пустая процедура - простой ядра. 
void IdleTask(void){
  
}
//-----------------------------------------------------------------------------














