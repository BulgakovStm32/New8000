
#include "main.h"

//*************************************************************************************************
//*************************************************************************************************

//*************************************************************************************************
//*************************************************************************************************

//*************************************************************************************************
//*************************************************************************************************
//Получение приоритета управления блоком.
uint8_t PriorityGet(void){
	
  //--------------------
  //Приоритет 1(высший) - Активация тангентой микрофона лицевой панели.
  if(FacePanel()->Mic == MIC_ACTIVE) return PRIORITY_1;
  //--------------------
	//Управление кнокой "ПУСК".	
	if(Button_IsToggle(PUSKbut)) return PUSK_Activation;
  //--------------------
  //Приоритет 2 - Активация встроенного тревожного сообщения.
  //Активация приоритета осуществляется входным шлейфом «Пож.1» или «Пож.2».
  if((FireLine(POJ_1)->LogicalState == FIRE_LINES_FIRE) ||
		 (FireLine(POJ_2)->LogicalState == FIRE_LINES_FIRE))	
		{
			return PRIORITY_2;
		}
  //--------------------
  //Приоритет 3 - ГО и ЧС. 
  //Активация приоритета осуществляется входным шлейфом «ЧС».
  if(FireLine(ChS)->FaultState == FIRE_LINES_NORM_CLOSE)
    {
			return PRIORITY_3;
    } 
  //--------------------
	//Управление оповещением кнопками СТАРТ и СТОП
//	if(ButGetToggleState(CONTROL_BUTTONS) & START_BUTTON)	
//		{
//			return START_Activation;
//		}		
	//--------------------	
  //Приоритет 6 - Ручное управление.
//  if(ButGetToggleState(ALG_BUTTONS) & (ALG1_BUTTON | ALG2_BUTTON))
//    {
//			return PRIORITY_6;
//    }
  //--------------------
	return 0;
}
//*************************************************************************************************
//*************************************************************************************************
void Task_Executors(void);

void LoggingLoop(void);

