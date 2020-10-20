
#ifndef _FP_FacePanel_H
#define _FP_FacePanel_H

//-----------------------------------------------------------------------------

#include "stm32f10x.h"

//--------------------
//AppLayer.
#include "PojLine.h"
#include "Rs485.h"
#include "Logging.h"
#include "Eep.h"
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Структура данных лицевой панели.
typedef struct{
	//uint8_t  KeyAndMicState; //Положением электронного замка и состояние микрофона.
	uint8_t  Mic;
	uint8_t  Key;
	uint8_t  Addres;         //Адресс блока.        
	uint8_t  Group;          //Группа блока.  
	uint8_t  ControlButtons; //Состояние кнопок управления.
	uint8_t  AlgButtons;     //Состояние кнопок предустановок.
	uint32_t TimeUTC;        //Время в формате UTC.
}FPDataSTR_Type;
//*********************************************************************************
//*********************************************************************************
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef	struct{
    //--------------------
	  uint8_t  Cmd_Code;
	  OneEventSTR_Type OneEvent;//Структура передачи записи журнала в FP.
    uint16_t 				 CRC16;   //два байта CRC.
    //--------------------
}FPResponseOneEvent_t;
//*********************************************************************************
//*********************************************************************************
//Структура контекста входа ШС для передачи в FP.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef	struct{
	//--------------------
	uint8_t  Cmd_Code;
	
	//FireLineContextSTR_Type ContextFireLine;//контекст входа
	uint8_t  FireLineNumber;
	uint8_t  FireLineType;  
	uint8_t  FireLineTimeOut;
	uint8_t  FireLinePhysicalState;
	uint16_t FireLineMeasValue;
	//--------------------
	uint16_t CRC16;          //два байта CRC.
	//--------------------
}FPResponseFireLineContext_t;
#pragma pack(pop)//вернули предыдущую настройку.
//*********************************************************************************
//*********************************************************************************
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef	struct{
    //--------------------
	  uint8_t  Cmd_Code;       
		EepInfoStr_t Info;
    uint16_t 		 CRC16;   //два байта CRC.
    //--------------------
}FPResponseEepInfo_t;
#pragma pack(pop)//вернули предыдущую настройку.
//*********************************************************************************
//*********************************************************************************
//Структура ответа лицевой панели на запрос от ЦП.
#define FPResponseBufSizeMax  64
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
	//Заголовок пакета
	uint8_t SinhroByte1;// Синхробайт.
	uint8_t SinhroByte2;// Синхробайт.
	uint8_t Type;       // Тип модуля.
	uint8_t Size;       // Размер пакета.
	//Данные.
	union
		{
			uint8_t Buf[FPResponseBufSizeMax];//буфер.
			//----------
			FPResponseOneEvent_t         OneEvent;//Структура передачи записи журнала в FP.
			FPResponseFireLineContext_t	 FireLine;
			FPResponseEepInfo_t          Eep;
			//----------
		}Data;
	//-----------
}FPResponse_t;
#pragma pack(pop)//вернули предыдущую настройку.
//*********************************************************************************
//*********************************************************************************
//Структура передачи записи журнала в FP.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef union {
	struct{
    //--------------------
    uint8_t  SinhroByte1;    //0-й байт - Синхробайт.
    uint8_t  SinhroByte2;    //1-й байт - Синхробайт.
		uint8_t  Type;           //2-й байт - Тип модуля.
    uint8_t  Size;           //3-й байт - Размер пакета.    
    uint8_t  Cmd_Code;       //4-й байт - Код команды.
    //----------
		OneEventSTR_Type OneEvent;
    //----------
    uint16_t CRC16;          //два байта CRC.
    //--------------------
  }STR;
	uint8_t BLK[7 + 8];
}FPLogPackUNION_Type;
#pragma pack(pop)//вернули предыдущую настройку.
//----------------------------------------
//Структура передачи контекста входа ШС в FP.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef union {
	struct{
    //--------------------
    uint8_t  SinhroByte1;    //0-й байт - Синхробайт.
    uint8_t  SinhroByte2;    //1-й байт - Синхробайт.
		uint8_t  Type;           //2-й байт - Тип модуля.
    uint8_t  Size;           //3-й байт - Размер пакета.    
    uint8_t  Cmd_Code;       //4-й байт - Код команды.
    //----------
		//FireLineContextSTR_Type ContextFireLine;//контекст входа
		uint8_t  FireLineNumber;
		uint8_t  FireLineType;  
	  uint8_t  FireLineTimeOut;
		uint8_t  FireLinePhysicalState;
		uint16_t FireLineMeasValue;
    //----------
    uint16_t CRC16;          //два байта CRC.
    //--------------------
  }STR;
	uint8_t BLK[7 + 6];
}FPFireLineContextPackUNION_Type;
#pragma pack(pop)//вернули предыдущую настройку.
//----------------------------------------
//Номер байта в пакете, передаваемые лицевой панелью(FP).
#define FP_KeyAndMicStatePoint   5 
#define FP_AddressPoint          6 
#define FP_GroupPoint            7
#define FP_ControlButtonsPoint   8
#define FP_PresetButtonsPoint    9
#define FP_TimeUTCPoint          10 //10 - 13 байты
#define FP_CRC16_HPoint          14
#define FP_CRC16_LPoint          15 
//----------------------------------------
#define FP_DefultAddressConst    0x00 //Значение адреса лицевой панели по умолчанию.
#define FP_TypeConst             0x02 //Тип модуля - лицевая панель.
//Команды FP.
#define CmdFPStatusConst         0x11 //Команда "Выдача статуса от FP".
#define CmdFPSetAddressConst     0x12 //Команда установки адрес блока
#define CmdFPSetGroupConst       0x13 //Команда устновки группы блока 
#define CmdFPSetTimeConst        0x14 //Команда установики времени
#define CmdFPSetDataConst        0x15 //Команда установики времени
#define CmdFPTetsZummerConst     0x16 //Команда влючение тестирования зуммера ЦП
#define CmdFPLcSaveRefConst      0x17 //Команда сохранения опорных значений для Lc.
#define CmdFPGetLogConst         0x18 //Команда получения одного события из журнала событий
#define CmdFPGetInputConfigConst 0x19 //Команда получени параметров одного входа пожарного шлейфа.
#define CmdFPSetInputConfigConst 0x20 //Команда сохранения параметров одного входа пожарного шлейфа.
#define CmdFPSetPowerCheckConst  0x21 //Команда сохранения контроля питания блока.

