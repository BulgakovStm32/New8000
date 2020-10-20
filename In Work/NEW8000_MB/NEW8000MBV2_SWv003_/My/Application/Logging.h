
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
#pragma pack(push, 1)//������ ������������ � 1 ����
typedef struct{
	//--------
	uint16_t SavedEvents;
	uint16_t UnreadEvents; 
	//--------
}LogCounts_t;
#pragma pack(pop)//������� ���������� ���������.
#define LOG_COUNTS_SIZE	(uint8_t)sizeof(LogCounts_t) 
//-------------------------------------------------
//��������� ������ ������� �������
#pragma pack(push, 1)//������ ������������ � 1 ����
typedef struct{
	//--------
	uint32_t TimeUtc;//����� �������� �������.
	uint16_t Number; //����� �������.
	uint8_t  ID;     //��� �������.
	uint8_t  Param;  //������� �������(����� ����, ����� ������ � �.�.).
	//--------
}Event_t;
#pragma pack(pop)//������� ���������� ���������.
#define EVENT_SIZE	(uint8_t)sizeof(Event_t) 
//-------------------------------------------------
//��������� ��� ������ ������� � ������ EEPROM.
#pragma pack(push, 1)//������ ������������ � 1 ����
typedef struct{
	//--------
	Event_t  Event;
	uint16_t EventCRC;   
	//--------
}OneEventEep_t;
#pragma pack(pop)//������� ���������� ���������.
#define EVENT_EEP_SIZE	(uint8_t)sizeof(OneEventEep_t) 	
//-------------------------------------------------	
#define MAX_EVENTS					 		 1024u	
#define ALL_EVENTS_BUF_SIZE 		(uint16_t)(MAX_EVENTS * EVENT_SIZE)

#define UNREAD_EVENT_FLAG			(1<<15)//������� ���������������� �������.
#define RESET_UNREAD_EVENTS	  (uint16_t)0xFFFFu
//*****************************************************************************
//��� ������� �������!!! 
//enum AllSystemEvents{
//	//�������� ������� �����.
//	MainPowerNorm = 0,	  
//	MainPowerNotWork,
//	//��������� ������� �����.
//	ReservePowerNorm, //
//	BatNotConnect,    //��� �� ���������.
//	BatNorm,					//
//	BatCharging,      //"�����"
//	BatBelowNorm,	    //"���� �����"
//	BatDeepCharge,    //"�������� ������"
//	//�������� ������� ����.
//	InverterDcDcNorm,    //
//	InverterDcDcNotWork, //����������.
//	//����.
//	PANorm,	
//	PAProt,
//	//����� ���1.
//	FireLine1Norm,
//	FireLine1Break,
//	FireLine1Short,
//	FireLine1Attention,
//	FireLine1Pusk,
//	//����� ���2.
//	FireLine2Norm,
//	FireLine2Break,
//	FireLine2Short,
//	FireLine2Attention,
//	FireLine2Pusk, //21
//	//����� ���3.
//	FireLine3Norm,
//	FireLine3Break,
//	FireLine3Short,
//	FireLine3Attention,
//	FireLine3Pusk,	
//	//����� �����.
//	FireLineChSNorm,
//	FireLineChSBreak,
//	FireLineChSShort,
//	FireLineChSAttention,
//	FireLineChSPusk, //31
//	//����� ��1.
//	SpeakerLine1Norm,
//	SpeakerLine1Break,
//	SpeakerLine1Short,
//	SpeakerLine1AboveNorm,
//	SpeakerLine1BelowNorm,
//	SpeakerLine1Active,
//	SpeakerLine1Deactive, //38
//	//����� ���������� ��1.
//	AttenLine1Norm,
//	AttenLine1Break,
//	AttenLine1Short,
//	AttenLine1AboveNorm,
//	AttenLine1BelowNorm,
//	AttenLine1Active,
//	AttenLine1Deactive,	 //45
//	//����� ��2.
//	SpeakerLine2Norm,
//	SpeakerLine2Break,
//	SpeakerLine2Short,
//	SpeakerLine2AboveNorm,
//	SpeakerLine2BelowNorm,
//	SpeakerLine2Active,
//	SpeakerLine2Deactive, //52
//	//����� ���������� ��2.
//	AttenLine2Norm,
//	AttenLine2Break,
//	AttenLine2Short,
//	AttenLine2AboveNorm,
//	AttenLine2BelowNorm,
//	AttenLine2Active,
//	AttenLine2Deactive,		//59
//	//����� ��������� ����� 1.  	
//	LightBoard1Norm,
//	LightBoard1Break,
//	LightBoard1Short,
//	LightBoard1Active,
//	LightBoard1Deactive,		//64
//	//����� ��������� ����� 2.  	
//	LightBoard2Norm,
//	LightBoard2Break,
//	LightBoard2Short,
//	LightBoard2Active,
//	LightBoard2Deactive, //69
//	//����� ��������� ����� 3.  	
//	LightBoard3Norm,
//	LightBoard3Break,
//	LightBoard3Short,
//	LightBoard3Active,
//	LightBoard3Deactive,	//74		
//	//��������
//	MicNotConnect,
//	MicAct,
//	MicDeact,
//	MicNotWork,	//78
//	//������ ������ �����.
//	LockMode,
//	ControlMode,
//	ConfigMode,
//	FireLineControlMode,//82
//	//
//};
//*****************************************************************************
//���� �������.
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
//������� �������(����� ����, ����� ������ � �.�.).
typedef enum{
	NoParam = 0,
	//----------------
//	//������ ������ �����.
//	LockMode,
//	ControlMode,
//	ConfigMode,
//	PojLineControlMode,
//	//----------------
//	//����� � ������� �����. 
//	MainPower,	  //�������� ������� �����.
//	ReservePower, //��������� ������� �����.
//	InverterDcDc, //�������� �������.
//	PA,						//����.
//	
//	FireLine1,		//����� ���1.
//	FireLine2,		//����� ���2.
//	FireLine3,		//����� ���3.
//	FireLineChS,	//����� �����.
//	
//	SpeakerLine1, //����� ��1.
//	AttenLine1,   //����� ���������� ��1.
//	SpeakerLine2, //����� ��2.
//	AttenLine2,		//����� ���������� ��2.	
//	
//	LightBoard1,  //����� ��������� ����� 1.
//	LightBoard2,  //����� ��������� ����� 2.
//	LightBoard3,	//����� ��������� ����� 3.
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



