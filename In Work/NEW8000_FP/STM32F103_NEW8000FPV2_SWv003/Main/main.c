
#include "main.h"

//-----------------------------------------------------------------------------
//static uint8_t  ControlModeUnitFSMState        = 0;
static uint8_t  ControlModeUnitAlgButState     = 0;
//static uint8_t  ControlModeUnitControlButState = 0;	

//----------------------------------------
static uint16_t mS_CounterForTEST = 0;
static uint16_t Address  = 1;
static uint16_t Group    = 1;

#define TESTmSConst 10000u
//----------------------------------------
//Флаговый регистр.
volatile uint16_t FlagReg = 0;
//Флаги 
#define TestOn_Flag	(1<<0) //Флаг активации тестирования .

//----------------------------------------
#define CONFIGURATION 0
#define OTHER_EVENTS  1
#define FAULT				  2
#define FIRE				  3
#define PUSK			    4
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//uint16_t CallBack(uint16_t val, uint16_t (*f)(void)){
//
//	return (val + f());
//}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Функция задержки на заданное количество м—ек.
void Delay_mSec(volatile uint32_t mSec){
  
 while(mSec)
    {
      if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) mSec--;
    }
}
//-----------------------------------------------------------------------------
//Управление светодиодом ОТКЛ.ЗВУК.
void LedDisactiveSound(uint32_t fault){
	
	static uint32_t FaultState = 0;
  //--------------------
  //Нет неисправностей - гасим светодиод и выходим.
  if(fault == 0)
    {
      LedZummer(LedOff);
      FaultState = 0;
    }
  //--------------------
  //Есть неисправность. 
  else if((FaultState ^ fault) & fault)
    {
      ButtonClrToggle(ZUMMERbut);//сброс состояния кнопки ОТКЛ.ЗВУК.
      LedZummer(LedOff);
      FaultState = fault;
    }
  else
    {
      if(ButtonToggle(ZUMMERbut)) 
        {
          LedZummer(LedOn);
          FaultState = fault;
        }
    }
  //--------------------
}
//-----------------------------------------------------------------------------
//Ручное включение алгоритмов оповещения по нажатию СТАРТ.
uint8_t AlgorithmAlert(void){

	static uint8_t state     = 0;
	static uint8_t selectAlg = 0;
	//-------------------------------------------------------	
	switch(state){
		//--------------------
		case(0):
			if(!ButtonInstant(STARTbut)) state = 1;
		break;
		//--------------------
		case(1):
			//Мигающая индикация светодиода ОПОВЕЩЕНИЕ.
			LedAlert(LedOn & Blink(BlinkInterval250mS));
			//Мигающая зеленая индикация у светодиодов выбранных АЛГОРИТМОВ ОПОВЕЩЕНИЯ
			if(ButtonToggle(ALG1but))
				{
					LedControl(Alg1Led, GreenColor & Blink(BlinkInterval250mS));
					selectAlg |= (1 << ALG1but); 
				}				
			else 	
				{
					LedControl(Alg1Led, LedOff);
					selectAlg &= ~(1 << ALG1but); 
				}
		
			if(ButtonToggle(ALG2but))
				{
					LedControl(Alg2Led, GreenColor & Blink(BlinkInterval250mS));
					selectAlg |= (1 << ALG2but); 
				}				
			else 	
				{
					LedControl(Alg2Led, LedOff);
					selectAlg &= ~(1 << ALG2but); 
				}
			//Переход на активацию выбранных АЛГОРИТМОВ ОПОВЕЩЕНИЯ по нажатию СТАРТ.
			if(ButtonInstant(STARTbut)) state = 2;	
			//Выход из ручного включения алгоритмов оповещения по нажатию СТОП.
			if(ButtonInstant(STOPbut))
				{
					ButtonClrToggle(STARTbut);
					ButtonClrToggle(STOPbut);
					ButtonClrToggle(ALG1but);
					ButtonClrToggle(ALG2but);
					
					ControlModeUnitAlgButState = 0;//Отключение линий Гр. 
					state     = 0;
					selectAlg = 0;
					return 1;
				}			
		break;
		//--------------------
		//Активацию выбранных АЛГОРИТМОВ ОПОВЕЩЕНИЯ по нажатию СТАРТ.		
		case(2):
			//Постоянно светящийся индикатор ОПОВЕЩЕНИЕ и ПУСК.
			LedAlert(LedOn);
		  LedPusk(LedOn);
		  //Красна индикация выбранных алгоритмов оповещения. 
			if(selectAlg & (1 << ALG1but)) 
				{
					LedControl(Alg1Led, RedColor & Blink(BlinkInterval500mS));
					ControlModeUnitAlgButState |= PRES1_BUTTON;
				}
			
			else LedControl(Alg1Led, LedOff);
			//-------
			if(selectAlg & (1 << ALG2but)) 
				{
					LedControl(Alg2Led, RedColor & Blink(BlinkInterval500mS));
					ControlModeUnitAlgButState |= PRES2_BUTTON;
				}	
			else LedControl(Alg2Led, LedOff);		
			
			//Выход из ручного включения алгоритмов оповещения по нажатию СТОП.
			if(ButtonInstant(STOPbut))
				{
					ButtonClrToggle(ALG1but);
					ButtonClrToggle(ALG2but);
					
					ControlModeUnitAlgButState = 0;//Отключение линий Гр.
					state     = 0;
					selectAlg = 0;
					return 1;
				}
		break;
		//--------------------
		case(3):
			
		

		break;
		//--------------------
		case(4):
//			LedControl(RESET_LED,GreenColor);
//		
//			if(ButtonGetToggle(PRESET_BUTTONS) & PRES1_BUTTON)
//				{
//					PresetButState |= PRES1_BUTTON; 
//					ButtonClrToggle(PRESET_BUTTONS, PRES1_BUTTON);
//				}
//			else LedControl(PRESET1_LED, RedColor & Blink(BlinkInterval1000mS));	
//				
//			if(ButtonGetToggle(PRESET_BUTTONS) & PRES2_BUTTON)
//				{
//					PresetButState |= PRES2_BUTTON; 
//					ButtonClrToggle(PRESET_BUTTONS, PRES2_BUTTON);
//				}
//			else LedControl(PRESET2_LED, RedColor & Blink(BlinkInterval1000mS));	

//			//if(ButtonGetToggle(CONTROL_BUTTONS) & STOP_BUTTON) State = 2;
		break;
		//--------------------
		default:
		break;	
		//--------------------
	}	
	//-------------------------------------------------------
	return 0;
}
//-----------------------------------------------------------------------------
//Вывод на дисплей состояния линий Гр. поставленных на контроль.
void DisplayTextLcState(uint8_t state){

	if(state & (1 << Line1))MenuPage(Page3)->PageItems.StringText[String1] = MenuGetTextStatLc(LcGetForTxt(Line1));
	else									  MenuPage(Page3)->PageItems.StringText[String1] = RusText()->LcControlOff;
	
	if(state & (1 << Att1)) MenuPage(Page3)->PageItems.StringText[String2] = MenuGetTextStatLc(LcGetForTxt(Att1));
	else									  MenuPage(Page3)->PageItems.StringText[String2] = RusText()->LcControlOff;
	
	if(state & (1 << Line2))MenuPage(Page3)->PageItems.StringText[String3] = MenuGetTextStatLc(LcGetForTxt(Line2));
	else									  MenuPage(Page3)->PageItems.StringText[String3] = RusText()->LcControlOff;
	
	if(state & (1 << Att2)) MenuPage(Page3)->PageItems.StringText[String4] = MenuGetTextStatLc(LcGetForTxt(Att2));
	else									  MenuPage(Page3)->PageItems.StringText[String4] = RusText()->LcControlOff;			
}
//**************************************************************************************************************
//**************************************************************************************************************
void Task_PriorityGet  (void);//Получение приоритета управления блоком. Запускает сам себя
void Task_FaultsGet    (void);//Получение неисправностей.

void Task_DisplayUpdate(void);
void Task_LedUpdate		 (void);

void Task_Begin        (void);
void Task_GetControl   (void);


void Task_RS485ParsingCmdFromMB(void);
void Task_RS485TxDataForMB     (void);

void Task_RS485Protocol        (void);

void Task_ErrorConnectMB(void);
void Task_KeyControl    (void);
//**************************************************************************************************************
//**************************************************************************************************************

