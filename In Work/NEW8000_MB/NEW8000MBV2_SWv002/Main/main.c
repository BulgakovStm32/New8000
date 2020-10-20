
#include "main.h"
//#include <stdlib.h>
//#include <string.h>
//-----------------------------------------------------------------------------
#define	MUX_Mic		     1
#define	MUX_Multimedia 2
#define	MUX_LinIn1		 3
#define	MUX_LinIn2		 4
#define	MUX_Priority5  5
#define	MUX_JQ         6
#define	MUX_AllOff		 7

//-----------------------------------------------------------------------------
#define TESTmSConst 10000u

//-----------------------------------
//Флаговый регистр.
volatile uint16_t MAIN_FlagReg = 0;
//Флаги.
#define ZummerTestFlag	    (1<<0)
#define MicFaultFlag	      (1<<1)
#define Poj1FaultFlag	      (1<<2)
#define Poj2FaultFlag	      (1<<3)
#define Poj3FaultFlag	      (1<<4)
#define ChSFaultFlag	      (1<<5)
//#define LcSaveRefFlag	      (1<<6)
//-----------------------------------
//Соятояние кнопок для каждого режима работы блока.
//static uint16_t ButTempUnitLock    = 0;
volatile uint16_t ButTempUnitControl = 0;
//*************************************************************************************************
//*************************************************************************************************
typedef struct{
	
	uint32_t OldState;//предыдущее состояние.
	uint32_t Instant;	//Фиксация действующих неисправностей.
	uint32_t Rise;	 	//Фиксация установки флага.
  uint32_t Fall;   	//Фиксация сброса флага.

}SystemStateSTR_Type;
SystemStateSTR_Type SystemStateSTR;
//-----------------------------------------------------------------------------
//Состояние системы.
SystemStateSTR_Type* SystemState(void){

	return &SystemStateSTR;
}
//-----------------------------------------------------------------------------
SystemStateSTR_Type SystemStateArray[4];

//------------------------------------------------
//неотлажено!!!!!!
void SystemStateSet(enum AllSystemEvents event){
	
	uint8_t  arrayIndex = 0;
	uint32_t bitMask		= 0;
	//--------------------
	//Определение в элемента массива.
	if(event < 32) arrayIndex = 0;
	if(event >= 32 && event < 64)
		{
			arrayIndex = 1;
			event -= 32;
		}
	if(event >= 64 && event < 96)
		{
			arrayIndex = 2;
			event -= 64;
		}
	if(event >= 96 )//&& event < 128)
		{
			arrayIndex = 3;
			event -= 96;
		}	
	//--------------------	
	//выставляем флаг
	bitMask |= (1 << event);	
  //--------------------
	SystemStateArray[arrayIndex].Instant  |= (1 << event);										                            //Фиксация действующео состояния.
	SystemStateArray[arrayIndex].Rise      = (SystemStateArray[arrayIndex].OldState ^ bitMask) & bitMask;//Фиксация установки флага.
	SystemStateArray[arrayIndex].Fall      = (~bitMask) & SystemStateArray[arrayIndex].OldState;         //Фиксация сброса флага.
	SystemStateArray[arrayIndex].OldState |= (1 << event);
	//--------------------
//	SystemStateSTR.Instant = systemState;														      //Фиксация действующео состояния.
//	SystemStateSTR.Rise    = (systemStateOld ^ systemState) & systemState;//Фиксация установки флага.
//	SystemStateSTR.Fall    = (~systemState) & systemStateOld;					    //Фиксация сброса флага.
//	systemStateOld         = systemState;
	//--------------------	
}
//*************************************************************************************************
//*************************************************************************************************
//Функция задержки на заданное количество м—ек.
void Delay_mSec(volatile uint32_t mSec){
  
  while(mSec)
    {
      if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) mSec--;
    }
}
//-----------------------------------------------------------------------------
//Управление приоритетным мультиплексором.
void SetMuxPrior(uint8_t ch){
  
  switch(ch)
    {
      //-----------
      case(MUX_Mic):
        MuxS0Low;
        MuxS1Low;
        MuxS2Low;
      break;
      //-----------
      case(MUX_Multimedia):
        MuxS0Hight;
        MuxS1Low;
        MuxS2Low;        
      break;
      //-----------
      case(MUX_LinIn1):
        MuxS0Low;
        MuxS1Hight;
        MuxS2Low;
      break;
      //----------- 
      case(MUX_LinIn2):
        MuxS0Hight;
        MuxS1Hight;
        MuxS2Low;
      break;
      //-----------
      case(MUX_Priority5):
        MuxS0Low;
        MuxS1Low;
        MuxS2Hight;                                            
      break;
      //-----------
      case(MUX_JQ):
        MuxS0Hight;
        MuxS1Low;
        MuxS2Hight;
      break;
      //-----------
      case(MUX_AllOff):
        MuxS0Hight;
        MuxS1Hight;
        MuxS2Hight;  
      break;
      //-----------
    }
  //--------------------
}
//-----------------------------------------------------------------------------
//Получение приоритета управления блоком.
uint8_t PriorityGet(void){
	
  //--------------------
  //Приоритет 1(высший). 
  //Активация тангентой микрофона лицевой панели.
  if(FacePanel()->Mic == MicActiveConst) return Priority_1;
  //--------------------
	//Управление кнокой "ПУСК".	
	if((ButGetInstantState(CONTROL_BUTTONS) & START_BUTTON) || (ButTempUnitControl & START_BUTTON))	
		{
			return START_Activation;
		}	
  //--------------------
  //Приоритет 2 - Активация встроенного тревожного сообщения.
  //Активация приоритета осуществляется входным шлейфом «Пож.1» или «Пож.2».
  if((FireLineGetLogicState(Poj1) == FireLineFIRE) || (FireLineGetLogicState(Poj2) == FireLineFIRE))	
		{
			return Priority_2;
		}
  //--------------------
  //Приоритет 3 - ГО и ЧС. 
  //Активация приоритета осуществляется входным шлейфом «ЧС».
  if(FireLine(ChS)->FaultState == FireLineNormClose)
    {
			return Priority_3;
    } 
  //--------------------
	//Управление оповещением кнопками СТАРТ и СТОП
	if(ButGetToggleState(CONTROL_BUTTONS) & START_BUTTON)	
		{
			return START_Activation;
		}		
	//--------------------	
  //Приоритет 6 - Ручное управление.
  if(ButGetToggleState(ALG_BUTTONS) & (ALG1_BUTTON | ALG2_BUTTON))
    {
			return Priority_6;
    }
  //--------------------
//  //Управление выходным реле "ПОЖ.3".
//  if(Poj3StatReg == NormCloseSost)RelayOrLedOn (RelPoj3);
//  if(Poj3StatReg == NormOpenSost) RelayOrLedOff(RelPoj3);
		
	return 0;
}
//***************************************************************************************************** 
//*****************************************************************************************************
void Task_FireLineFSM     (void);//Работа с входами ШС.
void Task_SpeakerLineFSM  (void);//Работа с линиями Гр.
void Task_FaultsGet       (void);//Получение неисправностей.
void Task_SystemState     (void);//Определения состояния блока.
void Task_LoggingEvent    (void);//Логирование событий блока.
void Task_Relay           (void);//Управление выходными реле.
void Task_InternalProtocol(void);//Работа с протоколом внутреблочного взаимодействия.

void Task_Begin           (void);//Начало работы.
void Task_ParsingCmdFP    (void);//Выполнение команд от FP.
void Task_ParsingCmdSB    (void);//Выполнение команд от SB.
void Task_GetControl			(void);//