#define CmdFPGetEepromConst  		 0x22 //Команда получения информацио о EEPROM.
//----------------------------------------
//Режима работы блока.
#define KeyMaskConst          0x0F
#define UnitPojLineControl   (0<<0)
#define UnitLockModeConst	   (1<<0)
#define UnitControlModeConst (2<<0)
#define UnitCofigModeConst	 (3<<0)
#define UnitErrorConst       (4<<0)
//Состояния микрофона.
#define MicMaskConst          0xF0
#define MicConnectedConst	   (1<<4)//микрофон подключен
#define MicActiveConst	     (2<<4)//на микрофоне нажата тангента, можно говорить.
#define MicNotConnectConst	 (3<<4)//микрофон не подключен
#define MicFaultConst        (4<<4)//капсуль микрофона неисправен.   
//*****************************************************************************
//*****************************************************************************
//Данные лицевой панели
FPDataSTR_Type* FacePanel(void);

//Работа с журналом событий.
void 								 FacePanelGetLog(uint16_t numEvent);
FPLogPackUNION_Type* FacePanelTxLogPtr(void);

//Работа с логическими входами 
void 											 			 FacePanelGetPojLineContext(uint8_t input);
FPFireLineContextPackUNION_Type* FacePanelTxPojLineContextPtr(void);

//Передача информации о микросхеме EEPROM в лицевую панель.
FPResponse_t* FacePanelGetEepInfo(void);
//*****************************************************************************
//*****************************************************************************
#endif /*_FP_FacePanel_H*/


