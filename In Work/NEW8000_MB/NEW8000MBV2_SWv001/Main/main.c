
#include "main.h"

//-----------------------------------------------------------------------------
#define	MUX_Mic		     1
#define	MUX_Multimedia 2
#define	MUX_LinIn1		 3
#define	MUX_LinIn2		 4
#define	MUX_Priority5  5
#define	MUX_JQ         6
#define	MUX_AllOff		 7

//-----------------------------------------------------------------------------
//Глобальное время.
RtcTimeSTR  Time;

//Регистры хранения состояни от FP
volatile uint8_t MicStatusReg  = 0;
volatile uint8_t KeyStatusReg  = 0;

volatile uint8_t Poj1StatReg   = 0;
volatile uint8_t Poj2StatReg   = 0;
volatile uint8_t Poj3StatReg   = 0;
volatile uint8_t ChSStatReg    = 0;

volatile uint8_t Zone1StatReg  = 0;
volatile uint8_t Zone2StatReg  = 0;
volatile uint8_t Zone3StatReg  = 0;
volatile uint8_t Zone4StatReg  = 0;
volatile uint8_t Zone5StatReg  = 0;
volatile uint8_t Zone6StatReg  = 0;

volatile uint8_t PriorityReg   = 0;

//-----------------------------------
//Управление сообщениями.
volatile uint8_t JqCmdReg = 0;
//#define	PlayMessage		 1
//#define	StopMessage		 2
//#define	ReplayMessage	 3
//#define	PlayMessage1	 4
//#define	PlayMessage2	 5
//-----------------------------------
volatile static uint16_t Blink250mSReg  = 0;
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

//Неисправности 
volatile uint16_t  FaultReg = 0;

//Флаги неисправности.
#define NoFaults            0     //нет неисправностей
#define FaultPoj1_Flag     (1<<0) 
#define FaultPoj2_Flag     (1<<1) 
#define FaultPoj3_Flag     (1<<2) 
#define FaultChS_Flag      (1<<3) 
#define FaultLc1Line_Flag  (1<<4) 
#define FaultLc1Att_Flag   (1<<5) 
#define FaultLc2Line_Flag  (1<<6) 
#define FaultLc2Att_Flag   (1<<7) 
#define FaultACNo_Flag     (1<<8)
#define FaultDCFault_Flag  (1<<9)  
#define FaultBat_Flag      (1<<10)
#define FaultPA_Flag       (1<<11)
#define FaultMic_Flag      (1<<12)
#define FaultConnect_Flag  (1<<13)

//Соятояние кнопок для каждого режима работы блока.
//static uint16_t ButTempUnitLock    = 0;
volatile uint16_t ButTempUnitControl = 0;
//-----------------------------------
//Структура для записи во флеш.
typedef union {
	struct{
    //----------
		uint8_t  ParamAddress; 
		uint8_t  ParamGroup;
    char     ParamTxtSW[2];//
		char     ParamSW[3];	 //Версия ПО
    char     ParamTxtHW[2];//
 		char     ParamHW[3];	 //Версия железа
    uint16_t ParamLcDeviation;
    uint16_t ParamCheck;
    //----------
	}STR;
	unsigned char BLK[16];//Размер должен быть кратен 2м
}ParamForFlashType;

#define ParamForFlashSize 16//Размер
ParamForFlashType  ParamForFlashUNION;
unsigned char     *ParamForFlashPoint;

//-----------------------------------------------------------------------------
ParamForFlashType* SetParamForFlash(void){

  return &ParamForFlashUNION; 
}
//-----------------------------------------------------------------------------
unsigned char* GetAddrParamForFlash(void){
  
  return &ParamForFlashUNION.BLK[0];
}
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
//************************************************************************************************** 
//**************************************************************************************************
//Задачи, вызываемые в прерывании SysTick.
//void Task_RS485ProtocolLoop(void);//Автомат состояний, определяющий порядок выдачи кодограмм.
//void Task_RelayOrLedLoop   (void);//Управление реле.
void Task_BlinkReg          (void);
void Task_Begin             (void);//Начало работы - Получение состояний пофарных шлейфов и линий Гр.
void Task_CheckFault        (void);//Анализ неисправностей блока.

void Task_RS485             (void);//анализ данных протокола
void Task_RS485NoConnect    (void);//Отсутствует обмен по протоколу.

void Task_RS485ParsingCmdFP (void);//Выполнение команд от FP.
void Task_RS485ParsingCmdSB (void);//Выполнение команд от SB.

void Task_GetKeyState       (void);//Положение ключа FP.
//Режим блока.
void Task_UnitLock          (void);//Режим "Блокировка управления"
void Task_UnitControl       (void);//Режим "Управление"
void Task_UnitConfig        (void);//Режим "Настройка"
void Task_UnitPojLineControl(void);//Управление по сухим контактам.

//void Task_VoiceMessage      (void);//Управление модулем сообщений JQ6500.
//*******************************************************************************************
//*********************Задачи, вызываемые в прерывании SysTick.******************************
//Автомат состояний, определяющий порядок выдачи кодограмм.
//void Task_RS485ProtocolLoop(void){

//  RS485MB_SetPriority(PriorityReg);
//  RS485MB_ProtocolLoop();
//}
//----------------------------------------------------------------------------
//Управление реле. - Сам себя запускает.
//void Task_RelayOrLed(void){

