
#include "main.h"

//************************************************************************************************** 
//**************************************************************************************************
void Task_Apply(void){
	
	//Отключение всех оповещателей при отсутсвии питания.
	if(Siren_GetMeas(SIREN_POWER) < SIREN_POWER_MIN) 
		{
			Siren_OutputFSM(SIREN_1, SIREN_OUT_OFF);
			Siren_OutputFSM(SIREN_2, SIREN_OUT_OFF);
			Siren_OutputFSM(SIREN_3, SIREN_OUT_OFF);			
		}
	//Управление выходами.
	else 
		{
			Siren_ControlFromMB(SIREN_1);
			Siren_ControlFromMB(SIREN_2);
			Siren_ControlFromMB(SIREN_3);
		}	
	//--------------------	
	Scheduler_SetTask(Task_Apply);			
}
//***********************************************************************
void Task_ParsingCmd(void){

	//Сохранение данных.
	SaveDataFromMB(RS485_RxBuf()->Str.Data);
	//Передача ответа.
	RS485_Flags()->SB_CMD_SET_f = FlagClr;			
	RS485_TxStatusSB();
}
//***********************************************************************
void Task_PollingRS485(void){
	
	if(RS485_Flags()->SB_CMD_SET_f) Scheduler_SetTask(Task_ParsingCmd);
	//Истек таймаут ожидания запроса от ЦП.
	else if(RS485_Flags()->TIMEOUT_ERR_f) 
		{
			Siren_OutputFSM(SIREN_1, SIREN_OUT_OFF);
			Siren_OutputFSM(SIREN_2, SIREN_OUT_OFF);
			Siren_OutputFSM(SIREN_3, SIREN_OUT_OFF);	
			Led_ACT_Set(Blink(INTERVAL_1000_mS));
		}
	else Led_ACT_Set(Blink(INTERVAL_50_mS));
	//--------------------
	Scheduler_SetTask(Task_PollingRS485);			
}
//************************************************************************************************** 
//**************************************************************************************************
int main(void){
    
  //--------------------
  //После сброса микроконтроллера программа преходит в main(), минуя SystemInit().
  //В фейле startup_stm32f070x6.s закоментирован переход на SystemInit.
  Sys_Init();  //Настройка тактирования микроконтроллера. Внешний кварц 8 МГц. 
  Gpio_Init();   
  //-------------------- 
  //Инициализация прикладного уровня.
  RS485_Init();
	Siren_Init();
  //--------------------
	//Инициализация службы таймеров и службы сообщений для работы с конечными автоматами.
	SwTimers_Init();	 
	FsmMessages_Init();
	//--------------------
  Scheduler_Init();
  Scheduler_SetTask(Task_PollingRS485);
	Scheduler_SetTask(Task_Apply);	
	
  SysTick_Init();  //Запуск службы таймеров.
  __enable_irq();  //Глобальное разрешение рерываний.
  //--------------------
  while(1)
    {  		
			Scheduler();  //Вызов диспетчера.
    }
  //--------------------
}
//************************************************************************************************** 
//************************************************************************************************** 
//Прерываени системного таймера SysTick.
void SysTick_Handler(void){
  	
	Blink_Loop();				
  RS485_TimeOutLoop(); 		 //Отсчет таймаута приема пакета.
  Siren_MeasLoop();   		 //Определение состояния оповещателей.	
	Scheduler_TimerService();//Служба таймеров диспетчера.	
	SwTimers_Loop();         //Виртуальные таймера для машины состояний.
}
//************************************************************************************************** 
//************************************************************************************************** 





