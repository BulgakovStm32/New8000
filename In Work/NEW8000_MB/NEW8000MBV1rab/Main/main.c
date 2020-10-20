
#include "main.h"

//-----------------------------------------------------------------------------
//Кнопки на лицевой панели.    
//#define PRES1_BUTTON	  (1<<0)  
//#define PRES2_BUTTON	  (1<<1) 
//#define PRES3_BUTTON	  (1<<2)  
//#define PRES4_BUTTON	  (1<<3)  
//#define PRES5_BUTTON	  (1<<4)
//#define PRES6_BUTTON	  (1<<5)
//#define PRES7_BUTTON	  (1<<6)
//#define PRES8_BUTTON	  (1<<7)

//-----------------------------------------------------------------------------
#define	MUX_Mic		     0
#define	MUX_Multimedia 1
#define	MUX_LinIn1		 2
#define	MUX_LinIn2		 3
#define	MUX_Priority5  4
#define	MUX_AllOff		 5

//Сосятояния микрофона.
#define MicNotConnected	  (1<<0)//микрофон не подключен
#define MicConnected	    (1<<1)//микрофон подключен
#define MicActive	        (1<<2)//на микрофоне нажата тангента, можно говорить.
#define MicFault          (1<<3)//капсуль микрофона неисправен.
//-----------------------------------------------------------------------------

//Глобальное время.
//RtcTimeSTR  Time; 

