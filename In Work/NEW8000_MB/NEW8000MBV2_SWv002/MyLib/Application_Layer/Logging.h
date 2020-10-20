
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
//Структура одного события системы
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
		//--------
		uint16_t EventNumber; //Номер события.
		uint32_t EventTimeUtc;//Время фиксации события.
		uint8_t  EventID;     //Код события.
		uint8_t  EventParam;  //Парметр события(номер зоны, номер шлейфа и т.п.).
		//--------
}OneEventSTR_Type;
#pragma pack(pop)//вернули предыдущую настройку.
//-------------------------------------------------
//Структура для записи события в память
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef union{
	struct{
		//--------
		OneEventSTR_Type OneEvent;
		uint16_t EventCRC;   
		//--------
		}STR;
	uint8_t BLK[10];
}OneEventForSaveUNION_Type;
#pragma pack(pop)//вернули предыдущую настройку.
//-------------------------------------------------
#define MaxEvent 					1024	//максимальное количество записей.
#define NotReadEventFlag (1<<15)//признак непросмотренно события.
//-------------------------------------------------
//#define NoParam	     0
//Флаги неисправности.
//#define NoFaults            0     //нет неисправностей
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
//Все события системы!!! 
enum AllSystemEvents{
	
	//Основное питание блока.
	MainPowerNorm = 0,	  
	MainPowerNotWork,
	//Резервное питание блока.
	ReservePowerNorm, //
	BatNotConnect,    //АКБ не подключен.
	BatNorm,					//
	BatCharging,      //"Заряд"
	BatBelowNorm,	    //"Ниже нормы"
	BatDeepCharge,    //"Глубокий разряд"
	//Инвертер питания УМЗЧ.
	InverterDcDcNorm,    //
	InverterDcDcNotWork, //неисправен.
	//УМЗЧ.
	PANorm,	
	PAProt,
	//Линии ПОЖ1.
	FireLine1Norm,
	FireLine1Break,
	FireLine1Short,
	FireLine1Attention,
	FireLine1Pusk,
	//Линии ПОЖ2.
	FireLine2Norm,
	FireLine2Break,
	FireLine2Short,
	FireLine2Attention,
	FireLine2Pusk, //21
	//Линии ПОЖ3.
	FireLine3Norm,
	FireLine3Break,
	FireLine3Short,
	FireLine3Attention,
	FireLine3Pusk,	
	//Линии ГОиЧС.
	FireLineChSNorm,
	FireLineChSBreak,
	FireLineChSShort,
	FireLineChSAttention,
	FireLineChSPusk, //31
	//Линия Гр1.
	SpeakerLine1Norm,
	SpeakerLine1Break,
	SpeakerLine1Short,
	SpeakerLine1AboveNorm,
	SpeakerLine1BelowNorm,
	SpeakerLine1Active,
	SpeakerLine1Deactive, //38
	//Линия аттенюации Гр1.
	AttenLine1Norm,
	AttenLine1Break,
	AttenLine1Short,
	AttenLine1AboveNorm,
	AttenLine1BelowNorm,
	AttenLine1Active,
	AttenLine1Deactive,	 //45
	//Линия Гр2.
	SpeakerLine2Norm,
	SpeakerLine2Break,
	SpeakerLine2Short,
	SpeakerLine2AboveNorm,
	SpeakerLine2BelowNorm,
	SpeakerLine2Active,
	SpeakerLine2Deactive, //52
	//Линия аттенюации Гр2.
	AttenLine2Norm,
	AttenLine2Break,
	AttenLine2Short,
	AttenLine2AboveNorm,
	AttenLine2BelowNorm,
	AttenLine2Active,
	AttenLine2Deactive,		//59
	//Линия светового табло 1.  	
	LightBoard1Norm,
	LightBoard1Break,
	LightBoard1Short,
	LightBoard1Active,
	LightBoard1Deactive,		//64
	//Линия светового табло 2.  	
	LightBoard2Norm,
	LightBoard2Break,
	LightBoard2Short,
	LightBoard2Active,
	LightBoard2Deactive, //69
	//Линия светового табло 3.  	
	LightBoard3Norm,
	LightBoard3Break,
	LightBoard3Short,
	LightBoard3Active,
	LightBoard3Deactive,	//74		
	//Микрофон
	MicNotConnect,
	MicAct,
	MicDeact,
	MicNotWork,	//78
	//Режимы работы блока.
	LockMode,
	ControlMode,
	ConfigMode,
	FireLineControlMode,//82
	//
	
};
//------------------------------------------------
//коды событий.
typedef enum{
	//----------------
	//Состояния линий и систем блока. 
	Prot = 0,    	//"Защита!";
//	BatCharging,  //"Заряд"
//	BatDeepCharge,//"Глубокий разряд"
	
	Attention,  	//"ВНИМАНИЕ!";
	Fire,					//"ПОЖАР!"
	Pusk,					//"ПУСК";
	
	Active,				//"Активно"
	Deactive,			//"Отключено"
	Norm,					//"Норма"
	NotWork,    	//"Неиспр."
	AboveNorm,		//"Выше нормы"
	BelowNorm,		//"Ниже нормы"
	NotConnec,  	//не подключен.
	Break,				//"Обрыв"
	Short,				//"КЗ"
	//----------------
}EventIDEnum_t;
//------------------------------------------------
//Парметр события(номер зоны, номер шлейфа и т.п.).
typedef enum{
	NoParam = 0,
	//----------------
	//Режимы работы блока.
//	LockMode,
//	ControlMode,
//	ConfigMode,
//	PojLineControlMode,
	//----------------
	//Линии и системы блока. 
	MainPower,	  //Основное питание блока.
	ReservePower, //Резервное питание блока.
	InverterDcDc, //Инвертер питания.
	PA,						//УМЗЧ.
	
	FireLine1,		//Линия ПОЖ1.
	FireLine2,		//Линия ПОЖ2.
	FireLine3,		//Линия ПОЖ3.
	FireLineChS,	//Линия ГОиЧС.
	
	SpeakerLine1, //Линия Гр1.
	AttenLine1,   //линия аттенюации Гр1.
	SpeakerLine2, //Линия Гр2.
	AttenLine2,		//линия аттенюации Гр2.	
	
	LightBoard1,  //Линия светового табло 1.
	LightBoard2,  //Линия светового табло 2.
	LightBoard3,	//Линия светового табло 3.
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