void Task_InternalProtocol(void);
void Task_ParsingCmdSB(void);
void Task_ParsingCmdFP(void);
//*************************************************************************************************
//*************************************************************************************************
//Максимальное время выполнения 25мкС.
void Task_Executors(void){
	
	Config_SaveLoop();	  		//Запись конфигурации после ее изменения.
	SpeakerLine_FSMLoop();		//Работа с линиями Гр.
	FireLine_FSMLoop();	  		//Работа с входами ШС.
  Relay_UpdateLoop();	  		//Обновление состояний реле.
	VoiceMessage_UpdateLoop();//
						
	FaultsLoop();				  //Определение неисправностей блока.
	LoggingLoop();        //Логирование неисправностей блока.
	//--------------------	
	Scheduler_SetTimerTask(Task_Executors, 10); 
}
//*************************************************************************************************
//Логирование событий блока.
void LoggingLoop(void){
	
	uint32_t faultsRise	= Faults()->Rise;
	uint32_t faultsFall	= Faults()->Fall;	
	//--------------------
	//Основное питание.
	if(faultsRise & ACFaultFlag)  Log_SaveEvent(LogACFault, NoParam);
	if(faultsFall & ACFaultFlag)  Log_SaveEvent(LogACNorm,  NoParam);
	//Инвертор.
	if(faultsRise & DCFaultFlag)  Log_SaveEvent(LogDCFault, NoParam);
	if(faultsFall & DCFaultFlag)  Log_SaveEvent(LogDCNorm,  NoParam);
	//АКБ.
	if(faultsRise & BatFaultFlag) Log_SaveEvent(LogBatFault, NoParam);
	if(faultsFall & BatFaultFlag) Log_SaveEvent(LogBatNorm,  NoParam);
	//шлейфы пожарные.
	if(faultsRise & FireLine1FaultFlag) Log_SaveEvent(LogFireLine1Fault, NoParam);
	if(faultsFall & FireLine1FaultFlag) Log_SaveEvent(LogFireLine1Norm,  NoParam);
	
	if(faultsRise & FireLine2FaultFlag) Log_SaveEvent(LogFireLine2Fault, NoParam);
	if(faultsFall & FireLine2FaultFlag) Log_SaveEvent(LogFireLine2Norm,  NoParam);
	
	if(faultsRise & FireLine3FaultFlag) Log_SaveEvent(LogFireLine3Fault, NoParam);
	if(faultsFall & FireLine3FaultFlag) Log_SaveEvent(LogFireLine3Norm,  NoParam);
	
	if(faultsRise & ChSLineFaultFlag)   Log_SaveEvent(LogChSLineFault, NoParam);	
	if(faultsFall & ChSLineFaultFlag)   Log_SaveEvent(LogChSLineNorm,  NoParam);		
	//линии связи с Гр.
	if(faultsRise & SpLine1FaultFlag) Log_SaveEvent(LogSpLine1Fault, NoParam);
	if(faultsFall & SpLine1FaultFlag) Log_SaveEvent(LogSpLine1Norm,  NoParam);	
	
	if(faultsRise & SpAtten1FaultFlag) Log_SaveEvent(LogSpAtten1Fault, NoParam);
	if(faultsFall & SpAtten1FaultFlag) Log_SaveEvent(LogSpAtten1Norm,  NoParam);	
	
	if(faultsRise & SpLine2FaultFlag) Log_SaveEvent(LogSpLine2Fault, NoParam);
	if(faultsFall & SpLine2FaultFlag) Log_SaveEvent(LogSpLine2Norm,  NoParam);	
	
	if(faultsRise & SpAtten2FaultFlag) Log_SaveEvent(LogSpAtten2Fault, NoParam);
	if(faultsFall & SpAtten2FaultFlag) Log_SaveEvent(LogSpAtten2Norm,  NoParam);	
	//Линии связи с табличками ВЫХОД.
	if(faultsRise & Siren1FaultFlag) Log_SaveEvent(LogSiren1Fault, NoParam);
	if(faultsFall & Siren1FaultFlag) Log_SaveEvent(LogSiren1Norm,  NoParam);	
	
	if(faultsRise & Siren2FaultFlag) Log_SaveEvent(LogSiren2Fault, NoParam);
	if(faultsFall & Siren2FaultFlag) Log_SaveEvent(LogSiren2Norm,  NoParam);	
	
	if(faultsRise & Siren3FaultFlag) Log_SaveEvent(LogSiren3Fault, NoParam);
	if(faultsFall & Siren3FaultFlag) Log_SaveEvent(LogSiren3Norm,  NoParam);	
	//
	if(faultsRise & ConnectFaultFlag) Log_SaveEvent(LogConnectFault, NoParam);
	if(faultsFall & ConnectFaultFlag) Log_SaveEvent(LogConnectNorm,  NoParam);	
}
//*************************************************************************************************
//Работа с протоколом внутреблочного взаимодействия.
void Task_InternalProtocol(void){

	//Анализ данных протокола.
  if(RS485_Flags()->FPNewData) Scheduler_SetTask(Task_ParsingCmdFP); 
	if(RS485_Flags()->SBNewData) Scheduler_SetTask(Task_ParsingCmdSB);
	//Индикация обмена.
	if(RS485_Flags()->FPNAck)
		{
			LedRs485Act(Blink(INTERVAL_1000_mS));//Отсутствует обмен по протоколу.
			FacePanel()->Key = NO_KEY;
		}
	else LedRs485Act(Blink(INTERVAL_50_mS));	 //Обмен по протоколу в норме.		
	//--------------------			
	Scheduler_SetTimerTask(Task_InternalProtocol, 1);
}
//*************************************************************************************************
//Обработка команд от FP.
void Task_ParsingCmdFP(void){
	
	FPData_t   *dataFromFacePanel = (FPData_t*)RS485_RxBuf()->Str.Data;
	FireLine_t *rxFireLineData    = (FireLine_t*)RS485_RxBuf()->Str.Data;
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
			RS485_FP_BuildAndTxFireLineContextPack(rxFireLineData->Number);
		break;		
		//--------------------
		//Команда сохранения параметров одного входа пожарного шлейфа.
		case(FP_CMD_SET_INPUT_CONFIG):
			Zummer_Beep(3, 50);
		
			temp = rxFireLineData->Number;
		
			FireLine(temp)->LogicalState = FIRE_LINES_CONTROL_OFF;
			FireLine(temp)->Type         = rxFireLineData->Type;
			FireLine(temp)->TimeOut      = rxFireLineData->TimeOut;
		
			if(FireLine(temp)->Type > FIRE_LINES_TYPE_ON_CONTROL_ON_OPEN) 
				{
					FireLine(temp)->Type = FIRE_LINES_TYPE_ON_CONTROL_ON_OPEN;
				}
			if(FireLine(temp)->TimeOut > FIRE_LINES_TIMEOUT_MAX)
				{
					FireLine(temp)->Type = FIRE_LINES_TIMEOUT_MAX;
				}
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
//*************************************************************************************************
//Обработка команд от SB.
void Task_ParsingCmdSB(void){
	
	RS485_Flags()->SBNewData = FLAG_CLEAR;
	//Сохранение данных от SB.
	SirenBoard_SaveData(RS485_RxBuf()->Str.Data);
}
//*************************************************************************************************
//*************************************************************************************************
//Режим "Блокировка управления"
void Task_LockModeUnit(void){

	//--------------------
  //Режим ТЕСТ.
  if(MotherBoard_WorkReg()->State == MB_TEST_STATE) return;
  //--------------------
//  //Если был активирован ТЕСТ, то при переходе в режим Блокировка режим ТЕСТ отключаем.
//  //MAIN_FlagReg &= ~ZummerTestFlag;
//  //--------------------
//  //Приоритет 1(высший). 
//  //Активация тангентой микрофона лицевой панели.
//  if(FacePanel()->Mic == MicActiveConst)
//    {	
//			SpeakerLine(SpLine1)->Action = ActionMicOn;//Активация всех зон.
//			SpeakerLine(SpLine2)->Action = ActionMicOn;
//			
//			OutputRelays(Off, RelPoj | RelChS);//Сброс выходных реле.
//			OutputRelays(On, RelPusk);         //Активация выходного реле "ПУСК". 
//      
//      SetMuxPrior(MUX_Mic);               //Мик.Пр. на вход УМ
//      AmpControl(AmpOn);                  //Активация УМЗЧ.
//    }
//  //--------------------
//  //Управление кнокой "ПУСК".
//  else if(ButTempUnitControl & START_BUTTON) 
//    {			
//			SpeakerLine(SpLine1)->Action = ActionPuskOn;//Активация всех зон. 
//			SpeakerLine(SpLine2)->Action = ActionPuskOn;
//			
//			OutputRelays(Off, RelPoj | RelChS);//Сброс выходных реле.
//			OutputRelays(On, RelPusk);         //Активация выходного реле "ПУСК". 
//			
//			*VoiceMessageCmd() = PlayMessage;//Запуск сообщения.
//      SetMuxPrior(MUX_JQ);             //JQ на вход УМ  
//      AmpControl(AmpOn);               //Активация УМЗЧ.
//    }
//  //--------------------
//	//Приоритет 2 - Активация встроенного тревожного сообщения.
//	//Активация приоритета осуществляется входным шлейфом «Пож.1» или «Пож.2».
//  else if((FireLine(POJ_1)->LogicalState == FireLineFIRE) || (FireLine(POJ_2)->LogicalState == FireLineFIRE))
//    {
//      //Активация зоны 1.
////      if(FireLine(POJ_1)->LogicalState == FireLineFIRE)
////				{
////					SpeakerLine(SpLine1)->Action = ActionFireLine1On;
////					SirenSet(Siren1, SirenOutON);//Активация оповещателя 1.
////				}
////      else
////				{
////					SpeakerLine(SpLine1)->Action = ActionOff;
////					SirenSet(Siren1, SirenOutOFF);//Отключение оповещателя 1.
////				}				
////      //Активация зоны 2.	
////      if(FireLine(POJ_2)->LogicalState == FireLineFIRE)
////				{
////					SpeakerLine(SpLine2)->Action = ActionFireLine2On;
////					SirenSet(Siren2, SirenOutON);//Активация оповещателя 2.
////				}
////      else
////				{
////					SpeakerLine(SpLine2)->Action = ActionOff;
////					SirenSet(Siren2, SirenOutOFF);//Отключение оповещателя 2.
////				}
////				
////			OutputRelays(Off, RelChS);         //Сброс выходных реле. 
////			OutputRelays(On, RelPoj | RelPusk);//Активация выходного реле "ПОЖАР" и "ПУСК".
////				
////			*VoiceMessageCmd() = PlayMessage;//Запуск сообщения.
////      SetMuxPrior(MUX_JQ);             //JQ на вход УМ  
////      AmpControl(AmpOn);               //Активация УМЗЧ.
//    }
//  //--------------------
//  //Приоритет 3 - ГО и ЧС. 
//  //Активация приоритета осуществляется входным шлейфом «ЧС».
//  else if(FireLine(ChS)->LogicalState == FireLineFIRE)
//    {
////			SpeakerLine(SpLine1)->Action = ActionFireLineChSOn;//Активация всех зон.  
////			SpeakerLine(SpLine2)->Action = ActionFireLineChSOn;
////			
////			OutputRelays(Off, RelPoj);//Сброс выходных реле.
////			OutputRelays(On, RelChS); //Активация выходного реле "ГОиЧС".	
////			
////      SetMuxPrior(MUX_LinIn1);         //Лин.Вх.1 на вход УМ
////      AmpControl(AmpOn);               //Активация УМЗЧ.
////			*VoiceMessageCmd() = StopMessage;//Останов сообщения.
////      //SetMuxPrior(MUX_Multimedia);     //Отладка для Вити
//    } 
//  //--------------------
//  //Приоритет 6 - Ручное управление.
//  else if(ButGetToggleState(ALG_BUTTONS) & (ALG1_BUTTON | ALG2_BUTTON))
//    {
////			OutputRelays(Off, RelPusk | RelPoj | RelChS);//Сброс выходных реле.
////			
////			*VoiceMessageCmd() = StopMessage;//Останов сообщения.
////      SetMuxPrior(MUX_LinIn2);         //Лин.Вх.2 на вход УМ
////      AmpControl(AmpOn);               //Активация УМЗЧ.

////      if(ButGetToggleState(ALG_BUTTONS) & ALG1_BUTTON)SpeakerLine(SpLine1)->Action = ActionManualOn;//ZoneSet(Zone1, ZoneManualOn); 
////      else                                            SpeakerLine(SpLine1)->Action = ActionOff;//ZoneSet(Zone1, ZoneOff);                                         
////      if(ButGetToggleState(ALG_BUTTONS) & ALG2_BUTTON)SpeakerLine(SpLine2)->Action = ActionManualOn;//ZoneSet(Zone2, ZoneManualOn); 
////      else                                            SpeakerLine(SpLine2)->Action = ActionOff;//ZoneSet(Zone2, ZoneOff);                                    
////      //-----------
//    }
//  //--------------------
//  else
//    {
////			SpeakerLine(SpLine1)->Action = ActionOff; //Сброс зон.
////			SpeakerLine(SpLine2)->Action = ActionOff;
////			
////			OutputRelays(Off, RelPusk | RelPoj | RelChS);//Сброс выходных реле.
////			SirenSet(SirenAll, SirenOutOFF); 	           //Отключение оповещателей.
////			
////      AmpControl(AmpOff);              //Отключение УМЗЧ.
////      SetMuxPrior(MUX_AllOff);         //вход УМ на землю
////			*VoiceMessageCmd() = StopMessage;//Сброс модуля JQ.
//    }
//  //--------------------
//  //Управление сообщениями.
//	VoiceMessage(*VoiceMessage_Cmd());	
//  //--------------------
}
//*********************************************************************
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
		case(PRIORITY_1):
			//Активация зон. 
			SpeakerRelays(SP_LINE_1, ActionMicOn);//   При нажатии тангенты активируется первая зона.
		  SpeakerLine(SP_LINE_1)->Action = ActionMicOn;
			//Дополнительные зоны можно довключить кнопками "АЛГОРИТМЫ ОПОВЕЩЕНИЯ".
			if(Button_IsToggle(ALG2but)) 
				{
					SpeakerLine(SP_LINE_2)->Action = ActionMicOn;
				}
      else SpeakerLine(SP_LINE_2)->Action = ActionOff;
			
			SirenBoard_Set(SIREN_ALL, SIREN_OUT_OFF);       //Отключение оповещателей.
			OutputRelays(OFF, RELAY_FIRE | RELAY_GO_ChS);//Сброс выходных реле.
			OutputRelays(ON, RELAY_PUSK);                //Активация выходного реле "ПУСК". 	
				
			AnalogSwitch_Prior_Set(MIC);//Мик.Пр. на вход УМ
			PAmp_Control(PAMP_ON);      //Активация УМЗЧ.	
		break;
		//--------------------
		//Управление кнокой "ПУСК".
		case(PUSK_Activation):
			//Активация всех зон кнопкой ПУСК.
			SpeakerLine(SP_LINE_1)->Action = ActionPuskOn;
			SpeakerLine(SP_LINE_2)->Action = ActionPuskOn;
		
			OutputRelays(OFF, RELAY_FIRE | RELAY_GO_ChS);//Сброс выходных реле.
			OutputRelays(ON, RELAY_PUSK);                //Активация выходного реле "ПУСК". 	
		
			VoiceMessage_Cmd(PLAY_MESSAGE);       //Запуск сообщения.
			AnalogSwitch_Prior_Set(VOICE_MESSAGE);//JQ на вход УМ 
			PAmp_Control(PAMP_ON);          		  //Активация УМЗЧ.
		break;		
		//--------------------
		//Приоритет 2 - Активация встроенного тревожного сообщения.
		//Активация приоритета осуществляется входным шлейфом «Пож.1» или «Пож.2».
		case(PRIORITY_2):
			
//		  //Активация зоны 1.
//			if(FireLine(Poj1)->LogicalState == FireLineFIRE)
//				{
//					//ZoneSet (Zone1, ZoneFireLine1On);
//					SpeakerLine(SpLine1)->Action = ActionFireLine1On;
//					SirenSet(Siren1, SirenOutON);    //Активация оповещателя 1.
//				}
//			if(FireLine(Poj1)->LogicalState != FireLineFIRE)
//				{
//					//ZoneSet (Zone1, ZoneOff);
//					SpeakerLine(SpLine1)->Action = ActionOff;
//					SirenSet(Siren1, SirenOutOFF);    //Отключение оповещателя 1.
//				}	
//			
//      //Активация зоны 2.	
//      if(FireLine(Poj2)->LogicalState == FireLineFIRE)
//				{
//					//ZoneSet (Zone2, ZoneFireLine2On);
//					SpeakerLine(SpLine2)->Action = ActionFireLine2On;
//					SirenSet(Siren2, SirenOutON);    //Активация оповещателя 2.
//				}
//      if(FireLine(Poj2)->LogicalState != FireLineFIRE)
//				{
//					//ZoneSet (Zone2, ZoneOff);
//					SpeakerLine(SpLine2)->Action = ActionOff;
//					SirenSet(Siren2, SirenOutOFF);    //Отключение оповещателя 2.
//				}
//				
//		  OutputRelays(Off, RelChS);          //Сброс выходных реле. 
//			OutputRelays(Off, RelPoj | RelPusk);//Активация выходного реле "ПОЖАР" и "ПУСК".
//				
//      VoiceMessage(PlayMessage);      //Запуск сообщения.
//      SetMuxPrior(MUX_JQ);            //JQ на вход УМ  
//      AmpControl(AmpOn);              //Активация УМЗЧ.
		
		break;
		//--------------------
		case(PRIORITY_3):
			
		break;
		//--------------------
		case(PRIORITY_4):
			
		break;
		//--------------------
		case(PRIORITY_5):
			
		break;
		//--------------------
		//Приоритет 6 - Ручное управление.
		case(PRIORITY_6):	
			
//			SirenSet(SirenAll, SirenOutOFF); 				     //Отключение оповещателей.
//		  OutputRelays(Off, RelPusk | RelPoj | RelChS);//Сброс выходных реле.
//      VoiceMessage(StopMessage);                   //Останов сообщения.
//      SetMuxPrior(MUX_LinIn2);                     //Лин.Вх.2 на вход УМ
//      AmpControl(AmpOn);                           //Активация УМЗЧ.      
//      //SetMuxPrior(MUX_Multimedia);//Отладка для Вити

//      if(ButGetToggleState(ALG_BUTTONS) & ALG1_BUTTON)
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
//      if(ButGetToggleState(ALG_BUTTONS) & ALG2_BUTTON)
//			{
//				//ZoneSet(Zone2, ZoneManualOn);
//				SpeakerLine(SpLine2)->Action = ActionManualOn;
//			}
//      else
//        {
//          //ButClrToggleState(PRESET_BUTTONS, PRES2_BUTTON);
//          //ZoneSet(Zone2, ZoneOff);
//					SpeakerLine(SpLine2)->Action = ActionOff;					
//        }
//				
//			//Работа с кнопкой "СБРОС". При ее нажатии отключаются все зоны, активированные в ручную.
//			if(ButGetInstantState(CONTROL_BUTTONS) & RESET_BUTTON) 
//				{
//					ButClrToggleState(ALG_BUTTONS, ALL_BUTTON);
//					//ZoneSet(ZoneAll, ZoneOff);
//					SpeakerLine(SpLine1)->Action = ActionOff;	
//					SpeakerLine(SpLine2)->Action = ActionOff;	
//				}
		break;
		//--------------------
		//Управление оповещением кнопками СТАРТ и СТОП
		case(START_Activation):
			
//			if(ButGetToggleState(ALG_BUTTONS) & ALG1_BUTTON) SpeakerLine(SpLine1)->Action = ActionAutoOn;//ZoneSet(Zone1, ZoneAutoOn); 
//      else 																						 SpeakerLine(SpLine1)->Action = ActionOff;//ZoneSet(Zone1, ZoneOff); 
//                                                   
//      if(ButGetToggleState(ALG_BUTTONS) & ALG2_BUTTON) SpeakerLine(SpLine2)->Action = ActionAutoOn;//ZoneSet(Zone2, ZoneAutoOn);
//      else																						 SpeakerLine(SpLine2)->Action = ActionOff;//ZoneSet(Zone2, ZoneOff); 
     
		break;
		//--------------------
		default:
			//Сброс зон.
			SpeakerLine(SP_LINE_1)->Action = ActionOff;	
			SpeakerLine(SP_LINE_2)->Action = ActionOff;				
			//Отключение оповещателей		
			SirenBoard_Set(SIREN_ALL, SIREN_OUT_OFF); 
			//Сброс выходных реле.
      OutputRelays(OFF, RELAY_PUSK | RELAY_FIRE | RELAY_GO_ChS);
		
			PAmp_Control(PAMP_OFF);        //Отключение УМЗЧ.
			AnalogSwitch_Prior_Set(GND);	 //вход УМ на землю
			VoiceMessage_Cmd(STOP_MESSAGE);//Останов сообщения.
		break;	
		//--------------------
		}
	//-----------------------------------------------------	
}
//*********************************************************************
//Режим "Настройка"
void Task_ConfigModeUnit(void){

  //Сброс состояний всех кнопок.
  //Button_ClrToggleState(CONTROL_BUTTONS, ALL_BUTTON);
  //Button_ClrToggleState(ALG_BUTTONS, ALL_BUTTON);
  //ButTempUnitControl = 0;
  //---------------------
	SpeakerLine(SP_LINE_1)->Action = ActionOff;//Отключение всех зон.
	SpeakerLine(SP_LINE_2)->Action = ActionOff;//
  SirenBoard_Set(SIREN_ALL, SIREN_OUT_OFF);  //Отключение оповещателей.
	OutputRelays(OFF, RELAY_ALL);	             //Сброс выходных реле.
  PAmp_Control(PAMP_OFF);                    //Отключение УМЗЧ.
	AnalogSwitch_Prior_Set(GND);	             //вход УМ на землю
  VoiceMessage_Cmd(STOP_MESSAGE);            //Останов сообщения.
}
//*********************************************************************
//Управление сухими контактами.
void Task_PojLineControlModeUnit(void){

  //Приоритет 2 - Активация встроенного тревожного сообщения.
  //Активация приоритета осуществляется входным шлейфом «Пож.1» или «Пож.2».
  if((FireLine(POJ_1)->FaultState == FIRE_LINES_NORM_CLOSE) ||
     (FireLine(POJ_2)->FaultState == FIRE_LINES_NORM_CLOSE))
    {
      //Активация зоны 1.
      if(FireLine(POJ_1)->FaultState == FIRE_LINES_NORM_CLOSE)
				{
					SpeakerLine(SP_LINE_1)->Action = ActionFireLine1On;
					SirenBoard_Set(SIREN_1, SIREN_OUT_ON);    //Активация оповещателя 1.
				}
      if(FireLine(POJ_1)->FaultState == FIRE_LINES_NORM_OPEN)
				{
					SpeakerLine(SP_LINE_1)->Action = ActionOff;
					SirenBoard_Set(SIREN_1, SIREN_OUT_OFF);    //Отключение оповещателя 1.
				}				
      //Активация зоны 2.	
      if(FireLine(POJ_2)->FaultState == FIRE_LINES_NORM_CLOSE)
				{
					SpeakerLine(SP_LINE_2)->Action = ActionFireLine1On;
					SirenBoard_Set(SIREN_2, SIREN_OUT_ON);    //Активация оповещателя 2.
				}
      if(FireLine(POJ_2)->FaultState == FIRE_LINES_NORM_OPEN)
				{
					SpeakerLine(SP_LINE_2)->Action = ActionOff;
					SirenBoard_Set(SIREN_2, SIREN_OUT_OFF);    //Отключение оповещателя 2.
				}			
			
			OutputRelays(OFF, RELAY_GO_ChS);          //Сброс выходных реле.
			OutputRelays(ON, RELAY_PUSK | RELAY_FIRE);//Активация выходного реле "ПОЖАР" и "ПУСК".	
				
			VoiceMessage_Cmd(PLAY_MESSAGE);       //Запуск сообщения.	
      AnalogSwitch_Prior_Set(VOICE_MESSAGE);//JQ на вход УМ  
      PAmp_Control(PAMP_ON);                //Активация УМЗЧ.
    }
  //--------------------
  //Приоритет 3 - ГО и ЧС. 
  //Активация приоритета осуществляется входным шлейфом «ЧС».
  else if(FireLine(ChS)->FaultState == FIRE_LINES_NORM_CLOSE)
    {
			SpeakerLine(SP_LINE_1)->Action = ActionFireLineChSOn;//Активация всех зон. 
			SpeakerLine(SP_LINE_2)->Action = ActionFireLineChSOn;
			
		  OutputRelays(OFF, RELAY_PUSK |RELAY_FIRE);//Сброс выходных реле.
			OutputRelays(ON, RELAY_GO_ChS);           //Активация выходного реле "ГОиЧС".			
			
			VoiceMessage_Cmd(STOP_MESSAGE);  //Остнаов сообщения.    
			AnalogSwitch_Prior_Set(LIN_IN_1);//Лин.Вх.1 на вход УМ
			PAmp_Control(PAMP_ON);           //Активация УМЗЧ.
    } 
  //--------------------
	else 
    {
			SpeakerLine(SP_LINE_1)->Action = ActionOff;//Сброс зон.
			SpeakerLine(SP_LINE_2)->Action = ActionOff;
			
			SirenBoard_Set(SIREN_ALL, SIREN_OUT_OFF); 				        //Отключение оповещателей.
			OutputRelays(OFF, RELAY_PUSK | RELAY_FIRE | RELAY_GO_ChS);//Сброс выходных реле.
			
			VoiceMessage_Cmd(STOP_MESSAGE);//Остнаов сообщения.    
			AnalogSwitch_Prior_Set(GND);	 //вход УМ на землю
			PAmp_Control(PAMP_OFF);        //Отключение УМЗЧ.
    }
  //--------------------
	//Упарвление реле ПОЖАР3(ТРЕВОГА).
	if(FireLine(POJ_3)->FaultState == FIRE_LINES_NORM_CLOSE) OutputRelays(ON, RELAY_FIRE3); 
	if(FireLine(POJ_3)->FaultState == FIRE_LINES_NORM_OPEN)  OutputRelays(OFF, RELAY_FIRE3);
  //--------------------		
}
//*********************************************************************
void Task_GetKeyState(void){

  //--------------------
	//Определение управления по положению ключа.	
	if(FacePanel()->Key == KEY_LOCK_STATE)    Scheduler_SetTask(Task_LockModeUnit);   //Режим "Блокировка управления"
	if(FacePanel()->Key == KEY_CONTROL_STATE) Scheduler_SetTask(Task_ControlModeUnit);//Режим "Управление"
	if(FacePanel()->Key == KEY_CONFIG_STATE)  Scheduler_SetTask(Task_ConfigModeUnit); //Режим "Настройка"
	if(FacePanel()->Key == NO_KEY) 				    Scheduler_SetTask(Task_PojLineControlModeUnit);//Управление сухими контактами.
  //--------------------
	Scheduler_SetTimerTask(Task_GetKeyState, 10);
}
//*************************************************************************************************
//*************************************************************************************************


