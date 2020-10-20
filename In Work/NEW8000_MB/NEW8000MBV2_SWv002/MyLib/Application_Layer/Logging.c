
#include "Logging.h"

//-----------------------------------------------------------------------------
OneEventForSaveUNION_Type OneEventForSaveUNION;
OneEventForSaveUNION_Type LogEventsArray[MaxEvent];//Массив событий.

//static EventSTR_Type EventSTR;

static uint16_t EventsSaveCount   = 0;//указатель на событие.
static uint16_t EventNotReadCount = 0;//Количество непросмотренных событий.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//запись одного события в журнал.
//argument: evntID - код события; eventParam - параметр собятия, если он есть.
//result  : 1 - запись произведена успешно; 0 - сбой при записи.
static uint8_t LogSaveEvent(EventIDEnum_t eventID, EventParamEnum_t eventParam){
	
	static uint8_t eventIDTemp    = 0; 
	static uint8_t eventParamTemp = 0;
	//--------------------
	//Если поступает событие которое уже было записано то выходим.
	if((eventID == eventIDTemp) && (eventParam == eventParamTemp)) return 1;	
	eventIDTemp    = eventID;
	eventParamTemp = eventParam;
	
	EventsSaveCount++;  //+1 к общему количеству записей.
	EventNotReadCount++;//+1 к количеству непросмотренных событий.
	
	if(EventsSaveCount > MaxEvent) EventsSaveCount = 1;
	//Формирование события для записи в журнал.
	OneEventForSaveUNION.STR.OneEvent.EventNumber  = EventsSaveCount | NotReadEventFlag;
	OneEventForSaveUNION.STR.OneEvent.EventTimeUtc = TimeUTCGet();
	OneEventForSaveUNION.STR.OneEvent.EventID      = eventID;			//
	OneEventForSaveUNION.STR.OneEvent.EventParam   = eventParam;  //
	//Сохраниение одной записи в журнал.	
	OneEventForSaveUNION.STR.EventCRC = Get_Crc16(OneEventForSaveUNION.BLK, 8);
	LogEventsArray[EventsSaveCount-1] = OneEventForSaveUNION;
	
	return 1;	
	
	//Проверка записанного события.
//	if(LogGetEvent(LogEventsIndex)->STR.EventCRC == Get_Crc16(OneEventUNION.BLK, 8))
//		{
//			if(++LogEventsIndex >= MaxEvent) LogEventsIndex = 0;
//			return 1;
//		}
//	return 0;
}
//-----------------------------------------------------------------------------
//Получение указателя на одну запись в журнале событий.
//argument: numEvent - номер записи в журнале. Принимаемые значения от 1 до 1024.
//result  : указатель за одну запись в журнале..
OneEventSTR_Type* LogGetEvent(uint16_t numEvent){

	if(numEvent > MaxEvent) numEvent = MaxEvent;
	if(numEvent != 0)       numEvent -= 1;
	//Если запрашиваемое событие еще не было просмотренно то -1 к количеству непросмотренных событий.
	if(LogEventsArray[numEvent].STR.OneEvent.EventNumber & NotReadEventFlag)
		{
			if(EventNotReadCount != 0) EventNotReadCount -= 1;
		}	
	LogEventsArray[numEvent].STR.OneEvent.EventNumber &= ~NotReadEventFlag;//Сброс признака непрочитанного сообщения.	
	return &LogEventsArray[numEvent].STR.OneEvent;
}
//-----------------------------------------------------------------------------
//Получение количества событий записанных в журнал.
//argument: нет.
//result  : общее количество событий записанных в журнал.
uint16_t LogGetTotalEventsSave(void){

	return EventsSaveCount;
}
//-----------------------------------------------------------------------------
//Получение количества непросмотренных событий.
//argument: нет.
//result  : количество непросмотренных событий.
uint16_t LogGetNotReadEvents(void){

	return EventNotReadCount;
}
//-----------------------------------------------------------------------------
//Сброс непросмотренных событий.
//argument: нет.
//result  : нет.
void LogClrNotReadEvents(void){

	EventNotReadCount = 0;
}
//-----------------------------------------------------------------------------
void Logging(EventIDEnum_t eventID, EventParamEnum_t eventParam){

//	static uint32_t eventIDReg = 0;
//	//--------------------
//	if((eventIDReg & eventID) == 0)
//		{
//			eventIDReg |= eventID;
//			LogSaveEvent(eventID, eventParam);
//		}

	LogSaveEvent(eventID, eventParam);
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------





















