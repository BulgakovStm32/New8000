
#ifndef _MB_MotherBoard_H
#define _MB_MotherBoard_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

//--------------------
//AppLayer.
#include "Rs485.h"
#include "LineCheck.h"
#include "PojLine.h"
#include "Relay.h"
#include "Eep.h"
#include "Buttons.h"
#include "Amplifier.h"
#include "PowerAndCharge.h"
#include "Time.h"
//-----------------------------------------------------------------------------
//Структура данны от ЦП.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef union {
	struct{
    //----------------------------------------
    uint8_t  SinhroByte1;// Синхробайт.
    uint8_t  SinhroByte2;// Синхробайт.
    uint8_t  Type;			 // Тип модуля.
    uint8_t  Size;       // Размер пакета.   
    uint8_t  CmdCode;    // Код команды.
    //--------------------
    uint8_t  Addres;         
    uint8_t  Group;
    uint8_t  Priority;
    uint8_t  StatusZone1;
    uint8_t  StatusZone2;
    uint8_t  StatusZone3;
    uint8_t  StatusZone4;
    uint8_t  StatusZone5;
    uint8_t  StatusZone6;
    uint8_t  StatusPoj1;
    uint8_t  StatusPoj2;
    uint8_t  StatusPoj3;
    uint8_t  StatusGOChS;
    uint8_t  StatusPOWER;
    uint8_t  StatusPA;
    uint32_t TimeUTC;
    uint16_t DebugData1;
    uint16_t DebugData2;
    uint16_t LcDeviationVolue;
		//--------------------	
		//Состояние журнала событий.
		uint16_t TotalEvents;
		uint16_t NotReadEvents;
		//--------------------
		//Состояния оповещателей.
		uint16_t SirenPower;	 
		uint16_t Siren1Status; 
		uint16_t Siren2Status; 
		uint16_t Siren3Status; 
    //--------------------
		//Состояния платы IP.		

    //--------------------
		//Состояния платы ZE.		
		
		//--------------------
		uint16_t CRC16;
    //----------------------------------------
		  }STR;
	uint8_t BLK[44];
}MBDataUNION_Type;
#pragma pack(pop)//вернули предыдущую настройку.
//-----------------------------------------------------------------------------
//Номер байта в шапке пакета.
#define SinhroByte1Point        0   
#define SinhroByte2Point        1  
#define TypePoint               2 // Тип модуля.
#define SizePoint               3 // Размер пакета.
#define CommandCodePoint        4 // Код команды.
//Номер байта в пакете, передаваемые центральной платой (ЦП).
#define AddresPoint             5 
#define GroupPoint              6
#define PriorityPoint           7
#define StatusZone1Point        8
#define StatusZone2Point        9
#define StatusZone3Point        10
#define StatusZone4Point        11
#define StatusZone5Point        12
#define StatusZone6Point        13
#define StatusPoj1Point         14
#define StatusPoj2Point         15
#define StatusPoj3Point         16
#define StatusGOChSPoint        17
#define StatusPOWERPoint        18
#define StatusPAPoint           19
#define TimeUTCPoint            20 //20 - 23 байт
#define DebugData1Point					24 //24 - 25 байт
#define DebugData2Point					26 //26 - 27 байт
#define LcDeviationVoluePoint		28 //28 - 29 байт

#define TotalEventsPoint   			30 //30 - 31 
#define NotReadEventsPoint  		32 //32 - 33 

#define SirenPowerPoint   			34 //34 - 35 
#define Siren1StatusPoint  			36 //36 - 37
#define Siren2StatusPoint  			38 //38 - 39
#define Siren3StatusPoint  			40 //40 - 41

#define CRC16_HPoint            42
#define CRC16_LPoint            43 
//-----------------------------------------------------------------------------
//Команды MB.
#define MB_DefultAddressConst    0x01 //Значение адреса материнской платы по умолчанию.
#define MB_TypeConst             0x01 //Тип модуля - материнская плата.

#define CmdMBStatusConst         0xA0 //Команда "Выдача статуса от MB".
#define CmdBroadcastRequestConst 0xFF //Широковещательный запрос.
//----------------------------------------
#define StandbyMode			0		//Дежурный режим.
#define AttentionMode		1		//Режим ВНИМАНИЕ	
#define PuskMode		    2		//Режим ПУСК	

#define Priority_1	    1
#define Priority_2	    2
#define Priority_3	    3
#define Priority_4	    4
#define Priority_5	    5
#define Priority_6	    6

#define PUSK_Activation  7
#define START_Activation 8
//*****************************************************************************
//*****************************************************************************
MBDataUNION_Type* MotherBoardRS485TxBuf(void);

void    SetAddress(uint8_t adr);
uint8_t GetAddress(void);
void    SetGroup(uint8_t grp);
uint8_t GetGroup(void);
void    SetPriority(uint8_t prt);
uint8_t GetPriority(void);
//*****************************************************************************
//*****************************************************************************
#endif /*_MB_MotherBoard_H*/



