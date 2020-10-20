
#include "main.h"

//-----------------------------------------------------------------------------
//Глобальное переменные
//RtcTimeSTR_TypeDef  TimeUTC;
static uint16_t  Command  = 0;
static uint16_t  Address  = 1;
static uint16_t  Group    = 1;
static uint32_t  TimeUTC  = 0;

//Неисправности 
static uint16_t  FaultReg = 0;

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

//Регистры для организации мигающей индикации. 
volatile static uint16_t Blink250mSReg  = 0;
volatile static uint16_t Blink500mSReg  = 0;
volatile static uint16_t Blink1000mSReg = 0;

//Флаговый регистр.
volatile uint16_t FlagReg = 0;
//Флаги 
#define SM1628_Flag         (1<<0) //Работа с микросхемой SM1628.
#define TestOn_Flag         (1<<1) //Флаг активации тестирования .
#define mS_Flag             (1<<2) //Флаг активации тестирования .
//-----------------------------------------------------------------------------
#pragma pack(push, 1)//размер выравнивания в 1 байт
struct{
    //--------------------
    uint16_t mS_CounterForProtocolRS485;
    uint16_t mS_CounterForCheckGpio;
    uint16_t mS_CounterForTEST;
    uint16_t DebugCount;
    uint16_t ButtonCheck_mS_Counter;
    //--------------------
}SysTicCountersSTR;
#pragma pack(pop)//вернули предыдущую настройку.
//-----------------------------------------------------------------------------
//Функция задержки на заданное количество м—ек.
void Delay_mSec(volatile uint32_t mSec){
  
 while(mSec)
    {
      if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) mSec--;
    }
}
//-----------------------------------------------------------------------------
void DisplayError(char* txt){

  LcdClear(); 
  LcdOutStr(txt);
  LcdString(1, 1);
  //LcdUpdate();
}
//-----------------------------------------------------------------------------
uint8_t GetBlinkReg(void){

  return Blink250mSReg;
}
//-----------------------------------------------------------------------------
//Упарвление светодиодом ОТКЛ.ЗВУК.
void LedDisactiveSound(uint16_t fault){

  static uint16_t FaultState = 0;
  //--------------------
  //Нет неисправностей - гасим светодиод и выходим.
  if(fault == 0)
    {
      LedControl(ZUMMER_LED, LedOff);
      FaultState = 0;
    }
  //--------------------
  //Есть неисправность. 
  else if((FaultState ^ fault) & fault)
    {
      ButClrToggleState(CONTROL_BUTTONS, ZUMMERbut);//сброс состояния кнопки ОТКЛ.ЗВУК.
      LedControl(ZUMMER_LED, LedOff);  
      FaultState = fault;
    }
  else
    {
      if(ButGetToggleState(CONTROL_BUTTONS) & ZUMMERbut) 
        {
          LedControl(ZUMMER_LED, LedOn);
          FaultState = fault;
        }
    }
  //--------------------
}
//*******************************************************************************************
//*******************************************************************************************
void Task_LockModeUnit(void){

  //--------------------
  //Если был активирован ТЕСТ, то при переходе в режим Блокировка режим ТЕСТ отключаем.
  //FlagReg &= ~TestOn_Flag;
  SysTicCountersSTR.mS_CounterForTEST = 20000;
  //--------------------
  LcdClear();                                              //Очистка дисплея
  RtcToTime(TimeUTC);                                      //Отображение времени. 
  MenuIndexRegCleare();                                    //Сброс указателей меню.
  ButClrToggleState(CONTROL_BUTTONS, (STARTbut | STOPbut));//Сброс состояний кнопок меню.
  //--------------------
  //Управление светодиодами.
  LedPresetControl(PRESET1_LED, Line1, Zone1, Blink250mSReg);//Cветодиод ПРЕСЕТ1.
  LedPresetControl(PRESET2_LED, Line2, Zone2, Blink250mSReg);//Cветодиод ПРЕСЕТ2.
  //Гашение светодиодов СТАРТ и ПОЖАР.
  if((ZoneFpGet(Zone1) <= ZoneManualOn) && (ZoneFpGet(Zone2) <= ZoneManualOn))
    {
      LedStart(LedOff);
      LedPoj(LedOff);
    }
  //--------------------
  //Передача состояний кнопки ОТКЛ.ЗВУК в ЦП.
  RS485FP_TxBufSet()->STR.Control_Buttons = (ButGetInstantState(CONTROL_BUTTONS) & ZUMMERbut);
  //Вывод на дисплей надписи о состоянии блока.  
  MenuGetMicStatus(MenuBackgroundPage());//Вывод на дисплей состояния микрофона.
  //Вывод на дисплей состояния линий Гр.
  if(LcFpGetForLed(Line1) >= LineBreak) MenuTextDisplay(RusText_Zone1_Pointer, MenuGetTextStatLc(LcFpGetForLed(Line1)), String5); 
  if(LcFpGetForLed(Line2) >= LineBreak) MenuTextDisplay(RusText_Zone2_Pointer, MenuGetTextStatLc(LcFpGetForLed(Line2)), String6);
  
  MenuBackgroundPage()->NamePage                      = MenuGetRusText(RusText_LockModeUnit_Pointer);
  MenuBackgroundPage()->PageItems.StringName[String1] = MenuGetRusText(RusText_Address_Pointer);
  MenuBackgroundPage()->PageItems.StringName[String2] = MenuGetRusText(RusText_Group_Pointer);
  MenuBackgroundPageDisplay(MenuBackgroundPage());
  //--------------------
  RTOS_SetTimerTask(Task_IndicationUpdate, 1);
}
//-----------------------------------------------------------------------------
void Task_ControlModeUnit(void){

  //--------------------
  //Работа с кнопкой "ТЕСТ".
  if(FlagReg & TestOn_Flag)
    {
      //Вывод на дисплей надписи "РЕЖИМ ТЕСТИРОВАНИЯ."
      LcdClear();
      LcdStringBig(2, 4);
      LcdOutStr(MenuGetRusText(RusText_TESTMode_Pointer)); 
      LcdUpdate();
      //-----------
      RTOS_SetTask(Task_IndicationUpdate);
      return;
    }
  if((ButGetInstantState(CONTROL_BUTTONS) & TESTbut)) 
    {
      FlagReg |= TestOn_Flag;
      Command  = CmdFPTetsZummerConst;
    }
  //--------------------
  LcdClear();                                              //Очистка дисплея
  RtcToTime(TimeUTC);                                      //Отображение времени. 
  MenuIndexRegCleare();                                    //Сброс указателей меню.
  ButClrToggleState(CONTROL_BUTTONS, (STARTbut | STOPbut));//Сброс состояний кнопок меню.
  //--------------------
  //Управление светодиодами.
  LedPresetControl(PRESET1_LED, Line1, Zone1, Blink250mSReg);//Cветодиод ПРЕСЕТ1.
  LedPresetControl(PRESET2_LED, Line2, Zone2, Blink250mSReg);//Cветодиод ПРЕСЕТ2.
  //Гашение светодиодов СТАРТ и ПОЖАР.
  if((ZoneFpGet(Zone1) <= ZoneManualOn) && (ZoneFpGet(Zone2) <= ZoneManualOn))
    {
      LedStart(LedOff);
      LedPoj(LedOff);
    }
  //--------------------
  //Передача состояний кнопок в ЦП.
  RS485FP_TxBufSet()->STR.Control_Buttons = ButGetInstantState(CONTROL_BUTTONS);
  RS485FP_TxBufSet()->STR.Preset_Buttons  = ButGetInstantState(PRESET_BUTTONS);
  //--------------------
  //Вывод на дисплей надписи о состоянии блока.  
  MenuGetMicStatus(MenuBackgroundPage());//Вывод на дисплей состояния микрофона
  //Вывод на дисплей состояния линий Гр.
  if(LcFpGetForLed(Line1) >= LineBreak) MenuTextDisplay(RusText_Zone1_Pointer, MenuGetTextStatLc(LcFpGetForLed(Line1)), String5); 
  if(LcFpGetForLed(Line2) >= LineBreak) MenuTextDisplay(RusText_Zone2_Pointer, MenuGetTextStatLc(LcFpGetForLed(Line2)), String6);
  
  MenuBackgroundPage()->NamePage                      = MenuGetRusText(RusText_ControlModeUnit_Pointer);//Заголовок страницы
  MenuBackgroundPage()->PageItems.StringName[String1] = MenuGetRusText(RusText_Address_Pointer);
  MenuBackgroundPage()->PageItems.StringName[String2] = MenuGetRusText(RusText_Group_Pointer);
  MenuBackgroundPageDisplay(MenuBackgroundPage());
  //--------------------
  RTOS_SetTimerTask(Task_IndicationUpdate, 1);
}
//-----------------------------------------------------------------------------
//void CofigModeUnit(void){
void Task_CofigModeUnit(void){

  static uint16_t IndexTime    = 3;
  static uint16_t IndexAddress = 1;
  static uint16_t State        = 0;
  //--------------------
  //LedSetAll(Blink250mSReg);//мигающая индикация всеми светодиодами.
  //По нажатию кнопки СТОП перемещение по пунктам меню
  if(ButGetToggleState(CONTROL_BUTTONS) & STOPbut)
    {
      //--------------------         
      //Определение редактируемой страницы.
      switch(MenuIndexReg()->IndexMenuPage){
          //--------------------   
          //1-я страница меню - адрес, группа, время, дата.
          case(Page1):
            //Блокировка перемещения по пунктам меню в случае редактирования пункта
            if(State == 0) ButModParam1(&MenuIndexReg()->IndexPageItem, String1, String4);
            //-----------
            //Редактирование адреса.
            if(MenuIndexReg()->IndexPageItem == String1) //на первой строке.
              {
                ButModParam2(&IndexAddress, 0, 1);
                if(IndexAddress < 1)
                  {
                    State = 1;            
                    ButModParam1(&Address, 0, 64);
                    MenuAddressUpdate(IndexAddress, Address, Blink250mSReg);

                    if(ButGetInstantState(CONTROL_BUTTONS) & STARTbut)
                      {
                        State        = 0;
                        IndexAddress = 1;
                        Command = CmdFPSetAddressConst;//Команда установки адреса.
                        ButClrToggleState(CONTROL_BUTTONS, STOPbut);
                      }
                    RTOS_SetTimerTask(Task_IndicationUpdate, 1); 
                    return; 
                  }
                 else
                  {
                    State        = 0;
                    IndexAddress = 1;
                  }                     
               }
            //-----------
            //Редактирование группы.
            if(MenuIndexReg()->IndexPageItem == String2) //на второй строке.
              {
                ButModParam2(&IndexAddress, 0, 1);
                if(IndexAddress < 1)
                  {
                    State = 1;            
                    ButModParam1(&Group, 0, 64);
                    MenuGroupUpdate(IndexAddress, Group, Blink250mSReg);

                    if(ButGetInstantState(CONTROL_BUTTONS) & STARTbut)
                      {
                        State        = 0;
                        IndexAddress = 1;
                        Command = CmdFPSetGroupConst;//Команда установки группы.
                        ButClrToggleState(CONTROL_BUTTONS, STOPbut);
                      }
                    RTOS_SetTimerTask(Task_IndicationUpdate, 1);
                    return; 
                  }
                 else
                  {
                    State        = 0;
                    IndexAddress = 1;
                  }                     
               }
            //-----------
            //Редактирование времени.
            if(MenuIndexReg()->IndexPageItem == String3) //на третьей строке.
              {
                ButModParam2(&IndexTime, 0, 3);
                if(IndexTime < 3)
                  {
                    State = 1;
                    //Часы
                    if(IndexTime == 0)
                      {
                        ButModParam1(&RtcGetTime()->hour, 0, 23);
                        MenuTimeUpdate(IndexTime, RtcGetTime()->hour, Blink250mSReg);
                      }
                    //минуты
                    if(IndexTime == 1)
                      {
                        ButModParam1(&RtcGetTime()->min, 0, 59);
                        MenuTimeUpdate(IndexTime, RtcGetTime()->min, Blink250mSReg);
                      }
                    //секунды
                    if(IndexTime == 2)
                      {
                        ButModParam1(&RtcGetTime()->sec, 0, 59);
                        MenuTimeUpdate(IndexTime, RtcGetTime()->sec, Blink250mSReg);
                      }             
                    if(ButGetInstantState(CONTROL_BUTTONS) & STARTbut)
                      {
                        State     = 0;
                        IndexTime = 3;
                        Command                         = CmdFPSetTimeConst;      //Команда установки времени.
                        RS485FP_TxBufSet()->STR.TimeUTC = TimeToRtc(RtcGetTime());
                        ButClrToggleState(CONTROL_BUTTONS, STOPbut);
                      } 
                    RTOS_SetTimerTask(Task_IndicationUpdate, 1);
                    return;   
                  }
                else
                  {
                    State     = 0;
                    IndexTime = 3;
                  }  
              }
            //-----------
          break;
          //--------------------   
          //2-я страница меню - Состояние пожарных шлейфов.
          case(Page2):
          break;
          //--------------------   
          //3-я страница меню - Состояние линий Гр.
          case(Page3):
            //Перемещение по пунктам меню
            ButModParam1(&MenuIndexReg()->IndexPageItem, String1, String5);
            //Редактирование отклонения линии Гр.
            if(MenuIndexReg()->IndexPageItem == String5)
              {
                ButModParam2(&MenuPage(Page3)->PageItems.StringVar[String5], 1, 99);
              }
            else MenuPage(Page3)->PageItems.StringVar[String5] = LcDeviationGet();
            //Нажатие кнопки СТАРТ - сохранение.         
            if(ButGetInstantState(CONTROL_BUTTONS) & STARTbut)
              {
                ButClrToggleState(CONTROL_BUTTONS, STOPbut);//Выход из редактирования пунктов страницы.
                //Команда сохранения опорных значений для Lc.
                if(MenuIndexReg()->IndexPageItem <= String4)
                  {
                    Command = CmdFPLcSaveRefConst;
                    RS485FP_TxBufSet()->STR.Address = MenuIndexReg()->IndexPageItem;    
                  }
                //Передача команды установки отклолнения линии Гр.
                else
                  {
                    Command = CmdFPLcSetDeviationConst; 
                    RS485FP_TxBufSet()->STR.Address = MenuPage(Page3)->PageItems.StringVar[String5];            
                  }
                //------
              }
          break;
          //--------------------   
          //4-я страница меню.
          case(Page4):
          break;
          //--------------------   
          //5-я страница меню.
          case(Page5):
          break;
          //--------------------   
          //6-я страница меню.
          case(Page6):
          break;
          //--------------------   
					//7-я страница меню.
          case(Page7):
          break;
          //--------------------   
          default:
          break;
          //--------------------   
        }
      //--------------------   
      RtcToTime(TimeUTC);
      //Обновление состояния страницы меню.
      MenuUpdate(MenuPage(MenuIndexReg()->IndexMenuPage), 1, MenuIndexReg()->IndexPageItem); 
    }
  //--------------------
  //Перемещение по страницам меню.
  else
    { 
      State                         = 0;//Разрешение перемещения по пунктам меню.
      IndexTime                     = 3;
      IndexAddress                  = 1;
      MenuIndexReg()->IndexPageItem = 0;
      Address      = MenuPage(Page1)->PageItems.StringVar[String1];    //адрес от ЦП
      Group        = MenuPage(Page1)->PageItems.StringVar[String2];    //группа от ЦП.
      MenuPage(Page3)->PageItems.StringVar[String5] = LcDeviationGet();//отклонения от установившегося значения для фиксации перех-го проц-са на линии.        
      RtcToTime(TimeUTC);                                              //Отображение времени.                   
      ButModParam1(&MenuIndexReg()->IndexMenuPage, 0, MaxMenuPages-1); //Перемещение по страницам меню.
      MenuUpdate(MenuPage(MenuIndexReg()->IndexMenuPage), 0, 0);       //Обновление состояния страницы меню.
    }
  //--------------------
  RTOS_SetTimerTask(Task_IndicationUpdate, 1);
}
//*******************************************************************************************
//*******************************************************************************************
//Процесс для мигания светодиодами - Сам себя запускает.
void Task_BlinkReg(void){

  Blink250mSReg ^= 0xFFFF;
}
//-----------------------------------------------------------------------------
//Обновление изображения на дисплее.
void Task_DisplayUpdate(void){

  LcdUpdate();
  //Очистка видеопамяти в MenuUpdate(), LockModeUnit() и в ControlModeUnit().
  //LcdClear();
}
//-----------------------------------------------------------------------------
//Обновление светодиодов.
void Task_IndicationUpdate(void){

  //Гашение всех светодиодов если не в режиме настройки.
  //if(GetKeyState() != UnitCofigModeConst) LedSetAll(LedAllOFF);
  //--------------------- 
  //Управление светодиодами пресетов. 
  //LedPresetControl(PRESET1_LED, Line1, Zone1, Blink250mSReg);//Cветодиод ПРЕСЕТ1.
  //LedPresetControl(PRESET2_LED, Line2, Zone2, Blink250mSReg);//Cветодиод ПРЕСЕТ2.
  //Гашение светодиодов СТАРТ и ПОЖАР.
  //if((ZoneFpGet(Zone1) <= ZoneManualOn) && (ZoneFpGet(Zone2) <= ZoneManualOn))
  //  {
  //    LedStart(LedOff);
  //    LedPoj(LedOff);
  //  }
   //---------------------
   //Работа с кнопкой "ТЕСТ".
   if(FlagReg & TestOn_Flag)
    { 
      LedUpdate(); //Обновление светодиодов. 
      //Command = CmdFPTetsZummerConst;
      return;
    }
  //--------------------
  //Индикатор "ПИТАНИЕ"
  if(PowerFpGet() <= PowerDCOk)        //Питание в норме.
    {
      LedPower(GreenColor);
      FaultReg &= ~(FaultACNo_Flag | FaultDCFault_Flag);
    }
  else if(PowerFpGet() == PowerACNo)   //Отсутствует основное питание.
    {
      LedPower(RedColor);
      FaultReg |= FaultACNo_Flag;
    }
  else if(PowerFpGet() == PowerDCFault)//Неисправен инвертор.
    {
      LedPower(YellowColor & Blink250mSReg);
      FaultReg |= FaultDCFault_Flag;
    } 
  //--------------------
  //Индикатор "АКБ" 
  if(BatFpGet() == BatChargeEnd) LedBattery(GreenColor);
  if(BatFpGet() == BatBlink)     LedBattery(GreenColor  & Blink250mSReg);
  if(BatFpGet() == BatAttention) LedBattery(YellowColor & Blink250mSReg);
  if(BatFpGet() >= BatDeep)      LedBattery(RedColor);

  if(BatFpGet() >= BatAttention) FaultReg |=  FaultBat_Flag;
  else                           FaultReg &= ~FaultBat_Flag;
  //--------------------
  //Индикатор "УМЗЧ" 
  //if(AmpFpGet() == AmpActive)    LedAmplifier(GreenColor);
  //if(AmpFpGet() == AmpDisactive) LedAmplifier(GreenColor  & Blink250mSReg);
  if(AmpFpGet() == AmpProt)
    {
      LedAmplifier(YellowColor & Blink250mSReg);
      FaultReg |=  FaultPA_Flag;
    }
  else  
    {
      LedAmplifier(GreenColor);
      FaultReg &= ~FaultPA_Flag;
    }   
  //Вывод на дисплей состояния УМЗЧ.
  if(AmpFpGet() == AmpDisactive) MenuPage(Page4)->PageItems.StringText[String3] = MenuGetRusText(RusText_Disactive_Pointer);
  if(AmpFpGet() == AmpActive)    MenuPage(Page4)->PageItems.StringText[String3] = MenuGetRusText(RusText_Active_Pointer);
  if(AmpFpGet() == AmpProt)      MenuPage(Page4)->PageItems.StringText[String3] = MenuGetRusText(RusText_PAProt_Pointer);
  //--------------------
  //Индикатор "СВЯЗЬ" 
  if(!(RS485FlagGet() & RS485_TimeOutFlag))LedCommunication(GreenColor  & Blink250mSReg);//есть обмен - мигающий земеный.
  else                                     LedCommunication(YellowColor & Blink250mSReg);//нет обмена - мигающий желтый.
  //--------------------
  //Индикатор "НЕИСП.ОБЩ." 
		
//#define FaultPoj1_Flag     (1<<0) 
//#define FaultPoj2_Flag     (1<<1) 
//#define FaultPoj3_Flag     (1<<2) 
//#define FaultChS_Flag      (1<<3) 
//#define FaultLc1Line_Flag  (1<<4) 
//#define FaultLc1Att_Flag   (1<<5) 
//#define FaultLc2Line_Flag  (1<<6) 
//#define FaultLc2Att_Flag   (1<<7) 
		
//#define FaultACNo_Flag     (1<<8)
//#define FaultDCFault_Flag  (1<<9)  
//#define FaultBat_Flag      (1<<10)
//#define FaultPA_Flag       (1<<11)
//#define FaultMic_Flag      (1<<12)
//#define FaultConnect_Flag  (1<<13)

	if(FaultReg & (FaultACNo_Flag | FaultDCFault_Flag | FaultBat_Flag))
		{
			 LedControl(GENERAL_FAULT_LED, LedOn);		
		}		
	else LedControl(GENERAL_FAULT_LED, LedOff);	
  //--------------------
  //Индикатор "НЕИСП.Л.О."
  if((LcFpGetForLed(Line1) >= LineBreak) || (LcFpGetForLed(Att1) >= LineBreak)||
     (LcFpGetForLed(Line2) >= LineBreak) || (LcFpGetForLed(Att2) >= LineBreak))
    {
       LedControl(ALERT_LINES_FAUL_LED, LedOn);
    }
  else LedControl(ALERT_LINES_FAUL_LED, LedOff); 

  if(LcFpGetForLed(Line1) >= LineBreak) FaultReg |=  FaultLc1Line_Flag;
  else                                  FaultReg &= ~FaultLc1Line_Flag;
  if(LcFpGetForLed(Att1)  >= LineBreak) FaultReg |=  FaultLc1Att_Flag;
  else                                  FaultReg &= ~FaultLc1Att_Flag;

  if(LcFpGetForLed(Line2) >= LineBreak) FaultReg |=  FaultLc2Line_Flag;
  else                                  FaultReg &= ~FaultLc2Line_Flag;
  if(LcFpGetForLed(Att2)  >= LineBreak) FaultReg |=  FaultLc2Att_Flag;
  else                                  FaultReg &= ~FaultLc2Att_Flag;
  //--------------------
  //Вывод на дисплей состояния линий Гр. в режиме НАСТРОЙКА.
  MenuPage(Page3)->PageItems.StringText[String1] = MenuGetTextStatLc(LcFpGetForTxt(Line1));
  MenuPage(Page3)->PageItems.StringText[String2] = MenuGetTextStatLc(LcFpGetForTxt(Att1));
  MenuPage(Page3)->PageItems.StringText[String3] = MenuGetTextStatLc(LcFpGetForTxt(Line2));
  MenuPage(Page3)->PageItems.StringText[String4] = MenuGetTextStatLc(LcFpGetForTxt(Att2));
  //MenuPage(Page3)->PageItems.StringVar[String1]  = RS485FP_GetZoneFromMB(StatusZone1Point);
  //MenuPage(Page3)->PageItems.StringVar[String2]  = RS485FP_GetZoneFromMB(StatusZone2Point);

  //Вывод на дисплей состояния линий Гр. в режимах БЛОКИРОВКА УПР. и УПРАВЛЕНИЕ.
  //if(LcFpGetForLed(Line1) >= LineBreak) MenuTextDisplay(RusText_Zone1_Pointer, MenuGetTextStatLc(LcFpGetForLed(Line1)), String5); 
  //if(LcFpGetForLed(Line2) >= LineBreak) MenuTextDisplay(RusText_Zone2_Pointer, MenuGetTextStatLc(LcFpGetForLed(Line2)), String6);
  //--------------------
  //Индикатор "НЕИСП.П.Ш." 
  if((PojLineFpGet(Poj1) > NormCloseSost)||(PojLineFpGet(Poj2) > NormCloseSost)||
     (PojLineFpGet(Poj3) > NormCloseSost)||(PojLineFpGet(ChS)  > NormCloseSost))
    {
       LedControl(FIRE_LINES_FAUL_LED, LedOn);
    }
  else LedControl(FIRE_LINES_FAUL_LED, LedOff);

  if(PojLineFpGet(Poj1) > NormCloseSost) FaultReg |=  FaultPoj1_Flag;
  else                                   FaultReg &= ~FaultPoj1_Flag;
  if(PojLineFpGet(Poj2) > NormCloseSost) FaultReg |=  FaultPoj2_Flag;
  else                                   FaultReg &= ~FaultPoj2_Flag;
  if(PojLineFpGet(Poj3) > NormCloseSost) FaultReg |=  FaultPoj3_Flag;
  else                                   FaultReg &= ~FaultPoj3_Flag;
  if(PojLineFpGet(ChS) > NormCloseSost)  FaultReg |=  FaultChS_Flag;
  else                                   FaultReg &= ~FaultChS_Flag;
  //Вывод на дисплей состояния пожарных шлейфов.
  MenuPage(Page2)->PageItems.StringText[String1] = MenuGetTextStatPojLine(PojLineFpGet(Poj1));
  MenuPage(Page2)->PageItems.StringText[String2] = MenuGetTextStatPojLine(PojLineFpGet(Poj2));
  MenuPage(Page2)->PageItems.StringText[String3] = MenuGetTextStatPojLine(PojLineFpGet(Poj3));
  MenuPage(Page2)->PageItems.StringText[String4] = MenuGetTextStatPojLine(PojLineFpGet(ChS));  
  //---------------------
  //Вывод отладочной информации
  //MenuPage(Page4)->PageItems.StringVar[String2] = RS485FP_GetDebugFromMB(Debug1);
  //MenuPage(Page1)->PageItems.StringVar[String6] = RS485FP_GetDebugFromMB(Debug2);
  //--------------------- 
  //Неисправность МИК.Пр.
  if(GetMicState() >= MicNotConnectedConst) FaultReg |=  FaultMic_Flag;
  else                                      FaultReg &= ~FaultMic_Flag;
  //--------------------- 
  //Управление светодиодом ОТКЛ.ЗВУК.
  LedDisactiveSound(FaultReg);
  //---------------------
  //Работа с кнопкой "ТЕСТ".
  if(FlagReg & TestOn_Flag)
    { 
//       //Вывод на дисплей надписи "РЕЖИМ ТЕСТИРОВАНИЯ."
//       LcdClear();
//       LcdStringBig(2, 4);
//       LcdOutStr(MenuGetRusText(RusText_TESTMode_Pointer)); 
//       LcdUpdate();
       Command = CmdFPTetsZummerConst;
    }
  //Говорим ЦП что находимся в режиме Блокировка Управления.
  //else Command = CmdFPStatusConst;
  //---------------------
  //Мигающая индикация в режиме настройки блока.
  if(GetKeyState() == UnitCofigModeConst) LedSetAll(Blink250mSReg);//мигающая индикация всеми светодиодами.                                                        
  //---------------------  
  LedUpdate(); //Обновление светодиодов. 
  //--------------------- 
  //RTOS_SetTimerTask(Task_IndicationUpdate, 1);
}
//-----------------------------------------------------------------------------
//Обработка данных протокола. - этот процесс вызывается в прерывании по приему пакета.
void Task_RS485Protocol(void){

  PojLineFpSet    (&RS485RxBuf()->STR.StatusPoj1); //(RS485RxBuf(StatusPoj1Point));       //Сохранение состояния линий пожарных линий.
  LcFpSet         (&RS485RxBuf()->STR.StatusZone1);//(RS485RxBuf(StatusZone1Point));      //Сохранение состояния линий Гр.
  LcDeviationSet  (&RS485RxBuf()->STR.LcDeviationVolue); //(RS485RxBuf(LcDeviationVolueLPoint));//Сохранение отклонения линий Гр.
  ZoneFpSet       (&RS485RxBuf()->STR.StatusZone1); //(RS485RxBuf(StatusZone1Point));      //Сохранение состояния зон - кем были активированы зоны.
  PowerAndBatFpSet(&RS485RxBuf()->STR.StatusPOWER); //(RS485RxBuf(StatusPOWERPoint));      //Сохранение состояния питания и ЗУ.
  AmpFpSet        (&RS485RxBuf()->STR.StatusPA);//(RS485RxBuf(StatusPAPoint));         //Сохранение состояния УМЗЧ.
  //---------------------
  //Сохранение адреса и группы ЦП.
  MenuBackgroundPage()->PageItems.StringVar[String1] = RS485RxBuf()->STR.Addres;//(AddresPoint);
  MenuBackgroundPage()->PageItems.StringVar[String2] = RS485RxBuf()->STR.Group;//RS485RxGetDataFromBuf(GroupPoint);
  //---------------------
  //Вывод на дисплей состояния питания блока.
  MenuPage(Page4)->PageItems.StringVar[String1] = RS485RxBuf()->STR.StatusPOWER & PowerMask; //(RS485RxGetDataFromBuf(StatusPOWERPoint) & PowerMask);
  MenuPage(Page4)->PageItems.StringVar[String2] = RS485RxBuf()->STR.DebugData1;             // RS485FP_GetDebugFromMB(Debug1);//Uакб
  //---------------------
  //Вывод на дисплей состояний оповещателей.	
	MenuPage(Page5)->PageItems.StringVar[String1] = RS485RxBuf()->STR.SirenPower;
	MenuPage(Page5)->PageItems.StringVar[String2] = RS485RxBuf()->STR.Siren1Status;
	MenuPage(Page5)->PageItems.StringVar[String3] = RS485RxBuf()->STR.Siren2Status;
	MenuPage(Page5)->PageItems.StringVar[String4] = RS485RxBuf()->STR.Siren3Status;
  //--------------------------------------------
  //Формирование ответа на запрос от ЦП.  
  switch(Command){//Определение команды для передачи в ЦП.
      //--------------------
      //Команда установки адрес блока
      case(CmdFPSetAddressConst):
        RS485FP_TxBufSet()->STR.Command_Code               = CmdFPSetAddressConst;
        RS485FP_TxBufSet()->STR.Address                    = Address;
        MenuBackgroundPage()->PageItems.StringVar[String1] = Address;
        Command = 0;
      break;
      //--------------------
      //Команда устновки группы блока
      case(CmdFPSetGroupConst):
        RS485FP_TxBufSet()->STR.Command_Code               = CmdFPSetGroupConst;
        RS485FP_TxBufSet()->STR.Group                      = Group;
        MenuBackgroundPage()->PageItems.StringVar[String2] = Group;
        Command = 0;    
      break;
      //--------------------
      //Команда установики времени
      case(CmdFPSetTimeConst):
        RS485FP_TxBufSet()->STR.Command_Code = CmdFPSetTimeConst;
        //RS485FP_TxBufSet()->STR.TimeUTC      = TimeToRtc(RtcGetTime());
        Command = 0;          
      break;
      //-------------------- 
      //Команда установики даты
      case(CmdFPSetDataConst):
       
      break;
      //-------------------- 
      //Команда влючение тестирования зуммера ЦП
      case(CmdFPTetsZummerConst):
        RS485FP_TxBufSet()->STR.Command_Code   = CmdFPTetsZummerConst; 
        RS485FP_TxBufSet()->STR.KeyAndMicState = GetBlinkReg(); 
        Command = 0;
      break;
      //-------------------- 
      //Команда сохранения опорных значений для Lc.
      case(CmdFPLcSaveRefConst):
        RS485FP_TxBufSet()->STR.Command_Code = CmdFPLcSaveRefConst;
        //RS485FP_TxBufSet()->STR.Address      = 2;//Номер канала для которого сохряняются опроные значения.
        Command = 0;
      break;
      //-------------------- 
      //Команда установки отклонения от установившегося значения для фиксации перех-го проц-са на линии.     
      case(CmdFPLcSetDeviationConst):
        RS485FP_TxBufSet()->STR.Command_Code = CmdFPLcSetDeviationConst;
        //RS485FP_TxBufSet()->STR.Address      = 2;//Номер канала для которого сохряняются опроные значения.
        Command = 0;
      break;
      //--------------------
      //Команда "Выдача статуса от FP".
      default:
        RS485FP_TxBufSet()->STR.Command_Code    = CmdFPStatusConst;               
        RS485FP_TxBufSet()->STR.KeyAndMicState  = (GetMicState() | GetKeyState());//состояние микрофона и положения ключа.
        RS485FP_TxBufSet()->STR.Address         = RS485RxBuf()->STR.Addres;//RS485RxGetDataFromBuf(AddresPoint);
        RS485FP_TxBufSet()->STR.Group           = RS485RxBuf()->STR.Group; //RS485RxGetDataFromBuf(GroupPoint);
        //RS485FP_TxBufSet()->STR.Control_Buttons = ButGetInstantState(CONTROL_BUTTONS);
        //RS485FP_TxBufSet()->STR.Preset_Buttons  = ButGetInstantState(PRESET_BUTTONS);

			 //RS485FP_TxBufSet()->STR.TimeUTC = 0x12345678;//Отладка для ЦП.
			
        MenuPage(Page1)->PageItems.StringVar[String1] = RS485RxBuf()->STR.Addres;//RS485RxGetDataFromBuf(AddresPoint);//адрес от ЦП
        MenuPage(Page1)->PageItems.StringVar[String2] = RS485RxBuf()->STR.Group; //RS485RxGetDataFromBuf(GroupPoint); //группа от ЦП.
        TimeUTC = RS485RxBuf()->STR.TimeUTC;//RS485FP_GetTimeUTCFromMB();                                              //время от ЦП.
      break;
      //-------------------- 
    }
  //Запуск передачи ответа на запрос производится ф-ии uint8_t RS485RxPacket(uint8_t data)
  //RS485FP_TxStarResponse();         //Запуск передачи ответа на запрос.

  //Очистка "буфера" для приема нового запроса производится после передачи ответа на запрос.
  //В Ф-ии void RS485FP_TxEnd(void) производится сброс. 

  RS485RxClrBuf();                  //Очистка "буфера" для приема нового запроса
  RS485FlagClr(RS485_MBNewDataFlag);//
  RS485FP_TxStartResponse();        //Запуск передачи ответа на запрос.
  //--------------------------------------------
  //RTOS_SetTimerTask(Task_GetControl, 1);
  RTOS_SetTask(Task_GetControl);
}
//*******************************************************************************************
//*******************************************************************************************
//Начало работы программы.
void Task_Begin(void){

  //Вывод на дисплей логотопа.
  MenuLogo();
  Delay_mSec(2000);
  //Горизонтальный прогресбар.
  //MenuProgressBar();
  //Delay_mSec(2000);

  while(!Sm1628ReadButton() & !CheckGpioPins()) Delay_mSec(10);//Определение состояние кнопок.
  Uart1Init();                                                 //Запуск протокола обмена.
  //--------------------
  RTOS_SetTask(Task_GetControl);
}
//-----------------------------------------------------------------------------
//Определение управления.
void Task_GetControl(void){

  //Отсутствие обмена с ЦП.
       if(RS485FlagGet() & RS485_TimeOutFlag)    RTOS_SetTimerTask(Task_ErrorConnectMB, 1);
  //Управление от кнопок.
  else if(Sm1628ReadButton() || CheckGpioPins()) RTOS_SetTimerTask(Task_ControlUnit, 1);
  //Обновление изображения на дисплее.
  RTOS_SetTask(Task_DisplayUpdate);
  //--------------------
  RTOS_SetTimerTask(Task_GetControl, 1);
}
//-----------------------------------------------------------------------------
//Управление блоком.
void Task_ControlUnit(void){

  static uint8_t OldKeyState = 0;
  //--------------------
  //Это нужно для отключения всех светодиодов при выходе из режима Настройки.
  if((OldKeyState == UnitCofigModeConst) && (GetKeyState() != UnitCofigModeConst))
    {
      LedSetAll(LedAllOFF);
    }
  OldKeyState = GetKeyState();
  //--------------------
  switch(OldKeyState){
      //--------------------
      //Режим "Блокировка управления".
      case(UnitLockModeConst):
        //RTOS_SetTimerTask(Task_LockModeUnit, 1);
        RTOS_SetTask(Task_LockModeUnit);
      break;
      //--------------------
      //Режим "Управление".
      case(UnitControlModeConst):
        //RTOS_SetTimerTask(Task_ControlModeUnit, 1);
        RTOS_SetTask(Task_ControlModeUnit);
      break;
      //--------------------
      //Режим "Настройка".
      case(UnitCofigModeConst):
        //RTOS_SetTimerTask(Task_CofigModeUnit, 1);
        RTOS_SetTask(Task_CofigModeUnit);
      break;
      //-------------------- 
      default:
        DisplayError("Task_ControlUnit_Err");
        RTOS_SetTimerTask(Task_GetControl, 1);
      break;
      //-------------------- 
    }
  //--------------------
}
//-----------------------------------------------------------------------------
//Ошибка обмена с ЦП.
void Task_ErrorConnectMB(void){

  LedSetAll(LedAllOFF);
  LedCommunication(YellowColor);
  //LedControl(GENERAL_FAULT_LED, LedOn);
  LedUpdate(); //Обновление светодиодов.
  MenuTextErrorConnect();
  RTOS_SetTimerTask(Task_GetControl, 1);
}
//************************************************************************************************** 
//**************************************************************************************************
int main(void){
    
  //--------------------
  //После сброса микроконтроллера программа переходит в main(), минуя SystemInit().
  //В фейле startup_stm32f10x_md_vl.s закоментирован переход на SystemInit.
  SysInit();      //Настройка тактирования микроконтроллера 72МГц. Внешний кварц 8 МГц.
  GpioInit();     //Инициализация портов.
  //CRC_Init();     //
  Spi2Init();     // 
  //Uart1Init();    //
  //LedInit();      //
  Lcd12864Init(); //Инициализация дисплея WG12864A.
  MenuInit();     //Инициализация пунктов меню.
  //-------------------- 
  RTOS_Init(); 	              //Инициализация диспетчера
  RTOS_SetTask(Task_Begin);   //Начало!!!

  SysTickInit(); //Запуск службы таймеров.
  __enable_irq();//Глобальное разрешение рерываний.
  //--------------------
  while(1)
    { 
      RTOS_TaskManager();//Вызов диспетчера ОС.
    }
  //--------------------
}
//************************************************************************************************** 
//**************************************************************************************************
//Прерывание системного таймера SysTick.
void SysTick_Handler(void){

  static uint16_t mScountForBlink = 0;
  //--------------------
  if(++mScountForBlink >= 250)
    {
      mScountForBlink = 0;
      RTOS_SetTask(Task_BlinkReg);
    }
  //--------------------
  //Опрос кнопок.
		
  //Lcd12864LedToggel;
	
		
  //--------------------
  //Служба таймеров ОС.
  RTOS_TimerService();
  //--------------------
  //Отсчет таймаута приема пакета.
  RS485TimeOutInc();//Отсчет таймаута приема пакета.
  //--------------------
  //Отработка режима "ТЕСТ".
  if(FlagReg & TestOn_Flag)
    { 
      //Мигаем всеми индикаторами.
      LedSetAll(Blink250mSReg);
      Lcd12864BackLightConrtol(Blink250mSReg);
      LedControl(TEST_LED, LedOn);
      //Окончание тестирования.
      if((++SysTicCountersSTR.mS_CounterForTEST >= 20000) ||//Через 20 секунд или
         (RS485FlagGet() & RS485_TimeOutFlag))              //при отсутствии обмена с ЦП.
        {
          SysTicCountersSTR.mS_CounterForTEST = 0; //
          Lcd12864BackLightConrtol(LcdBackLightOn);//
          FlagReg &= ~TestOn_Flag;                 //Окончание тестирования индикации.
          LedSetAll(LedAllOFF);                    //
        }   
    }
  else SysTicCountersSTR.mS_CounterForTEST = 0;
  //--------------------
  //Счетчик для отладки.
//  if (++SysTicCountersSTR.DebugCount >= 10)
//    {
//      SysTicCountersSTR.DebugCount = 0;
//      //------------
//      //LcdUpdate();//Обновление изображения на дисплее.
//      //LcdUpdate();//Обновление изображения на дисплее.
//      //LcdClear(); 
//    }   
  //-------------------- 
}
//************************************************************************************************** 
//**************************************************************************************************


