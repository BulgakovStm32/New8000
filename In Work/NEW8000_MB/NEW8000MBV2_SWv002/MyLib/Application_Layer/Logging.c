
#include "Logging.h"

//-----------------------------------------------------------------------------
OneEventForSaveUNION_Type OneEventForSaveUNION;
OneEventForSaveUNION_Type LogEventsArray[MaxEvent];//������ �������.

//static EventSTR_Type EventSTR;

static uint16_t EventsSaveCount   = 0;//��������� �� �������.
static uint16_t EventNotReadCount = 0;//���������� ��������������� �������.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//������ ������ ������� � ������.
//argument: evntID - ��� �������; eventParam - �������� �������, ���� �� ����.
//result  : 1 - ������ ����������� �������; 0 - ���� ��� ������.
static uint8_t LogSaveEvent(EventIDEnum_t eventID, EventParamEnum_t eventParam){
	
	static uint8_t eventIDTemp    = 0; 
	static uint8_t eventParamTemp = 0;
	//--------------------
	//���� ��������� ������� ������� ��� ���� �������� �� �������.
	if((eventID == eventIDTemp) && (eventParam == eventParamTemp)) return 1;	
	eventIDTemp    = eventID;
	eventParamTemp = eventParam;
	
	EventsSaveCount++;  //+1 � ������ ���������� �������.
	EventNotReadCount++;//+1 � ���������� ��������������� �������.
	
	if(EventsSaveCount > MaxEvent) EventsSaveCount = 1;
	//������������ ������� ��� ������ � ������.
	OneEventForSaveUNION.STR.OneEvent.EventNumber  = EventsSaveCount | NotReadEventFlag;
	OneEventForSaveUNION.STR.OneEvent.EventTimeUtc = TimeUTCGet();
	OneEventForSaveUNION.STR.OneEvent.EventID      = eventID;			//
	OneEventForSaveUNION.STR.OneEvent.EventParam   = eventParam;  //
	//����������� ����� ������ � ������.	
	OneEventForSaveUNION.STR.EventCRC = Get_Crc16(OneEventForSaveUNION.BLK, 8);
	LogEventsArray[EventsSaveCount-1] = OneEventForSaveUNION;
	
	return 1;	
	
	//�������� ����������� �������.
//	if(LogGetEvent(LogEventsIndex)->STR.EventCRC == Get_Crc16(OneEventUNION.BLK, 8))
//		{
//			if(++LogEventsIndex >= MaxEvent) LogEventsIndex = 0;
//			return 1;
//		}
//	return 0;
}
//-----------------------------------------------------------------------------
//��������� ��������� �� ���� ������ � ������� �������.
//argument: numEvent - ����� ������ � �������. ����������� �������� �� 1 �� 1024.
//result  : ��������� �� ���� ������ � �������..
OneEventSTR_Type* LogGetEvent(uint16_t numEvent){

	if(numEvent > MaxEvent) numEvent = MaxEvent;
	if(numEvent != 0)       numEvent -= 1;
	//���� ������������� ������� ��� �� ���� ������������ �� -1 � ���������� ��������������� �������.
	if(LogEventsArray[numEvent].STR.OneEvent.EventNumber & NotReadEventFlag)
		{
			if(EventNotReadCount != 0) EventNotReadCount -= 1;
		}	
	LogEventsArray[numEvent].STR.OneEvent.EventNumber &= ~NotReadEventFlag;//����� �������� �������������� ���������.	
	return &LogEventsArray[numEvent].STR.OneEvent;
}
//-----------------------------------------------------------------------------
//��������� ���������� ������� ���������� � ������.
//argument: ���.
//result  : ����� ���������� ������� ���������� � ������.
uint16_t LogGetTotalEventsSave(void){

	return EventsSaveCount;
}
//-----------------------------------------------------------------------------
//��������� ���������� ��������������� �������.
//argument: ���.
//result  : ���������� ��������������� �������.
uint16_t LogGetNotReadEvents(void){

	return EventNotReadCount;
}
//-----------------------------------------------------------------------------
//����� ��������������� �������.
//argument: ���.
//result  : ���.
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





















