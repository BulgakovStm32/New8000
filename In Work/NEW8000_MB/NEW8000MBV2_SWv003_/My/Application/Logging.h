
#ifndef _Logging_H
#define _Logging_H

//*****************************************************************************

#include "crc16.h"

//--------------------
//AppLayer.
#include "Faults.h"
#include "Time.h"
#include "EepM95128.h"
//*****************************************************************************
//*****************************************************************************
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
	//--------
	uint16_t SavedEvents;
	uint16_t UnreadEvents; 
	//--------
}LogCounts_t;
#pragma pack(pop)//вернули предыдущую настройку.
#define LOG_COUNTS_SIZE	(uint8_t)sizeof(LogCounts_t) 
//-------------------------------------------------
//Структура одного события системы
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
	//--------
	uint32_t TimeUtc;//Время фиксации события.
	uint16_t Number; //Номер события.
	uint8_t  ID;     //Код события.
	uint8_t  Param;  //Парметр события(номер зоны, номер шлейфа и т.п.).
	//--------
}Event_t;
#pragma pack(pop)//вернули предыдущую настройку.
#define EVENT_SIZE	(uint8_t)sizeof(Event_t) 
//-------------------------------------------------
//Структура для записи события в память EEPROM.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
	//--------
	Event_t  Event;
	uint16_t EventCRC;   
	//--------
}OneEventEep_t;
#pragma pack(pop)//вернули предыдущую настройку.
#define EVENT_EEP_SIZE	(uint8_t)sizeof(OneEventEep_t) 	
//-------------------------------------------------	
#define MAX_EVENTS					 		 1024u	
#define ALL_EVENTS_BUF_SIZE 		(uint16_t)(MAX_EVENTS * EVENT_SIZE)

