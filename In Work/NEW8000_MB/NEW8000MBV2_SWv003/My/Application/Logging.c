
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
	ReadWorkRegFromEep();

	//ClearAllEventToEep();
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
	LogCountsStr.SavedEvents++; //+1 к общему количеству записей.
	LogCountsStr.UnreadEvents++;//+1 к количеству непросмотренных событий.
	
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
