static volatile uint8_t priorityState = NO_ACT;
//-----------------------------------------------------------------------------
//Получение приоритета управления блоком.
void Task_PriorityGet(void){
	
	//static uint8_t priorityState = NO_ACT;
	//--------------------	
	switch(priorityState){
		//--------------------	
		case(NO_ACT):
			if(MicGetState() == MicActiveConst) 												priorityState = MIC_ACT; 								 	 		
			else if(ButtonInstant(PUSKbut))         										priorityState = BUTTON_PUSK_ACT;
			else if(FireLineGetLogicState(Poj1) == FireLineATTENTION ||
					    FireLineGetLogicState(Poj2) == FireLineATTENTION)  	priorityState = FIRE_LINE_ACT;
			else if(FireLineGetLogicState(ChS)  == FireLineFIRE)				priorityState = ChS_ACT;
			else if(ButtonInstant(STARTbut))         										priorityState = BUTTON_START_ACT;
			else if(ButtonGetInstant(ALG_BUTTONS) != 0)									priorityState = BUTTONS_ALG_ACT;
		break;
		//--------------------	
		//Приоритет 1(высший) - Активация тангентой микрофона.
		case(MIC_ACT):
			
			if(MicGetState() != MicActiveConst) priorityState = NO_ACT; 			
		
		break;
		//--------------------	
		//Приоритет 2 - Управление кнокой "ПУСК".
		case(BUTTON_PUSK_ACT):
			
			if(!ButtonInstant(PUSKbut)) priorityState = NO_ACT;
		
		break;
		//--------------------
		//Приоритет 3 - Активация встроенного тревожного сообщения шлейфом «Пож.1» и/или «Пож.2».
		case(FIRE_LINE_ACT):
				
			
		
		break;
		//--------------------	
	  //Приоритет 4 - ГО и ЧС. Активация приоритета осуществляется входным шлейфом «ЧС».
		case(ChS_ACT):
		break;
		//--------------------	
		//Приоритет 5 - Управление оповещением кнопками СТАРТ и СТОП
		case(BUTTON_START_ACT):
		break;
		//--------------------	
		//Приоритет 6 - Ручное управление кнопками "Алгоритмы Оповещения".
		case(BUTTONS_ALG_ACT):
		break;
		//--------------------	
		default:
		break;		
		//--------------------	
	}
	//--------------------	
	RTOS_SetTimerTask(Task_PriorityGet, 5);	
	
	
	
	
	
	
//  //--------------------
//  //Приоритет 1(высший). 
//  //Активация тангентой микрофона лицевой панели.
//  if(MicGetState() == MicActiveConst) return PRIORITY_1;
//  //--------------------
	//Управление кнокой "ПУСК".	
//	if((ButGetInstantState(CONTROL_BUTTONS) & START_BUTTON) || (ButTempUnitControl & START_BUTTON))	
//		{
//			return START_Activation;
//		}	
  //--------------------
  //Приоритет 2 - Активация встроенного тревожного сообщения.
  //Активация приоритета осуществляется входным шлейфом «Пож.1» или «Пож.2».
//  else if((FireLineGetLogicState(Poj1) == FireLineFIRE) || (FireLineGetLogicState(Poj2) == FireLineFIRE))	
//		{
//			return PRIORITY_2;
//		}
  //--------------------
  //Приоритет 3 - ГО и ЧС. 
  //Активация приоритета осуществляется входным шлейфом «ЧС».
//  else if(FireLineGetLogicState(ChS) == FireLineNormClose)
//    {
//			return PRIORITY_3;
//    } 
  //--------------------
	//Управление оповещением кнопками СТАРТ и СТОП
//	else if(ButGetToggleState(CONTROL_BUTTONS) & START_BUTTON)	
//		{
//			return START_Activation;
//		}		
//	//--------------------	
//  //Приоритет 6 - Ручное управление.
//  if(ButGetToggleState(PRESET_BUTTONS) & (PRES1_BUTTON | PRES2_BUTTON))
//    {
//			return Priority_6;
//    }
//  //--------------------
////  //Управление выходным реле "ПОЖ.3".
////  if(Poj3StatReg == NormCloseSost)RelayOrLedOn (RelPoj3);
////  if(Poj3StatReg == NormOpenSost) RelayOrLedOff(RelPoj3);
//		
//	return 0;
}
//-----------------------------------------------------------------------------
//Определение неисправностей.
void Task_FaultsGet(void){

	FaultsLoop();
	//--------------------	
	RTOS_SetTimerTask(Task_FaultsGet, 5);	
}
//-----------------------------------------------------------------------------
//Очистка видеопамяти в MenuUpdate(), LockModeUnit() и в ControlModeUnit().
void Task_DisplayUpdate(void){
	
//	//Отображение событий системы: 
//	//ПУСК(самый высокий приоритет)-ПОЖАР-НЕИСПРАВНОСТЬ-дригие события-настройка блока.
//	static uint8_t display = 0;
//	//--------------------	
//	switch(display){
//		//--------------------	
//		//настройка блока.
//		case(CONFIGURATION):
//			
//		break;
//		//--------------------		
//		// дригие события системы.
//		case(OTHER_EVENTS):

//		break;
//		//--------------------	
//		//НЕИСПРАВНОСТЬ
//		case(FAULT):
//		
//		break;
//		//--------------------	
//		//ПОЖАР(или ВНИМАНИЕ)
//		case(FIRE):
//		
//		break;
//		//--------------------
//		//ПУСК(самый высокий приоритет)
//		case(PUSK):
//				
//		break;
//		//--------------------	
//		default:
//		break;		
//		//--------------------	
//	}
//	//--------------------	
  //LcdUpdate();
	LcdUpdateAll();
	//RTOS_SetTimerTask(Task_DisplayUpdate, 1);
}
//-----------------------------------------------------------------------------
//Обновление светодиодов.
void Task_LedUpdate(void){

  //---------------------
  //Работа с кнопкой "ТЕСТ".
	if(FlagReg & TestOn_Flag)
		{ 
			LedUpdate(); //Обновление светодиодов. 
			return;
		}
  //--------------------
	LedPowerIndication(PowerDevice()->MainPower);//Индикатор "ПИТАНИЕ"
	LedBatteryIndication(PowerDevice()->Bat);    //Индикатор "АКБ" 
	LedAmpIndication(PowerDevice()->Amp);		     //Индикатор "УМЗЧ" 	
  //--------------------
  //Индикатор "СВЯЗЬ" 
	LedCommunication(GreenColor  & Blink(BlinkInterval250mS));//есть обмен - мигающий земеный.	
  //--------------------
  //Индикатор "НЕИСП.ОБЩ." 
	if(Faults()->Instant & (FaultACNo_Flag   | FaultDCFault_Flag | FaultBat_Flag |
								          FaultSiren1_Flag | FaultSiren2_Flag  | FaultSiren3_Flag))
		{
			 LedControl(GENERAL_FAULT_LED, LedOn);		
		}		
	else LedControl(GENERAL_FAULT_LED, LedOff);	
  //--------------------
  //Индикатор "НЕИСП.Л.О."
  if((LcGetForLed(Line1) >= LineBreak) || (LcGetForLed(Att1) >= LineBreak)||
     (LcGetForLed(Line2) >= LineBreak) || (LcGetForLed(Att2) >= LineBreak))
    {
       LedControl(ALERT_LINES_FAUL_LED, LedOn);
    }
  else LedControl(ALERT_LINES_FAUL_LED, LedOff); 
  //--------------------
  //Индикатор "НЕИСП.П.Ш." 
  if((FireLineGetPhysState(Poj1) >= FireLineShort)||(FireLineGetPhysState(Poj2) >= FireLineShort)||
     (FireLineGetPhysState(Poj3) >= FireLineShort)||(FireLineGetPhysState(ChS)  >= FireLineShort))
    {
       LedControl(FIRE_LINES_FAUL_LED, LedOn);
    }
  else LedControl(FIRE_LINES_FAUL_LED, LedOff);
  //Вывод на дисплей состояния пожарных шлейфов.
  MenuPage(Page2)->PageItems.StringText[String1] = MenuGetTextStatPojLine(FireLineGetPhysState(Poj1));
  MenuPage(Page2)->PageItems.StringText[String2] = MenuGetTextStatPojLine(FireLineGetPhysState(Poj2));
  MenuPage(Page2)->PageItems.StringText[String3] = MenuGetTextStatPojLine(FireLineGetPhysState(Poj3));
  MenuPage(Page2)->PageItems.StringText[String4] = MenuGetTextStatPojLine(FireLineGetPhysState(ChS));  
  //---------------------
  //Вывод отладочной информации
  //MenuPage(Page4)->PageItems.StringVar[String2] = RS485FP_GetDebugFromMB(Debug1);
  //MenuPage(Page1)->PageItems.StringVar[String6] = RS485FP_GetDebugFromMB(Debug2);
  //--------------------- 
  //Управление светодиодом ОТКЛ.ЗВУК.
  LedDisactiveSound(Faults()->Instant);
  //---------------------
  //Работа с кнопкой "ТЕСТ".
  if(FlagReg & TestOn_Flag) *RS485Cmd() = FP_CMD_TEST_ZUMMER;
  //Говорим ЦП что находимся в режиме Блокировка Управления.
  //else Command = CmdFPStatusConst;
  //---------------------
  //Мигающая индикация в режиме настройки блока.
  if(KeyGetState() == UnitCofigModeConst) LedSetAll(Blink(BlinkInterval250mS));//мигающая индикация всеми светодиодами.                                                        
  //---------------------  
  LedUpdate(); //Обновление светодиодов. 
	//---------------------  
}
//*******************************************************************************************
//*******************************************************************************************
//Режим "БЛОКИРОВКА УПРАВЛЕНИЯ"
void Task_LockModeUnit(void){

	//--------------------
  //Работа с кнопкой "ТЕСТ".
  if(FlagReg & TestOn_Flag)
    {
			PrintTestmode();//Вывод на дисплей надписи "РЕЖИМ ТЕСТИРОВАНИЯ."
      return;
    }
  if((ButtonGetInstant(CONTROL_BUTTONS) & TEST_BUTTON)) 
    {
      FlagReg |= TestOn_Flag;
			*RS485Cmd() = FP_CMD_TEST_ZUMMER;			
    }
  //--------------------
  //Если был активирован ТЕСТ, то при переходе в режим Блокировка режим ТЕСТ отключаем.
  //mS_CounterForTEST = TESTmSConst;
  //--------------------
  LcdClear();                           //Очистка дисплея
  CounterToTimeAndDate(TimeFromMBGet());//Отображение времени.
  MenuIndexRegCleare();                 //Сброс указателей меню.
  ButtonClrToggle(PUSKbut);             //Сброс состояний кнопок меню.
	ButtonClrToggle(RESETbut);
  //--------------------
  //Управление светодиодами.
  LedPresetControl(Alg1Led, Line1, Zone1, Blink(BlinkInterval250mS));//Cветодиод ПРЕСЕТ1.
  LedPresetControl(Alg2Led, Line2, Zone2, Blink(BlinkInterval250mS));//Cветодиод ПРЕСЕТ2.
  //Гашение светодиодов СТАРТ и ПОЖАР.
  if((ZoneGet(Zone1) <= ActionManualOn) && (ZoneGet(Zone2) <= ActionManualOn))
    {
      LedPusk(LedOff);
      LedPoj(LedOff);
    }
  //--------------------
  //Передача состояний кнопки ОТКЛ.ЗВУК и ТЕСТ в ЦП.
  //RS485TxBufPtr()->STR.Control_Buttons = (ButtonGetInstant(CONTROL_BUTTONS) & (TEST_BUTTON | ZUMMER_BUTTON));
	RS485TxBufPtr()->Data.FP.Status.Control_Buttons = (ButtonGetInstant(CONTROL_BUTTONS) & (TEST_BUTTON | ZUMMER_BUTTON));
  //--------------------
	//Вывод на дисплей надписи о состоянии блока.  
  PrintMicState(MenuBackgroundPage());
	//--------------------
  //Вывод на дисплей состояния линий Гр.
  if(LcGetForLed(Line1) >= LineBreak) MenuTextDisplay(RusText_Zone1_Pointer, MenuGetTextStatLc(LcGetForLed(Line1)), String5); 
  if(LcGetForLed(Line2) >= LineBreak) MenuTextDisplay(RusText_Zone2_Pointer, MenuGetTextStatLc(LcGetForLed(Line2)), String6);
  
  MenuBackgroundPage()->NamePage                      = MenuGetRusText(RusText_LockModeUnit_Pointer);
  MenuBackgroundPage()->PageItems.StringName[String1] = MenuGetRusText(RusText_Address_Pointer);
  MenuBackgroundPage()->PageItems.StringName[String2] = MenuGetRusText(RusText_Group_Pointer);
  MenuBackgroundPageDisplay(MenuBackgroundPage());
  //--------------------
}
//*******************************************************************************************
//*******************************************************************************************
//Режим "УПРАЛЕНИЯ".
void Task_ControlModeUnit(void){

//	static uint8_t  presetButState  = 0;
//	static uint8_t  controlButState = 0;	
	static volatile uint8_t ControlModeUnitFSMState = 0;
  //--------------------
  //Работа с кнопкой "ТЕСТ".
  if(FlagReg & TestOn_Flag)
    {
			PrintTestmode();//Вывод на дисплей надписи "РЕЖИМ ТЕСТИРОВАНИЯ."
      return;
    }
	if(ButtonInstant(TESTbut))//активация режима ТЕСТ индикации 
    {
      FlagReg |= TestOn_Flag;
			*RS485Cmd() = FP_CMD_TEST_ZUMMER;
    }
  //--------------------
  LcdClear();                           //Очистка дисплея
	CounterToTimeAndDate(TimeFromMBGet());//Отображение времени.
  MenuIndexRegCleare();                 //Сброс указателей меню.
  ButtonClrToggle(PUSKbut);             //Сброс состояний кнопок меню.
	ButtonClrToggle(RESETbut);
	//-------------------------------------------------------	
	//-------------------------------------------------------	
	//Работа с выносным микрофоном (наивысший приоритет).
		
		
		
		
		
		
		
		
		
		
		
		
		
		
//  LedPresetControl(Alg1Led, Line1, Zone1, Blink(BlinkInterval250mS));//Cветодиод ПРЕСЕТ1.
//  LedPresetControl(Alg2Led, Line2, Zone3, Blink(BlinkInterval250mS));//Cветодиод ПРЕСЕТ2.
	
//	LcdString(Char1, String6);
//	LcdOutStr("Sound source: LinIn2");	
//		
//	LcdString(Char1, String7);
//	LcdOutStr("SpLine:1 2 3 4 5 6");
//		
//	LcdString(Char1, String8);
//	LcdOutStr("Action:- - - - - -");







	//-------------------------------------------------------		
	//-------------------------------------------------------	
//	//Машина состояний в режиме УПРАВЛЕНИЕ. 
//	switch(ControlModeUnitFSMState){
//			//--------------------
//			//Дежурный режим – состояние готовности прибора к передаче на оповещатели речевой информации
//			//о возникновении пожара, сопровождаемое отсутствием отображения иных режимов и наличием какой-либо трансляции 
//			case(0):
//				//Отключение светодиодов ПОЖАР, ПУСК, ОПОВЕЩЕНИЕ и всех светодиодов АЛГОРИТМЫ ОПОВЕЩЕНИЯ
//			  //LedPoj(LedOff);
//				LedSetAll(LedAllOff);
//				//Переход на ручное включение алгоритмов оповещения по нажатию СТАРТ.
//				if(ButtonInstant(STARTbut)) ControlModeUnitFSMState = 1;
//				//Управление кнопками АЛГОРИТМ(ручной выбор зон)
//				if(ButtonInstant(ALG1but)) ControlModeUnitFSMState = 2;
//			  //Переход в режим ВНИМАНИЕ в случае активации хотябы одного пож входа.
//				if(FireLineGetLogicState(Poj1) == FireLineATTENTION ||
//					 FireLineGetLogicState(Poj2) == FireLineATTENTION ||
//					 FireLineGetLogicState(Poj3) == FireLineATTENTION ||
//				   FireLineGetLogicState(ChS)  == FireLineATTENTION) 
//					{
//						ControlModeUnitFSMState = 3;
//					}
//				//Переход в режим ПОЖАР. 
//				if(FireLineGetLogicState(Poj1) == FireLineFIRE ||
//					 FireLineGetLogicState(Poj2) == FireLineFIRE ||
//					 FireLineGetLogicState(Poj3) == FireLineFIRE ||
//					 FireLineGetLogicState(ChS)  == FireLineFIRE) 
//					{
//						ControlModeUnitFSMState = 4;
//					}
//				//
//			
//				//Управление светодиодами.
//				//LedPresetControl(Alg1Led, Line1, Zone1, Blink(BlinkInterval250mS));//Cветодиод ПРЕСЕТ1.
//				//LedPresetControl(Alg2Led, Line2, Zone2, Blink(BlinkInterval250mS));//Cветодиод ПРЕСЕТ2.		
//				
////				if(PojLineGetLogicState(Poj1) == FireLineATTENTION) LedPoj(LedOn & Blink(BlinkInterval250mS));
////				else LedPresetControl(Alg1Led, Line1, Zone1, Blink(BlinkInterval250mS));//Cветодиод ПРЕСЕТ1.
//			break;
//			//--------------------
//			//ручное включение алгоритмов оповещения по нажатию СТАРТ.
//			case(1):		

//				if(AlgorithmAlert()) ControlModeUnitFSMState = 0;
//				//ControlModeUnitFSMState = 0;
//			
//			break;
//			//--------------------
//			//Управление кнопками АЛГОРИТМ(ручной выбор зон)
//			case(2):
//				
//				//Управление светодиодами.
//				LedPresetControl(Alg1Led, Line1, Zone1, Blink(BlinkInterval250mS));//Cветодиод ПРЕСЕТ1.
//				LedPresetControl(Alg2Led, Line2, Zone2, Blink(BlinkInterval250mS));//Cветодиод ПРЕСЕТ2.	
//				//Передача состояний кнопок в ЦП.
//				RS485TxBufPtr()->STR.Control_Buttons = ButtonGetInstant(CONTROL_BUTTONS);
//				RS485TxBufPtr()->STR.Alg_Buttons     = ButtonGetInstant(ALG_BUTTONS);
//				 
//				//Возврат в дежурный режим по нажатию кнопки СБРОС
//				if(ButtonInstant(RESETbut)) 
//					{
//						ControlModeUnitFSMState = 0;
//					}
//			break;
//			//--------------------
//			//Режим ВНИМАНИЕ.
//			case(3):
//				//мигающая индикация режима ВНИМАНИЕ светодиода ПОЖАР.	
//				LedPoj(LedOn & Blink(BlinkInterval250mS));
//				//Переход в режим ПОЖАР. 
//				if(FireLineGetLogicState(Poj1) == FireLineFIRE ||
//					 FireLineGetLogicState(Poj2) == FireLineFIRE ||
//					 FireLineGetLogicState(Poj3) == FireLineFIRE ||
//					 FireLineGetLogicState(ChS)  == FireLineFIRE) 
//					{
//						ControlModeUnitFSMState = 4;
//					}
//				//Возврат в дежурный режим если был переход из сотояния ВНИМАНИЕ в состояние ПОДКОНТРОЛЕМ
//				if(FireLineGetLogicState(Poj1) <= FireLineUNDERCONTROL &&
//					 FireLineGetLogicState(Poj2) <= FireLineUNDERCONTROL &&
//					 FireLineGetLogicState(Poj3) <= FireLineUNDERCONTROL &&
//					 FireLineGetLogicState(ChS)  <= FireLineUNDERCONTROL) 
//					{
//						ControlModeUnitFSMState = 0;
//					}
//				//Возврат в дежурный режим по нажатию кнопки СБРОС
//				if(ButtonInstant(RESETbut)) ControlModeUnitFSMState = 0;
//			break;
//			//--------------------
//			//Режим ПОЖАР.
//			case(4):
//				//постоянно горящая индикация режима ПОЖАР светодиода ПОЖАР.	
//				LedPoj(LedOn);	
//				//Возврат в дежурный режим если был переход из сотояния ВНИМАНИЕ в состояние ПОДКОНТРОЛЕМ
//				if(FireLineGetLogicState(Poj1) <= FireLineUNDERCONTROL &&
//					 FireLineGetLogicState(Poj2) <= FireLineUNDERCONTROL &&
//					 FireLineGetLogicState(Poj3) <= FireLineUNDERCONTROL &&
//					 FireLineGetLogicState(ChS)  <= FireLineUNDERCONTROL) 
//					{
//						ControlModeUnitFSMState = 0;
//						LedPoj(LedOff);
//					}			
//			break;	
//			//--------------------
//			default:
//			break;	
//			//--------------------
//		}		
	//-------------------------------------------------------		
	//-------------------------------------------------------
  //Гашение светодиодов СТАРТ и ПОЖАР.
//  if((ZoneGet(Zone1) <= ZoneManualOn) && (ZoneGet(Zone2) <= ZoneManualOn))
//    {
//      LedStart(LedOff);
//      LedPoj(LedOff);
//    }
  //--------------------
  //Передача состояний кнопок в ЦП.
  RS485TxBufPtr()->Data.FP.Status.Control_Buttons = ButtonGetInstant(CONTROL_BUTTONS);
  RS485TxBufPtr()->Data.FP.Status.Alg_Buttons     = ButtonGetInstant(ALG_BUTTONS);
		
	//Передача состояний кнопок ЗВУК.ОТКЛ. и ТЕСТ в ЦП.
	//RS485TxBufPtr()->STR.Control_Buttons = (ButtonGetInstant(CONTROL_BUTTONS) & (ZUMMER_BUTTON | TEST_BUTTON));		
	//--------------------
	//Вывод на дисплей надписи о состоянии микрофона.  
  PrintMicState(MenuBackgroundPage());
	//--------------------
  //Вывод на дисплей состояния линий Гр.
  if(LcGetForLed(Line1) >= LineBreak) MenuTextDisplay(RusText_Zone1_Pointer, MenuGetTextStatLc(LcGetForLed(Line1)), String5); 
  if(LcGetForLed(Line2) >= LineBreak) MenuTextDisplay(RusText_Zone2_Pointer, MenuGetTextStatLc(LcGetForLed(Line2)), String6);
  //--------------------
	//Обновление надписей в "шапке"
  MenuBackgroundPage()->NamePage                      = MenuGetRusText(RusText_ControlModeUnit_Pointer);//Заголовок страницы
  MenuBackgroundPage()->PageItems.StringName[String1] = MenuGetRusText(RusText_Address_Pointer);
  MenuBackgroundPage()->PageItems.StringName[String2] = MenuGetRusText(RusText_Group_Pointer);
  MenuBackgroundPageDisplay(MenuBackgroundPage());
  //--------------------
}
//*******************************************************************************************
//*******************************************************************************************
//Режим "НАСТРОЙКА"
void Task_CofigModeUnit(void){

	static uint16_t Redaction      = Off;
		
  static uint16_t IndexTime     = 3;
  static uint16_t IndexCalendar = 3;	
  static uint16_t IndexAddress  = 1;
	
	static uint16_t FireLineIndex = 2;
	
	//static uint16_t LogEventIndex    = 1;
	//static uint16_t LogEventIndexOld = 0;
	
	static uint16_t autoExitTimeoutCount = 0;
  //--------------------	
  //По нажатию кнопки СБРОС перемещение по пунктам меню
  if(ButtonToggle(RESETbut))
    {
      //--------------------         
      //Определение редактируемой страницы.
      switch(MenuIndexReg()->IndexMenuPage){
          //----------------------------------------------- 
					//----------------------------------------------- 
          //1-я страница меню - адрес, группа, время, дата.
          case(Page1):
            //Блокировка перемещения по пунктам страницы в случае редактирования пункта
            if(Redaction == Off) ButtonUpDown(&MenuIndexReg()->IndexPageItem, String1, String4);
            //-----------
            //Редактирование адреса.
            if(MenuIndexReg()->IndexPageItem == String1) //на первой строке.
              {
                ButtonLeftRigth(&IndexAddress, 0, 1);
                if(IndexAddress < 1)
                  {
                    Redaction = On;            
                    ButtonUpDown(&Address, 0, 64);
                    MenuAddressUpdate(IndexAddress, Address, Blink(BlinkInterval250mS));

                    //if(ButtonGetInstant(CONTROL_BUTTONS) & PUSK_BUTTON)
										if(ButtonInstant(PUSKbut))
                      {
                        Redaction    = Off;
                        IndexAddress = 1;
												*RS485Cmd()  = FP_CMD_SET_ADDRES;//Команда установки адреса.
                        ButtonClrToggle(RESETbut);
                      }
                    return; 
                  }
                 else
                  {
                    Redaction    = Off;
                    IndexAddress = 1;
                  }                     
               }
            //----------------------------
            //Редактирование группы.
            if(MenuIndexReg()->IndexPageItem == String2) //на второй строке.
              {
                ButtonLeftRigth(&IndexAddress, 0, 1);
                if(IndexAddress < 1)
                  {
                    Redaction = On;            
                    ButtonUpDown(&Group, 0, 64);
                    MenuGroupUpdate(IndexAddress, Group, Blink(BlinkInterval250mS));

                    //if(ButtonGetInstant(CONTROL_BUTTONS) & PUSK_BUTTON)
										if(ButtonInstant(PUSKbut))
                      {
                        Redaction    = Off;
                        IndexAddress = 1;
                        *RS485Cmd()  = FP_CMD_SET_GROUP;//Команда установки группы.
                        ButtonClrToggle(RESETbut);
                      }
                    return; 
                  }
                 else
                  {
                    Redaction    = Off;
                    IndexAddress = 1;
                  }                     
               }
            //----------------------------
            //Редактирование времени.
            if(MenuIndexReg()->IndexPageItem == String3) //на третьей строке.
              {
                ButtonLeftRigth(&IndexTime, 0, 3);
                if(IndexTime < 3)
                  {
                    Redaction = On;
                    //Часы
                    if(IndexTime == 0)
                      {
												ButtonUpDown(&DateAhdTimeGet()->hour, 0, 23);
                        MenuTimeUpdate(IndexTime, DateAhdTimeGet()->hour, Blink(BlinkInterval250mS), DateAhdTimeGet());
											}
                    //минуты
                    if(IndexTime == 1)
                      {
												ButtonUpDown(&DateAhdTimeGet()->min, 0, 59);
                        MenuTimeUpdate(IndexTime, DateAhdTimeGet()->min, Blink(BlinkInterval250mS), DateAhdTimeGet());
                      }
                    //секунды
                    if(IndexTime == 2)
                      {
												ButtonUpDown(&DateAhdTimeGet()->sec, 0, 59);
                        MenuTimeUpdate(IndexTime, DateAhdTimeGet()->sec, Blink(BlinkInterval250mS), DateAhdTimeGet());
                      }             
                    if(ButtonGetInstant(CONTROL_BUTTONS) & PUSK_BUTTON)
                      {
                        Redaction   = Off;
                        IndexTime   = 3;
												*RS485Cmd() = FP_CMD_SET_TIME;//Команда установки времени..
                        RS485TxBufPtr()->Data.FP.Status.TimeUTC = TimeAndDateToCounter(DateAhdTimeGet());
                        ButtonClrToggle(RESETbut);
                      } 
                    return;   
                  }
                else
                  {
                    Redaction = Off;
                    IndexTime = 3;
                  }  
              }
            //----------------------------
            //Редактирование даты.
						if(MenuIndexReg()->IndexPageItem == String4) //на четвертой строке.
              {	
								ButtonLeftRigth(&IndexCalendar, 0, 3);
                if(IndexCalendar < 3)
                  {
                    Redaction = On;
                    //День
                    if(IndexCalendar == 0)
                      {
                        ButtonUpDown(&DateAhdTimeGet()->day, 1, 31);
                        MenuCalendarUpdate(IndexCalendar, DateAhdTimeGet()->day, Blink(BlinkInterval250mS), DateAhdTimeGet());
                      }
                    //Месяц
                    if(IndexCalendar == 1)
                      {
                        ButtonUpDown(&DateAhdTimeGet()->month, 1, 12);
                        MenuCalendarUpdate(IndexCalendar, DateAhdTimeGet()->month, Blink(BlinkInterval250mS), DateAhdTimeGet());
                      }
                    //Год.
                    if(IndexCalendar == 2)
                      {
												ButtonUpDown(&DateAhdTimeGet()->year, 1970, 2050);
                        MenuCalendarUpdate(IndexCalendar, DateAhdTimeGet()->year, Blink(BlinkInterval250mS), DateAhdTimeGet());
                      }             
                    if(ButtonGetInstant(CONTROL_BUTTONS) & PUSK_BUTTON)
                      {
												Redaction     = Off;
                        IndexCalendar = 3;
												*RS485Cmd()   = FP_CMD_SET_DATA;//Команда установки даты.
                        RS485TxBufPtr()->Data.FP.Status.TimeUTC = TimeAndDateToCounter(DateAhdTimeGet());
                        ButtonClrToggle(RESETbut);
                      } 
                    return;   
								}
							else
								{
									Redaction     = Off;
									IndexCalendar = 3;
								}  
							}
						//-----------
          break;
          //----------------------------------------------- 
					//----------------------------------------------- 
          //2-я страница меню - Состояние пожарных шлейфов.
          case(Page2):
						//Блокировка перемещения по пунктам страницы в случае редактирования пункта.
					  //Редактируются параметры с 2-й строки по 5-ю строку.
            if(Redaction == Off) ButtonUpDown(&MenuIndexReg()->IndexPageItem, String2, String5);	
						//Вход в редактируемый параметр по нажатию кнопки ЗВУК ОКТЛ.
						ButtonLeftRigth(&FireLineIndex, 0, 2);//Выбор редактируемого разряда(парметра)
						if(FireLineIndex < 2)
							{
//								//Автовыход из редактирования параметра чере 10 сек.
//								if(ButtonGetInstant(CONTROL_BUTTONS) == 0)
//									{
//										if(++autoExitTimeoutCount >= 300)// 300 - эмпирическая контстанта для выдержки в примерно 10 секунд.
//											{
//												FireLineIndex = 2;
//												autoExitTimeoutCount = 0;
//											}
//									}
//								else autoExitTimeoutCount = 0;
								//------
								//Режим редактирования выбранного параметра.
								Redaction = On;
								//Номер входа, для которого производится насройка 
								//FireLine(MenuIndexReg()->IndexPageItem - 1)->Number = (MenuIndexReg()->IndexPageItem - 1);
								FireLine(Poj1)->Number = Poj1;
								//Режим работы входа. Всего 3 режима (0 - 2).
								if(FireLineIndex == 1)
									{
										//ButtonUpDown((uint16_t*)&FireLine(MenuIndexReg()->IndexPageItem - 1)->Type, 0, 2);
										
										FireLine(Poj1)->Type    = 1;
										FireLine(Poj1)->TimeOut = 66;
										
										MenuInputConfigUpdate(MenuIndexReg()->IndexPageItem, FireLineIndex, FireLine(Poj1));
										
										//MenuInputConfigUpdate(MenuIndexReg()->IndexPageItem, FireLineIndex, FireLine(MenuIndexReg()->IndexPageItem - 1));
									}
								//Таймаут перехода из режима ВНИМАНИЕ в режим ПУСК. От 0 до 99 сек.
								if(FireLineIndex == 0)
									{
										ButtonUpDown((uint16_t*)&FireLine(MenuIndexReg()->IndexPageItem - 1)->TimeOut, 0, 99);
										MenuInputConfigUpdate(MenuIndexReg()->IndexPageItem, FireLineIndex, FireLine(MenuIndexReg()->IndexPageItem - 1));
									}  
								//Cохранение параметров по нажатию кнопки ПУСК.    
								if(ButtonInstant(PUSKbut))
									{
										//Redaction     = Off;//Указатель "<=" остается у редактируемой строки после применения параметра
										FireLineIndex = 2;
										*RS485Cmd()   = FP_CMD_SET_INPUT_CONFIG;//Команда получени параметров одного входа пожарного шлейфа.
										//RS485TxBufPtr()->STR.KeyAndMicState = FireLineGetInputConfig(MenuIndexReg()->IndexPageItem - 1)->Number;//Номер входа, для которого производится насройка 
										//ButtonClrToggle(RESETbut);
									} 
								return;   
							}
						else
							{
								Redaction     = Off;
								FireLineIndex = 2;
							}  
						//-----------
          break;
          //----------------------------------------------- 
					//----------------------------------------------- 
          //3-я страница меню - Состояние линий Гр.
          case(Page3):
            ButtonUpDown(&MenuIndexReg()->IndexPageItem, String1, String5);//Перемещение по пунктам страницы.
						DisplayTextLcState(LcGetControl(AllLines));  									 //Вывод на дисплей состояния линий Гр.
						//Постановка линий Гр. на контроль.
						if(MenuIndexReg()->IndexPageItem <= String4)
							{
								if(ButtonInstant(ZUMMERbut)) LcSetControl((uint8_t)MenuIndexReg()->IndexPageItem, ControlOn);
								if(ButtonInstant(TESTbut))   LcSetControl((uint8_t)MenuIndexReg()->IndexPageItem, ControlOff);
							}
            //Редактирование отклонения линии Гр.
            if(MenuIndexReg()->IndexPageItem == String5)
              {
                ButtonLeftRigth(&MenuPage(Page3)->PageItems.StringVar[String5], 1, 500);
              }
            //else MenuPage(Page3)->PageItems.StringVar[String5] = LcDeviationGet();
						//Cохранение параметров по нажатию кнопки ПУСК.          
						if(ButtonInstant(PUSKbut))
              {  
								*RS485Cmd() = FP_CMD_LC_SAVE_REF;
								//RS485TxBufPtr()->Data.FP.Status.KeyAndMicState = LcGetControl(AllLines);							        //линии поставленные на контроль.           								
								//RS485TxBufPtr()->Data.FP.Status.Address        = MenuIndexReg()->IndexPageItem;							  //сохранения опорной величины для выбранной линииГр.	
								//RS485TxBufPtr()->Data.FP.Status.Group          = MenuPage(Page3)->PageItems.StringVar[String5];//значение отклолнения линии Гр.
								ButtonClrToggle(RESETbut);//Для выхода из редактирования 
              }
          break;
          //----------------------------------------------- 
					//----------------------------------------------- 
					//4-я страница меню - питание блока.
          case(Page4):	
						ButtonUpDown(&MenuIndexReg()->IndexPageItem, String1, String2);//Перемещение по пунктам страницы.
						PrintPowerState();  									 
					  //постановка на контроль основного питания.
						if(MenuIndexReg()->IndexPageItem == String1)
							{
								if(ButtonInstant(ZUMMERbut)) PowerDevice()->Check.bit.MainPower = On;
								if(ButtonInstant(TESTbut))   PowerDevice()->Check.bit.MainPower = Off;
							}
            //постановка на контроль резервного питания.
            if(MenuIndexReg()->IndexPageItem == String2)
              {
								if(ButtonInstant(ZUMMERbut)) PowerDevice()->Check.bit.Bat = On;
								if(ButtonInstant(TESTbut))   PowerDevice()->Check.bit.Bat = Off;
              }	
						//Cохранение параметров по нажатию кнопки ПУСК.          
						if(ButtonInstant(PUSKbut))
              {
								*RS485Cmd() = FP_CMD_SET_POWER_CHECK;
								RS485TxBufPtr()->Data.FP.Status.MicState = PowerDevice()->Check.Byte;        							
								ButtonClrToggle(RESETbut);//Для выхода из редактирования 
              }							
          break;
					//----------------------------------------------- 
					//----------------------------------------------- 
          //5-я страница меню - состяние платы управления оповещателями.
          case(Page5):
						ButtonClrToggle(RESETbut);	
          break;
          //----------------------------------------------- 
					//----------------------------------------------- 
          //6-я страница меню - просмотр журнала событий
          case(Page6):				
            ButtonUpDown(&LogWorkReg()->EventIndex, 1, LogWorkReg()->TotalEvents);            //Перемещение по записям в журнале событий.
						if(ButtonInstant(ZUMMERbut)) LogWorkReg()->EventIndex = 1;                  			 //Получение первой записи по нажатию ЗВУК.ОТКЛ.
						if(ButtonInstant(TESTbut))   LogWorkReg()->EventIndex = LogWorkReg()->TotalEvents;//Получение последней записи по нажатию ТЕСТ.
						if(ButtonInstant(PUSKbut))   LogWorkReg()->EventIndex = 0xFFFF;             			 //Все непросмотренные события сделать просмотренными при нажатии ПУСК.
						if(LogWorkReg()->EventOldIndex != LogWorkReg()->EventIndex)
							{
								LogWorkReg()->EventOldIndex = LogWorkReg()->EventIndex;
							  //RS485TxBufPtr()->Data.FP.Status.TimeUTC = (uint32_t)(LogWorkReg()->EventIndex & 0x0000FFFF);
								*RS485Cmd() = FP_CMD_GET_LOG;
								if(LogWorkReg()->EventIndex > MaxEvent) LogWorkReg()->EventIndex = LogWorkReg()->TotalEvents;
							}
          break;
          //----------------------------------------------- 
					//----------------------------------------------- 
					//7-я страница меню.
          case(Page7):
						if(ButtonInstant(RESETbut))
							{
								*RS485Cmd() = FP_CMD_GET_EEPROM_INFO;
							}					
						ButtonClrToggle(RESETbut);	
					break;
          //----------------------------
          default:
          break;
          //----------------------------
        }
      //--------------------   
			CounterToTimeAndDate(TimeFromMBGet());																								//Отображение времени.
			MenuUpdate(MenuPage(MenuIndexReg()->IndexMenuPage), 1, MenuIndexReg()->IndexPageItem);//Обновление состояния страницы меню. 
    }
  //--------------------
  //Перемещение по страницам меню.
  else
    { 
      Redaction    									= Off;//Разрешение перемещения по пунктам меню.
      IndexTime                     = 3;
			IndexCalendar                 = 3;	
      IndexAddress                  = 1;
			FireLineIndex                 = 2;
      MenuIndexReg()->IndexPageItem = 0;
			DisplayTextLcState(LcGetControlFromMB());									 //Вывод на дисплей состояния линий Гр от ЦП.
			PrintPowerState();   							 												 //Вывод на дисплей состояния питания.
			LogWorkReg()->EventIndex = LogWorkReg()->TotalEvents;      //Отображение журнала с последнего записаного события
      Address = MenuPage(Page1)->PageItems.StringVar[String1];   //адрес от ЦП
      Group   = MenuPage(Page1)->PageItems.StringVar[String2];   //группа от ЦП.
      MenuPage(Page3)->PageItems.StringVar[String5] = LcDeviationGet();//отклонения от установившегося значения для фиксации перех-го проц-са на линии.        
			CounterToTimeAndDate(TimeFromMBGet());                           //Отображение времени.
			//--------------------
			if(ButtonInstant(TESTbut))   MenuIndexReg()->IndexMenuPage = MaxMenuPages-1;//Переход на последнюю страницу при нажатии кнапки ТЕСТ.
			if(ButtonInstant(ZUMMERbut)) MenuIndexReg()->IndexMenuPage = Page1;				  //Переход на первую страницу при нажатии кнапки ЗВУК ОТКЛ..
      ButtonUpDown(&MenuIndexReg()->IndexMenuPage, 0, MaxMenuPages-1); 					  //Перемещение по страницам меню.
      //--------------------
			MenuUpdate(MenuPage(MenuIndexReg()->IndexMenuPage), 0, 0);       //Обновление состояния страницы меню.
    }
  //--------------------
}
//*******************************************************************************************
//*******************************************************************************************
void Task_RS485ParsingCmdFromMB(void){
	
	static uint8_t FireLineCount = 0;
 	//---------------------
	//Пришла команда "Статус ЦП". Сохраняем данные от ЦП
	if(RS485RxBufPtr()->CmdCode == CmdMBStatusConst)
		{
			FireLineSave   (&RS485RxBufPtr()->Data.MB.Status.StatusPoj1);      //Сохранение состояния линий пожарных линий.
			LcSave         (&RS485RxBufPtr()->Data.MB.Status.StatusZone1);     //Сохранение состояния линий Гр.
			LcDeviationSave(&RS485RxBufPtr()->Data.MB.Status.LcDeviationVolue);//Сохранение отклонения линий Гр.
			ZoneSave       (&RS485RxBufPtr()->Data.MB.Status.StatusZone1);     //Сохранение состояния зон - кем были активированы зоны.
			
			PowerDevice()->MainPower = RS485RxBufPtr()->Data.MB.Status.StatusPOWER & PowerMask;//Сохранение состояния питания и ЗУ.
			PowerDevice()->Bat			 = RS485RxBufPtr()->Data.MB.Status.StatusPOWER & BatMask;
			
			if(PowerDevice()->MainPower == PowerControlOff) PowerDevice()->CheckFromMB.bit.MainPower = Off;
			else                                            PowerDevice()->CheckFromMB.bit.MainPower = On;
			
			if(PowerDevice()->Bat == BatControlOff) PowerDevice()->CheckFromMB.bit.Bat = Off;
			else                                    PowerDevice()->CheckFromMB.bit.Bat = On;			
			
			PowerDevice()->Amp = RS485RxBufPtr()->Data.MB.Status.StatusPA;    //Сохранение состояния УМЗЧ.
			
			Siren()->SirenPower = RS485RxBufPtr()->Data.MB.Status.SirenPower;  //Cохранение состояний оповещателей.
			Siren()->Siren1     = RS485RxBufPtr()->Data.MB.Status.Siren1Status;
			Siren()->Siren2     = RS485RxBufPtr()->Data.MB.Status.Siren2Status;
			Siren()->Siren3     = RS485RxBufPtr()->Data.MB.Status.Siren3Status;
			
			TimeFromMBSave(RS485RxBufPtr()->Data.MB.Status.TimeUTC);			    //Cохранение времени ЦП.
			
			MenuPage(Page1)->PageItems.StringVar[String1] = RS485RxBufPtr()->Data.MB.Status.Addres;//адрес от ЦП
      MenuPage(Page1)->PageItems.StringVar[String2] = RS485RxBufPtr()->Data.MB.Status.Group; //группа от ЦП.		
			//---------------------
			//Сохранение адреса и группы ЦП.
			MenuBackgroundPage()->PageItems.StringVar[String1] = RS485RxBufPtr()->Data.MB.Status.Addres;
			MenuBackgroundPage()->PageItems.StringVar[String2] = RS485RxBufPtr()->Data.MB.Status.Group;
			//---------------------
			//Вывод на дисплей состояния питания блока.
			//MenuPage(Page4)->PageItems.StringText[String1] = TextGetPowerState(PowerGet());
			//MenuPage(Page4)->PageItems.StringText[String2] = TextGetBatState(BatGet()); 
			MenuPage(Page4)->PageItems.StringVar[String3]  = RS485RxBufPtr()->Data.MB.Status.DebugData1;//Uакб
			MenuPage(Page4)->PageItems.StringText[String4] = TextGetAmpState(PowerDevice()->Amp);
			//---------------------
			//Вывод на дисплей состояний оповещателей.
			MenuPage(Page5)->PageItems.StringVar[String1]  = Siren()->SirenPower; 
			MenuPage(Page5)->PageItems.StringText[String2] = TextGetSirenSate(Siren()->Siren1);
			MenuPage(Page5)->PageItems.StringText[String3] = TextGetSirenSate(Siren()->Siren2);
			MenuPage(Page5)->PageItems.StringText[String4] = TextGetSirenSate(Siren()->Siren3);
			//---------------------
			//сохранение журнала событий. 
			LogWorkReg()->TotalEvents   = RS485RxBufPtr()->Data.MB.Status.TotalEvents;
			LogWorkReg()->NotReadEvents = RS485RxBufPtr()->Data.MB.Status.NotReadEvents;
			//Вывод на дисплей информации о журнале событий.
			MenuPage(Page6)->PageItems.StringVar[String1] = RS485RxBufPtr()->Data.MB.Status.TotalEvents;
			MenuPage(Page6)->PageItems.StringVar[String2] = RS485RxBufPtr()->Data.MB.Status.NotReadEvents;
			//-----------
		}
	//--------------------------------------------	
	//Пришла команда "Получения журнала событий".
	if(RS485RxBufPtr()->CmdCode == FP_CMD_GET_LOG)
		{
			//По приходу записи о собитии ее нужно сохранить в FP.
			LogSaveEvent(RS485RxBufPtr()->Data.Buf);
			//После сохранения нужно вывести информацию о событии на экран в читаемом виде.
			MenuPage(Page6)->PageItems.StringVar[String3] = LogEvent()->EventNumber;
			MenuPage(Page6)->PageItems.StringVar[String4] = LogEvent()->EventID;
			MenuPage(Page6)->PageItems.StringVar[String5] = LogEvent()->EventParam;
			//-----------
		}
	//--------------------------------------------	
	//В режиме настройки выбрана страница "Шлейфы сигнализации". 
	//Начинаю запрошивать контекст логических входов.	
	if(MenuIndexReg()->IndexMenuPage == Page2) 
		{
			//Пришла команда получения параметров одного входа пожарного шлейфа.
			if(RS485RxBufPtr()->CmdCode == FP_CMD_GET_INPUT_CONFIG)
				{
					//По приходу параметров входа их нужно сохранить в FP.
					FireLine(RS485RxBufPtr()->Data.FireLine.Number)->Number    = RS485RxBufPtr()->Data.FireLine.Number;
					FireLine(RS485RxBufPtr()->Data.FireLine.Number)->Type      = 1;//RS485RxBufPtr()->Data.FireLine.Type;
					FireLine(RS485RxBufPtr()->Data.FireLine.Number)->TimeOut   = RS485RxBufPtr()->Data.FireLine.TimeOut;
					FireLine(RS485RxBufPtr()->Data.FireLine.Number)->PhysState = RS485RxBufPtr()->Data.FireLine.PhysState;
					FireLine(RS485RxBufPtr()->Data.FireLine.Number)->MeasValue = RS485RxBufPtr()->Data.FireLine.MeasValue;		
				}
			//--------------
			//Запросим параметры для всех 4-х входов (с 0 по 3).	
			else if(FireLineCount <= 3)
				{
					//Отправление команды получени параметров одного входа пожарного шлейфа в ЦП.
					//в MicState номер входа для которого запрашиваем конфигурацию.
					RS485TxBufPtr()->CmdCode = FP_CMD_GET_INPUT_CONFIG;
					RS485TxBufPtr()->Data.FP.Status.MicState = FireLineCount;	
					
					FireLineCount++;
					//--------------
					*RS485Cmd() = 0;
					RS485Flags()->MBNewDataFlag = FlagClr;
					RS485RxBufClr();                      //Очистка "буфера" для приема нового запроса
					RS485StartTxResponse();               //Запуск передачи ответа на запрос.
					return;
				}
			//--------------
			//Команда применения параметров одного входа пожарного шлейфа.
			//в KeyAndMicState номер входа для которого применяется конфигурацию.
			else if(*RS485Cmd() == FP_CMD_SET_INPUT_CONFIG)
				{
					RS485TxBufPtr()->CmdCode = FP_CMD_SET_INPUT_CONFIG;
					RS485TxBufPtr()->Data.FP.Status.MicState  = FireLine(MenuIndexReg()->IndexPageItem - 1)->Number;
					RS485TxBufPtr()->Data.FP.Status.Address 	= FireLine(MenuIndexReg()->IndexPageItem - 1)->Type;
					RS485TxBufPtr()->Data.FP.Status.Group   	= FireLine(MenuIndexReg()->IndexPageItem - 1)->TimeOut;
					//Запросим параметры для всех 4-х входов (с 0 по 3).
					FireLineCount = 0;
					//Для выхода из редактирования 
					ButtonClrToggle(RESETbut);
					//--------------
					*RS485Cmd() = 0;
					RS485Flags()->MBNewDataFlag = FlagClr;
					RS485RxBufClr();                      //Очистка "буфера" для приема нового запроса
					RS485StartTxResponse();               //Запуск передачи ответа на запрос.
					return;
				}
			//--------------
		}
	else
		{
			//При каждом переходе в режим НАСТРОЙКА и на страницу "Шлейфы сигнализации" 
			//запрос параметров для всех 4-х входов (с 0 по 3).
			FireLineCount = 0;
		}
	//--------------------------------------------	
	//--------------------------------------------
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//ОТЛАДКА!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		
	//Пришла команда с данными от EEPROM.
	if(MenuIndexReg()->IndexMenuPage == Page7)
		{
			//Пришла команда с данными от EEPROM.
			if(RS485RxBufPtr()->CmdCode == FP_CMD_GET_EEPROM_INFO)
				{
					//По приходу параметров входа их нужно сохранить в FP.
					Eeprom()->BadSectorsCount = RS485RxBufPtr()->Data.EepInfo.BadSectorsCount;
					Eeprom()->WriteCount 			= RS485RxBufPtr()->Data.EepInfo.WriteCount;
					Eeprom()->ReadCount  			= RS485RxBufPtr()->Data.EepInfo.ReadCount;
					Eeprom()->StatReg    			= RS485RxBufPtr()->Data.EepInfo.StatReg;
				}
			//--------------				
		} 	
	//КОНЕЦ ОТЛАДКИ!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!		
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//--------------------------------------------	
	//--------------------------------------------
	Task_RS485TxDataForMB();	
}
//*******************************************************************************************
//*******************************************************************************************
//Отправка данных в ЦП.
void Task_RS485TxDataForMB(void){
	
	//Определение команды для передачи в ЦП.
	switch(*RS485Cmd()){	
      //--------------------
      //Команда установки адрес блока
      case(FP_CMD_SET_ADDRES):
        RS485TxBufPtr()->CmdCode = FP_CMD_SET_ADDRES;
        RS485TxBufPtr()->Data.FP.Status.Address = Address;
        MenuBackgroundPage()->PageItems.StringVar[String1] = Address;
      break;
      //--------------------
      //Команда устновки группы блока
      case(FP_CMD_SET_GROUP):
        RS485TxBufPtr()->CmdCode = FP_CMD_SET_GROUP;
        RS485TxBufPtr()->Data.FP.Status.Group = Group;
        MenuBackgroundPage()->PageItems.StringVar[String2] = Group;
      break;
      //--------------------
      //Команда установики времени
      case(FP_CMD_SET_TIME):
        RS485TxBufPtr()->CmdCode = FP_CMD_SET_TIME;
      break;
      //-------------------- 
      //Команда установики даты
      case(FP_CMD_SET_DATA):
        RS485TxBufPtr()->CmdCode = FP_CMD_SET_DATA;
      break;
      //-------------------- 
      //Команда влючение тестирования зуммера ЦП
      case(FP_CMD_TEST_ZUMMER):
        RS485TxBufPtr()->CmdCode = FP_CMD_TEST_ZUMMER; 
        RS485TxBufPtr()->Data.FP.Status.MicState = Blink(BlinkInterval250mS); 
        //Command = 0;
      break;
      //-------------------- 
      //Команда сохранения опорных значений для Lc.
			//В KeyAndMicState будут активированные линии.
			//В Address будет номер канала для которого сохряняются опроные значения. 
			//В Group будет отклонения от установившегося значения для фиксации перех-го проц-са на линии. 
      case(FP_CMD_LC_SAVE_REF):
        RS485TxBufPtr()->CmdCode 								 = FP_CMD_LC_SAVE_REF;
				RS485TxBufPtr()->Data.FP.Status.MicState = LcGetControl(AllLines);							         //линии поставленные на контроль.           								
				RS485TxBufPtr()->Data.FP.Status.Address  = MenuIndexReg()->IndexPageItem;							   //сохранения опорной величины для выбранной линииГр.	
				RS485TxBufPtr()->Data.FP.Status.Group    = MenuPage(Page3)->PageItems.StringVar[String5];//значение отклолнения линии Гр.
      break;
			//--------------------
			//Команда получения одного собятия из журнала.
			//в TimeUTC номер записи журнала.
			case(FP_CMD_GET_LOG):
				RS485TxBufPtr()->CmdCode = FP_CMD_GET_LOG;
			  RS485TxBufPtr()->Data.FP.Status.TimeUTC = LogWorkReg()->EventIndex;
			break;						
			//--------------------
			//Команда сохранения контроля питания блока.
			case(FP_CMD_SET_POWER_CHECK):
				RS485TxBufPtr()->CmdCode = FP_CMD_SET_POWER_CHECK;
			break;	
			//--------------------
			//Команда получения информацио о EEPROM.
			case(FP_CMD_GET_EEPROM_INFO):
				RS485TxBufPtr()->CmdCode = FP_CMD_GET_EEPROM_INFO;
			break;	
			//--------------------
      //Команда "Выдача статуса от FP".
			//case(CmdFPStatusConst)://
      default: 
        RS485TxBufPtr()->CmdCode = FP_CMD_SEND_STATUS;               
				RS485TxBufPtr()->Data.FP.Status.MicState = MicGetState();
				RS485TxBufPtr()->Data.FP.Status.KeyState = KeyGetState();
        RS485TxBufPtr()->Data.FP.Status.Address  = Address;
        RS485TxBufPtr()->Data.FP.Status.Group    = Group;   
      break;
			//--------------------
    }
	//--------------------------------------------
	*RS485Cmd() = 0;											//
  RS485Flags()->MBNewDataFlag = FlagClr;//
  RS485RxBufClr();                      //Очистка "буфера" для приема нового запроса
	RS485StartTxResponse();               //Запуск передачи ответа на запрос.
}
//*******************************************************************************************
//*******************************************************************************************
//Начало работы программы.
void Task_Begin(void){

	//uint8_t i = 0;
	//--------------------
  //Вывод на дисплей логотопа.
//  MenuLogo();
//	Delay_mSec(2000);
	
//	for(i=0; i < 50; i++)
//		{
//			LcdBar(0, 0, 64, 63, i);
//			LcdUpdateAll();
//			Delay_mSec(10);	
//		}
//	LcdClear();
		
  //Горизонтальный прогресбар.
  //MenuProgressBar();
  //Delay_mSec(2000);
	//--------------------
	while(!GpioCheck()) Delay_mSec(10);//Определение состояния ключа.
	MenuInit();     									 //Инициализация пунктов меню.
	RS485Init();                       //Запуск протокола обмена.	
	TimeInit();												 //Инициализация календаря.
  //--------------------
  RTOS_SetTask(Task_PriorityGet);//Получение приоритета управления блоком.
	RTOS_SetTask(Task_FaultsGet);  //Определение неисправностей.
  RTOS_SetTask(Task_GetControl); //Определение управления.
}
//-----------------------------------------------------------------------------
//Определение управления.
void Task_GetControl(void){

			 if(RS485Flags()->MBNewDataFlag) RTOS_SetTask(Task_RS485ParsingCmdFromMB);//Анализ данных от МВ.
	else if(RS485Flags()->TimeOutFlag)   RTOS_SetTask(Task_ErrorConnectMB);       //Отсутствие обмена с ЦП.
  else if(ButtonCheck() || GpioCheck())RTOS_SetTask(Task_KeyControl);           //Управление блоком в соответсвии с положение ключа.
  //--------------------
  RTOS_SetTask(Task_DisplayUpdate);//Обновление изображения на дисплее. 
  RTOS_SetTimerTask(Task_GetControl, 5);
}
//-----------------------------------------------------------------------------
//Управление блоком в соответсвии с положение ключа.
void Task_KeyControl(void){

  static uint8_t OldKeyState = 0;
  //--------------------
  //Это нужно для отключения всех светодиодов при выходе из режима Настройки.
  if((OldKeyState == UnitCofigModeConst) && (KeyGetState() != UnitCofigModeConst))
    {
      LedSetAll(LedAllOff);
    }
	OldKeyState = KeyGetState();
  //--------------------
	if(OldKeyState == UnitLockModeConst)    RTOS_SetTask(Task_LockModeUnit);   //Режим "Блокировка управления".
	if(OldKeyState == UnitControlModeConst)	RTOS_SetTask(Task_ControlModeUnit);//Режим "Управление".
	if(OldKeyState == UnitCofigModeConst)	  RTOS_SetTask(Task_CofigModeUnit);  //Режим "Настройка".
  //--------------------		
	Task_LedUpdate(); //Обновление светодиодов.
	//--------------------		
}
//-----------------------------------------------------------------------------
//Ошибка обмена с ЦП.
void Task_ErrorConnectMB(void){

  LedSetAll(LedAllOff);
  LedCommunication(YellowColor & Blink(BlinkInterval250mS));//нет обмена - мигающий желтый.
  LedControl(GENERAL_FAULT_LED, LedOn);
  LedUpdate(); 					//Обновление светодиодов.
  PrintErrConnect();    //Вывод надписи "Нет связи с ЦП!!!".
	mS_CounterForTEST = 0;//Окончание тестирования индикации.
}
//************************************************************************************************** 
//**************************************************************************************************
int main(void){
    
  //--------------------
  //После сброса микроконтроллера программа переходит в main(), минуя SystemInit().
  //В фейле startup_stm32f10x_md_vl.s закоментирован переход на SystemInit.
  SysInit();     //Настройка тактирования микроконтроллера 72МГц. Внешний кварц 8 МГц.
  GpioInit();    //Инициализация портов.
  Spi2Init();    // 
  //LedInit();      //
  Lcd12864Init();//Инициализация дисплея WG12864A.
	//--------------------
	//Инициализация службы таймеров и службы сообщений для работы с конечными автоматами.
	SwTimersInit();		//Инициализация программных таймеров.
	FsmMessagesInit();//Инициализация службы сообщений.
  //-------------------- 
  RTOS_Init(); 	              //Инициализация диспетчера
  RTOS_SetTask(Task_Begin);   //Начало!!!
	
  SysTickInit(); 							//Запуск службы таймеров.
  __enable_irq();							//Глобальное разрешение рерываний.
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

	//--------------------
	//Счетчик для отладки.
//  static uint16_t mScountDebug = 0;	
//  if (++mScountDebug >= 10)
//    {
//      mScountDebug = 0;
//      //------------
//    }   
  //--------------------
  BlinkLoop();
  RS485TimeOutInc();  //Отсчет таймаута приема пакета.
	RTOS_TimerService();//Служба таймеров ОС.
	SwTimersLoop();			//Программные таймеры.
  //--------------------
  //Отработка режима "ТЕСТ".
  if(FlagReg & TestOn_Flag)
    { 
      //Мигаем всеми индикаторами.
      LedSetAll(Blink(BlinkInterval250mS));
      Lcd12864BackLightConrtol(Blink(BlinkInterval250mS));
      LedControl(TEST_LED, LedOn);
      //Окончание тестирования.
      if((++mS_CounterForTEST >= TESTmSConst) ||//Через 10 секунд или
         (RS485Flags()->TimeOutFlag))           //при отсутствии обмена с ЦП.
        {
          mS_CounterForTEST = 0; //
          Lcd12864BackLightConrtol(LcdBackLightOn);//
          FlagReg &= ~TestOn_Flag;                 //Окончание тестирования индикации.
          LedSetAll(LedAllOff);                    //
        }   
    }
  else mS_CounterForTEST = 0;
  //-------------------- 
}
//************************************************************************************************** 
//**************************************************************************************************


