
#include "main.h"

//*************************************************************************************************
//*************************************************************************************************
//Получение управления блоком.
uint8_t PriorityGet(void){
	
  //--------------------
  //Приоритет 1(высший) - Активация тангентой микрофона лицевой панели.
  if(FacePanel()->Mic == MIC_ACTIVE)
		{
			ButtonForMic_Update();
			return MIC_ACTIVATION;
		}
  //--------------------
	//Управление кнокой "ПУСК".	
	if(Button_IsToggle(PUSKbut)) 
		{
			return PUSK_BUTTON_ACTIVATION;
		}
  //--------------------
  //Приоритет 2 - Активация встроенного тревожного сообщения.
  //Активация приоритета осуществляется входным шлейфом «Пож.1» или «Пож.2».
  if((FireLine(POJ_1)->LogicalState == FIRE_LINES_FIRE) ||
		 (FireLine(POJ_2)->LogicalState == FIRE_LINES_FIRE))	
		{
			return FIRE_LINE_ACTIVATION;
		}
  //--------------------
  //Приоритет 3 - ГО и ЧС. 
  //Активация приоритета осуществляется входным шлейфом «ЧС».
  if(FireLine(ChS)->LogicalState == FIRE_LINES_FIRE)
    {
			return ChS_ACTIVATION;
    } 
  //--------------------
	//Управление оповещением кнопками СТАРТ и СТОП
	
	//--------------------		
  //Ручное управление.
	ButtonForManual_Update();
	return MANUAL_CONTROL;
  //--------------------
}
//*************************************************************************************************
//*************************************************************************************************
//Выполнение основных процессов в блоке. Максимальное время выполнения функции 25мкС.
void Task_Executors(void){
	
	uint32_t faultsInst = Faults()->Instant;
	//--------------------
	Config_SaveLoop();//Запись конфигурации после ее изменения.
	Faults_Loop();    //Определение неисправностей блока.
	Log_Loop();       //Логирование неисправностей блока.
	//--------------------
	//Управление зуммером.
  if(MotherBoard_WorkReg()->State == MB_WORK_STATE && 
		 FacePanel()->Key != KEY_CONFIG_STATE) 						
		{
			Zummer_Fault(faultsInst);
		}
	//В режиме настройки зуммер не работает.
	else
		{
			Zummer_Fault(0);
		}
	//--------------------
  //Управление реле "НЕИСПРАВНОСТЬ ОБЩАЯ".
	if(faultsInst != 0) Relay_On (RELAY_FAULT_GENERAL);//Активация реле "Н.О.".
  else                Relay_Off(RELAY_FAULT_GENERAL);//Отключение реле "Н.О.". 
  //Управление реле "НЕИСПРАВНОСТЬ ПИТАНИЯ". 
	if(faultsInst & FAULT_MASK_FOR_RELAY_FAULT_POWER) Relay_On(RELAY_FAULT_POWER); //Активация реле "Н.П.". 
  else 																							Relay_Off(RELAY_FAULT_POWER);//Отключение реле "Н.П.". 
	//--------------------
	if(FacePanel()->Key == KEY_CONFIG_STATE) Relay_Off(RELAY_ALL); 
	SpeakerLine_FSMLoop();		//Работа с линиями Гр.
	FireLine_FSMLoop();	  		//Работа с входами ШС.
	Relay_UpdateLoop();	  		//Обновление состояний реле.
	VoiceMessage_UpdateLoop();//			
	//--------------------
	Scheduler_SetTimerTask(Task_Executors, 10); 
}
//*******************************************************************
//Обработка команд от FP.
void Task_ParsingCmdFP(void){
	
	FPData_t   *dataFromFacePanel     = (FPData_t*)RS485_RxBuf()->Str.Data;
	FireLine_t *configDataForFireLine = (FireLine_t*)RS485_RxBuf()->Str.Data;
	uint8_t	temp;
	//--------------------
	RS485_Flags()->FPNewData = FLAG_CLEAR;
  //Обработка команд от FP:
  switch(RS485_RxBuf()->Str.CmdCode){
		//--------------------
		//Команда "Выдача статуса от FP".
		case(FP_CMD_SEND_STATUS):
			FacePanelSaveData(RS485_RxBuf()->Str.Data);//Сохранение даных от FP.
		break;
		//--------------------
		//Команда установки адрес блока
		case(FP_CMD_SET_ADDRES):
			Zummer_Beep(3, 50);                        				 					//Пикнем по приему команды.
			MotherBoard_WorkReg()->Address = dataFromFacePanel->Address;//Установка адреса.
			Config_Save()->Address = dataFromFacePanel->Address;        //Сохранение во флеш микроконтроллера.
		break;
		//--------------------
		//Команда установки группы блока 
		case(FP_CMD_SET_GROUP):
			Zummer_Beep(3, 50);                                     //Пикнем по приему команды.
			MotherBoard_WorkReg()->Group = dataFromFacePanel->Group;//Установка группы.
			Config_Save()->Group = dataFromFacePanel->Group;        //Сохранение во флеш микроконтроллера.
		break;
		//--------------------
		//Команда установки времени 
		case(FP_CMD_SET_TIME):
			Zummer_Beep(3, 50);            //Пикнем по приему команды.
			TimeUTC_Set(dataFromFacePanel->TimeUTC);//Установка времени.
		break;
		//--------------------
		//Команда установки даты 
		case(FP_CMD_SET_DATA):
			Zummer_Beep(3, 50);            //Пикнем по приему команды.
			TimeUTC_Set(dataFromFacePanel->TimeUTC);//Установка времени.
		break;
		//--------------------
		//Команда влючение тестирования зуммера ЦП
		case(FP_CMD_TEST_ZUMMER):
			MotherBoard_WorkReg()->State = MB_TEST_STATE;
		break;
		//--------------------
		//Команда сохранения опорных значений для Lc.
		//В KeyAndMicState будут линии установленные на контроль.
		//В Address будет номер канала для которого сохряняются опроные значения. 
		//В Group будет занчение чуствительности (отклонения от установившегося значения для фиксации перех-го проц-са на линии). 
		case(FP_CMD_LC_SAVE_REF):
			Zummer_Beep(1, 100);//Пикнем по приему команды.
			//Сохранение линий установленных на контроль.
			SpeakerLine_Param()->Check = dataFromFacePanel->Mic;
			Config_Save()->SpCheck     = dataFromFacePanel->Mic;
			//Сохранение импеданса линии (опорных величин для определния состояния линии).
			if(SpeakerLine_SaveRef(dataFromFacePanel->Address) != 0) 
				{
					Zummer_Beep(2, 100);//Пикнем по приему команды.
				}
			//Сохранение чувствительности.
		  if(dataFromFacePanel->TimeUTC >= SP_LINE_DEVIATION_MIN &&
				 dataFromFacePanel->TimeUTC <= SP_LINE_DEVIATION_MAX)
				{
					Zummer_Beep(3, 100);//Пикнем по приему команды.
					SpeakerLine_Param()->Deviation  = (uint16_t)dataFromFacePanel->TimeUTC;
					Config_Save()->SpDeviation = SpeakerLine_Param()->Deviation;
				}
			else SpeakerLine_Param()->Deviation = SP_LINE_DEVIATION_DEFAULT;
		break;
		//--------------------
		//Команда получения журнала событий. 
		case(FP_CMD_GET_LOG):
			//Очистка непросмотренных сообщения.
			if((uint16_t)dataFromFacePanel->TimeUTC == RESET_UNREAD_EVENTS) Log_ResetCountUnreadEvents();
			//Передача события из журнала на лицевую панель.
			else RS485_FP_BuildAndTxEventPack((uint16_t)dataFromFacePanel->TimeUTC);		
		break;
		//--------------------
		//Команда получения параметров одного логического входа.
		//в Address номер входа для которого запрашиваем конфигурацию.
		case(FP_CMD_GET_INPUT_CONFIG):
			RS485_FP_BuildAndTxFireLineContextPack(configDataForFireLine->Number);
		break;		
		//--------------------
		//Команда сохранения параметров одного входа пожарного шлейфа.
		case(FP_CMD_SET_INPUT_CONFIG):
			Zummer_Beep(3, 50);
		
			temp = configDataForFireLine->Number;
		
			FireLine(temp)->LogicalState = FIRE_LINES_CONTROL_OFF;
			FireLine(temp)->Type         = configDataForFireLine->Type;
			FireLine(temp)->TimeOut      = configDataForFireLine->TimeOut;
		
			//Проверка.
			//if(FireLine(temp)->Type > FIRE_LINES_TYPE_ON_CONTROL_ON_OPEN) 
			//	{
			//		FireLine(temp)->Type = FIRE_LINES_TYPE_ON_CONTROL_ON_OPEN;
			//	}
			//if(FireLine(temp)->TimeOut > FIRE_LINES_TIMEOUT_MAX)
			//	{
			//		FireLine(temp)->Type = FIRE_LINES_TIMEOUT_MAX;
			//	}
		
			//Сохранение во флеш микроконтроллера.
			Config_Save()->FireLineConfig[temp].Type    = FireLine(temp)->Type;	
			Config_Save()->FireLineConfig[temp].Timeout = FireLine(temp)->TimeOut;
		break;	
		//--------------------
		//Команда сохранения контроля питания блока.
		case(FP_CMD_SET_POWER_CHECK):
			Zummer_Beep(3, 50);
			//Power()->Check.Byte = RS485RxBuf()->Data.FP.KeyAndMicState;
			//Сохранение во флеш микроконтроллера.
			//ConfigSave()->PowerCheckOn = RS485RxBufPtr()->BLK[FP_KeyAndMicStatePoint]; 
		break;			
		//--------------------
		//Команда получения информации о EEPROM.
//		case(FP_CMD_GET_EEPROM_INFO):
//			//ZummerBeep(3, 100);
//			RS485_FP_BuildAndTxEepInfoPack();
//		return;
		//break;			
		//--------------------
		//Неверный код команды.
		default:
//			Zummer_Beep(3, 200); 
		break;			
		//--------------------
  }
  //--------------------
}
//*******************************************************************
//Обработка команд от SB.
void Task_ParsingCmdSB(void){
	
	RS485_Flags()->SBNewData = FLAG_CLEAR;
	//Сохранение данных от SB.
	SirenBoard_SaveData(RS485_RxBuf()->Str.Data);
}
//*******************************************************************
//Работа с протоколом внутреблочного взаимодействия.
void Task_PollingRS485(void){

	//Анализ данных протокола.
  if(RS485_Flags()->FPNewData) Scheduler_SetTask(Task_ParsingCmdFP); 
	if(RS485_Flags()->SBNewData) Scheduler_SetTask(Task_ParsingCmdSB);
	//Индикация обмена.
	if(RS485_Flags()->FPNAck)
		{
			LedRs485Act(Blink(INTERVAL_1000_mS));//Отсутствует обмен по протоколу.
			FacePanel()->Key = NO_KEY;//
			//ButtonFP_Clr();           //Сброс состояния кнопок.
		}
	else LedRs485Act(Blink(INTERVAL_50_mS));//Обмен по протоколу в норме.		
	//--------------------			
	Scheduler_SetTask(Task_PollingRS485);
}
//*************************************************************************************************
//*************************************************************************************************
//Режим "Блокировка управления"
void Task_LockModeUnit(void){

	//--------------------
  //Режим ТЕСТ.
  if(MotherBoard_WorkReg()->State == MB_TEST_STATE) return;
  //--------------------
	//Определение приоритета.	
	switch(PriorityGet()){
		//--------------------
		//Приоритет 1(высший) - Активация тангентой микрофона лицевой панели.
		case(MIC_ACTIVATION):
			//Активация зон. 
		  SpeakerLine(SP_LINE_1)->IsActivated = ActionMicOn;//   При нажатии тангенты активируется первая зона.
			//Дополнительные зоны можно довключить кнопками "АЛГОРИТМЫ ОПОВЕЩЕНИЯ".
			if(Button_IsToggle(ALG2but)) 
				{
					SpeakerLine(SP_LINE_2)->IsActivated = ActionMicOn;
				}
      else SpeakerLine(SP_LINE_2)->IsActivated = ActionOff;
			
			SirenBoard_Set(SIREN_ALL, SIREN_OUT_OFF);    //Отключение оповещателей.
			Relay_Off(RELAY_FIRE | RELAY_GO_ChS);//Сброс выходных реле.
			Relay_On (RELAY_PUSK);               //Активация выходного реле "ПУСК". 	
				
			AnalogSwitch_Prior_Set(MIC);//Мик.Пр. на вход УМ
			PAmp_Control(PAMP_ON);      //Активация УМЗЧ.	
		break;
		//--------------------
		//Управление кнокой "ПУСК".
		case(PUSK_BUTTON_ACTIVATION):
			//Активация всех зон кнопкой ПУСК.
			SpeakerLine(SP_LINE_1)->IsActivated = ActionPuskOn;
			SpeakerLine(SP_LINE_2)->IsActivated = ActionPuskOn;
		
			Relay_Off(RELAY_FIRE | RELAY_GO_ChS);//Сброс выходных реле.
			Relay_On (RELAY_PUSK);               //Активация выходного реле "ПУСК". 	
		
			VoiceMessage_Cmd(PLAY_MESSAGE);       //Запуск сообщения.
			AnalogSwitch_Prior_Set(VOICE_MESSAGE);//JQ на вход УМ 
			PAmp_Control(PAMP_ON);          		  //Активация УМЗЧ.
		break;		
		//--------------------
		//Приоритет 2 - Активация встроенного тревожного сообщения.
		//Активация приоритета осуществляется входным шлейфом «Пож.1» или «Пож.2».
		case(FIRE_LINE_ACTIVATION):
		  //Активация зоны 1.
			if(FireLine(POJ_1)->LogicalState == FIRE_LINES_FIRE)
				{
					SpeakerLine(SP_LINE_1)->IsActivated = ActionFireLine1On;
					SirenBoard_Set(SIREN_1, SIREN_OUT_ON);    //Активация оповещателя 1.
				}
			if(FireLine(POJ_1)->LogicalState != FIRE_LINES_FIRE)
				{
					SpeakerLine(SP_LINE_1)->IsActivated = ActionOff;
					SirenBoard_Set(SIREN_1, SIREN_OUT_OFF);    //Отключение оповещателя 1.
				}				
      //Активация зоны 2.	
      if(FireLine(POJ_2)->LogicalState == FIRE_LINES_FIRE)
				{
					SpeakerLine(SP_LINE_2)->IsActivated = ActionFireLine2On;
					SirenBoard_Set(SIREN_2, SIREN_OUT_ON);      //Активация оповещателя 2.
				}
      if(FireLine(POJ_2)->LogicalState != FIRE_LINES_FIRE)
				{
					SpeakerLine(SP_LINE_2)->IsActivated = ActionOff;
					SirenBoard_Set(SIREN_2, SIREN_OUT_OFF);    //Отключение оповещателя 2.
				}
				
		  Relay_Off(RELAY_GO_ChS);           //Сброс выходных реле. 
			Relay_On (RELAY_FIRE | RELAY_PUSK);//Активация выходного реле "ПОЖАР" и "ПУСК".
				
      VoiceMessage_Cmd(PLAY_MESSAGE);       //Запуск сообщения.
      AnalogSwitch_Prior_Set(VOICE_MESSAGE);//JQ на вход УМ  
      PAmp_Control(PAMP_ON);                //Активация УМЗЧ.
		break;
		//--------------------
		case(ChS_ACTIVATION):
			
		break;
		//--------------------
		case(PRIORITY_4):
			
		break;
		//--------------------
		case(PRIORITY_5):
			
		break;
		//--------------------
		//Управление оповещением кнопками СТАРТ и СТОП
		case(START_BUTTON_ACTIVATION):
			
		break;
		//--------------------
		default:
			//Сброс зон.
			SpeakerLine(SP_LINE_1)->IsActivated = ActionOff;	
			SpeakerLine(SP_LINE_2)->IsActivated = ActionOff;				
			//Отключение оповещателей		
			SirenBoard_Set(SIREN_ALL, SIREN_OUT_OFF); 
			//Сброс выходных реле.
      Relay_Off(RELAY_PUSK | RELAY_FIRE | RELAY_GO_ChS);
		
			PAmp_Control(PAMP_OFF);        //Отключение УМЗЧ.
			AnalogSwitch_Prior_Set(GND);	 //вход УМ на землю
			VoiceMessage_Cmd(STOP_MESSAGE);//Останов сообщения.
		break;	
		//--------------------
		}
	//-----------------------------------------------------	
	//Упарвление реле ПОЖАР3(ТРЕВОГА).
	if(FireLine(POJ_3)->LogicalState == FIRE_LINES_FIRE) Relay_On (RELAY_FIRE3); 
	else  																							 Relay_Off(RELAY_FIRE3);		
}
//*************************************************************************************************
//*************************************************************************************************
//Режим "Управление"
void Task_ControlModeUnit(void){

  //--------------------
  //Режим ТЕСТ.
  if(MotherBoard_WorkReg()->State == MB_TEST_STATE) return;
  //--------------------
	//Определение приоритета.	
	switch(PriorityGet()){
		//--------------------
		//Приоритет 1(высший) - Активация тангентой микрофона лицевой панели.
		case(MIC_ACTIVATION):
			//Активация зон. 
		  SpeakerLine(SP_LINE_1)->IsActivated = ActionMicOn;//При нажатии тангенты активируется первая зона.
		
			//Дополнительные зоны можно довключить кнопками "АЛГОРИТМЫ ОПОВЕЩЕНИЯ".
			if(ButtonForMic()->isToggle & ALG2but) SpeakerLine(SP_LINE_2)->IsActivated = ActionMicOn;
      else 											             SpeakerLine(SP_LINE_2)->IsActivated = ActionOff;
			
			SirenBoard_Set(SIREN_ALL, SIREN_OUT_OFF);//Отключение оповещателей.
			Relay_Off(RELAY_FIRE | RELAY_GO_ChS);    //Сброс выходных реле.
			Relay_On (RELAY_PUSK);                   //Активация выходного реле "ПУСК". 	
				
			AnalogSwitch_Prior_Set(MIC);//Мик.Пр. на вход УМ
			PAmp_Control(PAMP_ON);      //Активация УМЗЧ.	
		break;
		//--------------------
		//Управление кнокой "ПУСК".
		case(PUSK_BUTTON_ACTIVATION):
			//Активация всех зон кнопкой ПУСК.
			SpeakerLine(SP_LINE_1)->IsActivated = ActionPuskOn;
			SpeakerLine(SP_LINE_2)->IsActivated = ActionPuskOn;
		
			Relay_Off(RELAY_FIRE | RELAY_GO_ChS);//Сброс выходных реле.
			Relay_On (RELAY_PUSK);               //Активация выходного реле "ПУСК". 	
		
			VoiceMessage_Cmd(PLAY_MESSAGE);       //Запуск сообщения.
			AnalogSwitch_Prior_Set(VOICE_MESSAGE);//JQ на вход УМ 
			PAmp_Control(PAMP_ON);          		  //Активация УМЗЧ.
		break;		
		//--------------------
		//Приоритет 2 - Активация встроенного тревожного сообщения.
		//Активация приоритета осуществляется входным шлейфом «Пож.1» или «Пож.2».
		case(FIRE_LINE_ACTIVATION):
		  //Активация зоны 1.
			if(FireLine(POJ_1)->LogicalState == FIRE_LINES_FIRE)
				{
					SpeakerLine(SP_LINE_1)->IsActivated = ActionFireLine1On;
					SirenBoard_Set(SIREN_1, SIREN_OUT_ON);    //Активация оповещателя 1.
				}
			if(FireLine(POJ_1)->LogicalState != FIRE_LINES_FIRE)
				{
					SpeakerLine(SP_LINE_1)->IsActivated = ActionOff;
					SirenBoard_Set(SIREN_1, SIREN_OUT_OFF);    //Отключение оповещателя 1.
				}				
      //Активация зоны 2.	
      if(FireLine(POJ_2)->LogicalState == FIRE_LINES_FIRE)
				{
					SpeakerLine(SP_LINE_2)->IsActivated = ActionFireLine2On;
					SirenBoard_Set(SIREN_2, SIREN_OUT_Blink1);      //Активация оповещателя 2.
				}
      if(FireLine(POJ_2)->LogicalState != FIRE_LINES_FIRE)
				{
					SpeakerLine(SP_LINE_2)->IsActivated = ActionOff;
					SirenBoard_Set(SIREN_2, SIREN_OUT_OFF);    //Отключение оповещателя 2.
				}
				
		  Relay_Off(RELAY_GO_ChS);           //Сброс выходных реле. 
			Relay_On (RELAY_FIRE | RELAY_PUSK);//Активация выходного реле "ПОЖАР" и "ПУСК".
				
      VoiceMessage_Cmd(PLAY_MESSAGE);       //Запуск сообщения.
      AnalogSwitch_Prior_Set(VOICE_MESSAGE);//JQ на вход УМ  
      PAmp_Control(PAMP_ON);                //Активация УМЗЧ.
		break;
		//--------------------
		case(ChS_ACTIVATION):
			
		break;
		//--------------------
		case(PRIORITY_4):
			
		break;
		//--------------------
		case(PRIORITY_5):
			
		break;
		//--------------------
		//Управление оповещением кнопками СТАРТ и СТОП
		case(START_BUTTON_ACTIVATION):
			
		break;
		//--------------------
		//Ручное управление.
		case(MANUAL_CONTROL):
			SirenBoard_Set(SIREN_ALL, SIREN_OUT_OFF);         //Отключение оповещателей.
			Relay_Off(RELAY_PUSK | RELAY_FIRE | RELAY_GO_ChS);//Сброс выходных реле.
			VoiceMessage_Cmd(STOP_MESSAGE);                   //Останов сообщения.
			AnalogSwitch_Prior_Set(LIN_IN_2);									//Лин.Вх.2 на вход УМ
			PAmp_Control(PAMP_ON);														//Активация УМЗЧ.         
			//AnalogSwitch_Prior_Set(MULTIMEDIA);//Отладка для Вити
			//Активация зон.
			if(ButtonForManual()->isToggle & ALG1but) SpeakerLine(SP_LINE_1)->IsActivated = ActionManualOn;
			else                         							SpeakerLine(SP_LINE_1)->IsActivated = ActionOff;
				
			if(ButtonForManual()->isToggle & ALG2but) SpeakerLine(SP_LINE_2)->IsActivated = ActionManualOn;
			else 												 							SpeakerLine(SP_LINE_2)->IsActivated = ActionOff;		
			//Работа с кнопкой "СБРОС". При ее нажатии отключаются все зоны, активированные в ручную.
			if(ButtonForManual()->isPress & RESETbut) 
				{
					ButtonForManual()->isToggle = 0;
					SpeakerLine(SP_LINE_1)->IsActivated = ActionOff;	
					SpeakerLine(SP_LINE_2)->IsActivated = ActionOff;	
				}		
		break;	
		//--------------------
		default:
//			//Сброс зон.
//			SpeakerLine(SP_LINE_1)->IsActivated = ActionOff;	
//			SpeakerLine(SP_LINE_2)->IsActivated = ActionOff;				
//			//Отключение оповещателей		
//			SirenBoard_Set(SIREN_ALL, SIREN_OUT_OFF); 
//			//Сброс выходных реле.
//      Relay_Off(RELAY_PUSK | RELAY_FIRE | RELAY_GO_ChS);
//		
//			PAmp_Control(PAMP_OFF);        //Отключение УМЗЧ.
//			AnalogSwitch_Prior_Set(GND);	 //вход УМ на землю
//			VoiceMessage_Cmd(STOP_MESSAGE);//Останов сообщения.			
		break;		
		//--------------------
		}
	//-----------------------------------------------------	
	//Упарвление реле ПОЖАР3(ТРЕВОГА).
	if(FireLine(POJ_3)->LogicalState == FIRE_LINES_FIRE) 
		{
			Relay_On (RELAY_FIRE3); 
			SirenBoard_Set(SIREN_3, SIREN_OUT_Blink1);    
		}
	else 
		{
			SirenBoard_Set(SIREN_3, SIREN_OUT_OFF);  
			Relay_Off(RELAY_FIRE3);			
		}
}
//*************************************************************************************************
//*************************************************************************************************
//Режим "Настройка"
void Task_ConfigModeUnit(void){

  //Сброс состояний всех кнопок.
  //Button_ClrToggleState(CONTROL_BUTTONS, ALL_BUTTON);
  //Button_ClrToggleState(ALG_BUTTONS, ALL_BUTTON);
  //ButTempUnitControl = 0;
  //---------------------
	SpeakerLine(SP_LINE_1)->IsActivated = ActionOff;//Отключение всех зон.
	SpeakerLine(SP_LINE_2)->IsActivated = ActionOff;//
  SirenBoard_Set(SIREN_ALL, SIREN_OUT_OFF);  			//Отключение оповещателей.
	Relay_Off(RELAY_ALL);	             							//Сброс выходных реле.
  PAmp_Control(PAMP_OFF);                    			//Отключение УМЗЧ.
	AnalogSwitch_Prior_Set(GND);	             			//вход УМ на землю
  VoiceMessage_Cmd(STOP_MESSAGE);            			//Останов сообщения.
	Charger_Deactivate();	                          //Отключение ЗУ.
}
//*************************************************************************************************
//*************************************************************************************************
//Управление сухими контактами. Это в случае когда нет связи с лицевой панелью.
void Task_PojLineControlModeUnit(void){

  //Приоритет 2 - Активация встроенного тревожного сообщения.
  //Активация приоритета осуществляется входным шлейфом «Пож.1» или «Пож.2».
  if((FireLine(POJ_1)->FaultState == FIRE_LINES_NORM_CLOSE) ||
     (FireLine(POJ_2)->FaultState == FIRE_LINES_NORM_CLOSE))
    {
      //Активация зоны 1.
      if(FireLine(POJ_1)->FaultState == FIRE_LINES_NORM_CLOSE)
				{
					SpeakerLine(SP_LINE_1)->IsActivated = ActionFireLine1On;
					SirenBoard_Set(SIREN_1, SIREN_OUT_ON);    //Активация оповещателя 1.
				}
      if(FireLine(POJ_1)->FaultState == FIRE_LINES_NORM_OPEN)
				{
					SpeakerLine(SP_LINE_1)->IsActivated = ActionOff;
					SirenBoard_Set(SIREN_1, SIREN_OUT_OFF);    //Отключение оповещателя 1.
				}				
      //Активация зоны 2.	
      if(FireLine(POJ_2)->FaultState == FIRE_LINES_NORM_CLOSE)
				{
					SpeakerLine(SP_LINE_2)->IsActivated = ActionFireLine1On;
					SirenBoard_Set(SIREN_2, SIREN_OUT_ON);    //Активация оповещателя 2.
				}
      if(FireLine(POJ_2)->FaultState == FIRE_LINES_NORM_OPEN)
				{
					SpeakerLine(SP_LINE_2)->IsActivated = ActionOff;
					SirenBoard_Set(SIREN_2, SIREN_OUT_OFF);    //Отключение оповещателя 2.
				}			
			
			Relay_Off(RELAY_GO_ChS);           //Сброс выходных реле.
			Relay_On (RELAY_PUSK | RELAY_FIRE);//Активация выходного реле "ПОЖАР" и "ПУСК".	
				
			VoiceMessage_Cmd(PLAY_MESSAGE);       //Запуск сообщения.	
      AnalogSwitch_Prior_Set(VOICE_MESSAGE);//JQ на вход УМ  
      PAmp_Control(PAMP_ON);                //Активация УМЗЧ.
    }
  //--------------------
  //Приоритет 3 - ГО и ЧС. 
  //Активация приоритета осуществляется входным шлейфом «ЧС».
  else if(FireLine(ChS)->FaultState == FIRE_LINES_NORM_CLOSE)
    {
			SpeakerLine(SP_LINE_1)->IsActivated = ActionFireLineChSOn;//Активация всех зон. 
			SpeakerLine(SP_LINE_2)->IsActivated = ActionFireLineChSOn;
			
		  Relay_Off(RELAY_PUSK |RELAY_FIRE);//Сброс выходных реле.
			Relay_On (RELAY_GO_ChS);          //Активация выходного реле "ГОиЧС".			
			
			VoiceMessage_Cmd(STOP_MESSAGE);  //Остнаов сообщения.    
			AnalogSwitch_Prior_Set(LIN_IN_1);//Лин.Вх.1 на вход УМ
			PAmp_Control(PAMP_ON);           //Активация УМЗЧ.
    } 
  //--------------------
	else 
    {
			SpeakerLine(SP_LINE_1)->IsActivated = ActionOff;//Сброс зон.
			SpeakerLine(SP_LINE_2)->IsActivated = ActionOff;
			
			SirenBoard_Set(SIREN_ALL, SIREN_OUT_OFF); 				//Отключение оповещателей.
			Relay_Off(RELAY_PUSK | RELAY_FIRE | RELAY_GO_ChS);//Сброс выходных реле.
			
			VoiceMessage_Cmd(STOP_MESSAGE);//Остнаов сообщения.    
			AnalogSwitch_Prior_Set(GND);	 //вход УМ на землю
			PAmp_Control(PAMP_OFF);        //Отключение УМЗЧ.
    }
  //--------------------
	//Упарвление реле ПОЖАР3(ТРЕВОГА).
	if(FireLine(POJ_3)->FaultState == FIRE_LINES_NORM_CLOSE) Relay_On (RELAY_FIRE3); 
	if(FireLine(POJ_3)->FaultState == FIRE_LINES_NORM_OPEN)  Relay_Off(RELAY_FIRE3);
  //--------------------		
}
//*************************************************************************************************
//*************************************************************************************************
void Task_GetKeyState(void){

  //--------------------
	Button_Update();
	//Определение управления по положению ключа.	
	if(FacePanel()->Key == KEY_LOCK_STATE)    Scheduler_SetTask(Task_ControlModeUnit); //Task_LockModeUnit);   //Режим "Блокировка управления"
	if(FacePanel()->Key == KEY_CONTROL_STATE) Scheduler_SetTask(Task_ControlModeUnit);//Режим "Управление"
	if(FacePanel()->Key == KEY_CONFIG_STATE)  Scheduler_SetTask(Task_ConfigModeUnit); //Режим "Настройка"
	if(FacePanel()->Key == NO_KEY) 				    Scheduler_SetTask(Task_PojLineControlModeUnit);//Управление сухими контактами.
  //--------------------
	Scheduler_SetTimerTask(Task_GetKeyState, 10);
}
//*************************************************************************************************
//*************************************************************************************************
int main(void){
    
	uint8_t   count;
	Config_t* pConfig = Config();
  //--------------------
  //После сброса микроконтроллера программа преходит в main(), минуя SystemInit().
  //В фейле startup_stm32f10x_md_vl.s закоментирован переход на SystemInit.
  Sys_Init(); //Настройка тактирования микроконтроллера. Внешний кварц 8 МГц. 
  Gpio_Init();//Инициализация портов.
  Adc_Init(); //
  //-------------------- 
  //Инициализация прикладного уровня.
	Time_Init();
	FireLine_Init();
  SpeakerLine_Init();
  PAmp_Init();
	AnalogSwitch_Init();
  PowerAndCharge_Init();
  VoiceMessage_Init();
  RS485_Init();
	Config_Init(); 		
	Zummer_Init();
	Log_Init();
	Relay_Init();
	SirenBoard_Init();
	//-------------------- 
	Charger_Activate();		  //Активация ЗУ.
  AnalogSwitch_Activate();//Включение выходов мультиплексора.
	
	Relay_On(RELAY_SP1_ATT_LIN | RELAY_SP2_ATT_LIN);//Переключение выхода УМ с линии Аттенюации на линию Оповещения.
  //--------------------
	//Применение конфигурации.
	MotherBoard_WorkReg()->Address = pConfig->Address;
  MotherBoard_WorkReg()->Group   = pConfig->Group;
	SpeakerLine_Param()->Deviation = pConfig->SpDeviation;
	SpeakerLine_Param()->Check     = pConfig->SpCheck;
	Power()->Check.Byte 					 = pConfig->PowerCheckOn;
	
	for(count = 0; count < FIRE_LINES_NUMBER; count++)
		{
			FireLine(count)->Type    = pConfig->FireLineConfig[count].Type;
			FireLine(count)->TimeOut = pConfig->FireLineConfig[count].Timeout;	
		}
	//--------------------
	Log_SaveEvent(DeviceOn, NoParam);	
	//-------------------- 
	//Инициализация службы таймеров и службы сообщений для работы с конечными автоматами.
	SwTimers_Init();	 
	FsmMessages_Init();
	//--------------------
  Scheduler_Init(); 	   //Инициализация диспетчера
	
	Scheduler_SetTask(Task_Executors);       
	Scheduler_SetTask(Task_GetKeyState);
	Scheduler_SetTask(Task_PollingRS485);
		
  SysTick_Init();       //Запуск службы таймеров.
  __enable_irq();       //Глобальное разрешение рерываний.	
  //--------------------
  while(1)
    {  
      Scheduler();//Диспетчер.
    }
  //--------------------
}
//*************************************************************************************************
//*************************************************************************************************
//Прерываени системного таймера SysTick.
void SysTick_Handler(void){
 
	static uint16_t mScountForZumTEST = 0;
	//--------------------  
	Scheduler_TimerService();//Служба таймеров диспетчера.
	SwTimers_Loop();         //Виртуальные таймера для машины состояний.
  //--------------------
	RS485_MB_MasterLoop();//Протокол RS485.
	Blink_Loop();				  //Мигающая индикация.
  Gpio_CheckLoop();     //Антидребезг для всех выводов контроллера.
  PowerAndCharge_Loop();//Получение состояния основного ввода питания 220В и напряжения на АКБ. 
  //-------------------- 
  FireLine_MeasLoop();   //Измерение линий пож. шлейфов.
  SpeakerLine_MeasLoop();//Измерение линий Гр.
  //-------------------- 
  //Отработка режима "ТЕСТ".
  if(MotherBoard_WorkReg()->State == MB_TEST_STATE)
    { 			
			Zummer_Control(Blink(INTERVAL_250_mS) & ZUMM_ON);
      //Окончание тестирования.
      if((++mScountForZumTEST >= TEST_TIME)||//Через 10 секунд или
         (RS485_Flags()->FPNAck))            //при отсутствии обмена с FP.
        {
          mScountForZumTEST = 0;
					Zummer_Control(ZUMM_OFF);
					MotherBoard_WorkReg()->State = MB_WORK_STATE;
        }   
    }
  else 
    {
      mScountForZumTEST = 0;
      Zummer_Loop();
    }
	//--------------------
}
//************************************************************************************************** 
//************************************************************************************************** 