//Флаговый регистр.
//volatile uint16_t MAIN_FlagReg = 0;
//-----------------------------------------------------------------------------
struct{
    //--------------------
    uint16_t mS_CounterForProtocolRS485;
    uint16_t mS_CounterForSpiLoop;
//    uint16_t mS_CounterForAdcLoop;
//    uint16_t mS_CounterForAdcLCLoop;  
    uint16_t DebugCount;
    //--------------------
}SysTicCountersSTR;
//-----------------------------------------------------------------------------
//Функция задержки на заданное количество м—ек.
void Delay_mSec(volatile uint32_t mSec){
  
  //static uint32_t count = 0;
  //--------------------
  while(mSec)
    {
      if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) mSec--;
    }
}
//-----------------------------------------------------------------------------
//Звуковая индикация.
static void ZummerBeep(uint8_t number_of_times, uint16_t interval){

	do 
		{
			RelayOrLedOff(Zummer);
      Delay_mSec(interval);
			RelayOrLedOn(Zummer);
      Delay_mSec(interval);
		} 
	while (--number_of_times);
	RelayOrLedOff(Zummer);
}
//-----------------------------------------------------------------------------
//Управление мультиплексором.
void SetMux(uint8_t chanel){
  
  GPIOB->ODR = (GPIOB->ODR & ~(7 << 7)) | (chanel << 7);
//  GPIOB->BSRR = GPIO_BSRR_BR7;
//  GPIOB->BSRR = GPIO_BSRR_BR8;
//  GPIOB->BSRR = GPIO_BSRR_BS9;
}
//----------------------------------------------------------------------------
void Priority1_Work(void){
  
}
//----------------------------------------------------------------------------
void Priority2_Work(void){

  //--------------------
  if(PojLineGetState(Poj1) == NormCloseSost)
    {
      RelayOrLedOn(RelSp1AttLin | RelSp1OnOff); 
    }
  else
   {
     RelayOrLedOff(RelSp1AttLin | RelSp1OnOff);
   }
  //--------------------  
  if(PojLineGetState(Poj2) == NormCloseSost)
    {
      RelayOrLedOn(RelSp2AttLin | RelSp2OnOff);  
    }
  else
   {
     RelayOrLedOff(RelSp2AttLin | RelSp2OnOff);
   }
  //--------------------
  RelayOrLedOn(Led2 | RelPoj); 
  RelayOrLedOff(Led5 | Led4 | Led3 | Led1);
   
  SetMux(MUX_Multimedia);
}
//----------------------------------------------------------------------------
void Priority3_Work(void){
  
//  STPIC_SostReg |= (STPIC_Rel_GO | STPIC_Led3 |
//                    STPIC_Rel_SP1_Att_Lin | STPIC_Rel_SP1_On_Off |
//                    STPIC_Rel_SP2_Att_Lin | STPIC_Rel_SP2_On_Off);
  
  RelayOrLedOn(Led2); 
  RelayOrLedOff(Led5 | Led4 | Led3 | Led1); 
  
  SetMux(MUX_Mic);
}
//----------------------------------------------------------------------------
void Priority4_Work(void){

  //--------------------
  //Приоритет 4 у Пож3.
  if(PojLineGetState(Poj3) == NormCloseSost)
    {
//      STPIC_SostReg |= (STPIC_Rel_Poj3 | STPIC_Led4 |
//                        STPIC_Rel_SP1_Att_Lin | STPIC_Rel_SP1_On_Off |
//                        STPIC_Rel_SP2_Att_Lin | STPIC_Rel_SP2_On_Off);
      
      RelayOrLedOn(Led4);
      SetMux(MUX_LinIn1);
    }
  //--------------------
  //Приоритет 4 у Консоли.
  else
   {
     
   }
  //-------------------- 
  RelayOrLedOff(Led5 | Led3 | Led2 | Led1);       
}
//----------------------------------------------------------------------------
void Priority5_Work(void){
  
}
//----------------------------------------------------------------------------
//Приоритет 6 - Мик./Лин.Вх.2. -  Ручное управление.
void Priority6_Work(void){


  //RS485ClearFlagReg(RS485_FP_PresentFlag);
  //RS485ClearReceiverBuffer(); 

  //SetMux(MUX_AllOff);
  //SetMux(MUX_Mic);
  //SetMux(MUX_LinIn2);
  //--------------------
  //RelayOrLedOn(Led5 | RelSp1OnOff  | RelSp2OnOff );
               //Led4 | RelSp1AttLin | RelSp2AttLin);

  //RelayOrLedOff(Led4 | Led3 | Led2 | Led1); 
  //RelayOrLedOn(RelSp1AttLin);
  //--------------------
  //Управление тангетой микрофона
  if(RS485RxGetDataFromBuf(FP_KeyAndMicStatePoint) & MicActive)
    {
      RelayOrLedOn(RelSp1OnOff | RelSp2OnOff | Led1 | Led2);
    }
  else
    {
      //Проверка управление кнопками с лицевой панели
      if(RS485RxGetDataFromBuf(FP_PresetButtonsPoint) & PRES1_BUTTON) RelayOrLedOn(Led1 | RelSp1OnOff);
      else RelayOrLedOff(Led1 | RelSp1OnOff);

      if(RS485RxGetDataFromBuf(FP_PresetButtonsPoint) & PRES2_BUTTON) RelayOrLedOn(Led2 | RelSp2OnOff);
      else RelayOrLedOff(Led2 | RelSp2OnOff);

      if(RS485RxGetDataFromBuf(FP_PresetButtonsPoint) & PRES3_BUTTON) RelayOrLedOn(Led3);
      else RelayOrLedOff(Led3);

      if(RS485RxGetDataFromBuf(FP_PresetButtonsPoint) & PRES4_BUTTON) RelayOrLedOn(Led4);
      else RelayOrLedOff(Led4);
    }
  //Если микрофон не подключен или неисправен то пищим.
  if(RS485RxGetDataFromBuf(FP_KeyAndMicStatePoint) & (MicFault | MicNotConnected)) ZummerBeep(3, 100);
  //--------------------
  //Запус приема нового запроса от ЦП.
  //RS485FlagClr(RS485_FPNewDataFlag);
  //RS485RxClrBuf();
  //--------------------
}
//----------------------------------------------------------------------------
//Получение активного приоритета блока.
//uint8_t GetPriority(void){
//  
//  //--------------------
////  if()
////    {
////      
////      return PRIORITY_1;
////    }
////  //--------------------
////  if((GetLineSost(LinePoj1) == NormCloseSost) || (GetLineSost(LinePoj2) == NormCloseSost))
////    {
////      return PRIORITY_2;
////    }
////  //--------------------
////  if(GetLineSost(LineChS) == NormCloseSost)
////    {
////      return PRIORITY_3;
////    }
////  //--------------------
////  if(GetLineSost(LinePoj3) == NormCloseSost)
////    {
////      return PRIORITY_4;
////    }
////  //--------------------
////   if()
////    {
////      
////      return PRIORITY_5;
////    }
//  //--------------------
//  //Приоритет 6 - Мик./Лин.Вх.2. -  Ручное управление.
//  if(RS485GetFlagReg() & RS485_FPOkFlag)
//      {
//        RS485ClearFlagReg(RS485_FPErrorFlag);
//        
//        return PRIORITY_6;
//      }
//  //--------------------
//  if(RS485GetFlagReg() & RS485_FPErrorFlag)
//      {
//        return ErrorFacePanel;
//      }
//  //--------------------
//}
//************************************************************************************************** 
//************************************************************************************************** 
void FPStatus(void){

  //--------------------
  //Управление тангетой микрофона
  if(RS485RxGetDataFromBuf(FP_KeyAndMicStatePoint) & MicActive)
    {
      RelayOrLedOn(RelSp1OnOff | RelSp2OnOff | Led1 | Led2);
    }
  else
    {
      //Проверка управление кнопками с лицевой панели
      if(RS485RxGetDataFromBuf(FP_PresetButtonsPoint) & PRES1_BUTTON) RelayOrLedOn(Led1 | RelSp1OnOff);
      else RelayOrLedOff(Led1 | RelSp1OnOff);

      if(RS485RxGetDataFromBuf(FP_PresetButtonsPoint) & PRES2_BUTTON) RelayOrLedOn(Led2 | RelSp2OnOff);
      else RelayOrLedOff(Led2 | RelSp2OnOff);

      if(RS485RxGetDataFromBuf(FP_PresetButtonsPoint) & PRES3_BUTTON) RelayOrLedOn(Led3);
      else RelayOrLedOff(Led3);

      if(RS485RxGetDataFromBuf(FP_PresetButtonsPoint) & PRES4_BUTTON) RelayOrLedOn(Led4);
      else RelayOrLedOff(Led4);
    }
  //Если микрофон не подключен или неисправен то пищим.
  if(RS485RxGetDataFromBuf(FP_KeyAndMicStatePoint) & (MicFault | MicNotConnected))
    {
      ZummerBeep(3, 20);
      Delay_mSec(1000);
    }
  //--------------------
  RelayOrLedOff(Zummer);
}
//************************************************************************************************** 
//************************************************************************************************** 
void Task_GetControlForUnit(void){

  if(RS485FlagGet() & RS485_FPNewDataFlag) RTOS_SetTimerTask(Task_FPControl, 1);
  if(RS485FlagGet() & RS485_TimeOutFlag)   RTOS_SetTimerTask(Task_FPError,   1); 

  RTOS_SetTimerTask(Task_GetControlForUnit, 1);
}
//----------------------------------------------------------------------------
//Режим "Ошибка обмена с FP".
void Task_FPError(void){

  //ZummerBeep(10, 10);//Писщим зумером.
  RS485ActLedOn;     //Моргаем светодиодом.
  ZummerBeep(10, 5); //Пищим зумером.
  RS485ActLedOff;          
  Delay_mSec(1000);
  //RS485FlagClr(RS485_TimeOutFlag);

  RTOS_SetTimerTask(Task_GetControlForUnit, 1);
}
//----------------------------------------------------------------------------
//Запус приема нового пакета.
//void Task_RS485StartRxNewData(void){

