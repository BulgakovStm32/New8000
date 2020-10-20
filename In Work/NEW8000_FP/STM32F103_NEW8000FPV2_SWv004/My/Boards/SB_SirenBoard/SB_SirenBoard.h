
#ifndef _SB_SirenBoard_H
#define _SB_SirenBoard_H
//-----------------------------------------------------------------------------

#include "MB_MotherBoard.h"

//-----------------------------------------------------------------------------
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
	//--------------------
	uint16_t PowerVol;//Напряжение питания оповещателей, в мВ.
	Line_t	 Siren1;
	Line_t   Siren2;
	Line_t   Siren3;
	//--------------------	
}SBData_t;
#pragma pack(pop)//вернули предыдущую настройку.
//--------------------
//Режимы работы выхода.
#define SIREN_OUT_OFF			0	//Выход отключен.
#define SIREN_OUT_ON		 	1	//Выход активен.
#define SIREN_OUT_Blink1 	2	//Режим МИГАНИЕ-1. 100мС - ON, 900мС - OFF.
#define SIREN_OUT_Blink2 	3	//Режим МИГАНИЕ-2. 500мС - ON, 500мС - OFF.
//--------------------
//Состояния шлейфов.
#define SIREN_LINE_UNDEF  0
#define SIREN_LINE_NORM   1
#define SIREN_LINE_BREAK  2
#define SIREN_LINE_SHORT  3
//-----------------------------------------------------------------------------
void      Siren_SaveData(uint8_t* buf);
SBData_t* Siren(void);
//-----------------------------------------------------------------------------
#endif /*_SB_SirenBoard_H*/