//void Task_GetKeyState       (void);//Положение ключа FP.

void Task_LockModeUnit          (void);//Режим "Блокировка управления"
void Task_ControlModeUnit       (void);//Режим "Управление"
void Task_ConfigModeUnit        (void);//Режим "Настройка"
void Task_PojLineControlModeUnit(void);//Управление по сухим контактам.
//*******************************************************************************************
//*******************************************************************************************
//Работа с входами ШС.
void Task_FireLineFSM(void){

	static uint8_t fireLineNumber = 0;
	//--------------------
	FireLineLogicFSM(fireLineNumber);//определение логического состояния ШС.
	FireLineFaultFSM(fireLineNumber);//определение неисправности ШС.
	if(++fireLineNumber >= NumberOfFireLines) fireLineNumber = 0;//Работа с 4-мя входами ШС.
	//--------------------
	RTOS_SetTimerTask(Task_FireLineFSM, 5);
}
//*******************************************************************************************
//Работа с линиями Гр.
void Task_SpeakerLineFSM(void){
	
	static uint8_t speakerLineNumber = 0;
	//--------------------
	SpeakerLineFSM(speakerLineNumber);
	if(++speakerLineNumber > (SpeakerLineNumbers - 1)) speakerLineNumber = 0;//Работа с 4-мя линиями Гр.	
	//--------------------
	RTOS_SetTimerTask(Task_SpeakerLineFSM, 5);
}
//*******************************************************************************************
//Определение неисправностей блока.
void Task_FaultsGet(void){

	FaultsLoop();
	Task_LoggingEvent();
	//--------------------	
	RTOS_SetTimerTask(Task_FaultsGet, 5);	
}
//*******************************************************************************************
//Определения состояния блока.
void Task_SystemState(void){

//				 uint32_t systemState    = 0;
//	static uint32_t systemStateOld = 0;
//	//--------------------
////	SystemStateSTR.Rise = 0;
////	SystemStateSTR.Fall = 0;
//	//--------------------
//	//Состояние основного питания
//	if(Power()->PowerState == PowerACOk) systemState |= (1 << MainPower);//LogEvent(PowerAcOk, NoParam);
//	//Состояние батареи.
////	if(BatGetState() == BatOk)       systemState |= (1 << );
//	if(Power()->BatState <= BatChargeEnd)systemState |= (1 << 1);//LogEvent(PowerDcOk, NoParam);
//	//--------------------	
//	//Определение управления по положению ключа.	
//	if(FacePanel()->Key == UnitLockModeConst)    systemState |= (1 << 2);//Режим "Блокировка управления"
//	if(FacePanel()->Key == UnitControlModeConst) systemState |= (1 << 3);//Режим "Управление"
//	if(FacePanel()->Key == UnitCofigModeConst)   systemState |= (1 << 4);//Режим "Настройка"
//	//if(KeyStatusReg == UnitPojLineControl) 	RTOS_SetTask(Task_PojLineControlModeUnit);//Управление сухими контактами.
//	//--------------------	
//	SystemStateSTR.Instant = systemState;														      //Фиксация действующео состояния.
//	SystemStateSTR.Rise    = (systemStateOld ^ systemState) & systemState;//Фиксация установки флага.
//	SystemStateSTR.Fall    = (~systemState) & systemStateOld;					    //Фиксация сброса флага.
//	systemStateOld         = systemState;
//	//--------------------	
//	
//	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	//!!!!!!!!!!!!!!!!!!!!!!ОТЛАДКА!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	if(FacePanel()->Key == UnitLockModeConst)    SystemStateSet(LockMode);   //Режим "Блокировка управления"
//	if(FacePanel()->Key == UnitControlModeConst) SystemStateSet(ControlMode);//Режим "Управление"
//	if(FacePanel()->Key == UnitCofigModeConst)   SystemStateSet(ConfigMode); //Режим "Настройка"
	
	
	
	
	
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//--------------------
	RTOS_SetTimerTask(Task_SystemState, 5);	
}
//*******************************************************************************************
//Логирование событий блока.
void Task_LoggingEvent(void){

	uint32_t faultsTemp      = Faults()->Rise;
	uint32_t systemStateTemp = SystemState()->Rise;
	//--------------------
	//логирование состояний системы.
//	if(systemStateTemp & (1 << 0)) LogEvent(PowerAcOk, NoParam);
//	if(systemStateTemp & (1 << 1)) LogEvent(PowerDcOk, NoParam);
//	
//	
//	if(systemStateTemp & (1 << 2)) LogEvent(LockMode,    NoParam);
//	if(systemStateTemp & (1 << 3)) LogEvent(ControlMode, NoParam);
//	if(systemStateTemp & (1 << 4)) LogEvent(ConfigMode,  NoParam);
//	//--------------------
//	//Логирование неисправностей системы.
//	if(faultsTemp & ACFault)  LogEvent(PowerAcFault, NoParam);
//	if(faultsTemp & BatFault) LogEvent(PowerDcFault, NoParam);
//	
//	if(faultsTemp & FireLine1Fault) LogEvent(Poj1_Break, NoParam);
//	if(faultsTemp & FireLine2Fault) LogEvent(Poj2_Break, NoParam);
//	if(faultsTemp & FireLine3Fault) LogEvent(Poj3_Break, NoParam);	

}
//*******************************************************************************************
//Управление выходными реле.
void Task_Relay(void){
	
  //--------------------
  //Управление реле "НЕИСПРАВНОСТЬ ОБЩАЯ".
	if(Faults()->Instant != 0) OutputRelays(On, RelFaultGeneral); //Активация реле "Н.О.".
  else                       OutputRelays(Off, RelFaultGeneral);//Отключение реле "Н.О.". 
  //--------------------
  //Управление реле "НЕИСПРАВНОСТЬ ПИТАНИЯ". 
	if(Faults()->Instant & (ACFault | DCFault | BatFault)) OutputRelays(On, RelFaultPower); //Активация реле "Н.П.". 
  else  																								 OutputRelays(Off, RelFaultPower);//Отключение реле "Н.П.". 
  //--------------------
  //Управление выходным реле "ПОЖ.3".
  if(FireLine(Poj3)->LogicalState == FireLineFIRE) OutputRelays(On, RelPoj3);
  else                                             OutputRelays(Off, RelPoj3);
	//--------------------
  RelayUpdate();//Обновление состояний реле.
	//--------------------
	RTOS_SetTimerTask(Task_Relay, 5); 
}
//*******************************************************************************************
//Работа с протоколом внутреблочного взаимодействия.
void Task_InternalProtocol(void){

	//Анализ данных протокола.
  if(RS485Flags()->FPNewDataFlag) RTOS_SetTask(Task_ParsingCmdFP); 
	if(RS485Flags()->SBNewDataFlag) RTOS_SetTask(Task_ParsingCmdSB);
	//Отсутствует обмен по протоколу.
	if(RS485Flags()->TimeOutFlag) LedRs485Act(Blink(Interval1S));//Моргаем светодиодом.
	else                          LedRs485Act(Blink(Interval50mS));
	//--------------------			
	RTOS_SetTimerTask(Task_InternalProtocol, 1); 
}
//*******************************************************************************************
void Task_Begin(void){	
	
	ConfigLoop();//Сохранение конфигурации в случае ее изменения.
	//--------------------
  //Управление зуммером при нажатии кнопки ОТКЛ.ЗВУК.
  //В режиме "Настройка Блока" управление зуммером не производится.
  if((FacePanel()->Key != UnitCofigModeConst)   &&
    ((FacePanel()->Key == UnitControlModeConst) || (FacePanel()->Key == UnitLockModeConst)))
    {
      //Если активирован режим Тест, то управление зуммером не производится.
      //if(!(MAIN_FlagReg & ZummerTestFlag)) 
				ZummerFault(Faults()->Instant);
    }
	//--------------------
	RTOS_SetTimerTask(Task_Begin, 5); 	
}
//*******************************************************************************************
//Обработка команд от FP.
void Task_ParsingCmdFP(void){
	
	uint8_t temp;
	//--------------------
  //Обработка команд от FP:
  switch(RS485RxBuf()->CmdCode){
		//--------------------
		//Команда "Выдача статуса от FP".
		case(CmdFPStatusConst):
			//if((FaultReg == NoFaults) && !(MAIN_FlagReg & ZummerTestFlag)) ZummerBeepOff();//Сброс зуммера
			//Сохранение даных от FP.
//			FacePanel()->Mic            = RS485RxBufPtr()->BLK[FP_KeyAndMicStatePoint] & MicMaskConst;
//			FacePanel()->Key            = RS485RxBufPtr()->BLK[FP_KeyAndMicStatePoint] & KeyMaskConst;
//			//FacePanel()->ControlButtons = RS485RxBufPtr()->BLK[FP_ControlButtonsPoint];
//			//FacePanel()->AlgButtons     = RS485RxBufPtr()->BLK[FP_PresetButtonsPoint];
//			ButFPControl(RS485RxBufPtr()->BLK[FP_ControlButtonsPoint]);
//			ButFPPreset (RS485RxBufPtr()->BLK[FP_PresetButtonsPoint]);		
		
			//Сохранение даных от FP.
//			FacePanel()->Mic = RS485RxBuf()->Data.FP.KeyAndMicState & MicMaskConst;
//			FacePanel()->Key = RS485RxBuf()->Data.FP.KeyAndMicState & KeyMaskConst;
//			ButFPControl(RS485RxBuf()->Data.FP.Control_Buttons);
//			ButFPPreset (RS485RxBuf()->Data.FP.Alg_Buttons);	
		
			FacePanel()->Mic = RS485RxBuf()->Data.FP.Status.KeyAndMicState & MicMaskConst;
			FacePanel()->Key = RS485RxBuf()->Data.FP.Status.KeyAndMicState & KeyMaskConst;
			ButFPControl(RS485RxBuf()->Data.FP.Status.Control_Buttons);
			ButFPPreset (RS485RxBuf()->Data.FP.Status.Alg_Buttons);	
		break;
		//--------------------
		//Команда установки адрес блока
		case(CmdFPSetAddressConst):
			ZummerBeep(2, 100);                                          //Пикнем по приему команды.
			SetAddress(RS485RxBuf()->Data.FP.Status.Address);            //Установка адреса.
			ConfigSave()->Address = RS485RxBuf()->Data.FP.Status.Address;//Сохранение во флеш микроконтроллера.
		break;
		//--------------------
		//Команда установки группы блока 
		case(CmdFPSetGroupConst):
			ZummerBeep(2, 100);                                  		 //Пикнем по приему команды.
			SetGroup(RS485RxBuf()->Data.FP.Status.Group);            //Установка группы.
			ConfigSave()->Group = RS485RxBuf()->Data.FP.Status.Group;//Сохранение во флеш микроконтроллера.
		break;
		//--------------------
		//Команда установки времени 
		case(CmdFPSetTimeConst):
			ZummerBeep(2, 100);            									 //Пикнем по приему команды.
			TimeUTCSet(RS485RxBuf()->Data.FP.Status.TimeUTC);//Установка времени.
		break;
		//--------------------
		//Команда установки даты 
		case(CmdFPSetDataConst):
			ZummerBeep(2, 100);            									 //Пикнем по приему команды.
			TimeUTCSet(RS485RxBuf()->Data.FP.Status.TimeUTC);//Установка времени.
		break;
		//--------------------
		//Команда влючение тестирования зуммера ЦП
		case(CmdFPTetsZummerConst):
			MAIN_FlagReg |= ZummerTestFlag;
		break;
		//--------------------
		//Команда сохранения опорных значений для Lc.
		//В KeyAndMicState будут линии установленные на контроль.
		//В Address будет номер канала для которого сохряняются опроные значения. 
		//В Group будет отклонения от установившегося значения для фиксации перех-го проц-са на линии. 
		case(CmdFPLcSaveRefConst):
			ZummerBeep(1, 100);//Пикнем по приему команды.
		
			*SpeakerLineControl() = RS485RxBuf()->Data.FP.Status.KeyAndMicState;
			//
			if(SpeakerLineSaveRef(RS485RxBuf()->Data.FP.Status.Address)) 
				{
					ZummerBeep(2, 100);//Пикнем по приему команды.
				}
			//	
		  if(RS485RxBuf()->Data.FP.Status.Group > 0)
				{
					ZummerBeep(3, 100);//Пикнем по приему команды.
					*SpeakerLineDeviation()  = RS485RxBuf()->Data.FP.Status.Group;
					//Сохранение во флеш микроконтроллера.
					ConfigSave()->SpDeviation = *SpeakerLineDeviation();
					ConfigSave()->SpCheckOn   = *SpeakerLineControl();
				}
			else *SpeakerLineDeviation() = LcDeviationDefaultConst;
		break;
		//--------------------
		//Команда получения журнала событий. В (KeyAndMicStatePoint & Addres) номер записи в журнале.
		case(CmdFPGetLogConst):
			if(*(uint16_t*)&RS485RxBuf()->Data.FP.Status.KeyAndMicState == 0xFFFF)
				{
					LogClrNotReadEvents();//Очистка непросмотренных сообщения.
				}
			else
				{//Получение события из журнала и передача его на лицевую панель
					FacePanelGetLog(*(uint16_t*)&RS485RxBuf()->Data.FP.Status.KeyAndMicState);
					RS485StartTx(FacePanelTxLogPtr()->BLK, FacePanelTxLogPtr()->STR.Size);
				}				
		break;
		//--------------------
		//Команда получения параметров одного логического входа.
		//в Address номер входа для которого запрашиваем конфигурацию.
		case(CmdFPGetInputConfigConst):
			//Формирование пакета с контекcтом входа. 	
			FacePanelGetPojLineContext(RS485RxBuf()->Data.FP.Status.KeyAndMicState); 
			RS485StartTx(FacePanelTxPojLineContextPtr()->BLK, FacePanelTxPojLineContextPtr()->STR.Size);
		break;		
		//--------------------
		//Команда сохранения параметров одного входа пожарного шлейфа.
		case(CmdFPSetInputConfigConst):
			ZummerBeep(3, 100);  
			temp = RS485RxBuf()->Data.FP.Status.KeyAndMicState;
		
			FireLine(temp)->LogicalState = FireLineCONTROLOFF;
		  FireLine(temp)->Type         = RS485RxBuf()->Data.FP.Status.Address;
			FireLine(temp)->TimeOut      = RS485RxBuf()->Data.FP.Status.Group;
		
			//Сохранение во флеш микроконтроллера.
			if(temp == Poj1)
				{
					ConfigSave()->FireLine1Type    = FireLine(Poj1)->Type; 
					ConfigSave()->FireLine1Timeout = FireLine(Poj1)->TimeOut;
				}
			if(temp == Poj2)
				{
					ConfigSave()->FireLine2Type    = FireLine(Poj2)->Type; 
					ConfigSave()->FireLine2Timeout = FireLine(Poj2)->TimeOut;
				}				
			if(temp == Poj3)
				{
					ConfigSave()->FireLine3Type    = FireLine(Poj3)->Type; 
					ConfigSave()->FireLine3Timeout = FireLine(Poj3)->TimeOut;
				}				
			if(temp == ChS)
				{
					ConfigSave()->FireLineChSType    = FireLine(ChS)->Type; 
					ConfigSave()->FireLineChSTimeout = FireLine(ChS)->TimeOut;
				}
		break;	
		//--------------------
		//Команда сохранения контроля питания блока.
		case(CmdFPSetPowerCheckConst):
			ZummerBeep(3, 100);
			//Power()->Check.Byte = RS485RxBuf()->Data.FP.KeyAndMicState;
			//Сохранение во флеш микроконтроллера.
			//ConfigSave()->PowerCheckOn = RS485RxBufPtr()->BLK[FP_KeyAndMicStatePoint]; 
		break;			
		//--------------------
		//Команда получения информации о EEPROM.
		case(CmdFPGetEepromConst):
			ZummerBeep(3, 100);
			
			RS485StartTx((uint8_t*)FacePanelGetEepInfo(), FacePanelGetEepInfo()->Size);
		break;			
		//--------------------
		//Неверный код команды.
		default:
			ZummerBeep(3, 200);    
		break;
		//--------------------
  }
  //--------------------
  //Запус приема.
	RS485Flags()->FPNewDataFlag = FlagClr;
  RS485RxClrBuf();
}
//*******************************************************************************************
//Обработка команд от SB.
void Task_ParsingCmdSB(void){
	
  //Сохранение данных от SB.
//	SirenSaveData((uint8_t*)&RS485RxBuf()->Data.SB);
  //--------------------
  //Запуск приема.
	RS485Flags()->SBNewDataFlag = FlagClr;
  RS485RxClrBuf();
}
//***************************************************************************************************** 
//*****************************************************************************************************
void Task_GetControl(void){

  //--------------------
	//Отсутствие обмена с платами.
	if(RS485Flags()->SBNAckFlag) 
		{
			//LogLinkErr(LinkErrSB);
			SirenClrData(); 
		}	
	if(RS485Flags()->FPNAckFlag) 
		{
			//LogLinkErr(LinkErrFP);
			FacePanel()->Key = UnitPojLineControl;
		}
	
//	if(Blink(BlinkInterval100mS))RelayOrLedOn(LedRS485);//Индикация приема пакетов	
//	else	                       RelayOrLedOff(LedRS485);
  //--------------------
	//Определение управления по положению ключа.	
	if(FacePanel()->Key == UnitLockModeConst)   RTOS_SetTask(Task_LockModeUnit);          //Режим "Блокировка управления"
	if(FacePanel()->Key == UnitControlModeConst)RTOS_SetTask(Task_ControlModeUnit);       //Режим "Управление"
	if(FacePanel()->Key == UnitCofigModeConst)  RTOS_SetTask(Task_ConfigModeUnit);        //Режим "Настройка"
	//if(KeyStatusReg == UnitPojLineControl) 	RTOS_SetTask(Task_PojLineControlModeUnit);//Управление сухими контактами.
  //--------------------
	RTOS_SetTimerTask(Task_GetControl, 10);
}
//1111111111111111111111111111111111111111111111111111111111111111111111111111
//1111111111111111111111111111111111111111111111111111111111111111111111111111
//Режим "Блокировка управления"
void Task_LockModeUnit(void){

	//--------------------
  //Режим ТЕСТ.
  if(MAIN_FlagReg & ZummerTestFlag)
    {
      //RTOS_SetTimerTask(Task_Begin, 1); 
      return; 
    }
  //--------------------
  //Если был активирован ТЕСТ, то при переходе в режим Блокировка режим ТЕСТ отключаем.
  //MAIN_FlagReg &= ~ZummerTestFlag;
  //--------------------
  //Приоритет 1(высший). 
  //Активация тангентой микрофона лицевой панели.
  if(FacePanel()->Mic == MicActiveConst)
    {	
			SpeakerLine(SpLine1)->Action = ActionMicOn;//Активация всех зон.
			SpeakerLine(SpLine2)->Action = ActionMicOn;
			
			OutputRelays(Off, RelPoj | RelChS);//Сброс выходных реле.
			OutputRelays(On, RelPusk);         //Активация выходного реле "ПУСК". 
      
      SetMuxPrior(MUX_Mic);               //Мик.Пр. на вход УМ
      AmpControl(AmpOn);                  //Активация УМЗЧ.
    }
  //--------------------
  //Управление кнокой "ПУСК".
  else if(ButTempUnitControl & START_BUTTON) 
    {			
			SpeakerLine(SpLine1)->Action = ActionPuskOn;//Активация всех зон. 
			SpeakerLine(SpLine2)->Action = ActionPuskOn;
			
			OutputRelays(Off, RelPoj | RelChS);//Сброс выходных реле.
			OutputRelays(On, RelPusk);         //Активация выходного реле "ПУСК". 
			
			*VoiceMessageCmd() = PlayMessage;//Запуск сообщения.
      SetMuxPrior(MUX_JQ);             //JQ на вход УМ  
      AmpControl(AmpOn);               //Активация УМЗЧ.
    }
  //--------------------
	//Приоритет 2 - Активация встроенного тревожного сообщения.
	//Активация приоритета осуществляется входным шлейфом «Пож.1» или «Пож.2».
  else if((FireLine(Poj1)->LogicalState == FireLineFIRE) || (FireLine(Poj2)->LogicalState == FireLineFIRE))
    {
      //Активация зоны 1.
      if(FireLine(Poj1)->LogicalState == FireLineFIRE)
				{
					SpeakerLine(SpLine1)->Action = ActionFireLine1On;
					SirenSet(Siren1, SirenOutON);//Активация оповещателя 1.
				}
      else
				{
					SpeakerLine(SpLine1)->Action = ActionOff;
					SirenSet(Siren1, SirenOutOFF);//Отключение оповещателя 1.
				}				
      //Активация зоны 2.	
      if(FireLine(Poj2)->LogicalState == FireLineFIRE)
				{
					SpeakerLine(SpLine2)->Action = ActionFireLine2On;
					SirenSet(Siren2, SirenOutON);//Активация оповещателя 2.
				}
      else
				{
					SpeakerLine(SpLine2)->Action = ActionOff;
					SirenSet(Siren2, SirenOutOFF);//Отключение оповещателя 2.
				}
				
			OutputRelays(Off, RelChS);         //Сброс выходных реле. 
			OutputRelays(On, RelPoj | RelPusk);//Активация выходного реле "ПОЖАР" и "ПУСК".
				
			*VoiceMessageCmd() = PlayMessage;//Запуск сообщения.
      SetMuxPrior(MUX_JQ);             //JQ на вход УМ  
      AmpControl(AmpOn);               //Активация УМЗЧ.
    }
  //--------------------
  //Приоритет 3 - ГО и ЧС. 
  //Активация приоритета осуществляется входным шлейфом «ЧС».
  else if(FireLine(ChS)->LogicalState == FireLineFIRE)
    {
			SpeakerLine(SpLine1)->Action = ActionFireLineChSOn;//Активация всех зон.  
			SpeakerLine(SpLine2)->Action = ActionFireLineChSOn;
			
			OutputRelays(Off, RelPoj);//Сброс выходных реле.
			OutputRelays(On, RelChS); //Активация выходного реле "ГОиЧС".	
			
      SetMuxPrior(MUX_LinIn1);         //Лин.Вх.1 на вход УМ
      AmpControl(AmpOn);               //Активация УМЗЧ.
			*VoiceMessageCmd() = StopMessage;//Останов сообщения.
      //SetMuxPrior(MUX_Multimedia);     //Отладка для Вити
    } 
  //--------------------
  //Приоритет 6 - Ручное управление.
  else if(ButGetToggleState(ALG_BUTTONS) & (ALG1_BUTTON | ALG2_BUTTON))
    {
			OutputRelays(Off, RelPusk | RelPoj | RelChS);//Сброс выходных реле.
			
			*VoiceMessageCmd() = StopMessage;//Останов сообщения.
      SetMuxPrior(MUX_LinIn2);         //Лин.Вх.2 на вход УМ
      AmpControl(AmpOn);               //Активация УМЗЧ.

      if(ButGetToggleState(ALG_BUTTONS) & ALG1_BUTTON)SpeakerLine(SpLine1)->Action = ActionManualOn;//ZoneSet(Zone1, ZoneManualOn); 
      else                                            SpeakerLine(SpLine1)->Action = ActionOff;//ZoneSet(Zone1, ZoneOff);                                         
      if(ButGetToggleState(ALG_BUTTONS) & ALG2_BUTTON)SpeakerLine(SpLine2)->Action = ActionManualOn;//ZoneSet(Zone2, ZoneManualOn); 
      else                                            SpeakerLine(SpLine2)->Action = ActionOff;//ZoneSet(Zone2, ZoneOff);                                    
      //-----------
    }
  //--------------------
  else
    {
			SpeakerLine(SpLine1)->Action = ActionOff; //Сброс зон.
			SpeakerLine(SpLine2)->Action = ActionOff;
			
			OutputRelays(Off, RelPusk | RelPoj | RelChS);//Сброс выходных реле.
			SirenSet(SirenAll, SirenOutOFF); 	           //Отключение оповещателей.
			
      AmpControl(AmpOff);              //Отключение УМЗЧ.
      SetMuxPrior(MUX_AllOff);         //вход УМ на землю
			*VoiceMessageCmd() = StopMessage;//Сброс модуля JQ.
    }
  //--------------------
  //Управление сообщениями.
	VoiceMessage(*VoiceMessageCmd());	
  //--------------------
}
//2222222222222222222222222222222222222222222222222222222222222222222222222222
//2222222222222222222222222222222222222222222222222222222222222222222222222222
//Режим "Управление"
void Task_ControlModeUnit(void){

  //--------------------
  //Режим ТЕСТ.
  if(MAIN_FlagReg & ZummerTestFlag)
    {
      //RTOS_SetTimerTask(Task_Begin, 1); 
      return; 
    }
	//-----------------------------------------------------	
	//ОТЛАДКА	
		
//     if(ButGetToggleState(PRESET_BUTTONS) & PRES1_BUTTON)
//				{
//					//ZoneSet(Zone1, ZoneManualOn); 
//					SpeakerLine(SpLine1)->Action = ActionManualOn;
//				}
//      else 
//        {
//          //ButClrToggleState(PRESET_BUTTONS, PRES1_BUTTON);
//          //ZoneSet(Zone1, ZoneOff);
//					SpeakerLine(SpLine1)->Action = ActionOff;
//        } 
//				
//      if(ButGetToggleState(PRESET_BUTTONS) & PRES2_BUTTON)
//				{
//					//ZoneSet(Zone2, ZoneManualOn);
//					SpeakerLine(SpLine2)->Action = ActionManualOn;
//				}
//      else
//        {
//          //ButClrToggleState(PRESET_BUTTONS, PRES2_BUTTON);
//          //ZoneSet(Zone2, ZoneOff);
//					SpeakerLine(SpLine2)->Action = ActionOff;					
//        }




	//-----------------------------------------------------
	//Определение приоритета.	
	switch(PriorityGet()){
		//--------------------
		//Приоритет 1(высший). 
		//Активация тангентой микрофона лицевой панели.
		case(Priority_1):
			//Активация зон. 
			//ZoneSet(Zone1, ZoneMicOn);//   При нажатии тангенты активируется первая зона.
		  SpeakerLine(SpLine1)->Action = ActionMicOn;
			//Дополнительные зоны можно довключить кнопками Пресетов.
			if(ButGetToggleForMic() & ALG2_BUTTON) 
				{
					//ZoneSet(Zone2, ZoneMicOn);
					SpeakerLine(SpLine2)->Action = ActionMicOn;
				}
      else 
				{
					//ZoneSet(Zone2, ZoneOff);
					SpeakerLine(SpLine2)->Action = ActionOff;
				}
			
			SirenSet(SirenAll, SirenOutOFF);//Отключение оповещателей.
				
			OutputRelays(Off, RelPoj | RelChS);//Сброс выходных реле.
			OutputRelays(On, RelPusk);         //Активация выходного реле "ПУСК". 	
				
      SetMuxPrior(MUX_Mic);           //Мик.Пр. на вход УМ
      AmpControl(AmpOn);              //Активация УМЗЧ.	
		
		break;
		//--------------------
		//Управление кнокой "ПУСК".
		case(PUSK_Activation):
			
			SpeakerLine(SpLine1)->Action = ActionPuskOn;//Активация всех зон кнопкой ПУСК.
			SpeakerLine(SpLine2)->Action = ActionPuskOn;
		
			OutputRelays(Off, RelPoj | RelChS);//Сброс выходных реле.
			OutputRelays(On, RelPusk);         //Активация выходного реле "ПУСК". 
		
      VoiceMessage(PlayMessage);         //Запуск сообщения.
      SetMuxPrior(MUX_JQ);               //JQ на вход УМ  
      AmpControl(AmpOn);                 //Активация УМЗЧ.
		
			ButTempUnitControl |= START_BUTTON;
      if(ButGetInstantState(CONTROL_BUTTONS) & STOP_BUTTON) ButTempUnitControl &= ~START_BUTTON;
			
		break;		
		//--------------------
		//Приоритет 2 - Активация встроенного тревожного сообщения.
		//Активация приоритета осуществляется входным шлейфом «Пож.1» или «Пож.2».
		case(Priority_2):
			
		  //Активация зоны 1.
			if(FireLine(Poj1)->LogicalState == FireLineFIRE)
				{
					//ZoneSet (Zone1, ZoneFireLine1On);
					SpeakerLine(SpLine1)->Action = ActionFireLine1On;
					SirenSet(Siren1, SirenOutON);    //Активация оповещателя 1.
				}
			if(FireLine(Poj1)->LogicalState != FireLineFIRE)
				{
					//ZoneSet (Zone1, ZoneOff);
					SpeakerLine(SpLine1)->Action = ActionOff;
					SirenSet(Siren1, SirenOutOFF);    //Отключение оповещателя 1.
				}	
			
      //Активация зоны 2.	
      if(FireLine(Poj2)->LogicalState == FireLineFIRE)
				{
					//ZoneSet (Zone2, ZoneFireLine2On);
					SpeakerLine(SpLine2)->Action = ActionFireLine2On;
					SirenSet(Siren2, SirenOutON);    //Активация оповещателя 2.
				}
      if(FireLine(Poj2)->LogicalState != FireLineFIRE)
				{
					//ZoneSet (Zone2, ZoneOff);
					SpeakerLine(SpLine2)->Action = ActionOff;
					SirenSet(Siren2, SirenOutOFF);    //Отключение оповещателя 2.
				}
				
		  OutputRelays(Off, RelChS);          //Сброс выходных реле. 
			OutputRelays(Off, RelPoj | RelPusk);//Активация выходного реле "ПОЖАР" и "ПУСК".
				
      VoiceMessage(PlayMessage);      //Запуск сообщения.
      SetMuxPrior(MUX_JQ);            //JQ на вход УМ  
      AmpControl(AmpOn);              //Активация УМЗЧ.
		
		break;
		//--------------------
		case(Priority_3):
			
		break;
		//--------------------
		case(Priority_4):
			
		break;
		//--------------------
		case(Priority_5):
			
		break;
		//--------------------
		//Приоритет 6 - Ручное управление.
		case(Priority_6):	
			
			SirenSet(SirenAll, SirenOutOFF); 				     //Отключение оповещателей.
		  OutputRelays(Off, RelPusk | RelPoj | RelChS);//Сброс выходных реле.
      VoiceMessage(StopMessage);                   //Останов сообщения.
      SetMuxPrior(MUX_LinIn2);                     //Лин.Вх.2 на вход УМ
      AmpControl(AmpOn);                           //Активация УМЗЧ.      
      //SetMuxPrior(MUX_Multimedia);//Отладка для Вити

      if(ButGetToggleState(ALG_BUTTONS) & ALG1_BUTTON)
				{
					//ZoneSet(Zone1, ZoneManualOn); 
					SpeakerLine(SpLine1)->Action = ActionManualOn;
				}
      else 
        {
          //ButClrToggleState(PRESET_BUTTONS, PRES1_BUTTON);
          //ZoneSet(Zone1, ZoneOff);
					SpeakerLine(SpLine1)->Action = ActionOff;
        } 
				
      if(ButGetToggleState(ALG_BUTTONS) & ALG2_BUTTON)
			{
				//ZoneSet(Zone2, ZoneManualOn);
				SpeakerLine(SpLine2)->Action = ActionManualOn;
			}
      else
        {
          //ButClrToggleState(PRESET_BUTTONS, PRES2_BUTTON);
          //ZoneSet(Zone2, ZoneOff);
					SpeakerLine(SpLine2)->Action = ActionOff;					
        }
				
			//Работа с кнопкой "СБРОС". При ее нажатии отключаются все зоны, активированные в ручную.
			if(ButGetInstantState(CONTROL_BUTTONS) & RESET_BUTTON) 
				{
					ButClrToggleState(ALG_BUTTONS, ALL_BUTTON);
					//ZoneSet(ZoneAll, ZoneOff);
					SpeakerLine(SpLine1)->Action = ActionOff;	
					SpeakerLine(SpLine2)->Action = ActionOff;	
				}
		break;
		//--------------------
		//Управление оповещением кнопками СТАРТ и СТОП
		case(START_Activation):
			
			if(ButGetToggleState(ALG_BUTTONS) & ALG1_BUTTON) SpeakerLine(SpLine1)->Action = ActionAutoOn;//ZoneSet(Zone1, ZoneAutoOn); 
      else 																						 SpeakerLine(SpLine1)->Action = ActionOff;//ZoneSet(Zone1, ZoneOff); 
                                                   
      if(ButGetToggleState(ALG_BUTTONS) & ALG2_BUTTON) SpeakerLine(SpLine2)->Action = ActionAutoOn;//ZoneSet(Zone2, ZoneAutoOn);
      else																						 SpeakerLine(SpLine2)->Action = ActionOff;//ZoneSet(Zone2, ZoneOff); 
     
		break;
		//--------------------
		default:
			
			SpeakerLine(SpLine1)->Action = ActionOff;	//Сброс зон.
			SpeakerLine(SpLine2)->Action = ActionOff;				
		
			SirenSet(SirenAll, SirenOutOFF); 				     //Отключение оповещателей.
      OutputRelays(Off, RelPusk | RelPoj | RelChS);//Сброс выходных реле.
		
			AmpControl(AmpOff);         //Отключение УМЗЧ.
      SetMuxPrior(MUX_AllOff);    //вход УМ на землю.
      VoiceMessage(StopMessage);  //Сброс модуля JQ.
		
		break;	
		//--------------------
		}
	//-----------------------------------------------------	
	//	
		
		
		
	//-----------------------------------------------------			
		
		
  //--------------------
//  //Приоритет 1(высший). 
//  //Активация тангентой микрофона лицевой панели.
//  if(MicStatusReg == MicActiveConst)
//    {
//			//LogSaveEvent(Mic_Activ, Zone1Activ);
//			
//			//Активация зон. При нажатии тангенты активируется первая зона. 
//			//Дополнительные зоны можно давключить кнопками Пресетов.
//			ZoneSet(Zone1, ZoneMicOn);    
//			if(ButGetToggleState(PRESET_BUTTONS) & PRES2_BUTTON) ZoneSet(Zone2, ZoneMicOn); 
//      else 
//        {
//          ButClrToggleState(PRESET_BUTTONS, PRES2_BUTTON);
//          ZoneSet(Zone2, ZoneOff); 
//        } 
//			//------------
//      RelayOrLedOff(RelPoj | RelGO);//Сброс выходных реле.      
//      RelayOrLedOn(RelSTART);       //Активация выходного реле "ПУСК".
//      SetMuxPrior(MUX_Mic);         //Мик.Пр. на вход УМ
//      AmpControl(AmpOn);            //Активация УМЗЧ.
//    }
//  //--------------------
//  //Управление кнокой "ПУСК".
//  else if((ButGetInstantState(CONTROL_BUTTONS) & START_BUTTON) || (ButTempUnitControl & START_BUTTON))
//    {
//      ButTempUnitControl |= START_BUTTON;
//      ZoneSet((Zone1 | Zone2), ZoneStartOn);//Активация всех зон, активация кнопкой ПУСК.
//      RelayOrLedOff(RelPoj | RelGO);        //Сброс выходных реле. 
//      RelayOrLedOn(RelSTART);               //Активация выходного реле "ПУСК".
//      JqCmdReg = PlayMessage;               //Запуск сообщения.
//      SetMuxPrior(MUX_JQ);                  //JQ на вход УМ  
//      AmpControl(AmpOn);                    //Активация УМЗЧ.
//      if(ButGetInstantState(CONTROL_BUTTONS) & STOP_BUTTON) ButTempUnitControl &= ~START_BUTTON;//ButClrToggleState(CONTROL_BUTTONS, START_BUTTON);
//    }
//  //--------------------
//  //Приоритет 2 - Активация встроенного тревожного сообщения.
//  //Активация приоритета осуществляется входным шлейфом «Пож.1» или «Пож.2».
//  else if((Poj1StatReg == NormCloseSost) || (Poj2StatReg == NormCloseSost))
//    {
//      //Активация зоны 1.
//      if(Poj1StatReg == NormCloseSost)
//				{
//					ZoneSet (Zone1, ZonePoj1On);//ZoneAutoOn);
//					SirenSet(Siren1, SirenOutON);    //Активация оповещателя 1.
//				}
//      if(Poj1StatReg == NormOpenSost)
//				{
//					ZoneSet (Zone1, ZoneOff);
//					SirenSet(Siren1, SirenOutOFF);    //Отключение оповещателя 1.
//				}				
//      //Активация зоны 2.	
//      if(Poj2StatReg == NormCloseSost)
//				{
//					ZoneSet (Zone2, ZonePoj2On);//ZoneAutoOn);
//					SirenSet(Siren2, SirenOutON);    //Активация оповещателя 2.
//				}
//      if(Poj2StatReg == NormOpenSost)
//				{
//					ZoneSet (Zone2, ZoneOff);
//					SirenSet(Siren2, SirenOutOFF);    //Отключение оповещателя 2.
//				}			
//      RelayOrLedOff(RelGO);						//Сброс выходных реле.
//      RelayOrLedOn(RelPoj | RelSTART);//Активация выходного реле "ПОЖАР" и "ПУСК".
//      JqCmdReg = PlayMessage;         //Запуск сообщения.
//      SetMuxPrior(MUX_JQ);            //JQ на вход УМ  
//      AmpControl(AmpOn);              //Активация УМЗЧ.
//    }
//  //--------------------
//  //Приоритет 3 - ГО и ЧС. 
//  //Активация приоритета осуществляется входным шлейфом «ЧС».
//  else if(ChSStatReg == NormCloseSost)
//    {
//      JqCmdReg = StopMessage;             //Останов голосового сообщения.
//      ZoneSet((Zone1 | Zone2), ZoneChSOn);//Активация всех зон.
//			SirenSet(SirenAll, SirenOutOFF); 		//Отключение оповещателей.
//      RelayOrLedOff(RelSTART | RelPoj);   //Сброс выходных реле.
//      RelayOrLedOn(RelGO);                //Активация выходного реле "ГОиЧС".
//      SetMuxPrior(MUX_LinIn1);            //Лин.Вх.1 на вход УМ
//      AmpControl(AmpOn);                  //Активация УМЗЧ.
//      //SetMuxPrior(MUX_Multimedia);//Отладка для Вити
//    } 
//  //--------------------
//  //Приоритет 6 - Ручное управление.
//  else if(ButGetToggleState(PRESET_BUTTONS) & (PRES1_BUTTON | PRES2_BUTTON))
//    {
//      RelayOrLedOff(RelSTART | RelPoj | RelGO);//Сброс выходных реле.
//      JqCmdReg = StopMessage;                  //Останов сообщения.
//      SetMuxPrior(MUX_LinIn2);                 //Лин.Вх.2 на вход УМ
//      AmpControl(AmpOn);                       //Активация УМЗЧ.      
//      //SetMuxPrior(MUX_Multimedia);//Отладка для Вити

//      if(ButGetToggleState(PRESET_BUTTONS) & PRES1_BUTTON)ZoneSet(Zone1, ZoneManualOn); 
//      else 
//        {
//          ButClrToggleState(PRESET_BUTTONS, PRES1_BUTTON);
//          ZoneSet(Zone1, ZoneOff); 
//        }                                                
//      if(ButGetToggleState(PRESET_BUTTONS) & PRES2_BUTTON) ZoneSet(Zone2, ZoneManualOn);
//      else
//        {
//          ButClrToggleState(PRESET_BUTTONS, PRES2_BUTTON);
//          ZoneSet(Zone2, ZoneOff); 
//        }    
//			//Работа с кнопкой "СБРОС". При ее нажатии отключаются все зоны, активированныке в ручную.
//			if(ButGetInstantState(CONTROL_BUTTONS) & STOP_BUTTON) ButClrToggleState(PRESET_BUTTONS, ALL_BUTTON);
//    }
//  //--------------------
//  else 
//    {
//      AmpControl(AmpOff);                      //Отключение УМЗЧ.
//      SetMuxPrior(MUX_AllOff);                 //вход УМ на землю
//      ZoneSet((Zone1 | Zone2), ZoneOff);       //Сброс зон.
//			SirenSet(SirenAll, SirenOutOFF); 				 //Отключение оповещателей.
//      RelayOrLedOff(RelSTART | RelPoj | RelGO);//Сброс выходных реле.
//      JqCmdReg = StopMessage;                  //Сброс модуля JQ.
//    }
//  //--------------------
//  //Управление выходным реле "ПОЖ.3".
//  if(Poj3StatReg == NormCloseSost)
//		{
//			LogSaveEvent(Poj3_Activ, NoParam);
//			RelayOrLedOn (RelPoj3);
//		}
//  if(Poj3StatReg == NormOpenSost) 
//		{
//			LogSaveEvent(Poj3_Off, NoParam);
//			RelayOrLedOff(RelPoj3);
//		}
//  //--------------------
//  //Управление сообщениями.
//  VoiceMessage(JqCmdReg);
//  //--------------------
//  //RTOS_SetTimerTask(Task_Begin, 1);
}
//3333333333333333333333333333333333333333333333333333333333333333333333333333
//3333333333333333333333333333333333333333333333333333333333333333333333333333
//Режим "Настройка"
void Task_ConfigModeUnit(void){

  //Сброс состояний всех кнопок.
  ButClrToggleState(CONTROL_BUTTONS, ALL_BUTTON);
  ButClrToggleState(ALG_BUTTONS, ALL_BUTTON);
  ButTempUnitControl = 0;
  //---------------------
	SpeakerLine(SpLine1)->Action = ActionOff;//Отключение всех зон.
	SpeakerLine(SpLine2)->Action = ActionOff;//
  SirenSet(SirenAll, SirenOutOFF);         //Отключение оповещателей.
	OutputRelays(Off, RelAllOff);	           //Сброс выходных реле.
  AmpControl(AmpOff);                      //Отключение УМЗЧ.
  SetMuxPrior(MUX_AllOff);                 //вход УМ на землю
  VoiceMessage(StopMessage);               //Останов сообщения.
}
//4444444444444444444444444444444444444444444444444444444444444444444444444444
//4444444444444444444444444444444444444444444444444444444444444444444444444444
//Управление сухими контактами.
void Task_PojLineControlModeUnit(void){

	//LogEvent(PojLineControlMode, NoParam);
  //Приоритет 2 - Активация встроенного тревожного сообщения.
  //Активация приоритета осуществляется входным шлейфом «Пож.1» или «Пож.2».
  if((FireLine(Poj1)->FaultState == FireLineNormClose) || (FireLine(Poj2)->FaultState == FireLineNormClose))
    {
      //Активация зоны 1.
      if(FireLine(Poj1)->FaultState == FireLineNormClose)
				{
					SpeakerLine(SpLine1)->Action = ActionFireLine1On;
					SirenSet(Siren1, SirenOutON);    //Активация оповещателя 1.
				}
      if(FireLine(Poj1)->FaultState == FireLineNormOpen)
				{
					SpeakerLine(SpLine1)->Action = ActionOff;
					SirenSet(Siren1, SirenOutOFF);    //Отключение оповещателя 1.
				}				
      //Активация зоны 2.	
      if(FireLine(Poj2)->FaultState == FireLineNormClose)
				{
					SpeakerLine(SpLine2)->Action = ActionFireLine1On;
					SirenSet(Siren2, SirenOutON);    //Активация оповещателя 2.
				}
      if(FireLine(Poj2)->FaultState == FireLineNormOpen)
				{
					SpeakerLine(SpLine2)->Action = ActionOff;
					SirenSet(Siren2, SirenOutOFF);    //Отключение оповещателя 2.
				}			
			
			OutputRelays(Off, RelChS);         //Сброс выходных реле.
			OutputRelays(On, RelPusk | RelPoj);//Активация выходного реле "ПОЖАР" и "ПУСК".	
				
      //JqCmdReg = PlayMessage;         //Запуск сообщения.
			*VoiceMessageCmd() = PlayMessage;//Запуск сообщения.	
      SetMuxPrior(MUX_JQ);             //JQ на вход УМ  
      AmpControl(AmpOn);               //Активация УМЗЧ.
    }
  //--------------------
  //Приоритет 3 - ГО и ЧС. 
  //Активация приоритета осуществляется входным шлейфом «ЧС».
  else if(FireLine(ChS)->FaultState == FireLineNormClose)
    {
			SpeakerLine(SpLine1)->Action = ActionFireLineChSOn;//Активация всех зон. 
			SpeakerLine(SpLine2)->Action = ActionFireLineChSOn;
			
		  OutputRelays(Off, RelPusk | RelPoj);//Сброс выходных реле.
			OutputRelays(On, RelChS);           //Активация выходного реле "ГОиЧС".			
			
      SetMuxPrior(MUX_LinIn1);         //Лин.Вх.1 на вход УМ
      AmpControl(AmpOn);               //Активация УМЗЧ.
			*VoiceMessageCmd() = StopMessage;//Остнаов сообщения.
    } 
  //--------------------
	else 
    {
			SpeakerLine(SpLine1)->Action = ActionOff;//Сброс зон.
			SpeakerLine(SpLine2)->Action = ActionOff;
			
			SirenSet(SirenAll, SirenOutOFF); 				     //Отключение оповещателей.
			OutputRelays(Off, RelPusk | RelPoj | RelChS);//Сброс выходных реле.
			
      SetMuxPrior(MUX_AllOff);         //вход УМ на землю
			AmpControl(AmpOff);              //Отключение УМЗЧ.
			*VoiceMessageCmd() = StopMessage;//Сброс модуля JQ.
    }
  //--------------------
  //Управление сообщениями.
	VoiceMessage(*VoiceMessageCmd());
  //--------------------
}
//***************************************************************************************************** 
//*****************************************************************************************************
int main(void){
    
  //--------------------
  //После сброса микроконтроллера программа преходит в main(), минуя SystemInit().
  //В фейле startup_stm32f10x_md_vl.s закоментирован переход на SystemInit.
  SysInit();   //Настройка тактирования микроконтроллера. Внешний кварц 8 МГц. 
  GpioInit();  //Инициализация переферии.
  //CRC_Init();
  //EepInit();   //Инициализация EEPROM. Должно быть до Spi2Init().
  Spi2Init();
  AdcInit();
  RtcInit(); //Инициализация RTC.
  Tim1Init();//Инициализация таймеря для работы с зуммером.
  //-------------------- 
  //Инициализация прикладного уровня.
	FireLineInit();
  SpeakerLineInit();
  AmpInit();
  PowerAndChargeInit();
  VoiceMessageInit();
  RS485Init();
	//-------------------- 
	//Инициализация машины состояний.
	SwTimersInit();		//Инициализация программных таймеров.
	FsmMessagesInit();//Инициализация службы сообщений.
	//-------------------- 
  ZuOn();//Активация ЗУ.
  
	StpicCLRHight;
	StpicCSLow;

  SetMuxPrior(MUX_AllOff);//
  MuxELow;                //Включение выходов мультиплексора.
  //--------------------
  //Проверка микросхемы EEPROM.
  //EepMemArrayCheck();
  //--------------------
  //Чтение конфигурации из Flash.
	ConfigInit();
	//Применение конфигурации.
	SetAddress(Config()->Address);
  SetGroup  (Config()->Group);
	Power()->Check.Byte     = Config()->PowerCheckOn;
	*SpeakerLineDeviation() = Config()->SpDeviation;
	*SpeakerLineControl()   = Config()->SpCheckOn;
	FireLine(Poj1)->Type    = Config()->FireLine1Type;
	FireLine(Poj1)->TimeOut = Config()->FireLine1Timeout;	
	FireLine(Poj2)->Type    = Config()->FireLine2Type;
	FireLine(Poj2)->TimeOut = Config()->FireLine2Timeout;	
	FireLine(Poj3)->Type    = Config()->FireLine3Type;
	FireLine(Poj3)->TimeOut = Config()->FireLine3Timeout;	
	FireLine(ChS)->Type     = Config()->FireLineChSType;
	FireLine(ChS)->TimeOut  = Config()->FireLineChSTimeout;				
  //--------------------
  //Установка времени.
//  Time.sec = 00;
//  Time.min = 54;
//  Time.hour= 18;
//  SetCounterRTC(TimeToRtc(&Time));//преобразовать время в формат "понятный" для RTC.
  //SetCounterRTC(0x0000FFFF);
  //--------------------
//	AxelFlag  = 1;
//	PowerGood = 1;
//	BatFlag   = 1;
//	
//	BitBandReg2_0 = 1;
//	BitBandReg2_1 = 1;
//	BitBandReg2_2 = 1;
//	BitBandReg2_3 = 1;
	//--------------------
  RTOS_Init(); 	                     //Инициализация диспетчера
	
	RTOS_SetTask(Task_FireLineFSM);     //
	RTOS_SetTask(Task_SpeakerLineFSM);  //
	RTOS_SetTask(Task_FaultsGet);			  //
	//RTOS_SetTask(Task_SystemState);     //
	RTOS_SetTask(Task_Relay);			      //
	RTOS_SetTask(Task_InternalProtocol);// 
	
	RTOS_SetTask(Task_Begin);           //
	RTOS_SetTask(Task_GetControl);      //Определение управления для блока.     

  SysTickInit();             //Запуск службы таймеров.
  __enable_irq();            //Глобальное разрешение рерываний.
  //--------------------	
	Delay_mSec(500);
//	Logging(Norm, MainPower);//лог "Подано питание".
  //--------------------
  while(1)
    {  
      RTOS_TaskManager();  //Вызов диспетчера ОС.
    }
  //--------------------
}
//************************************************************************************************** 
//************************************************************************************************** 
//Прерываени системного таймера SysTick.
void SysTick_Handler(void){
  	
  static uint16_t mScountForZumTEST = 0;
	//--------------------  
	//Счетчик для отладки.
	//static uint16_t DebugCount = 0;
//  if (++DebugCount >= 10)
//    {
//      DebugCount = 0;
//      //---------
//			//RTOS_SetTask(Task_Begin);
//    }     
  //--------------------
  //Отработка режима "ТЕСТ".
  if(MAIN_FlagReg & ZummerTestFlag)
    { 
      if(Blink(Interval250mS))Zummer(ZummerOn);
      else                    Zummer(ZummerOff);
      //Окончание тестирования.
      if((++mScountForZumTEST >= TESTmSConst) ||//Через 10 секунд или
         (RS485Flags()->TimeOutFlag))           //при отсутствии обмена с FP.
        {
          mScountForZumTEST = 0;
          MAIN_FlagReg &= ~ZummerTestFlag;
        }   
    }
  else 
    {
      mScountForZumTEST = 0;
      ZummerLoop();
    }
  //--------------------
	RS485MasterLoop();	 //Протокол RS485.
	BlinkLoop();				 //Мигающая индикация.
  GpioCheckLoop();     //Антидребезг для всех выводов контроллера.
  PowerAndChargeLoop();//Получение состояния основного ввода питания 220В и напряжения на АКБ.    
  //-------------------- 
  FireLineMeasLoop();  	 			 //Измерение пож.шлейфов.
  SpeakerLineMeasLoop(); 			 //Измерение линий Гр.
	SpeakerLineSteadyStateLoop();//Получение установившегося значения после переходного процесса.
  //--------------------
	SwTimersLoop();     //Виртуальные таймера для машины состояний.
	//--------------------	
  RTOS_TimerService();//Служба таймеров ОС.
  //-------------------- 
}
//************************************************************************************************** 
//************************************************************************************************** 