//  RS485FlagClr(RS485_FPNewDataFlag);
//  RS485RxClrBuf();

//  RTOS_SetTimerTask(Task_GetControlForUnit, 1);
//}
//----------------------------------------------------------------------------
void Task_FPControl(void){

  //Обработка команд от FP:
  switch(RS485RxGetDataFromBuf(Comand_CodePoint))
    {
      //Команда "Выдача статуса от FP".
      case(CmdFromFPStatusConst):
        FPStatus();
      break;
      //Команда установки адрес блока
      case(CmdFromFPSetAddressConst):
      break;
      //Команда устновки группы блока 
      case(CmdFromFPSetGroupConst):
      break;
      //Команда установики времени 
      case(CmdFromFPSetTimeConst):
      break;
      //Команда влючение тестирования зуммера ЦП
      case(CmdFromFPTetsZummerConst):
        if(RS485RxGetDataFromBuf(FP_KeyAndMicStatePoint)) RelayOrLedOn (Zummer);
        else                                              RelayOrLedOff(Zummer);
      break;
      //-----------
      default:
      break;
      //-----------
    }
  //--------------------
  //Запус приема пакета от FP.
  RS485FlagClr(RS485_FPNewDataFlag);
  RS485RxClrBuf();
  RTOS_SetTimerTask(Task_GetControlForUnit, 1);   
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

//************************************************************************************************** 
//**************************************************************************************************
int main(void){
    
  //uint16_t LcTemp = 0;
  //--------------------
  //После сброса микроконтроллера программа преходит в main(), минуя SystemInit().
  //В фейле startup_stm32f10x_md_vl.s закоментирован переход на SystemInit.
  SysInit();   //Настройка тактирования микроконтроллера. Внешний кварц 12 МГц. 
  GpioInit();  //Инициализация переферии.
  CRC_Init();
  //EepInit();   //Инициализация EEPROM. Должно быть до Spi2Init().
  //Uart1Init();
  Spi2Init();
  //AdcInit();
  RtcInit();  //Инициализация RTC.
  //LcInit();
  //--------------------   
	StpicCLRHight;
	StpicCSLow;

  SetMux(MUX_AllOff);
  //--------------------
  //Проверка микросхемы EEPROM.
  //EepMemArrayCheck();
  //--------------------
  //Установка времени.
//  Time.sec = 00;
//  Time.min = 49;
//  Time.hour= 16;
//  SetCounterRTC(TimeToRtc(&Time));//преобразовать время в формат "понятный" для RTC.
  //--------------------
  //После окончения пер.процессов сохранение значений линий как опорных.
  //LcSaveRefVolueLine();   //Сохранение опорных величин линий Гр. дя LC.
  //--------------------
  //Инициализация ОС.
  RTOS_Init(); 	 // Инициализируем ядро
  SysTickInit(); //Старт ядра.
  __enable_irq();//Глобальное разрешение рерываний.
  
  //Запуск фоновых задач. 
  RTOS_SetTask(Task_GetControlForUnit);
  //--------------------
  //После окончения пер.процессов сохранение значений линий как опорных.
  //LcSaveRefState();   //Сохранение опорных величин линий Гр. дя LC.

  Uart1Init();
  //--------------------
  while(1)
    {  
      RTOS_TaskManager(); //Вызов диспетчера ОС.
    }
  //--------------------
      //RelayOrLedOn(Led5 | RelSp1OnOff | RelSp1AttLin);
      //SetMux(MUX_LinIn2);
			//----------------------------------------------------------------------
  		//Управление по приоритетам.		
// 			switch(GetPriority()) 
// 				{	
//          //----------------------------
//          //Нет связи с лиц.панелью.
//          case(ErrorFacePanel):
//            //ZummerBeep(10, 10);
//            //Моргаем светодиодом.
//            RS485ActLedOff; 
//            Delay_mSec(1000);//500);
//            RS485ActLedOn;          
//            Delay_mSec(50);
// 					break;
//          //----------------------------
// 					//Приоритет 6 - Мик./Лин.Вх.2. -  Ручное управление.
//          case(PRIORITY_6):
//            Priority6_Work();		          
// 					break;					
//          //----------------------------
// 					//Приоритет 5 - IP-Радио. 
// 					case(PRIORITY_5):
// 						Priority5_Work();										
// 					break;
// 					//----------------------------
// 					//Приоритет 4 - Консоль или Мик./Лин.Вх.2 
// 					case(PRIORITY_4):
// 						Priority4_Work();				
// 					break;
// 					//----------------------------
// 					//Приоритет 3 - ГО и ЧС. 
//          //Активация приоритета осуществляется входным шлейфом «ЧС». 
// 					case(PRIORITY_3):
//            Priority3_Work();
// 					break;
// 					//----------------------------
// 					//Приоритет 2 - Активация встроенного тревожного сообщения.
//          //Активация приоритета осуществляется входным шлейфом «Пож.1» или «Пож.2».
// 					case(PRIORITY_2):
//            Priority2_Work();
// 					break;
// 					//----------------------------
// 					//Приоритет 1(высший). Активация кнопкой "МИК.Пр" на лицевой панели.
// 					case(PRIORITY_1):
//            Priority1_Work();
// 					break;
// 					//----------------------------
// 				}
    //----------------------------------------------------------------------   
//    //Работа с LC.  
//    if(LcGetLineStatus(Lc1Line) == LineNorm)
//      {
//        LC1SostRedLedOff;
//        //RelayOrLedOn(RelSp1OnOff | RelSp1AttLin);
//      }
//    else
//      {
//        LC1SostRedLedOn;
//        //RelayOrLedOff(RelSp1OnOff);
//      }
//    //----------------------------
//    if(LcGetLineStatus(Lc1Atten) == LineNorm)
//      {
//        LC2SostRedLedOff;
//      }
//    else
//      {
//        LC2SostRedLedOn;
//      }
    //----------------------------    

    //----------------------------------------------------------------------  
//    if(MAIN_FlagReg & RS485ProtocolLoop_Flag)
//      { 
//        MAIN_FlagReg &= ~RS485ProtocolLoop_Flag;
//        //RS485ProtocolLoop();
//      }
  //----------------------------------------------------------------------------      
  //---------------------------------------------------------------------------- 
//	}

}
//************************************************************************************************** 
//************************************************************************************************** 
//Прерываени системного таймера SysTick.
void SysTick_Handler(void){
  	
  //--------------------
  //Служба таймеров ОС.
  RTOS_TimerService();
  //--------------------
  //Отсчет таймаута приема пакета.
  RS485TimeOutInc();
  //-------------------- 
  //Запросы по протоколу каждые RS485ProtocolLoopPeriod мСек.
  if (++SysTicCountersSTR.mS_CounterForProtocolRS485 >= RS485ProtocolLoopPeriod)
    {
      SysTicCountersSTR.mS_CounterForProtocolRS485 = 0;
      RS485MB_ProtocolLoop();
      RelayOrLedLoop();  
    }  
  //-------------------- 
  
  //Работа с АЦП.
  //AdcAverageMeasForPojLineLoop();
  //AdcLcMeasLoop();
  //-------------------- 
  //Работа LC.
  //LcGetSteadyState(); //Получение установившегося значения после переходного процесса.
  //-------------------- 
  //Счетчик для отладки.
//  if (++SysTicCountersSTR.DebugCount >= 10)
//    {
//      SysTicCountersSTR.DebugCount = 0;
//      //-------------------- 
//      //LC1SostRedLedToggel;//Мигаем для отладки 
//      //OptSP1AttToggel;
//    }    
  //--------------------   
}
//************************************************************************************************** 
//************************************************************************************************** 
//Прерываени таймера TIM6. Прерывание каждую милисекунду.
void TIM6_DAC_IRQHandler(void){
  
  //--------------------
  TIM6->SR &= ~ (TIM_SR_UIF); // сброс флага прерывания.
  
  //--------------------
  //LC2SostRedLedToggel;
} 
//************************************************************************************************** 
//************************************************************************************************** 

//************************************************************************************************** 
//**************************************************************************************************


