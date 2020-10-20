
#ifndef _Logging_H
#define _Logging_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "crc16.h"
//--------------------
//AppLayer.
#include "Time.h"
//#include "Faults.h"
//-----------------------------------------------------------------------------
//��������� ������ ������� �������
#pragma pack(push, 1)//������ ������������ � 1 ����
typedef struct{
		//--------
		uint16_t EventNumber; //����� �������.
		uint32_t EventTimeUtc;//����� �������� �������.
		uint8_t  EventID;     //��� �������.
		uint8_t  EventParam;  //������� �������(����� ����, ����� ������ � �.�.).
		//--------
}OneEventSTR_Type;
#pragma pack(pop)//������� ���������� ���������.
//-------------------------------------------------
//��������� ��� ������ ������� � ������
#pragma pack(push, 1)//������ ������������ � 1 ����
typedef union{
	struct{
		//--------
		OneEventSTR_Type OneEvent;
		uint16_t EventCRC;   
		//--------
		}STR;
	uint8_t BLK[10];
}OneEventForSaveUNION_Type;
#pragma pack(pop)//������� ���������� ���������.
//-------------------------------------------------
#define MaxEvent 					1024	//������������ ���������� �������.
#define NotReadEventFlag (1<<15)//������� �������������� �������.
//-------------------------------------------------
//#define NoParam	     0
//����� �������������.
//#define NoFaults            0     //��� ��������������
//#define FaultPoj1_Flag     (1<<0) 
//#define FaultPoj2_Flag     (1<<1) 
//#define FaultPoj3_Flag     (1<<2) 
//#define FaultChS_Flag      (1<<3) 
//#define FaultLc1Line_Flag  (1<<4) 
//#define FaultLc1Att_Flag   (1<<5) 
//#define FaultLc2Line_Flag  (1<<6) 
//#define FaultLc2Att_Flag   (1<<7) 
//#define FaultACNo_Flag     (1<<8)
//#define FaultDCFault_Flag  (1<<9)  
//#define FaultBat_Flag      (1<<10)
//#define FaultPA_Flag       (1<<11)
//#define FaultMic_Flag      (1<<12)
//-------------------------------------------------
//��� ������� �������!!! 
enum AllSystemEvents{
	
	//�������� ������� �����.
	MainPowerNorm = 0,	  
	MainPowerNotWork,
	//��������� ������� �����.
	ReservePowerNorm, //
	BatNotConnect,    //��� �� ���������.
	BatNorm,					//
	BatCharging,      //"�����"
	BatBelowNorm,	    //"���� �����"
	BatDeepCharge,    //"�������� ������"
	//�������� ������� ����.
	InverterDcDcNorm,    //
	InverterDcDcNotWork, //����������.
	//����.
	PANorm,	
	PAProt,
	//����� ���1.
	FireLine1Norm,
	FireLine1Break,
	FireLine1Short,
	FireLine1Attention,
	FireLine1Pusk,
	//����� ���2.
	FireLine2Norm,
	FireLine2Break,
	FireLine2Short,
	FireLine2Attention,
	FireLine2Pusk, //21
	//����� ���3.
	FireLine3Norm,
	FireLine3Break,
	FireLine3Short,
	FireLine3Attention,
	FireLine3Pusk,	
	//����� �����.
	FireLineChSNorm,
	FireLineChSBreak,
	FireLineChSShort,
	FireLineChSAttention,
	FireLineChSPusk, //31
	//����� ��1.
	SpeakerLine1Norm,
	SpeakerLine1Break,
	SpeakerLine1Short,
	SpeakerLine1AboveNorm,
	SpeakerLine1BelowNorm,
	SpeakerLine1Active,
	SpeakerLine1Deactive, //38
	//����� ���������� ��1.
	AttenLine1Norm,
	AttenLine1Break,
	AttenLine1Short,
	AttenLine1AboveNorm,
	AttenLine1BelowNorm,
	AttenLine1Active,
	AttenLine1Deactive,	 //45
	//����� ��2.
	SpeakerLine2Norm,
	SpeakerLine2Break,
	SpeakerLine2Short,
	SpeakerLine2AboveNorm,
	SpeakerLine2BelowNorm,
	SpeakerLine2Active,
	SpeakerLine2Deactive, //52
	//����� ���������� ��2.
	AttenLine2Norm,
	AttenLine2Break,
	AttenLine2Short,
	AttenLine2AboveNorm,
	AttenLine2BelowNorm,
	AttenLine2Active,
	AttenLine2Deactive,		//59
	//����� ��������� ����� 1.  	
	LightBoard1Norm,
	LightBoard1Break,
	LightBoard1Short,
	LightBoard1Active,
	LightBoard1Deactive,		//64
	//����� ��������� ����� 2.  	
	LightBoard2Norm,
	LightBoard2Break,
	LightBoard2Short,
	LightBoard2Active,
	LightBoard2Deactive, //69
	//����� ��������� ����� 3.  	
	LightBoard3Norm,
	LightBoard3Break,
	LightBoard3Short,
	LightBoard3Active,
	LightBoard3Deactive,	//74		
	//��������
	MicNotConnect,
	MicAct,
	MicDeact,
	MicNotWork,	//78
	//������ ������ �����.
	LockMode,
	ControlMode,
	ConfigMode,
	FireLineControlMode,//82
	//
	
};
//------------------------------------------------
//���� �������.
typedef enum{
	//----------------
	//��������� ����� � ������ �����. 
	Prot = 0,    	//"������!";
//	BatCharging,  //"�����"
//	BatDeepCharge,//"�������� ������"
	
	Attention,  	//"��������!";
	Fire,					//"�����!"
	Pusk,					//"����";
	
	Active,				//"�������"
	Deactive,			//"���������"
	Norm,					//"�����"
	NotWork,    	//"������."
	AboveNorm,		//"���� �����"
	BelowNorm,		//"���� �����"
	NotConnec,  	//�� ���������.
	Break,				//"�����"
	Short,				//"��"
	//----------------
}EventIDEnum_t;
//------------------------------------------------
//������� �������(����� ����, ����� ������ � �.�.).
typedef enum{
	NoParam = 0,
	//----------------
	//������ ������ �����.
//	LockMode,
//	ControlMode,
//	ConfigMode,
//	PojLineControlMode,
	//----------------
	//����� � ������� �����. 
	MainPower,	  //�������� ������� �����.
	ReservePower, //��������� ������� �����.
	InverterDcDc, //�������� �������.
	PA,						//����.
	
	FireLine1,		//����� ���1.
	FireLine2,		//����� ���2.
	FireLine3,		//����� ���3.
	FireLineChS,	//����� �����.
	
	SpeakerLine1, //����� ��1.
	AttenLine1,   //����� ���������� ��1.
	SpeakerLine2, //����� ��2.
	AttenLine2,		//����� ���������� ��2.	
	
	LightBoard1,  //����� ��������� ����� 1.
	LightBoard2,  //����� ��������� ����� 2.
	LightBoard3,	//����� ��������� ����� 3.
	//----------------
}EventParamEnum_t;
//-----------------------------------------------------------------------------
//uint8_t             LogSaveEvent(uint8_t eventID, uint8_t eventParam);
OneEventSTR_Type* LogGetEvent(uint16_t numEvent);
uint16_t 					LogGetTotalEventsSave(void);
uint16_t 					LogGetNotReadEvents(void);
void 							LogClrNotReadEvents(void);


void Logging(EventIDEnum_t eventID, EventParamEnum_t eventParam);
//-----------------------------------------------------------------------------
#endif /*_Logging_H*/



