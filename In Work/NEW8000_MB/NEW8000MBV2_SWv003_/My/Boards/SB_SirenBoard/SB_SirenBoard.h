
#ifndef _SB_SirenBoard_H
#define _SB_SirenBoard_H
//*****************************************************************************

#include "stm32f10x.h"

//*****************************************************************************
//*****************************************************************************
typedef struct{
	//--------------------
	uint8_t OutState  :4;
	uint8_t LineState :4;	
	//--------------------
}SirenLine_t;

#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct {
	//--------------------
	uint16_t    SirenPower;//Напряжение питания оповещателей, в мВ.
	SirenLine_t Siren1;
	SirenLine_t Siren2;
	SirenLine_t Siren3;
	//--------------------	
}SBData_t;
#pragma pack(pop)     //вернули предыдущую настройку.
#define SB_DATA_SIZE	((uint8_t)sizeof(SBData_t))
//*****************************************************************************
//*****************************************************************************
//Команды платы управления оповещателями(Siren Board - SB).
#define SB_TYPE			(uint8_t)0x05u

#define SB_CMD_SET	(uint8_t)0x01u    //

#define NO_CONNECTION_WITH_SB	(uint16_t)0xffffu //признак отсутствия обмена данни с платой SB.
//--------------------
//Режимы работы выхода.
#define SIREN_OUT_OFF			0	//Выход отключен.
#define SIREN_OUT_ON		 	1	//Выход активен.
#define SIREN_OUT_Blink1 	2	//Режим МИГАНИЕ-1. 500мС - ON, 500мС - OFF.
#define SIREN_OUT_Blink2 	3	//Режим МИГАНИЕ-2. 250мС - ON, 250мС - OFF.
//--------------------
//Состояния шлейфов.
#define SIREN_LINE_UNDEF  0
#define SIREN_LINE_NORM   1
#define SIREN_LINE_BREAK  2
#define SIREN_LINE_SHORT  3
//--------------------
#define SIREN_STATUS		  0
#define SIREN_1				    1
#define SIREN_2				    2
#define SIREN_3				    3
#define SIREN_ALL			    4 
//*****************************************************************************
//*****************************************************************************
void      SirenBoard_Init(void);
void 			SirenBoard_SaveData(uint8_t *dataBuf);
void 			SirenBoard_BuildingPackForRS485(uint8_t *dataBuf);
void 		  SirenBoard_ClrData(void);
SBData_t* SirenBoard(void);
void      SirenBoard_Set(uint8_t siren, uint8_t cmd);

//*****************************************************************************
//*****************************************************************************
#endif /*_SB_SirenBoard_H*/




