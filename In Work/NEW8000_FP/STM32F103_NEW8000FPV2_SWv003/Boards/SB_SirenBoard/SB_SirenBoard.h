
#ifndef _SB_SirenBoard_H
#define _SB_SirenBoard_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "Rs485.h"
//#include "MB_MotherBoard.h"
//-----------------------------------------------------------------------------
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
	uint16_t  SirenPower; 
	uint16_t  Siren1;
	uint16_t  Siren2;
	uint16_t  Siren3;
}SirenData_STR;
#pragma pack(pop)//вернули предыдущую настройку.
//--------------------
//Режимы работы выхода.
#define SirenOutMask			 0x000F
#define SirenOutOFF				(0<<0)	//Выход отключен.
#define SirenOutON		  	(1<<0)	//Выход активен.
#define SirenOutBlink1 		(2<<0)	//Режим МИГАНИЕ-1. 100мС - ON, 900мС - OFF.
#define SirenOutBlink2 		(3<<0)	//Режим МИГАНИЕ-2. 500мС - ON, 500мС - OFF.
//--------------------
//Состояния линий оповещателей.
#define SirenLineMask		   0x00F0
#define SirenLineNorm     ((1<<4) & SirenLineMask)
#define SirenLineBreak    ((2<<4) & SirenLineMask)
#define SirenLineShort    ((3<<4) & SirenLineMask)
//-----------------------------------------------------------------------------
SirenData_STR* Siren(void);
//-----------------------------------------------------------------------------
#endif /*_SB_SirenBoard_H*/

