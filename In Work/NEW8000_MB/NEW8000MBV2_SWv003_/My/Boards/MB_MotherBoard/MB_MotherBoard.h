
#ifndef _MB_MotherBoard_H
#define _MB_MotherBoard_H
//***************************************************************************************************
//AppLayer.
#include "LineCheck.h"
#include "PojLine.h"
#include "Relay.h"
#include "Buttons.h"
#include "Amplifier.h"
#include "PowerAndCharge.h"
#include "Time.h"
#include "Logging.h"
#include "EepM95128.h"
//*********************
//Boards.
#include "SB_SirenBoard.h"
//***************************************************************************************************
//***************************************************************************************************
typedef struct{
	//--------------------
	uint8_t OutState  :4;
	uint8_t LineState :4;	
	//--------------------
}Line_t;
//**************************************************
//Структура данных ЦП.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
	//--------------------
	uint32_t TimeUTC;
	//--------------------	
	//Состояние журнала событий.
	uint16_t TotalEvents;
	uint16_t NotReadEvents;
	//--------------------
	//Состояния оповещателей.
	uint16_t SirenPower;	 
	Line_t   Siren1;
	Line_t   Siren2;
	Line_t   Siren3;
	//--------------------	
	uint16_t DebugData1;
	uint16_t DebugData2;
	//--------------------
	//Линии Гр.
	uint16_t  SpLineDeviation;
	Line_t 		Sp1;
	Line_t 		Sp2;
	Line_t 		Sp3;
	Line_t 		Sp4;
	Line_t 		Sp5;
	Line_t 		Sp6;	
	//--------------------
	//Линии шлейфов сигнализайии.
	Line_t 		Fr1;
	Line_t 		Fr2;
	Line_t 		Fr3;
	Line_t 		FrChS;
	//--------------------	
	uint8_t  Addres;         
	uint8_t  Group;
	uint8_t  MB_State;
	uint8_t  StatusPOWER;
	uint8_t  StatusPA;
	//--------------------
	//Состояния платы IP.		

	//--------------------
	//Состояния платы ZE.		

	//--------------------
}MBData_t;
#pragma pack(pop)//вернули предыдущую настройку.
//**************************************************
//Рабочие переменные.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
	uint8_t  Address;
	uint8_t  Group;
	uint8_t  State;
}MBWorkReg_t;
#pragma pack(pop)//вернули предыдущую настройку.
//**************************************************
#define MB_TYPE             0x01 //Тип модуля - материнская плата.
//Команды MB.
#define MB_CMD_SEND_STATUS  0xA0 //Команда "Выдача статуса от MB".
//********************************
#define MB_WORK_STATE		0
#define MB_TEST_STATE		1

//********************************
#define StandbyMode			0		//Дежурный режим.
#define AttentionMode		1		//Режим ВНИМАНИЕ	
#define PuskMode		    2		//Режим ПУСК

//********************************
enum ActivationEnum{

	MIC_ACTIVATION = 1,
	PUSK_BUTTON_ACTIVATION,
	FIRE_LINE_ACTIVATION,
	ChS_ACTIVATION,
	PRIORITY_4,
	PRIORITY_5,
	MANUAL_CONTROL,
	START_BUTTON_ACTIVATION,
};
//***************************************************************************************************
//***************************************************************************************************
void 			   MotherBoard_BuildingPackForRS485(uint8_t *dataBuf);
MBWorkReg_t* MotherBoard_WorkReg(void);

//***************************************************************************************************
//***************************************************************************************************
#endif /*_MB_MotherBoard_H*/




















