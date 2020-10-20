
#ifndef _SB_SirenBoard_H
#define _SB_SirenBoard_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "Rs485.h"

//-----------------------------------------------------------------------------
typedef struct {
	uint16_t SirenPower;
	uint16_t Siren1Status;
	uint16_t Siren2Status;
	uint16_t Siren3Status;
}SirenStatusSTR_Type;

//Структура ответа платы управления оповещателями на запрос от ЦП.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef union {
	struct{
    //--------------------
    uint8_t  SinhroByte1;  //0-й байт  - Синхробайт.
    uint8_t  SinhroByte2;  //1-й байт  - Синхробайт.
		uint8_t  Type;         //2-й байт  - Тип модуля.
    uint8_t  Size;         //3-й байт  - Размер пакета.    
    uint8_t  CmdCode;      //4-й байт  - Код команды.
    //--------------------
		uint16_t SirenPower;	 //5-6й байт
		uint16_t Siren1Status; //7-8й байт
		uint16_t Siren2Status; //9-10й байт
		uint16_t Siren3Status; //11-12й байт
    uint16_t DebugData1;   //13-14й байты
    uint16_t DebugData2;   //15-16й байты
    //--------------------
    uint16_t  CRC16;       //17-18й байты - два байта CRC.
		//--------------------
		  }STR;
	uint8_t BLK[19];
}SB_DataUNION_Type;
#pragma pack(pop)//вернули предыдущую настройку.
//Команды платы управления оповещателями(Siren Board - SB).
#define SB_TypeConst			          (uint8_t)0x05

#define SB_CmdStatusConst	          (uint8_t)0x01  //Команда "Выдача статуса от SB".
#define SB_CmdControlConst          (uint8_t)0x02  //Команда "Активация извещателя".

//Состояния линий оповещателей.
#define SB_LineBreak     1 //(1<<0)
#define SB_LineAbove     2 //(1<<1)
#define SB_LineNorm      3 //(1<<2)
#define SB_LineBelow     4 //(1<<3)
#define SB_LineShort     5 //(1<<4)
#define SB_LineUndef     6 //(1<<5) //Состояние не определено - идет переходной процесс.

//Номер байта в структуре ответа платы управления оповещателями на запрос от ЦП.
#define SB_SirenPowerPoint   	5 
#define SB_Siren1StatusPoint  7 
#define SB_Siren2StatusPoint  9
#define SB_Siren3StatusPoint  11
#define SB_DebugData1Point  	13
#define SB_DebugData2Point  	15
#define SB_CRC16_HPoint				17
#define SB_CRC16_LPoint				18

//-----------------------------------------------------------------------------
void 								 SirenSaveStatus(uint8_t *RxBuf);
void 								 SirenClrStatus (void);
SirenStatusSTR_Type* SirenGetStatus (void);


SB_DataUNION_Type*   SirenRS485TxBuf(void);
//-----------------------------------------------------------------------------
#endif /*_SB_SirenBoard_H*/