#define UNREAD_EVENT_FLAG			(1<<15)//признак непросмотренного события.
#define RESET_UNREAD_EVENTS	  (uint16_t)0xFFFFu
//*****************************************************************************
//Все события системы!!! 
//enum AllSystemEvents{
//	//Основное питание блока.
//	MainPowerNorm = 0,	  
//	MainPowerNotWork,
//	//Резервное питание блока.
//	ReservePowerNorm, //
//	BatNotConnect,    //АКБ не подключен.
//	BatNorm,					//
//	BatCharging,      //"Заряд"
//	BatBelowNorm,	    //"Ниже нормы"
//	BatDeepCharge,    //"Глубокий разряд"
//	//Инвертер питания УМЗЧ.
//	InverterDcDcNorm,    //
//	InverterDcDcNotWork, //неисправен.
//	//УМЗЧ.
//	PANorm,	
//	PAProt,
//	//Линии ПОЖ1.
//	FireLine1Norm,
//	FireLine1Break,
//	FireLine1Short,
//	FireLine1Attention,
//	FireLine1Pusk,
//	//Линии ПОЖ2.
//	FireLine2Norm,
//	FireLine2Break,
//	FireLine2Short,
//	FireLine2Attention,
//	FireLine2Pusk, //21
//	//Линии ПОЖ3.
//	FireLine3Norm,
//	FireLine3Break,
//	FireLine3Short,
//	FireLine3Attention,
//	FireLine3Pusk,	
//	//Линии ГОиЧС.
//	FireLineChSNorm,
//	FireLineChSBreak,
//	FireLineChSShort,
//	FireLineChSAttention,
//	FireLineChSPusk, //31
//	//Линия Гр1.
//	SpeakerLine1Norm,
//	SpeakerLine1Break,
//	SpeakerLine1Short,
//	SpeakerLine1AboveNorm,
//	SpeakerLine1BelowNorm,
//	SpeakerLine1Active,
//	SpeakerLine1Deactive, //38
//	//Линия аттенюации Гр1.
//	AttenLine1Norm,
//	AttenLine1Break,
//	AttenLine1Short,
//	AttenLine1AboveNorm,
//	AttenLine1BelowNorm,
//	AttenLine1Active,
//	AttenLine1Deactive,	 //45
//	//Линия Гр2.
//	SpeakerLine2Norm,
//	SpeakerLine2Break,
//	SpeakerLine2Short,
//	SpeakerLine2AboveNorm,
//	SpeakerLine2BelowNorm,
//	SpeakerLine2Active,
//	SpeakerLine2Deactive, //52
//	//Линия аттенюации Гр2.
//	AttenLine2Norm,
//	AttenLine2Break,
//	AttenLine2Short,
//	AttenLine2AboveNorm,
//	AttenLine2BelowNorm,
//	AttenLine2Active,
//	AttenLine2Deactive,		//59
//	//Линия светового табло 1.  	
//	LightBoard1Norm,
//	LightBoard1Break,
//	LightBoard1Short,
//	LightBoard1Active,
//	LightBoard1Deactive,		//64
//	//Линия светового табло 2.  	
//	LightBoard2Norm,
//	LightBoard2Break,
//	LightBoard2Short,
//	LightBoard2Active,
//	LightBoard2Deactive, //69
//	//Линия светового табло 3.  	
//	LightBoard3Norm,
//	LightBoard3Break,
//	LightBoard3Short,
//	LightBoard3Active,
//	LightBoard3Deactive,	//74		
//	//Микрофон
//	MicNotConnect,
//	MicAct,
//	MicDeact,
//	MicNotWork,	//78
//	//Режимы работы блока.
//	LockMode,
//	ControlMode,
//	ConfigMode,
//	FireLineControlMode,//82
//	//
//};
//*****************************************************************************
//коды событий.
typedef enum{
	//----------------
	NoFault = 0,
	DeviceOn,
	LogACFault,
	LogACNorm,
	LogDCFault, 
	LogDCNorm,
	LogBatFault,
	LogBatNorm,
	//----------------
	LogFireLine1Fault, 
	LogFireLine1Norm,
	LogFireLine2Fault,
	LogFireLine2Norm,
	LogFireLine3Fault,
	LogFireLine3Norm,
	LogChSLineFault, 
	LogChSLineNorm,
	//----------------
	LogSpLine1Fault,
	LogSpLine1Norm,
	LogSpAtten1Fault,
	LogSpAtten1Norm,
	LogSpLine2Fault, 
	LogSpLine2Norm, 
	LogSpAtten2Fault,
	LogSpAtten2Norm,
	//----------------
	LogSiren1Fault,
	LogSiren1Norm,
	LogSiren2Fault,
	LogSiren2Norm,
	LogSiren3Fault,
	LogSiren3Norm,
	//----------------
	LogPAFault,
	LogPANorm,
	LogMicFault,
	LogMicNorm,
	LogConnectFault,
	LogConnectNorm,
	//----------------
}EventIDEnum_t;
//------------------------------------------------
//Парметр события(номер зоны, номер шлейфа и т.п.).
typedef enum{
	NoParam = 0,
	//----------------
//	//Режимы работы блока.
//	LockMode,
//	ControlMode,
//	ConfigMode,
//	PojLineControlMode,
//	//----------------
//	//Линии и системы блока. 
//	MainPower,	  //Основное питание блока.
//	ReservePower, //Резервное питание блока.
//	InverterDcDc, //Инвертер питания.
//	PA,						//УМЗЧ.
//	
//	FireLine1,		//Линия ПОЖ1.
//	FireLine2,		//Линия ПОЖ2.
//	FireLine3,		//Линия ПОЖ3.
//	FireLineChS,	//Линия ГОиЧС.
//	
//	SpeakerLine1, //Линия Гр1.
//	AttenLine1,   //линия аттенюации Гр1.
//	SpeakerLine2, //Линия Гр2.
//	AttenLine2,		//линия аттенюации Гр2.	
//	
//	LightBoard1,  //Линия светового табло 1.
//	LightBoard2,  //Линия светового табло 2.
//	LightBoard3,	//Линия светового табло 3.
//	//----------------
}EventParamEnum_t;
//*****************************************************************************
//*****************************************************************************
void 		     Log_Init(void);
void 				 Log_Loop(void);
LogCounts_t *Log_Counts(void);
void 				 Log_ResetCountUnreadEvents(void);
void         Log_SaveEvent(EventIDEnum_t eventID, EventParamEnum_t eventParam);
void 		     Log_GetEvent(uint8_t *pBuf, uint16_t numEvent);
//*****************************************************************************
//*****************************************************************************
#endif /*_Logging_H*/



