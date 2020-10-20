
#ifndef _SB_SirenBoard_H
#define _SB_SirenBoard_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

//--------------------
//AppLayer.
#include "Rs485.h"
//-----------------------------------------------------------------------------
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef union{
 	struct {
		uint16_t SirenPowerMeas;//Напряжение питания оповещателей, в мВ.
		uint16_t Siren1Meas;    //Напряжение на оповещателе 1, в мВ.
		uint16_t Siren2Meas;    //Напряжение на оповещателе 2, в мВ.
		uint16_t Siren3Meas;    //Напряжение на оповещателе 3, в мВ.
		uint8_t  Siren1Status;	//
		uint8_t  Siren2Status;
		uint8_t  Siren3Status;
	}STR;
	uint8_t BLK[11];		
}Siren_t;
#pragma pack(pop)//вернули предыдущую настройку.

//Структура ответа платы управления оповещателями на запрос от ЦП.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef union{
	struct{
    //--------------------
    uint8_t  SinhroByte1;  // 0-й байт  - Синхробайт.
    uint8_t  SinhroByte2;  // 1-й байт  - Синхробайт.
		uint8_t  Type;         // 2-й байт  - Тип модуля.
    uint8_t  Size;         // 3-й байт  - Размер пакета.    
    uint8_t  CmdCode;      // 4-й байт  - Код команды.
    //--------------------
		uint16_t SirenPowerMeas;//Напряжение питания оповещателей, в мВ.
		uint16_t Siren1Meas;    //Напряжение на оповещателе 1, в мВ.
		uint16_t Siren2Meas;    //Напряжение на оповещателе 2, в мВ.
		uint16_t Siren3Meas;    //Напряжение на оповещателе 3, в мВ.
		uint8_t  Siren1Status;	//
		uint8_t  Siren2Status;
		uint8_t  Siren3Status;
    //--------------------
    uint16_t  CRC16;       //два байта CRC.
    //--------------------
		}STR;
	uint8_t BLK[18];
}SBDataPack_t;
#pragma pack(pop)//вернули предыдущую настройку.
//--------------------
//Команды платы управления оповещателями(Siren Board - SB).
#define SB_TypeConst			 (uint8_t)0x05
#define SB_CmdStatusConst	 (uint8_t)0x01  //Команда "Выдача статуса от SB".
#define SB_CmdControlConst (uint8_t)0x02  //Команда "Активация извещателя".
#define SB_NoConnection    	0xffff 				//признак отсутствия обмена данни с платой SB.
//--------------------
//Режимы работы выхода.
#define SirenOutMask			 0x0F
#define SirenOutOFF				(0<<0)	//Выход отключен.
#define SirenOutON		  	(1<<0)	//Выход активен.
#define SirenOutBlink1 		(2<<0)	//Режим МИГАНИЕ-1. 100мС - ON, 900мС - OFF.
#define SirenOutBlink2 		(3<<0)	//Режим МИГАНИЕ-2. 500мС - ON, 500мС - OFF.
//--------------------
//Состояния линий оповещателей.
#define SirenLineMask		   0xF0
#define SirenLineNorm     (1<<4)
#define SirenLineBreak    (2<<4)
#define SirenLineShort    (3<<4)
//--------------------
#define SirenStatus			 0
#define Siren1				   1
#define Siren2				   2
#define Siren3				   3
#define SirenAll			   4
//-----------------------------------------------------------------------------
//SBDataUNION_Type* SirenDataBuf(void);

void 		 SirenSaveData(uint8_t *Buf);
void 		 SirenClrData (void);
Siren_t* SirenGetData (void);
void     SirenSet     (uint8_t siren, uint8_t cmd);
SBDataPack_t*    SirenRS485TxBuf(void);
//-----------------------------------------------------------------------------
#endif /*_SB_SirenBoard_H*/