//*************************************************************************************************
//*************************************************************************************************


//*************************************************************************************************
//*************************************************************************************************
int main(void){
    
	uint8_t count;
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
	//-------------------- 
	Charger_Activate();		  //Активация ЗУ.
  AnalogSwitch_Activate();//Включение выходов мультиплексора.
  //--------------------
  //Проверка микросхемы EEPROM.
  //EepMemArrayCheck();
  //--------------------
	//Применение конфигурации.
	MotherBoard_WorkReg()->Address = Config()->Address;
  MotherBoard_WorkReg()->Group   = Config()->Group;
	SpeakerLine_Param()->Deviation = Config()->SpDeviation;
	SpeakerLine_Param()->Check     = Config()->SpCheck;
	Power()->Check.Byte = Config()->PowerCheckOn;
	
	for(count = 0; count < FIRE_LINES_NUMBER; count++)
		{
			FireLine(count)->Type    = Config()->FireLineConfig[count].Type;
			FireLine(count)->TimeOut = Config()->FireLineConfig[count].Timeout;	
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
	Scheduler_SetTask(Task_InternalProtocol);
		
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
  FireLine_MeasLoop();  	 			//Измерение пож.шлейфов.
  SpeakerLine_MeasLoop(); 			//Измерение линий Гр.
	SpeakerLine_SteadyStateLoop();//Получение установившегося значения после переходного процесса.
  //-------------------- 
  //Отработка режима "ТЕСТ".
  if(MotherBoard_WorkReg()->State == MB_TEST_STATE)
    { 
      if(Blink(INTERVAL_250_mS))Zummer(ZummerOn);
      else                      Zummer(ZummerOff);
      //Окончание тестирования.
      if((++mScountForZumTEST >= 10000) ||//Через 10 секунд или
         (RS485_Flags()->FPNAck))         //при отсутствии обмена с FP.
        {
          mScountForZumTEST = 0;
					Zummer(ZummerOff);
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





