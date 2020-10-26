
#include "main.h"

//**************************************************************************************************************
static uint8_t ControlModeUnitAlgButState = 0;
static uint8_t priorityState              = NO_ACT;

extern const char RusText_ControlModeUnit[];
extern const char RusText_LockModeUnit[];
//**************************************************************************************************************
void Task_LockModeUnit(void);
void Task_ControlModeUnit(void);
void Task_CofigModeUnit(void);
//**************************************************************************************************************
//**************************************************************************************************************
//Функция задержки на заданное количество м—ек.
void Delay_mSec(volatile uint32_t mSec){
  
 while(mSec)
    {
      if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) mSec--;
    }
}
//********************************************************************************
//Управление светодиодом ОТКЛ.ЗВУК.
void LedDisactiveSound(uint32_t fault){
	
	static uint32_t faultOldState = 0;
  //--------------------	
  //Нет неисправностей - гасим светодиод и выходим.
  if(fault == 0)
    {
      LedZummer(LedOff);
      faultOldState = 0;
    }
  //--------------------
  //Есть неисправность. 
  else if((faultOldState ^ fault) & fault)
    {
      Button_ClrToggle(ZUMMERbut);//сброс состояния кнопки ОТКЛ.ЗВУК.
      LedZummer(LedOff);
      faultOldState = fault;
    }
  else
    {
      if(Button_IsToggle(ZUMMERbut)) 
        {
          LedZummer(LedOn);
          faultOldState = fault;
        }
    }
  //--------------------
}
//********************************************************************************
//Ручное включение алгоритмов оповещения по нажатию СТАРТ.
uint8_t AlgorithmAlert(void){

	static uint8_t state     = 0;
	static uint8_t selectAlg = 0;
	//-------------------------------------------------------	
	switch(state){
		//--------------------
		case(0):
			if(Button_IsRelease(STARTbut)) state = 1;
		break;
		//--------------------
		case(1):
			//Мигающая индикация светодиода ОПОВЕЩЕНИЕ.
			LedAlert(LedOn & Blink(INTERVAL_250_mS));
			//Мигающая зеленая индикация у светодиодов выбранных АЛГОРИТМОВ ОПОВЕЩЕНИЯ
			if(Button_IsToggle(ALG1but))
				{
					LedControl(Alg1Led, GreenColor & Blink(INTERVAL_250_mS));
					selectAlg |= (1 << ALG1but); 
				}				
			else 	
				{
					LedControl(Alg1Led, LedOff);
					selectAlg &= ~(1 << ALG1but); 
				}
		
			if(Button_IsToggle(ALG2but))
				{
					LedControl(Alg2Led, GreenColor & Blink(INTERVAL_250_mS));
					selectAlg |= (1 << ALG2but); 
				}				
			else 	
				{
					LedControl(Alg2Led, LedOff);
					selectAlg &= ~(1 << ALG2but); 
				}
			//Переход на активацию выбранных АЛГОРИТМОВ ОПОВЕЩЕНИЯ по нажатию СТАРТ.
			if(Button_IsPress(STARTbut)) state = 2;	
			//Выход из ручного включения алгоритмов оповещения по нажатию СТОП.
			if(Button_IsPress(STOPbut))
				{
					Button_ClrToggle(STARTbut);
					Button_ClrToggle(STOPbut);
					Button_ClrToggle(ALG1but);
					Button_ClrToggle(ALG2but);
					
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
					LedControl(Alg1Led, RedColor & Blink(INTERVAL_250_mS));
					ControlModeUnitAlgButState |= ALG1_BUTTON;
				}
			
			else LedControl(Alg1Led, LedOff);
			//-------
			if(selectAlg & (1 << ALG2but)) 
				{
					LedControl(Alg2Led, RedColor & Blink(INTERVAL_250_mS));
					ControlModeUnitAlgButState |= ALG2_BUTTON;
				}	
			else LedControl(Alg2Led, LedOff);		
			
			//Выход из ручного включения алгоритмов оповещения по нажатию СТОП.
			if(Button_IsPress(STOPbut))
				{
					Button_ClrToggle(ALG1but);
					Button_ClrToggle(ALG2but);
					
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
//**************************************************************************************************************
//**************************************************************************************************************
//Получение приоритета управления блоком.
void Task_PriorityGet(void){
	
	//static uint8_t priorityState = NO_ACT;
	//--------------------	
	switch(priorityState){
		//--------------------	
		case(NO_ACT):
			     if(MicState() == MIC_ACTIVE) 										  priorityState = MIC_ACT; 								 	 		
			else if(Button_IsPress(PUSKbut))         								priorityState = BUTTON_PUSK_ACT;
			else if(FireLine_GetInState(Poj1) == FR_IN_ATTENTION ||
					    FireLine_GetInState(Poj2) == FR_IN_ATTENTION)  	priorityState = FIRE_LINE_ACT;
			else if(FireLine_GetInState(ChS)  == FR_IN_FIRE)				priorityState = ChS_ACT;
			else if(Button_IsPress(STARTbut))         							priorityState = BUTTON_START_ACT;
		break;
		//--------------------	
		//Приоритет 1(высший) - Активация тангентой микрофона.
		case(MIC_ACT):
			
			if(MicState() != MIC_ACTIVE) priorityState = NO_ACT; 			
		
		break;
		//--------------------	
		//Приоритет 2 - Управление кнокой "ПУСК".
		case(BUTTON_PUSK_ACT):
			
			if(Button_IsRelease(PUSKbut)) priorityState = NO_ACT;
		
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
	Scheduler_SetTask(Task_PriorityGet);	
}
//********************************************************************************
//Определение неисправностей.
void Task_FaultsGet(void){

	Faults_Loop();
	//--------------------		
	Scheduler_SetTask(Task_FaultsGet);
}
//********************************************************************************
//Очистка видеопамяти в MenuUpdate(), LockModeUnit() и в ControlModeUnit().
void Task_LcdUpdate(void){
	
  LcdUpdate();
	//---------------------
	Scheduler_SetTask(Task_LcdUpdate);
}
//********************************************************************************
//Обновление светодиодов.
void Task_LedUpdate(void){

	static uint8_t ledState = 0xff;
  //---------------------
  //Работа с кнопкой "ТЕСТ".
	if(MotherBoard()->MB_State == MB_TEST_STATE)
		{ 
			ledState = MB_TEST_STATE;
			LedUpdate(); //Обновление светодиодов. 
			return;
		}
	//это нужно для отключения светодиодов после тестирования.
	if(ledState != MB_WORK_STATE)
		{
			ledState = MB_WORK_STATE;
			LedSetAll(LedAllOff);
		}	
  //--------------------
	LedPowerIndication(PowerDevice()->MainPower);//Индикатор "ПИТАНИЕ"
	LedBatteryIndication(PowerDevice()->Bat);    //Индикатор "АКБ" 
	LedAmpIndication(PowerDevice()->Amp);		     //Индикатор "УМЗЧ" 	
  //--------------------
  //Индикатор "СВЯЗЬ" 
	LedCommunication(GreenColor & Blink(INTERVAL_250_mS));//есть обмен - мигающий земеный.	
  //--------------------
  //Индикатор "НЕИСП.ОБЩ." 
	if(Faults()->Instant & FAULTS_MASK) LedControl(GENERAL_FAULT_LED, LedOn);		
	else 																LedControl(GENERAL_FAULT_LED, LedOff);	
  //--------------------
  //Индикатор "НЕИСП.Л.О."
  if((SpLine_GetOutStateForLed(Line1) >= LineBreak) || (SpLine_GetOutStateForLed(Att1) >= LineBreak)||
     (SpLine_GetOutStateForLed(Line2) >= LineBreak) || (SpLine_GetOutStateForLed(Att2) >= LineBreak))
    {
       LedControl(ALERT_LINES_FAUL_LED, LedOn);
    }
  else LedControl(ALERT_LINES_FAUL_LED, LedOff); 
  //--------------------
  //Индикатор "НЕИСП.П.Ш." 
  if((FireLine_GetLineState(Poj1) >= FR_LINE_SHORT)||(FireLine_GetLineState(Poj2) >= FR_LINE_SHORT)||
     (FireLine_GetLineState(Poj3) >= FR_LINE_SHORT)||(FireLine_GetLineState(ChS)  >= FR_LINE_SHORT))
    {
       LedControl(FIRE_LINES_FAUL_LED, LedOn);
    }
  else LedControl(FIRE_LINES_FAUL_LED, LedOff);
  //--------------------- 
  //Управление светодиодом ОТКЛ.ЗВУК.
  LedDisactiveSound(Faults()->Instant);
  //---------------------
  //Работа с кнопкой "ТЕСТ".
  if(MotherBoard()->MB_State == MB_TEST_STATE) *RS485_Cmd() = FP_CMD_TEST_ZUMMER;
  //---------------------
  //Мигающая индикация в режиме настройки блока.
  if(KeyGetState() == KEY_CONFIG_STATE) LedSetAll(Blink(INTERVAL_250_mS));//мигающая индикация всеми светодиодами.                                                        
  //---------------------  
  LedUpdate(); //Обновление светодиодов. 
	//---------------------  
}
//********************************************************************************
void Task_RS485ParsingCmdFromMB(void){

	MBData_t *dataFromMB   = (MBData_t*)RS485_RxBuf()->Str.Data;
	FPData_t *dataForTx    = (FPData_t*)RS485_TxBuf()->Str.Data;
	FireLine_t *txFireLine = (FireLine_t*)RS485_TxBuf()->Str.Data;
	static uint8_t fireLineCount = 0;
 	//---------------------	
	RS485_Flags()->MB_NewData = FLAG_CLEAR;
	//---------------------
	//Пришла команда "Статус ЦП". Сохраняем данные от ЦП
	if(RS485_RxBuf()->Str.CmdCode == MB_CMD_SEND_STATUS)
		{
			//**************ОТЛАДКА****************
			
			MotherBoard_SaveData(RS485_RxBuf()->Str.Data);
			Siren_SaveData(RS485_RxBuf()->Str.Data);
			
			//*************************************
			PowerDevice()->MainPower = dataFromMB->StatusPOWER & PowerMask;//Сохранение состояния питания и ЗУ.
			PowerDevice()->Bat			 = dataFromMB->StatusPOWER & BatMask;
			
			if(PowerDevice()->MainPower == PowerControlOff) PowerDevice()->CheckFromMB.bit.MainPower = Off;
			else                                            PowerDevice()->CheckFromMB.bit.MainPower = On;
			
			if(PowerDevice()->Bat == BatControlOff) PowerDevice()->CheckFromMB.bit.Bat = Off;
			else                                    PowerDevice()->CheckFromMB.bit.Bat = On;			
			
			PowerDevice()->Amp = dataFromMB->StatusPA;//Сохранение состояния УМЗЧ.
			TimeFromMBSave(dataFromMB->TimeUTC);			//Cохранение времени ЦП.
			//---------------------
			//Вывод на дисплей адреса и группы
			Menu(Page1)->Item1.Var = dataFromMB->Addres;
      Menu(Page1)->Item2.Var = dataFromMB->Group;
			//Вывод на дисплей состояния питания блока.
			//MenuPage(Page4)->PageItems.StringText[String1] = TextGetPowerState(PowerGet());
			//MenuPage(Page4)->PageItems.StringText[String2] = TextGetBatState(BatGet()); 
			Menu(Page4)->Item3.Var  = dataFromMB->DebugData1;//Uакб
			Menu(Page4)->Item4.Text = Text_GetAmpState(PowerDevice()->Amp);
			//---------------------
			//сохранение параметров журнала событий. 
 		  Log()->TotalEvents   = dataFromMB->TotalEvents;
			Log()->NotReadEvents = dataFromMB->NotReadEvents;
			//Вывод на дисплей информации о журнале событий.
			Menu(Page6)->Item1.Var = dataFromMB->TotalEvents;
			Menu(Page6)->Item2.Var = dataFromMB->NotReadEvents;
		}
	//--------------------------------------------	
	//Пришла команда "Получения журнала событий".
	if(RS485_RxBuf()->Str.CmdCode == FP_CMD_GET_LOG)
		{
			//По приходу записи о собитии ее нужно сохранить в FP.
			LogSaveEvent(RS485_RxBuf()->Str.Data);
			//После сохранения нужно вывести информацию о событии на экран в читаемом виде.
			Menu(Page6)->Item3.Var = Log()->Event.Number;
			Menu(Page6)->Item4.Var = Log()->Event.ID;
			Menu(Page6)->Item5.Var = Log()->Event.Param;
		}
	//--------------------------------------------	
	//В режиме настройки выбрана страница "Шлейфы сигнализации". 
	//Начинаю запрошивать контекст логических входов.	
	if(Menu_IndexReg()->Page == Page2) 
		{
			//Пришла команда получения параметров одного входа пожарного шлейфа.
			if(RS485_RxBuf()->Str.CmdCode == FP_CMD_GET_INPUT_CONFIG)
				{
					//По приходу параметров входа их нужно сохранить в FP.
					FireLine_Save(RS485_RxBuf()->Str.Data);
				}
			//--------------
			//Запросим параметры для всех 4-х входов (с 0 по 3).	
			else if(fireLineCount <= 3)
				{
					//Отправление команды получени параметров одного входа пожарного шлейфа в ЦП.
					//в MicState номер входа для которого запрашиваем конфигурацию.
					*RS485_Cmd()       = FP_CMD_GET_INPUT_CONFIG;
					txFireLine->Number = fireLineCount;

					fireLineCount++;
					//--------------
				}
			//--------------
			//Команда применения параметров одного входа пожарного шлейфа.
			//в KeyAndMicState номер входа для которого применяется конфигурацию.
			else if(*RS485_Cmd() == FP_CMD_SET_INPUT_CONFIG)
				{
					txFireLine->Number  = FireLine(Menu_IndexReg()->Item - 1)->Number;
					txFireLine->Type	  = FireLine(Menu_IndexReg()->Item - 1)->Type;
					txFireLine->TimeOut = FireLine(Menu_IndexReg()->Item - 1)->TimeOut;
					//Запросим параметры для всех 4-х входов (с 0 по 3).
					fireLineCount = 0;
					//Для выхода из редактирования 
					Button_ClrToggle(RESETbut);
					//--------------
				}
			//--------------
		}
	else
		{
			//При каждом переходе в режим НАСТРОЙКА и на страницу "Шлейфы сигнализации" 
			//запрос параметров для всех 4-х входов (с 0 по 3).
			fireLineCount = 0;
		}
	//--------------------------------------------	
	RS485_TxDataToMB();//Передача ответа в ЦП.
}
//********************************************************************************
//Ошибка обмена с ЦП.
void Task_ErrorConnectMB(void){

  LedSetAll(LedAllOff);
  LedCommunication(YellowColor & Blink(INTERVAL_250_mS));//нет обмена - мигающий желтый.
  LedControl(GENERAL_FAULT_LED, LedOn);
  LedUpdate(); 					//Обновление светодиодов.
  Print_ErrConnect();    //Вывод надписи "Нет связи с ЦП!!!".
	//MotherBoard()->MB_State = MB_WORK_STATE;//Окончание тестирования индикации.
}
//********************************************************************************
void Task_PollingRS485(void){
	
	//Анализ данных от МВ.
	if(RS485_Flags()->MB_NewData) Scheduler_SetTask(Task_RS485ParsingCmdFromMB);
//	//Отсутствие обмена с ЦП.
//	else if(RS485_Flags()->TimeOutError) Scheduler_SetTask(Task_ErrorConnectMB);      
  //--------------------
  Scheduler_SetTask(Task_PollingRS485);
}
//********************************************************************************
//Управление блоком в соответсвии с положение ключа.
void Task_KeyControl(void){

  static uint8_t OldKeyState = 0;
  //--------------------
  //Это нужно для отключения всех светодиодов при выходе из режима Настройки.
  if((OldKeyState   == KEY_CONFIG_STATE) &&
 		 (KeyGetState() != KEY_CONFIG_STATE))
    {
      LedSetAll(LedAllOff);
    }
	OldKeyState = KeyGetState();
  //--------------------
	if(OldKeyState == KEY_LOCK_STATE)    Scheduler_SetTask(Task_LockModeUnit);   //Режим "Блокировка управления".
	if(OldKeyState == KEY_CONTROL_STATE) Scheduler_SetTask(Task_ControlModeUnit);//Режим "Управление".
	if(OldKeyState == KEY_CONFIG_STATE)	 Scheduler_SetTask(Task_CofigModeUnit);  //Режим "Настройка".	
	//--------------------		
	Task_LedUpdate(); //Обновление светодиодов.	
}
//********************************************************************************
//Определение управления.
void Task_GetControl(void){
	
	//Отсутствие обмена с ЦП.
	     if(RS485_Flags()->TimeOutError)       Scheduler_SetTask(Task_ErrorConnectMB);   
  else if(Button_CheckLoop() || GpioCheck()) Scheduler_SetTask(Task_KeyControl);	
  //--------------------
  Scheduler_SetTimerTask(Task_GetControl, 10);
}
//********************************************************************************
//Начало работы программы.
void Task_Begin(void){

	//Вывод на дисплей логотопа.
  Menu_Logo();
  Delay_mSec(2000);
	//--------------------
	while(!GpioCheck()){};// Delay_mSec(10);//Определение состояния ключа.
	Menu_Init();     			//Инициализация пунктов меню.
	RS485_Init();         //Инициализация обмена RS485.	
	TimeInit();						//Инициализация календаря.
  //--------------------
	Scheduler_SetTask(Task_LcdUpdate);	 //Обновление изображения на дисплее. 
	Scheduler_SetTask(Task_PriorityGet); //Получение приоритета управления блоком.
	Scheduler_SetTask(Task_FaultsGet);   //Определение неисправностей.
  Scheduler_SetTask(Task_GetControl);  //Определение управления.
	Scheduler_SetTask(Task_PollingRS485);//
}
//**************************************************************************************************************
//**************************************************************************************************************
//Режим "БЛОКИРОВКА УПРАВЛЕНИЯ"
void Task_LockModeUnit(void){

	FPData_t *txBuf = (FPData_t*)RS485_TxBuf()->Str.Data;
	//--------------------
  //Работа с кнопкой "ТЕСТ".
  if(MotherBoard()->MB_State == MB_TEST_STATE)
    {
			Display_Test();//Вывод на дисплей надписи "РЕЖИМ ТЕСТИРОВАНИЯ."
      return;
    }
  if(Button_IsPress(TESTbut)) 
    {
			*RS485_Cmd() = FP_CMD_TEST_ZUMMER;			
    }
  //--------------------
  CounterToTimeAndDate(TimeFromMBGet());//Отображение времени.
  Menu_IndexRegCleare();                 //Сброс указателей меню.
  Button_ClrToggle(PUSKbut);            //Сброс состояний кнопок меню.
	Button_ClrToggle(RESETbut);
	//-------------------------------------------------------	
	LcdClear(); //Очистка дисплея
	Display_Header(RusText_LockModeUnit);//Вывод шапки окна.
	//Отображение событий системы: 	 
			 if(MicState() == MIC_ACTIVE)                   Display_Mic();	
	else if(SpLine_GetOutState(Zone1) == ActionPuskOn)  Display_PuskButtonActivation();   
	else if(FireLine_CompareAllLinesWith(FR_IN_FIRE))   Display_Fire();   //ПУСК.	
	else if(Faults()->Instant & FAULTS_MASK)            Display_Faults(Faults()->Instant); //НЕИСПРАВНОСТЬ
	else if(SpLine_CompareAllLinesWith(ActionManualOn)) Display_Manual(); //ручное управление		
	else                                                Display_AddressGroupAndMicState();
	//-------------------------------------------------------	
  //Управление светодиодами.
  LedPresetControl(Alg1Led, Line1, Zone1, INTERVAL_250_mS);//Cветодиод ПРЕСЕТ1.
  LedPresetControl(Alg2Led, Line2, Zone3, INTERVAL_250_mS);//Cветодиод ПРЕСЕТ2.
  //Гашение светодиодов СТАРТ и ПОЖАР.
  if((SpLine_GetOutState(Zone1) <= ActionManualOn) && (SpLine_GetOutState(Zone2) <= ActionManualOn))
    {
      LedPusk(LedOff);
      LedPoj(LedOff);
    }
	//-------------------------------------------------------	
  //Передача состояний кнопки ОТКЛ.ЗВУК и ТЕСТ в ЦП.
	txBuf->Control_Buttons = (Button_GetControl() & (TEST_BUTTON | ZUMMER_BUTTON));
	//-------------------------------------------------------	
}
//**************************************************************************************************************
//**************************************************************************************************************
//Режим "УПРАЛЕНИЕ".
void Task_ControlModeUnit(void){

	FPData_t *txBuf = (FPData_t*)RS485_TxBuf()->Str.Data;
  //--------------------
  //Работа с кнопкой "ТЕСТ".
  if(MotherBoard()->MB_State == MB_TEST_STATE)
    {
			Display_Test();//Вывод на дисплей надписи "РЕЖИМ ТЕСТИРОВАНИЯ."
      return;
    }
	//активация режима ТЕСТ индикации 	
	if(Button_IsPress(TESTbut)) *RS485_Cmd() = FP_CMD_TEST_ZUMMER;
  //--------------------
	CounterToTimeAndDate(TimeFromMBGet());//Отображение времени.
  Menu_IndexRegCleare();                //Сброс указателей меню.
  Button_ClrToggle(PUSKbut);            //Сброс состояний кнопок меню.
	Button_ClrToggle(RESETbut);
	//-------------------------------------------------------	
	LcdClear(); //Очистка дисплея
	Display_Header(RusText_ControlModeUnit);//Вывод шапки окна.
	//Отображение событий системы: 
			 if(MicState() == MIC_ACTIVE)                   Display_Mic();	
	else if(SpLine_GetOutState(Zone1) == ActionPuskOn)  Display_PuskButtonActivation();   
	else if(FireLine_CompareAllLinesWith(FR_IN_FIRE))   Display_Fire();   //ПУСК.	
	else if(Faults()->Instant & FAULTS_MASK)            Display_Faults(Faults()->Instant); //НЕИСПРАВНОСТЬ
	else if(SpLine_CompareAllLinesWith(ActionManualOn)) Display_Manual(); //ручное управление		
	else                                                Display_AddressGroupAndMicState();
	//-------------------------------------------------------	
	//Управление светодиодами.
	LedPresetControl(Alg1Led, Line1, Zone1, INTERVAL_250_mS);//Cветодиод ПРЕСЕТ1.
  LedPresetControl(Alg2Led, Line2, Zone3, INTERVAL_250_mS);//Cветодиод ПРЕСЕТ2.
  //Гашение светодиодов СТАРТ и ПОЖАР.
  if((SpLine_GetOutState(Zone1) <= ActionManualOn) && (SpLine_GetOutState(Zone2) <= ActionManualOn))
    {
      LedPusk(LedOff);
      LedPoj(LedOff);
    }		
	//-------------------------------------------------------		
  //Передача состояний кнопок в ЦП.
//  txBuf->Control_Buttons = Button_GetControl();
//  txBuf->Alg_Buttons     = Button_GetAlg();
	//-------------------------------------------------------	
}
//**************************************************************************************************************
//**************************************************************************************************************
//Режим "НАСТРОЙКА"
void Task_CofigModeUnit(void){

	static uint16_t Redaction     = Off;
  static uint16_t IndexTime     = 3;
  static uint16_t IndexCalendar = 3;	
  static uint16_t IndexAddress  = 1;
	static uint16_t FireLineIndex = 2;
	//static uint16_t autoExitTimeoutCount = 0;
	
	FPData_t *txBuf = (FPData_t*)RS485_TxBuf()->Str.Data;
  //--------------------
  //По нажатию кнопки СБРОС перемещение по пунктам меню
	if(Button_IsToggle(RESETbut))
    {	//Определение редактируемой страницы.
			switch(Menu_IndexReg()->Page){
				//******************************************************************************
				//******************************************************************************
				case(Page1)://1-я страница меню - адрес, группа, время, дата.
					//Блокировка перемещения по пунктам страницы в случае редактирования пункта
					if(Redaction == Off) 
						{
							Button_UpDown(&Menu_IndexReg()->Item, String1, String4);
						}
					//Редактирование адреса.
					if(Menu_IndexReg()->Item == String1) 
						{
							Button_LeftRigth(&IndexAddress, 0, 1);
							if(IndexAddress < 1)
								{
									Redaction = On;            
									Button_UpDown(&FacePanel_WorkReg()->Address, 0, 64);//Адрес от 0 до 64.
									Menu_AddressUpdate(IndexAddress, FacePanel_WorkReg()->Address, Blink(INTERVAL_250_mS));//мигающий разряд адреса.
									//Cохранение адреса по нажатию кнопки ПУСК.  
									if(Button_IsPress(PUSKbut))
										{
											Button_ClrToggle(RESETbut);
											Redaction    = Off;
											IndexAddress = 1;
											
											*RS485_Cmd() = FP_CMD_SET_ADDRES;//передача команды "Установка адреса"
										}
									return; 
								}
							 else
								{
									Redaction    = Off;
									IndexAddress = 1;
								}                     
						 }
					//************************************************
					//Редактирование группы.
					if(Menu_IndexReg()->Item == String2) //на второй строке.
						{
							Button_LeftRigth(&IndexAddress, 0, 1);
							if(IndexAddress < 1)
								{
									Redaction = On;            
									Button_UpDown(&FacePanel_WorkReg()->Group, 0, 64);
									Menu_GroupUpdate(IndexAddress, FacePanel_WorkReg()->Group, Blink(INTERVAL_250_mS));
									//Cохранение группы по нажатию кнопки ПУСК.  
									if(Button_IsPress(PUSKbut))
										{
											Button_ClrToggle(RESETbut);
											Redaction    = Off;
											IndexAddress = 1;
											
											*RS485_Cmd() = FP_CMD_SET_GROUP;//передача команды "Установка группы"
										}
									return; 
								}
							 else
								{
									Redaction    = Off;
									IndexAddress = 1;
								}                     
						 }
					//************************************************
					//Редактирование времени.
					if(Menu_IndexReg()->Item == String3) //на третьей строке.
						{
							Button_LeftRigth(&IndexTime, 0, 3);
							if(IndexTime < 3)
								{
									Redaction = On;
									//Часы
									if(IndexTime == 0)
										{
											Button_UpDown(&DateAhdTimeGet()->hour, 0, 23);
											Menu_TimeUpdate(IndexTime, DateAhdTimeGet()->hour, Blink(INTERVAL_250_mS), DateAhdTimeGet());
										}
									//минуты
									if(IndexTime == 1)
										{
											Button_UpDown(&DateAhdTimeGet()->min, 0, 59);
											Menu_TimeUpdate(IndexTime, DateAhdTimeGet()->min, Blink(INTERVAL_250_mS), DateAhdTimeGet());
										}
									//секунды
									if(IndexTime == 2)
										{
											Button_UpDown(&DateAhdTimeGet()->sec, 0, 59);
											Menu_TimeUpdate(IndexTime, DateAhdTimeGet()->sec, Blink(INTERVAL_250_mS), DateAhdTimeGet());
										}             
									if(Button_IsPress(PUSKbut))
										{
											Button_ClrToggle(RESETbut);
											Redaction   = Off;
											IndexTime   = 3;
											//передача команды "Установка времени"
											txBuf->TimeUTC = TimeAndDateToCounter(DateAhdTimeGet());
											*RS485_Cmd() = FP_CMD_SET_TIME;	
										} 
									return;   
								}
							else
								{
									Redaction = Off;
									IndexTime = 3;
								}  
						}
					//************************************************
					//Редактирование даты.
					if(Menu_IndexReg()->Item == String4) //на четвертой строке.
						{	
							Button_LeftRigth(&IndexCalendar, 0, 3);
							if(IndexCalendar < 3)
								{
									Redaction = On;
									//День
									if(IndexCalendar == 0)
										{
											Button_UpDown(&DateAhdTimeGet()->day, 1, 31);
											Menu_CalendarUpdate(IndexCalendar, DateAhdTimeGet()->day, Blink(INTERVAL_250_mS), DateAhdTimeGet());
										}
									//Месяц
									if(IndexCalendar == 1)
										{
											Button_UpDown(&DateAhdTimeGet()->month, 1, 12);
											Menu_CalendarUpdate(IndexCalendar, DateAhdTimeGet()->month, Blink(INTERVAL_250_mS), DateAhdTimeGet());
										}
									//Год.
									if(IndexCalendar == 2)
										{
											Button_UpDown(&DateAhdTimeGet()->year, 1970, 2050);
											Menu_CalendarUpdate(IndexCalendar, DateAhdTimeGet()->year, Blink(INTERVAL_250_mS), DateAhdTimeGet());
										}             
									if(Button_IsPress(PUSKbut))
										{
											Button_ClrToggle(RESETbut);
											Redaction      = Off;
											IndexCalendar  = 3;
											//передача команды "Установка даты"
											txBuf->TimeUTC = TimeAndDateToCounter(DateAhdTimeGet());
											*RS485_Cmd() = FP_CMD_SET_DATA;	
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
				//******************************************************************************
				//****************************************************************************** 
				//2-я страница меню - Состояние пожарных шлейфов.
				case(Page2):
					//Блокировка перемещения по пунктам страницы в случае редактирования пункта.
					if(Redaction == Off) 
						{
							Button_UpDown(&Menu_IndexReg()->Item, String2, String5);//Редактируются параметры с 2-й строки по 5-ю строку.	
						}
					//Вход в редактируемый параметр по нажатию кнопки ЗВУК ОКТЛ.
					Button_LeftRigth(&FireLineIndex, 0, 2);//Выбор редактируемого разряда(парметра)
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
							FireLine(Menu_IndexReg()->Item - 1)->Number = (Menu_IndexReg()->Item - 1);
							//Режим работы входа. Всего 3 режима (0 - 2).
							if(FireLineIndex == 1)
								{
									Button_UpDown(&FireLine(Menu_IndexReg()->Item - 1)->Type, 0, 2);
									Menu_InputConfigUpdate(Menu_IndexReg()->Item, 
																				 FireLineIndex, 
																				 FireLine(Menu_IndexReg()->Item - 1));
								}
							//Таймаут перехода из режима ВНИМАНИЕ в режим ПУСК. От 0 до 99 сек.
							if(FireLineIndex == 0)
								{
									Button_UpDown(&FireLine(Menu_IndexReg()->Item - 1)->TimeOut, 0, 99);
									Menu_InputConfigUpdate(Menu_IndexReg()->Item, 
																				 FireLineIndex, 
																				 FireLine(Menu_IndexReg()->Item - 1));
								}  
							//Cохранение параметров по нажатию кнопки ПУСК.    
							if(Button_IsPress(PUSKbut))
								{
									Redaction = Off;//Указатель "<=" остается у редактируемой строки после применения параметра
									//Команда получени параметров одного входа пожарного шлейфа.										
									*RS485_Cmd() = FP_CMD_SET_INPUT_CONFIG;
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
				//******************************************************************************
				//******************************************************************************
				//3-я страница меню - Состояние линий Гр.
				case(Page3):
					Button_UpDown(&Menu_IndexReg()->Item, String1, String5);//Перемещение по пунктам страницы.
					Print_SpLineState(SpLine_Param()->Control);      			  //Вывод на дисплей состояния линий Гр.
					Menu(Page3)->Item5.Var = SpLine_Param()->Deviation;     //
					SpLine_Param()->ConfigFlag = FLAG_SET;
					//Постановка линий Гр. на контроль.
					if(Menu_IndexReg()->Item <= String4)
						{
							//Установка линии Гр. на контроль.
							if(Button_IsPress(ZUMMERbut))
								{
									SpLine_Param()->Control |= (1 << (uint8_t)Menu_IndexReg()->Item);
								}	
								//Снятие линии Гр. с контроля.
							if(Button_IsPress(TESTbut))
								{
									SpLine_Param()->Control &= ~(1 << (uint8_t)Menu_IndexReg()->Item);
								}
						}
					//Редактирование отклонения линии Гр.
					if(Menu_IndexReg()->Item == String5)
						{
							Button_LeftRigth(&SpLine_Param()->Deviation, 1, 500);
						}
					//Cохранение параметров по нажатию кнопки ПУСК.          
					if(Button_IsPress(PUSKbut))
						{ 
							Button_ClrToggle(RESETbut);        //Для выхода из редактирования 
							SpLine_Param()->LineRef = Menu_IndexReg()->Item;//Линия для которой сохраняется опорное значение.
							*RS485_Cmd() = FP_CMD_LC_SAVE_REF;//отправим команду в ЦП.
						}
				break;
				//******************************************************************************
				//******************************************************************************
				//4-я страница меню - питание блока.
				case(Page4):	
					Button_UpDown(&Menu_IndexReg()->Item, String1, String2);//Перемещение по пунктам страницы.
					Print_PowerState();  									 
					//постановка на контроль основного питания.
					if(Menu_IndexReg()->Item == String1)
						{
							if(Button_IsPress(ZUMMERbut)) PowerDevice()->Check.bit.MainPower = On;
							if(Button_IsPress(TESTbut))   PowerDevice()->Check.bit.MainPower = Off;
						}
					//постановка на контроль резервного питания.
					if(Menu_IndexReg()->Item == String2)
						{
							if(Button_IsPress(ZUMMERbut)) PowerDevice()->Check.bit.Bat = On;
							if(Button_IsPress(TESTbut))   PowerDevice()->Check.bit.Bat = Off;
						}	
					//Cохранение параметров по нажатию кнопки ПУСК.          
					if(Button_IsPress(PUSKbut))
						{
							Button_ClrToggle(RESETbut);
							
							txBuf->MicState = PowerDevice()->Check.Byte; 
							*RS485_Cmd() = FP_CMD_SET_POWER_CHECK;	
						}							
				break;
				//******************************************************************************
				//******************************************************************************
				//5-я страница меню - состяние платы управления оповещателями.
				case(Page5):
					Button_ClrToggle(RESETbut);	
				break;
				//******************************************************************************
				//****************************************************************************** 
				//6-я страница меню - просмотр журнала событий
				case(Page6):				
					Button_UpDown((uint16_t*)&Log()->EventIndex, 1, Log()->TotalEvents); //Перемещение по записям в журнале событий.
					if(Button_IsPress(ZUMMERbut)) Log()->EventIndex = 1;                  			 //Получение первой записи по нажатию ЗВУК.ОТКЛ.
					if(Button_IsPress(TESTbut))   Log()->EventIndex = Log()->TotalEvents;//Получение последней записи по нажатию ТЕСТ.
					if(Button_IsPress(PUSKbut))   Log()->EventIndex = 0xFFFF; 									 //Все непросмотренные события сделать просмотренными при нажатии ПУСК.					
					//Отправка запроса только по одному нажатию кнопки. 
					if(Log()->EventOldIndex != Log()->EventIndex)
						{
							Log()->EventOldIndex = Log()->EventIndex;
							//Все непросмотренные события сделать просмотренными при нажатии ПУСК. 
							if(Log()->EventIndex == 0xFFFF)
								{
									Log()->EventIndex = Log()->TotalEvents;
								}
							//Отправка запроса в ЦП.
							*RS485_Cmd() = FP_CMD_GET_LOG;
						}
				break;
				//******************************************************************************
				//******************************************************************************
				//7-я страница меню.
				case(Page7):
					if(Button_IsPress(RESETbut))
						{
							*RS485_Cmd() = FP_CMD_GET_EEPROM_INFO;
						}					
					Button_ClrToggle(RESETbut);	
				break;
				//******************************************************************************
				//******************************************************************************
				default:
				break;
				//******************************************************************************
				//******************************************************************************
      }
      //--------------------   
			//Отображение времени.
			CounterToTimeAndDate(TimeFromMBGet());	
			//Обновление состояния страницы меню. 
			Menu_Update(Menu(Menu_IndexReg()->Page),// 
								  1, 													//режим "Редактирование параметра" 
								  Menu_IndexReg()->Item);			//
    }
  //--------------------
  //Перемещение по страницам меню.
  else
    { 
      Redaction			= Off;//Разрешение перемещения по пунктам меню.
      IndexTime			= 3;
			IndexCalendar	= 3;	
      IndexAddress	= 1;
			FireLineIndex	= 2;
			
      Menu_IndexReg()->Item = 0;
			Print_PowerState();   							 									//Вывод на дисплей состояния питания.
			Log()->EventIndex = Log()->TotalEvents;								//Отображение журнала с последнего записаного события
			FacePanel_WorkReg()->Address = Menu(Page1)->Item1.Var;//адрес от ЦП
      FacePanel_WorkReg()->Group   = Menu(Page1)->Item2.Var;//группа от ЦП.
			CounterToTimeAndDate(TimeFromMBGet());                //Отображение времени.
			//--------------------
			Print_SpLineState(SpLine_Param()->Control);					  //Вывод на дисплей состояния линий Гр от ЦП.
      Menu(Page3)->Item5.Var = SpLine_Param()->Deviation;   //отклонения от установившегося значения для фиксации перех-го проц-са на линии.        
			//Если зашли а потом срузу-же вышли без изменения установленных на контроль линий
			// то сброс флага
			if(*RS485_Cmd() != FP_CMD_LC_SAVE_REF) SpLine_Param()->ConfigFlag = FLAG_CLEAR;
			//--------------------
			if(Button_IsPress(TESTbut))   Menu_IndexReg()->Page = PAGE_NUM-1;//Переход на последнюю страницу при нажатии кнапки ТЕСТ.
			if(Button_IsPress(ZUMMERbut)) Menu_IndexReg()->Page = Page1;			//Переход на первую страницу при нажатии кнапки ЗВУК ОТКЛ..
			Button_UpDown(&Menu_IndexReg()->Page, 0, PAGE_NUM-1); 					  //Перемещение по страницам меню.
      //--------------------
			Menu_Update(Menu(Menu_IndexReg()->Page), 0, 0);       
    }
  //--------------------
}
//**************************************************************************************************************
//**************************************************************************************************************
int main(void){
    
  //--------------------
  //После сброса микроконтроллера программа переходит в main(), минуя SystemInit().
  //В фейле startup_stm32f10x_md_vl.s закоментирован переход на SystemInit.
  SysInit();     //Настройка тактирования микроконтроллера 72МГц. Внешний кварц 8 МГц.
  GpioInit();    //Инициализация портов.
  Spi2Init();    // 
	//--------------------
	//Инициализация прикладного уровня.	
	LcdInit();		 //Дисплей.
	//--------------------	
	//Инициализация службы таймеров и службы сообщений для работы с конечными автоматами.
	SwTimersInit();		//программные таймера.
	FsmMessagesInit();//служба сообщений.
  //-------------------- 
	Scheduler_Init();							//Инициализация диспетчера
	Scheduler_SetTask(Task_Begin);//Начало!!!
	
  SysTickInit(); 						  	//Запуск службы таймеров.
  __enable_irq();							  
  //--------------------
  while(1)
    {
			Scheduler();//диспетчер.
    }
  //--------------------
}
//**************************************************************************************************************
//**************************************************************************************************************
//Прерывание системного таймера SysTick. Выполняется за 19 мкС.
void SysTick_Handler(void){
	
	uint8_t blink = Blink(INTERVAL_250_mS);
	//--------------------
	Scheduler_TimerService();//Служба таймеров ОС.
	SwTimersLoop();		       //Программные таймеры.
	//--------------------
	Blink_Loop();			 //
	RS485_TimeOutInc();//Отсчет таймаута приема пакета.
	//--------------------	
	//Отработка режима "ТЕСТ".
	if(MotherBoard()->MB_State == MB_TEST_STATE)
		{ 
			//Мигаем всеми индикаторами.
			LedSetAll(blink);
			LcdBackLight(blink);
			LedControl(TEST_LED, LedOn); 
		}
	else LcdBackLight(On);
	//--------------------
}
//**************************************************************************************************************
//**************************************************************************************************************


