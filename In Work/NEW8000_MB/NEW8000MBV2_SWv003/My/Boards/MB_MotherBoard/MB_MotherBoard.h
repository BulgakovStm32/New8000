
#ifndef _MB_MotherBoard_H
#define _MB_MotherBoard_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

//--------------------
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

#include "SB_SirenBoard.h"
//-----------------------------------------------------------------------------
//Структура данны от ЦП.
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
	uint8_t  Siren1Status; 
	uint8_t  Siren2Status; 
	uint8_t  Siren3Status; 
	//--------------------	
	uint16_t DebugData1;
	uint16_t DebugData2;
	//--------------------
	//
	uint16_t LcDeviationVolue;
	uint8_t  StatusZone1;
	uint8_t  StatusZone2;
	uint8_t  StatusZone3;
	uint8_t  StatusZone4;
	uint8_t  StatusZone5;
	uint8_t  StatusZone6;
	//--------------------
	//
	uint8_t  StatusPoj1;
	uint8_t  StatusPoj2;
	uint8_t  StatusPoj3;
	uint8_t  StatusGOChS;
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
#define MB_WORK_STATE	0
#define MB_TEST_STATE	1

//********************************
#define StandbyMode			0		//Дежурный режим.
#define AttentionMode		1		//Режим ВНИМАНИЕ	
#define PuskMode		    2		//Режим ПУСК	

#define PRIORITY_1	    1
#define PRIORITY_2	    2
#define PRIORITY_3	    3
#define PRIORITY_4	    4
#define PRIORITY_5	    5
#define PRIORITY_6	    6

#define PUSK_Activation  7
#define START_Activation 8
//*****************************************************************************
//*****************************************************************************
void 			   MotherBoard_BuildingPackForRS485(uint8_t *dataBuf);
MBWorkReg_t* MotherBoard_WorkReg(void);
//*****************************************************************************
//*****************************************************************************
#endif /*_MB_MotherBoard_H*/




















