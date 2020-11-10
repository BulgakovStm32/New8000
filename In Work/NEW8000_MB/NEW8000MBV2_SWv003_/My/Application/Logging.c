
#include "Logging.h"

//**************************************************************************************
static LogCounts_t LogCountsStr;
//**************************************************************************************
//**************************************************************************************

//static Event_t  EventsArr[MAX_EVENTS]; //

////*****************************************************************************
//static void WriteOneEventToArr(Event_t *event, uint16_t numEvent){
//	
//	uint32_t index = numEvent - 1;
//	//--------------------	
//	EventsArr[index].TimeUtc = event->TimeUtc;
//	EventsArr[index].Number  = event->Number;
//	EventsArr[index].ID      = event->ID;
//	EventsArr[index].Param   = event->Param;
//}
////*****************************************************************************
//static void ReadOneEventFromArr(Event_t *event, uint16_t numEvent){
//	
//	if(EventsArr[numEvent].Number & EVENT_NOT_READ_FLAG)
//		{
//			if(EventsNotReadCount != 0) EventsNotReadCount--;  //-1 к количеству непросмотренных событий.
//			EventsArr[numEvent].Number &= ~EVENT_NOT_READ_FLAG;//Сброс признака непрочитанного сообщения.	 	
//		}
//	event->TimeUtc = EventsArr[numEvent].TimeUtc;
//	event->Number  = EventsArr[numEvent].Number;
//	event->ID      = EventsArr[numEvent].ID;
//	event->Param   = EventsArr[numEvent].Param;
//}
//**************************************************************************************
//**************************************************************************************
static void WriteOneEventToEep(OneEventEep_t *event){
	
	uint16_t writeAddr;
	uint16_t writeBuf[2];
	//--------------------
	//Write Work regs
	writeBuf[0] = LogCountsStr.SavedEvents; //указатель на событие.
	writeBuf[1] = LogCountsStr.UnreadEvents;//Количество непросмотренных событий.
	
	EepM95128_WriteBuffer((uint8_t*)writeBuf, EEP_MEM_START_ADR, LOG_COUNTS_SIZE);
	//--------------------
	writeAddr  = (event->Event.Number & ~UNREAD_EVENT_FLAG) - 1;
	writeAddr *= EVENT_EEP_SIZE;
	writeAddr += LOG_COUNTS_SIZE;
	
	EepM95128_WriteBuffer((uint8_t*)event, writeAddr, EVENT_EEP_SIZE);
}
//*****************************************************************************
static void ReadOneEventFromEep(OneEventEep_t *event, uint16_t numEvent){
	
	uint16_t readAddr;
  //--------------------
	readAddr  = numEvent * EVENT_EEP_SIZE;
	readAddr += LOG_COUNTS_SIZE;//
	
	EepM95128_ReadBuffer((uint8_t*)event, readAddr, EVENT_EEP_SIZE);
}
//*****************************************************************************
static void ReadWorkRegFromEep(void){
		
	EepM95128_ReadBuffer((uint8_t*)&LogCountsStr, EEP_MEM_START_ADR, LOG_COUNTS_SIZE);
}
//*****************************************************************************
static void WriteWorkRegToEep(void){
	
	EepM95128_WriteBuffer((uint8_t*)&LogCountsStr, EEP_MEM_START_ADR, LOG_COUNTS_SIZE);
}
//**************************************************************************************
//static void ClearAllEventToEep(void){
//	
//	LogCountsStr.SavedEvents  = 0;//указатель на событие.
//	LogCountsStr.UnreadEvents = 0;//Количество непросмотренных событий.
//	WriteWorkRegToEep();
//}
//**************************************************************************************
//**************************************************************************************
void Log_Init(void){
 
	EepM95128_Init();
//	ClearAllEventToEep();//сброс счетчиков событий.
	ReadWorkRegFromEep();
}
//*****************************************************************************
//Логирование событий блока.
void Log_Loop(void){
	
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
	//Отсутствие связи с лицевой панелью.
	if(faultsRise & ConnectFaultFlag) Log_SaveEvent(LogConnectFault, NoParam);
	if(faultsFall & ConnectFaultFlag) Log_SaveEvent(LogConnectNorm,  NoParam);
	//Неисправность внешнего микрофона.
	if(faultsRise & MicFaultFlag) Log_SaveEvent(LogMicFault, NoParam);
	if(faultsFall & MicFaultFlag) Log_SaveEvent(LogMicNorm,  NoParam);	
}
//*****************************************************************************
LogCounts_t *Log_Counts(void){

	return &LogCountsStr;
}
//*****************************************************************************
void Log_ResetCountUnreadEvents(void){

	LogCountsStr.UnreadEvents = 0;
	WriteWorkRegToEep();
}
//*****************************************************************************
//запись одного события в журнал.
//argument: evntID - код события; eventParam - параметр собятия, если он есть.
void Log_SaveEvent(EventIDEnum_t eventID, EventParamEnum_t eventParam){
	
	static OneEventEep_t eventForEep;
	//--------------------
	if(LogCountsStr.SavedEvents == MAX_EVENTS) LogCountsStr.SavedEvents = 0;
	LogCountsStr.SavedEvents++; //+1 к общему количеству записей.
	
	if(LogCountsStr.UnreadEvents < MAX_EVENTS) LogCountsStr.UnreadEvents++;//+1 к количеству непросмотренных событий.

	if(LogCountsStr.SavedEvents > MAX_EVENTS) LogCountsStr.SavedEvents = 1;
	
	//Формирование события для записи в журнал.
	eventForEep.Event.TimeUtc = TimeUTC_Get();
	eventForEep.Event.Number  = LogCountsStr.SavedEvents | UNREAD_EVENT_FLAG;
	eventForEep.Event.ID      = eventID;			
	eventForEep.Event.Param   = eventParam;  
	eventForEep.EventCRC      = Get_Crc16((uint8_t*)&eventForEep.Event, EVENT_SIZE);
	//Сохраниение одной записи в EEPROM.
	WriteOneEventToEep(&eventForEep);
}
//*****************************************************************************
void Log_GetEvent(uint8_t *pBuf, uint16_t numEvent){
	
	static OneEventEep_t eventFromEep;
	Event_t *event = (Event_t*)pBuf;
	//--------------------	
	if(numEvent > MAX_EVENTS) numEvent = MAX_EVENTS;
	if(numEvent != 0)         numEvent--;
	
	ReadOneEventFromEep(&eventFromEep, numEvent);
	//Если запрашиваемое событие еще не было просмотренно то...
	if(eventFromEep.Event.Number & UNREAD_EVENT_FLAG)
		{
			//-1 к количеству непросмотренных событий.
			if(LogCountsStr.UnreadEvents != 0) LogCountsStr.UnreadEvents--;
			//Сброс признака непрочитанного сообщения.	 
			eventFromEep.Event.Number &= ~UNREAD_EVENT_FLAG;
			//
			WriteOneEventToEep(&eventFromEep);	
		}
	event->TimeUtc = eventFromEep.Event.TimeUtc;
	event->Number  = eventFromEep.Event.Number;	
	event->ID      = eventFromEep.Event.ID;
	event->Param   = eventFromEep.Event.Param;
	//--------------------
}
//**************************************************************************************
//**************************************************************************************
