//  RelayOrLedLoop();
//}
//----------------------------------------------------------------------------
//Процесс для мигания светодиодами - Сам себя запускает.
void Task_BlinkReg(void){

  //RTOS_SetTimerTask(Task_BlinkReg, 250);
  Blink250mSReg ^= 0xFFFF;
}
//*******************************************************************************************
//*******************************************************************************************
//Получение значений пофарных шлейфов и линий Гр.
void Task_Begin(void){
  
  //Получение значений пофарных шлейфов
  if(MeasFlagGet() & MeasPojLineEndFlag)
    {
      Poj1StatReg = PojLineGetStatus(Poj1);
      Poj2StatReg = PojLineGetStatus(Poj2);
      Poj3StatReg = PojLineGetStatus(Poj3);
      ChSStatReg  = PojLineGetStatus(ChS);
      MeasFlagClr(MeasPojLineEndFlag);
    }
  //Получение значений линий Гр.
//  if(MeasFlagGet() & MeasLcEndFlag)
//    {
//      Zone1StatReg = 0;//ZoneGetStatus(Zone1);
//      Zone2StatReg = 0;//ZoneGetStatus(Zone2);
//      Zone3StatReg = 0;//ZoneGetStatus(Zone3);
//      Zone4StatReg = 0;//ZoneGetStatus(Zone4);
//      Zone5StatReg = 0;//ZoneGetStatus(Zone5);
//      Zone6StatReg = 0;//ZoneGetStatus(Zone6);
//      MeasFlagClr(MeasLcEndFlag);
//    }
  //--------------------
  RTOS_SetTask(Task_CheckFault);
}
//----------------------------------------------------------------------------
//Анализ неисправностей блока. Управление реле "Н.О." и "Н.П.". Управление зуммером.
void Task_CheckFault(void){

  //-------------------- 
  //Питание в норме.
  if(PowerGetStatus() <= PowerDCOk)        FaultReg &= ~(FaultACNo_Flag | FaultDCFault_Flag);      
  //Отсутствует основное питание.
  else if(PowerGetStatus() == PowerACNo)   FaultReg |= FaultACNo_Flag;
  //Неисправен инвертор.
  else if(PowerGetStatus() == PowerDCFault)FaultReg |= FaultDCFault_Flag;
  //АКБ.
  if(PowerBatGetStatus() >= BatAttention) FaultReg |=  FaultBat_Flag;
  else                                    FaultReg &= ~FaultBat_Flag;
  //УМЗЧ.
  if(AmpGetStat() == AmpProt) FaultReg |=  FaultPA_Flag;
  else                        FaultReg &= ~FaultPA_Flag;
  //Линии Гр.
  if(LcGetDefStatus(Lc1Line)  >= LineBreak) FaultReg |=  FaultLc1Line_Flag;
  else                                      FaultReg &= ~FaultLc1Line_Flag;
  if(LcGetDefStatus(Lc1Atten) >= LineBreak) FaultReg |=  FaultLc1Att_Flag;
  else                                      FaultReg &= ~FaultLc1Att_Flag;
  if(LcGetDefStatus(Lc2Line)  >= LineBreak) FaultReg |=  FaultLc2Line_Flag;
  else                                      FaultReg &= ~FaultLc2Line_Flag;
  if(LcGetDefStatus(Lc2Atten) >= LineBreak) FaultReg |=  FaultLc2Att_Flag;
  else                                      FaultReg &= ~FaultLc2Att_Flag;
  //Пожарные шлейфы.
  if(Poj1StatReg > NormCloseSost) FaultReg |=  FaultPoj1_Flag;
  else                            FaultReg &= ~FaultPoj1_Flag;
  if(Poj2StatReg > NormCloseSost) FaultReg |=  FaultPoj2_Flag;
  else                            FaultReg &= ~FaultPoj2_Flag;
  if(Poj3StatReg > NormCloseSost) FaultReg |=  FaultPoj3_Flag;
  else                            FaultReg &= ~FaultPoj3_Flag;
  if(ChSStatReg  > NormCloseSost) FaultReg |=  FaultChS_Flag;
  else                            FaultReg &= ~FaultChS_Flag;
  //МИК.Пр.
  if(MicStatusReg >= MicNotConnectedConst) FaultReg |=  FaultMic_Flag;
  else                                     FaultReg &= ~FaultMic_Flag;
  //Управление зуммером при нажатии кнопки ОТКЛ.ЗВУК.
  //В режиме "Настройка Блока" управление зуммером не производится.
  if((KeyStatusReg != UnitCofigModeConst)   &&
    ((KeyStatusReg == UnitControlModeConst) || (KeyStatusReg == UnitLockModeConst)))
    {
      //Если активирован режим Тест, то управление зуммером не производится.
      if(!(MAIN_FlagReg & ZummerTestFlag)) DisactivSound(FaultReg);
    }
  //--------------------
  //Управление реле "НЕИСПРАВНОСТЬ ОБЩАЯ".
  if((LcGetDefStatus(Lc1Line) != LineNorm)      ||     //Неисправна линия оповещения  1.    
     (LcGetDefStatus(Lc1Atten)!= LineNorm)      ||     //Неисправна линия аттенюатора 1.  
     (LcGetDefStatus(Lc2Line) != LineNorm)      ||     //Неисправна линия оповещения  2.  
     (LcGetDefStatus(Lc2Atten)!= LineNorm)      ||     //Неисправна линия аттенюатора 2. 
     (Poj1StatReg             >= ShortLineSost) ||     //Неисправен шлейф ПОЖАР1. 
     (Poj2StatReg             >= ShortLineSost) ||     //Неисправен шлейф ПОЖАР2.
     (Poj3StatReg             >= ShortLineSost) ||     //Неисправен шлейф ПОЖАР3.
     (ChSStatReg              >= ShortLineSost) ||     //Неисправен шлейф ГОиЧС. 
     (AmpGetStat()            == AmpProt)       ||     //УМЗЧ в защите.
     (MicStatusReg            >= MicNotConnectedConst))//Неисправен микрофон. 
      {
        RelayOrLedOn(RelFaultGenerel);//Активация выходного реле "Н.О."(неисправность общяя).
      }
  else  
      {
        RelayOrLedOff(RelFaultGenerel);//Отключение выходного реле "Н.О.". 
      }
  //--------------------
  //Управление реле "НЕИСПРАВНОСТЬ ПИТАНИЯ". 
  if((PowerGetStatus()    >= PowerACNo) ||
     (PowerBatGetStatus() >= BatAttention))
      {
        RelayOrLedOn (RelFaultPower);//Активация выходного реле "Н.П."(неисправность питания).  
      }
  else  
      {
        RelayOrLedOff(RelFaultPower);//Отключение выходного реле "Н.П.". 
      }
  //--------------------
  RelayOrLedLoop();//Обновление состояний реле.
  //--------------------
  RTOS_SetTask(Task_RS485);
}
//----------------------------------------------------------------------------
//Анализ данных протокола.
void Task_RS485(void){

       if(RS485FlagGet() & RS485_FPNewDataFlag) RTOS_SetTask(Task_RS485ParsingCmdFP); 
	else if(RS485FlagGet() & RS485_SBNewDataFlag) RTOS_SetTask(Task_RS485ParsingCmdSB); 	
  else if(RS485FlagGet() & RS485_TimeOutFlag)   RTOS_SetTask(Task_UnitPojLineControl);  
  else                                          RTOS_SetTask(Task_Begin); 
}
//----------------------------------------------------------------------------
//Нет обмена с платами.
void Task_RS485NoConnect(void){
                                     
  RelayOrLedOn(LedRS485);//Моргаем светодиодом.
  //RS485FlagClr(RS485_AllFlags);
  RS485RxClrBuf();
  //--------------------
  //RTOS_SetTimerTask(Task_RS485NoConnect, 500);
  
}
//----------------------------------------------------------------------------
//Обработка команд от FP.
void Task_RS485ParsingCmdFP(void){

  //RelayOrLedToggle(LedRS485);//Индикация приема пакета от FP
  //--------------------
  //Обработка команд от FP:
  switch(RS485RxGetDataFromBuf(CommandCodePoint))
    {
      //-----------
      //Команда "Выдача статуса от FP".
      case(CmdFPStatusConst):
        //if((FaultReg == NoFaults) && !(MAIN_FlagReg & ZummerTestFlag)) ZummerBeepOff();//Сброс зуммера
        //Сохранение даных от FP.
        MicStatusReg = RS485RxGetDataFromBuf(FP_KeyAndMicStatePoint) & MicMaskConst;
        KeyStatusReg = RS485RxGetDataFromBuf(FP_KeyAndMicStatePoint) & KeyMaskConst;
        ButFPControl  (RS485RxGetDataFromBuf(FP_ControlButtonsPoint));
        ButFPPreset   (RS485RxGetDataFromBuf(FP_PresetButtonsPoint));
      break;
      //-----------
      //Команда установки адрес блока
      case(CmdFPSetAddressConst):
        ZummerBeep(2, 150);    //Пикнем по приему команды.
        SetAddress(RS485RxGetDataFromBuf(FP_AddressPoint));//Установка адреса.
        //Сохранеие во флеш микроконтроллера.
        SetParamForFlash()->STR.ParamAddress = GetAddress();
        Stm32FlashWriteBuf(GetAddrParamForFlash(), ParamForFlashSize);  
      break;
      //-----------
      //Команда устновки группы блока 
      case(CmdFPSetGroupConst):
        ZummerBeep(2, 150);    //Пикнем по приему команды.
        SetGroup(RS485RxGetDataFromBuf(FP_GroupPoint));//Установка группы.
        //Сохранеие во флеш микроконтроллера.
        SetParamForFlash()->STR.ParamGroup   = GetGroup();
        Stm32FlashWriteBuf(GetAddrParamForFlash(), ParamForFlashSize);
      break;
      //-----------
      //Команда установики времени 
      case(CmdFPSetTimeConst):
        ZummerBeep(2, 150);               //Пикнем по приему команды.
        SetCounterRTC(RS48RxGetTimeUTC());//Установка времени.
      break;
      //-----------
      //Команда влючение тестирования зуммера ЦП
      case(CmdFPTetsZummerConst):
        MAIN_FlagReg |= ZummerTestFlag;
      break;
      //-----------
      //Команда сохранения опорных значений для Lc.
      case(CmdFPLcSaveRefConst):
        //Сохранение опорных значений происходит только после того как закончен переходной процесс.
        if(LcSaveRef(RS485RxGetDataFromBuf(FP_AddressPoint))) ZummerBeep(2, 150);//Пикнем по приему команды.
      break;
      //-----------
      //Команда установки отклонения от установившегося значения для фиксации перех-го проц-са на линии Гр.
      case(CmdFPLcSetDeviationConst):
        ZummerBeep(2, 150);    //Пикнем по приему команды.
        if(RS485RxGetDataFromBuf(FP_AddressPoint) != 0) LcSetDeviation(RS485RxGetDataFromBuf(FP_AddressPoint));
        else                                            LcSetDeviation(LcDeviationConst);
        //Сохранеие во флеш микроконтроллера.
        SetParamForFlash()->STR.ParamLcDeviation = LcGetDeviation(); 
        Stm32FlashWriteBuf(GetAddrParamForFlash(), ParamForFlashSize);
      break;
      //-----------
      //Если неверный код команды то через 1мС повторный анализ данных.
      default:
        ZummerBeep(5, 200);    
        RTOS_SetTimerTask(Task_RS485, 1);
      break;
      //-----------
    }
  //--------------------
  //Запус приема.
  RS485FlagClr(RS485_FPNewDataFlag);
  RS485RxClrBuf();
  //--------------------
  RTOS_SetTask(Task_GetKeyState);
}
//----------------------------------------------------------------------------
//Обработка команд от SB.
void Task_RS485ParsingCmdSB(void){
	
  RelayOrLedToggle(LedRS485);//Индикация приема пакета от SB.
  //--------------------
  //Обработка команд от SB.
	SirenSaveStatus(RS485RxGetBufPtr()->BLK);
  //--------------------
  //Запус приема.
  RS485FlagClr(RS485_SBNewDataFlag);
  RS485RxClrBuf();
  //--------------------
  RTOS_SetTask(Task_GetKeyState);
}
//*******************************************************************************************
//*******************************************************************************************
//Положение ключа.
void Task_GetKeyState(void){

  static uint16_t FlagRegTemp = 0;
  //-------------------- 
  if(MAIN_FlagReg != FlagRegTemp)
    {
      if(MicStatusReg > MicActiveConst)MAIN_FlagReg |=  MicFaultFlag;
      else                             MAIN_FlagReg &= ~MicFaultFlag;

      if(Poj1StatReg > NormCloseSost)  MAIN_FlagReg |=  Poj1FaultFlag;
      else                             MAIN_FlagReg &= ~Poj1FaultFlag;

      if(Poj2StatReg > NormCloseSost)  MAIN_FlagReg |=  Poj2FaultFlag;
      else                             MAIN_FlagReg &= ~Poj2FaultFlag;

      if(Poj3StatReg > NormCloseSost)  MAIN_FlagReg |=  Poj3FaultFlag;
      else                             MAIN_FlagReg &= ~Poj3FaultFlag;

      if(ChSStatReg > NormCloseSost)   MAIN_FlagReg |=  ChSFaultFlag;
      else                             MAIN_FlagReg &= ~ChSFaultFlag;
      FlagRegTemp = MAIN_FlagReg;
    }
  //-------------------- 
  switch(KeyStatusReg){
      //-----------
      //Режим "Блокировка управления"
      case(UnitLockModeConst):
        RTOS_SetTask(Task_UnitLock);
      break;
      //-----------
      //Режим "Управление"
      case(UnitControlModeConst):
        RTOS_SetTask(Task_UnitControl);
      break;
      //-----------
      //Режим "Настройка"
      case(UnitCofigModeConst):
        RTOS_SetTask(Task_UnitConfig);
      break;
      //-----------
      //Непределенный режим.
      default:
        RTOS_SetTimerTask(Task_RS485, 1);
      break;
      //-----------
    }    
  //--------------------
}
//1111111111111111111111111111111111111111111111111111111111111111111111111111
//1111111111111111111111111111111111111111111111111111111111111111111111111111
//Режим "Блокировка управления"
void Task_UnitLock(void){

  //--------------------
  //Если был активирован ТЕСТ, то при переходе в режим Блокировка режим ТЕСТ отключаем.
  MAIN_FlagReg &= ~ZummerTestFlag;
  //--------------------
  //Приоритет 1(высший). 
  //Активация тангентой микрофона лицевой панели.
  if(MicStatusReg == MicActiveConst)
    {
      ZoneSet((Zone1 | Zone2), ZoneMicOn);//Активация всех зон.
      RelayOrLedOff(RelPoj | RelGO);      //Сброс выходных реле.      
      RelayOrLedOn(RelSTART);             //Активация выходного реле "ПУСК".       
      SetMuxPrior(MUX_Mic);               //Мик.Пр. на вход УМ
      AmpControl(AmpOn);                  //Активация УМЗЧ.
      //PriorityReg = Mic;
    }
  //--------------------
  //Управление кнокой "ПУСК".
  else if(ButTempUnitControl & START_BUTTON) //if(ButGetToggleState(CONTROL_BUTTONS) & START_BUTTON)
    {
      ZoneSet((Zone1 | Zone2), ZoneStartOn);//Активация всех зон. 
      RelayOrLedOff(RelPoj | RelGO);        //Сброс выходных реле. 
      RelayOrLedOn(RelSTART);               //Активация выходного реле "ПУСК".
      JqCmdReg = PlayMessage;               //Запуск сообщения.
      SetMuxPrior(MUX_JQ);                  //JQ на вход УМ  
      AmpControl(AmpOn);                    //Активация УМЗЧ.
    }
  //--------------------
  //Приоритет 2 - Активация встроенного тревожного сообщения.
  //Активация приоритета осуществляется входным шлейфом «Пож.1» или «Пож.2».
  else if((Poj1StatReg == NormCloseSost) || (Poj2StatReg == NormCloseSost))
    {
      //Активация зон.
      if(Poj1StatReg == NormCloseSost)ZoneSet(Zone1, ZonePoj1On);//ZoneAutoOn); 
      if(Poj1StatReg == NormOpenSost) ZoneSet(Zone1, ZoneOff);

      if(Poj2StatReg == NormCloseSost)ZoneSet(Zone2, ZonePoj2On);//ZoneAutoOn); 
      if(Poj2StatReg == NormOpenSost) ZoneSet(Zone2, ZoneOff);

      RelayOrLedOff(RelSTART | RelGO);//Сброс выходных реле.
      RelayOrLedOn(RelPoj);  //Активация выходного реле "ПОЖАР".
      JqCmdReg = PlayMessage;//Запуск сообщения.
      SetMuxPrior(MUX_JQ);   //JQ на вход УМ  
      AmpControl(AmpOn);     //Активация УМЗЧ.
    }
  //--------------------
  //Приоритет 3 - ГО и ЧС. 
  //Активация приоритета осуществляется входным шлейфом «ЧС».
  else if(ChSStatReg == NormCloseSost)
    {
      JqCmdReg = StopMessage;             //Остнаов сообщения.
      ZoneSet((Zone1 | Zone2), ZoneChSOn);//Активация всех зон.  
      RelayOrLedOff(RelSTART | RelPoj);   //Сброс выходных реле. 
      RelayOrLedOn(RelGO);                //Активация выходного реле "ГОиЧС".   
      SetMuxPrior(MUX_LinIn1);            //Лин.Вх.1 на вход УМ
      AmpControl(AmpOn);                  //Активация УМЗЧ.
      //SetMuxPrior(MUX_Multimedia);//Отладка для Вити
    } 
  //--------------------
  //Приоритет 6 - Ручное управление.
  else if(ButGetToggleState(PRESET_BUTTONS) & (PRES1_BUTTON | PRES2_BUTTON))
    {
      RelayOrLedOff(RelSTART | RelPoj | RelGO);//Сброс выходных реле.
      JqCmdReg = StopMessage;                  //Остнаов сообщения.
      SetMuxPrior(MUX_LinIn2);                 //Лин.Вх.2 на вход УМ
      AmpControl(AmpOn);                       //Активация УМЗЧ.

      if(ButGetToggleState(PRESET_BUTTONS) & PRES1_BUTTON)ZoneSet(Zone1, ZoneManualOn); 
      else                                                ZoneSet(Zone1, ZoneOff);                                         
      if(ButGetToggleState(PRESET_BUTTONS) & PRES2_BUTTON)ZoneSet(Zone2, ZoneManualOn); 
      else                                                ZoneSet(Zone2, ZoneOff);                                    
      //-----------
    }
  //--------------------
  else
    {
      AmpControl(AmpOff);                      //Отключение УМЗЧ.
      SetMuxPrior(MUX_AllOff);                 //вход УМ на землю
      RelayOrLedOff(RelSTART | RelPoj | RelGO);//Сброс выходных реле.
      ZoneSet((Zone1 | Zone2), ZoneOff);       //Сброс зон.
      JqCmdReg = StopMessage;                  //Сброс модуля JQ.
    }
  //--------------------
  //Управление выходным реле "ПОЖ.3".
  if(Poj3StatReg == NormCloseSost)RelayOrLedOn (RelPoj3);
  if(Poj3StatReg == NormOpenSost) RelayOrLedOff(RelPoj3);
  //--------------------
  //Управление сообщениями.
  VoiceMessage(JqCmdReg);
  //--------------------
  RTOS_SetTimerTask(Task_Begin, 1);
}
//2222222222222222222222222222222222222222222222222222222222222222222222222222
//2222222222222222222222222222222222222222222222222222222222222222222222222222
//Режим "Управление"
void Task_UnitControl(void){

  //--------------------
  //Режим ТЕСТ.
  if(MAIN_FlagReg & ZummerTestFlag)
    {
      //RTOS_SetTask(Task_VoiceMessage);
      RTOS_SetTimerTask(Task_Begin, 1); 
      return; 
    }
  //--------------------
  //Приоритет 1(высший). 
  //Активация тангентой микрофона лицевой панели.
  if(MicStatusReg == MicActiveConst)
    {
      ZoneSet((Zone1 | Zone2), ZoneMicOn);//Активация всех зон. 
      RelayOrLedOff(RelPoj | RelGO);      //Сброс выходных реле.      
      RelayOrLedOn(RelSTART);             //Активация выходного реле "ПУСК".
      SetMuxPrior(MUX_Mic);               //Мик.Пр. на вход УМ
      AmpControl(AmpOn);                  //Активация УМЗЧ.
    }
  //--------------------
  //Управление кнокой "ПУСК".
  else if((ButGetInstantState(CONTROL_BUTTONS) & START_BUTTON) || (ButTempUnitControl & START_BUTTON))
    {
      ButTempUnitControl |= START_BUTTON;
      ZoneSet((Zone1 | Zone2), ZoneStartOn);//Активация всех зон, активация кнопкой ПУСК.
      RelayOrLedOff(RelPoj | RelGO);        //Сброс выходных реле. 
      RelayOrLedOn(RelSTART);               //Активация выходного реле "ПУСК".
      JqCmdReg = PlayMessage;               //Запуск сообщения.
      SetMuxPrior(MUX_JQ);                  //JQ на вход УМ  
      AmpControl(AmpOn);                    //Активация УМЗЧ.
      if(ButGetInstantState(CONTROL_BUTTONS) & STOP_BUTTON) ButTempUnitControl &= ~START_BUTTON;//ButClrToggleState(CONTROL_BUTTONS, START_BUTTON);
    }
  //--------------------
  //Приоритет 2 - Активация встроенного тревожного сообщения.
  //Активация приоритета осуществляется входным шлейфом «Пож.1» или «Пож.2».
  else if((Poj1StatReg == NormCloseSost) || (Poj2StatReg == NormCloseSost))
    {
      //Активация зон.
      if(Poj1StatReg == NormCloseSost)
			{
				ZoneSet(Zone1, ZonePoj1On);//ZoneAutoOn);
				
//			SirenControl(Siren1On);    //Активация оповещателя.
//				Siren_RS485TxRequest()->STR.CmdCode = SB_CmdControlConst;
//				Siren_RS485TxRequest()->STR.Siren1Status = 1;
			}
			
      if(Poj1StatReg == NormOpenSost)
			{
				ZoneSet(Zone1, ZoneOff);
//				Siren_RS485TxRequest()->STR.CmdCode = SB_CmdControlConst;
//				Siren_RS485TxRequest()->STR.Siren1Status = 0;
			}				
			
			
			
			

      if(Poj2StatReg == NormCloseSost)ZoneSet(Zone2, ZonePoj2On);//ZoneAutoOn); 
      if(Poj2StatReg == NormOpenSost) ZoneSet(Zone2, ZoneOff);

      RelayOrLedOff(RelSTART | RelGO);//Сброс выходных реле.
      RelayOrLedOn(RelPoj);           //Активация выходного реле "ПОЖАР".
      JqCmdReg = PlayMessage;         //Запуск сообщения.
      SetMuxPrior(MUX_JQ);            //JQ на вход УМ  
      AmpControl(AmpOn);              //Активация УМЗЧ.
    }
  //--------------------
  //Приоритет 3 - ГО и ЧС. 
  //Активация приоритета осуществляется входным шлейфом «ЧС».
  else if(ChSStatReg == NormCloseSost)
    {
      JqCmdReg = StopMessage;             //Останов голосового сообщения.
      ZoneSet((Zone1 | Zone2), ZoneChSOn);//Активация всех зон.
      RelayOrLedOff(RelSTART | RelPoj);   //Сброс выходных реле.
      RelayOrLedOn(RelGO);                //Активация выходного реле "ГОиЧС".
      SetMuxPrior(MUX_LinIn1);            //Лин.Вх.1 на вход УМ
      AmpControl(AmpOn);                  //Активация УМЗЧ.
      //SetMuxPrior(MUX_Multimedia);//Отладка для Вити
    } 
  //--------------------
  //Приоритет 6 - Ручное управление.
  else if(ButGetToggleState(PRESET_BUTTONS) & (PRES1_BUTTON | PRES2_BUTTON))
    {
      RelayOrLedOff(RelSTART | RelPoj | RelGO);//Сброс выходных реле.
      JqCmdReg = StopMessage;                  //Остнаов сообщения.
      SetMuxPrior(MUX_LinIn2);                 //Лин.Вх.2 на вход УМ
      AmpControl(AmpOn);                       //Активация УМЗЧ.      
      //SetMuxPrior(MUX_Multimedia);//Отладка для Вити

      if(ButGetToggleState(PRESET_BUTTONS) & PRES1_BUTTON)ZoneSet(Zone1, ZoneManualOn); 
      else 
        {
          ButClrToggleState(PRESET_BUTTONS, PRES1_BUTTON);
          ZoneSet(Zone1, ZoneOff); 
        }                                                
      if(ButGetToggleState(PRESET_BUTTONS) & PRES2_BUTTON) ZoneSet(Zone2, ZoneManualOn);
      else
        {
          ButClrToggleState(PRESET_BUTTONS, PRES2_BUTTON);
          ZoneSet(Zone2, ZoneOff); 
        }                                     
    }
  //--------------------
  else 
    {
      AmpControl(AmpOff);                      //Отключение УМЗЧ.
      SetMuxPrior(MUX_AllOff);                 //вход УМ на землю
      ZoneSet((Zone1 | Zone2), ZoneOff);       //Сброс зон.
      RelayOrLedOff(RelSTART | RelPoj | RelGO);//Сброс выходных реле.
      JqCmdReg = StopMessage;                  //Сброс модуля JQ.
    }
  //--------------------
  //Управление выходным реле "ПОЖ.3".
  if(Poj3StatReg == NormCloseSost)RelayOrLedOn (RelPoj3);
  if(Poj3StatReg == NormOpenSost) RelayOrLedOff(RelPoj3);
  //--------------------
  //Управление сообщениями.
  VoiceMessage(JqCmdReg);
  //--------------------
  RTOS_SetTimerTask(Task_Begin, 1);
}
//3333333333333333333333333333333333333333333333333333333333333333333333333333
//3333333333333333333333333333333333333333333333333333333333333333333333333333
//Режим "Настройка"
void Task_UnitConfig(void){

  //Сброс состояний всех кнопок.
  ButClrToggleState(CONTROL_BUTTONS, ALL_BUTTON);
  ButClrToggleState(PRESET_BUTTONS, ALL_BUTTON);
  ButTempUnitControl = 0;
  //Отключение всех зон.
  ZoneSet(ZoneAll, ZoneOff);
  //Сброс выходных реле.
  RelayOrLedOff(RelSTART | RelFaultGenerel | RelFaultPower | 
                RelPoj3  | RelPoj | RelGO);                  

  AmpControl(AmpOff);       //Отключение УМЗЧ.
  SetMuxPrior(MUX_AllOff);  //вход УМ на землю
  VoiceMessage(StopMessage);//Останов сообщения.
  //--------------------
  RTOS_SetTask(Task_RS485);
}
//4444444444444444444444444444444444444444444444444444444444444444444444444444
//4444444444444444444444444444444444444444444444444444444444444444444444444444
//Управление сухими контактами.
void Task_UnitPojLineControl(void){

  //Приоритет 2 - Активация встроенного тревожного сообщения.
  //Активация приоритета осуществляется входным шлейфом «Пож.1» или «Пож.2».
  if((Poj1StatReg == NormCloseSost) || (Poj2StatReg == NormCloseSost))
    {
      //Активация зон.
      if(Poj1StatReg == NormCloseSost)ZoneSet(Zone1, ZoneAutoOn); 
      if(Poj1StatReg == NormOpenSost) ZoneSet(Zone1, ZoneOff);

      if(Poj2StatReg == NormCloseSost)ZoneSet(Zone2, ZoneAutoOn); 
      if(Poj2StatReg == NormOpenSost) ZoneSet(Zone2, ZoneOff);

      JqCmdReg = PlayMessage;//Запуск сообщения.
      SetMuxPrior(MUX_JQ);   //JQ на вход УМ  
      AmpControl(AmpOn);     //Активация УМЗЧ.
      //-----------
    }
  //--------------------
  //Приоритет 3 - ГО и ЧС. 
  //Активация приоритета осуществляется входным шлейфом «ЧС».
  else if(ChSStatReg == NormCloseSost)
    {
      JqCmdReg = StopMessage;             //Останов голосового сообщения.
      ZoneSet((Zone1 | Zone2), ZoneChSOn);//Активация всех зон.               
      SetMuxPrior(MUX_LinIn1);            //Лин.Вх.1 на вход УМ
      AmpControl(AmpOn);                  //Активация УМЗЧ.
    } 
  //--------------------
  else
    {
      AmpControl(AmpOff);               //Отключение УМЗЧ.
      SetMuxPrior(MUX_AllOff);          //вход УМ на землю
      ZoneSet((Zone1 | Zone2), ZoneOff);//Сброс зон.
      JqCmdReg = StopMessage;           //Сброс модуля JQ.
    }
  //--------------------
  //Управление сообщениями.
  VoiceMessage(JqCmdReg);
  //--------------------
  RTOS_SetTimerTask(Task_Begin, 1);
}
//************************************************************************************************** 
//**************************************************************************************************
int main(void){
    
  //--------------------
  //После сброса микроконтроллера программа преходит в main(), минуя SystemInit().
  //В фейле startup_stm32f10x_md_vl.s закоментирован переход на SystemInit.
  SysInit();   //Настройка тактирования микроконтроллера. Внешний кварц 12 МГц. 
  GpioInit();  //Инициализация переферии.
  //CRC_Init();
  //EepInit();   //Инициализация EEPROM. Должно быть до Spi2Init().
  Spi2Init();
  AdcInit();
  RtcInit(); //Инициализация RTC.
  Tim1Init();//Инициализация таймеря для работы с зуммером.
  //-------------------- 
  //Инициализация прикладного уровня.
  LcInit();
  AmpInit();
  PowerAndChargeInit();
  JQInit();
  RS485Init();

  ZuOn;//Активация ЗУ.
  
	StpicCLRHight;
	StpicCSLow;

  SetMuxPrior(MUX_AllOff);//
  MuxELow;                //Включение выходов мультиплексора.
  //--------------------
  //Проверка микросхемы EEPROM.
  //EepMemArrayCheck();
  //--------------------
  //Работа с Flash.
  if((Stm32FlashRead(FlashPage1)   == 0xFFFFFFFF) &&
     (Stm32FlashRead(FlashPage1+4) == 0xFFFFFFFF))
    {
      //Запись заводских установок во флеш.
      SetAddress(1);
      SetGroup(1);
   
      SetParamForFlash()->STR.ParamAddress     = 1;
      SetParamForFlash()->STR.ParamGroup       = 1;
      SetParamForFlash()->STR.ParamTxtSW[0]    = 'S';
      SetParamForFlash()->STR.ParamTxtSW[1]    = 'W';
      SetParamForFlash()->STR.ParamSW[0]       = '0';
      SetParamForFlash()->STR.ParamSW[1]       = '0';
      SetParamForFlash()->STR.ParamSW[2]       = '1';
      SetParamForFlash()->STR.ParamTxtHW[0]    = 'H';
      SetParamForFlash()->STR.ParamTxtHW[1]    = 'W';
      SetParamForFlash()->STR.ParamHW[0]       = '0';
      SetParamForFlash()->STR.ParamHW[1]       = '0';
      SetParamForFlash()->STR.ParamHW[2]       = '1';
      SetParamForFlash()->STR.ParamLcDeviation = 1;
      SetParamForFlash()->STR.ParamCheck       = 0xABCD;

      Stm32FlashWriteBuf(GetAddrParamForFlash(), ParamForFlashSize);
    }
  else
    {
      //Чтение настроек из флеш.
      SetAddress((uint8_t) Stm32FlashRead(FlashPage1));
      SetGroup  ((uint8_t)(Stm32FlashRead(FlashPage1)>>8));
      //Чтение настроек из флеш.
      SetParamForFlash()->STR.ParamAddress    = (uint8_t)( 0x000000FF & Stm32FlashRead(FlashPage1));
      SetParamForFlash()->STR.ParamGroup      = (uint8_t)((0x0000FF00 & Stm32FlashRead(FlashPage1))>>8);
      SetParamForFlash()->STR.ParamTxtSW[0]   = (uint8_t)((0x00FF0000 & Stm32FlashRead(FlashPage1))>>16);
      SetParamForFlash()->STR.ParamTxtSW[1]   = (uint8_t)((0xFF000000 & Stm32FlashRead(FlashPage1))>>24);

      SetParamForFlash()->STR.ParamSW[0]      = (uint8_t)( 0x000000FF & Stm32FlashRead(FlashPage1+4));
      SetParamForFlash()->STR.ParamSW[1]      = (uint8_t)((0x0000FF00 & Stm32FlashRead(FlashPage1+4))>>8);
      SetParamForFlash()->STR.ParamSW[2]      = (uint8_t)((0x00FF0000 & Stm32FlashRead(FlashPage1+4))>>16);
      SetParamForFlash()->STR.ParamTxtHW[0]   = (uint8_t)((0xFF000000 & Stm32FlashRead(FlashPage1+4))>>24);

      SetParamForFlash()->STR.ParamTxtHW[1]   = (uint8_t)( 0x000000FF & Stm32FlashRead(FlashPage1+8));
      SetParamForFlash()->STR.ParamHW[0]      = (uint8_t)((0x0000FF00 & Stm32FlashRead(FlashPage1+8))>>8);
      SetParamForFlash()->STR.ParamHW[1]      = (uint8_t)((0x00FF0000 & Stm32FlashRead(FlashPage1+8))>>16);
      SetParamForFlash()->STR.ParamHW[2]      = (uint8_t)((0xFF000000 & Stm32FlashRead(FlashPage1+8))>>24);

      SetParamForFlash()->STR.ParamLcDeviation = (uint16_t)( 0x0000FFFF & Stm32FlashRead(FlashPage1+12));
      SetParamForFlash()->STR.ParamCheck       = (uint16_t)((0xFFFF0000 & Stm32FlashRead(FlashPage1+12))>>16);
    }

  LcSetDeviation(SetParamForFlash()->STR.ParamLcDeviation);
  //--------------------
  //Установка времени.
//  Time.sec = 00;
//  Time.min = 54;
//  Time.hour= 18;
//  SetCounterRTC(TimeToRtc(&Time));//преобразовать время в формат "понятный" для RTC.
  //SetCounterRTC(0x0000FFFF);
  //--------------------
  //После окончения пер.процессов сохранение значений линий как опорных.
  //LcSaveRef();   //Сохранение опорных величин линий Гр. дя LC.
  //--------------------
  //После окончения пер.процессов сохранение значений линий как опорных.
  //LcSaveRefState();   //Сохранение опорных величин линий Гр. дя LC.
  //--------------------
  RTOS_Init(); 	           //Инициализация диспетчера
  RTOS_SetTask(Task_Begin);//Постановка задач в очередь.  
    
  SysTickInit();           //Запуск службы таймеров.
  __enable_irq();          //Глобальное разрешение рерываний.
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
  static uint16_t mScountForBlink   = 0;
  //--------------------
  if(++mScountForBlink >= 250)
    {
      mScountForBlink = 0;
      RTOS_SetTask(Task_BlinkReg);
    }
  //--------------------
  //Отработка режима "ТЕСТ".
  if(MAIN_FlagReg & ZummerTestFlag)
    { 
      if(Blink250mSReg)Zummer(ZummerOn);
      else             Zummer(ZummerOff);
      //Окончание тестирования.
      if((++mScountForZumTEST >= 20000) ||    //Через 20 секунд или
         (RS485FlagGet() & RS485_TimeOutFlag))//при отсутствии обмена с ЦП.
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
  GpioCheck();           //Антидребезг для всех выводов контроллера.
  PowerGetACStatus();    //Определение состояния основного питания ~230В      
  //-------------------- 
  PojLineMeasLoop();     //Измерение пож.шлейфов и АКБ.
  LcMeasLoop();          //Измерение линий Гр.
  LcSteadyStateLoop();   //Получение установившегося значения после переходного процесса.
  //--------------------
  RS485MasterLoop();		 //Протокол RS485.
  //--------------------
  RTOS_TimerService();   //Служба таймеров ОС.
  //-------------------- 
  //Счетчик для отладки.
//  if (++DebugCount >= 500)
//    {
//      DebugCount = 0;
//      //---------

//    }    
  //--------------------   
}
//************************************************************************************************** 
//************************************************************************************************** 





