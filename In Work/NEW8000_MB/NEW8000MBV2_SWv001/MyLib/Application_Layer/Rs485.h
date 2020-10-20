
#ifndef _Rs485_H
#define _Rs485_H
//-----------------------------------------------------------------------------

#include "uart_ST.h"

#include "crc16.h"

#include "adc_ST.h"
#include "rtc_ST.h"
#include "gpio_ST.h"

#include "LineCheck.h"
#include "Relay.h"
#include "Eep.h"
#include "Buttons.h"
#include "Amplifier.h"
#include "PowerAndCharge.h"

#include "SB_SirenBoard.h"
//-----------------------------------------------------------------------------
//Управление микросхемой драйвера RS485.
#define RS485DEOn         (GPIOC->BSRR = GPIO_BSRR_BS6) 
#define RS485REOn         (GPIOC->BSRR = GPIO_BSRR_BR6)

#define DE    1
#define RE    0
//--------------------
#define RS485ProtocolLoopPeriod   20 //Период запросов протокола, в мС.

#define SinhroByte1Const        0x72 // Значение синхробайта.
#define SinhroByte2Const        0xA5 // Значение синхробайта.

//Флаги.
#define RS485_MBNewDataFlag  (1<<0)//
#define RS485_MBErrorFlag    (1<<1)//

#define RS485_TimeOutFlag    (1<<2)//

#define RS485_FPNewDataFlag  (1<<3)//
#define RS485_FPErrorFlag    (1<<4)//

#define RS485_ZENewDataFlag  (1<<5)//
#define RS485_ZEWaitRespFlag (1<<6)//
#define RS485_ZEErrorFlag    (1<<7)//

#define RS485_IPNewDataFlag  (1<<8)//
#define RS485_IPWaitRespFlag (1<<9)//
#define RS485_IPErrorFlag    (1<<10)//

#define RS485_SBNewDataFlag  (1<<11)//
#define RS485_SBWaitRespFlag (1<<12)//
#define RS485_SBErrorFlag    (1<<13)//

#define RS485_AllFlags        0xFFFF
//-----------------------------------------------------------------------------
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
		//Состояния оповещателей.
		uint16_t SirenPower;	 
		uint16_t Siren1Status; 
		uint16_t Siren2Status; 
		uint16_t Siren3Status; 
    //--------------------
		//Состояния платы IP.		

    //--------------------
		//Состояния платы IP.		
		
		//--------------------	
		uint16_t CRC16;	
    //uint8_t CRC16_H;  // Конрольная сумма.
    //uint8_t CRC16_L;
    //----------------------------------------
		  }STR;
	uint8_t BLK[40];
}MB_DataUNION_Type;
#pragma pack(pop)//вернули предыдущую настройку.
//--------------------
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

#define SirenPowerPoint   			30 //30 - 31 
#define Siren1StatusPoint  			32 
#define Siren2StatusPoint  			33
#define Siren3StatusPoint  			34

#define CRC16_HPoint            35
#define CRC16_LPoint            36 
//--------------------
//Команды MB.
#define MB_DefultAddressConst    0x01 //Значение адреса материнской платы по умолчанию.
#define MB_TypeConst             0x01 //Тип модуля - материнская плата.

#define CmdMBStatusConst         0xA0 //Команда "Выдача статуса от MB".
#define CmdBroadcastRequestConst 0xFF //Широковещательный запрос.

#define PRIORITY_6	    6
#define PRIORITY_5	    5
#define PRIORITY_4	    4
#define PRIORITY_3	    3
#define PRIORITY_2	    2
#define PRIORITY_1	    1
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Структура ответа лицевой панели на запрос от ЦП.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef union {
	struct{
    //----------------------------------------
    uint8_t   SinhroByte1;    //0-й байт  - Синхробайт.
    uint8_t   SinhroByte2;    //1-й байт  - Синхробайт.
		uint8_t   Type;           //2-й байт  - Тип модуля.
    uint8_t   Size;           //3-й байт  - Размер пакета.    
    uint8_t   Comand_Code;    //4-й байт  - Код команды.
    //--------------------
    uint8_t   KeyAndMicState; //5-й байт  - Положением электронного замка и состояние микрофона.
    uint8_t   Addres;         //6-й байт  - Адресс блока.        
    uint8_t   Group;          //7-й байт  - Группа блока.  
    uint8_t   Control_Buttons;//8-й байт  - Состояние кнопок управления.
    uint8_t   Preset_Buttons; //9-й байт  - Состояние кнопок предустановок.
    uint32_t  TimeUTC;        //10-й байт - Время в формате UTC.
    //--------------------
    uint16_t  CRC16;          //два байта CRC.
    //----------------------------------------
		  }STR;
	uint8_t BLK[16];
}FP_DataUNION_Type;
#pragma pack(pop)//вернули предыдущую настройку.

//Номер байта в пакете, передаваемые лицевой панелью(FP).
#define FP_KeyAndMicStatePoint   5 
#define FP_AddressPoint          6 
#define FP_GroupPoint            7
#define FP_ControlButtonsPoint   8
#define FP_PresetButtonsPoint    9
#define FP_TimeUTCPoint          10 //10 - 13 байты
#define FP_CRC16_HPoint          14
#define FP_CRC16_LPoint          15 

//Команды FP.
#define FP_DefultAddressConst    0x00 //Значение адреса лицевой панели по умолчанию.
#define FP_TypeConst             0x02 //Тип модуля - лицевая панель.

#define CmdFPStatusConst         0x11 //Команда "Выдача статуса от FP".
#define CmdFPSetAddressConst     0x12 //Команда установки адрес блока
#define CmdFPSetGroupConst       0x13 //Команда устновки группы блока 
#define CmdFPSetTimeConst        0x14 //Команда установики времени
#define CmdFPSetDataConst        0x15 //Команда установики времени
#define CmdFPTetsZummerConst     0x16 //Команда влючение тестирования зуммера ЦП
#define CmdFPLcSaveRefConst      0x17 //Команда сохранения опорных значений для Lc.
#define CmdFPLcSetDeviationConst 0x18 //Команда установки отклонения от установившегося значения для фиксации перех-го проц-са на линии.
//--------------------
//Режима работы блока.
#define KeyMaskConst           0x0F
#define UnitLockModeConst	    (1<<0)
#define UnitControlModeConst  (2<<0)
#define UnitCofigModeConst	  (3<<0)
#define UnitErrorConst        (4<<0)
//Состояния микрофона.
#define MicMaskConst           0xF0
#define MicConnectedConst	    (1<<4)//микрофон подключен
#define MicActiveConst	      (2<<4)//на микрофоне нажата тангента, можно говорить.
#define MicNotConnectedConst	(3<<4)//микрофон не подключен
#define MicFaultConst         (4<<4)//капсуль микрофона неисправен.      
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------




//Команды платы расширения зон(zone expander - ZE).
#define ZE_TypeConst             0x03 // Тип модуля - плата расширения зон.

#define CmdZEStatusConst         0x21 //Команда "Выдача статуса ZE".
#define CmdZEControlConst        0x22 //Команда "Управление ZE".


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Структура данных платы IP.
//typedef struct {

//	uint16_t sync;
//  uint8_t  len;
//	uint8_t  type;
//	uint8_t  cmd;
//	//-----------------//
//	uint8_t __RESERV;
//	//-----------------//
//	union{

//		struct {
//			uint8_t status_flags;
//			uint8_t playTrack;
//		}CmdIPStatus;

//		struct {
//			uint8_t playTrack;
//			uint8_t CMDresult;
//		}CmdIPStart;

//		struct {
//			uint8_t __RESERV;
//			uint8_t CMDresult;
//		}CmdIPAbort;

//	}response;
//	uint16_t crc16;
//}IPDataUNION_TypeDef;

#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef union {
	struct{
    //----------------------------------------
    uint8_t   SinhroByte1;    //0-й байт  - Синхробайт.
    uint8_t   SinhroByte2;    //1-й байт  - Синхробайт.
		uint8_t   Type;           //2-й байт  - Тип модуля.
    uint8_t   Size;           //3-й байт  - Размер пакета.    
    uint8_t   Comand_Code;    //4-й байт  - Код команды.
    //--------------------
    uint8_t   Reserv;         //5-й байт  - резерв.
    //--------------------
    union{
      struct
        {
          uint8_t status_flags;
          uint8_t playTrack;
        }CmdIPStatus;
      struct
        {
          uint8_t playTrack;
          uint8_t CMDresult;
        }CmdIPStart;
      struct
        {
          uint8_t __RESERV;
          uint8_t CMDresult;
        }CmdIPAbort;
    }Response;
    //--------------------
    uint8_t CRC16_H;  // Конрольная сумма.
    uint8_t CRC16_L;
    //----------------------------------------
		  }STR;
	uint8_t BLK[14];
}IPDataUNION_TypeDef;
#pragma pack(pop)//вернули предыдущую настройку.

//Команды IP-платы.
#define IP_TypeConst			          (uint8_t)0x04u

#define CmdIPStatusConst	          (uint8_t)0x31u  //Команда "Выдача статуса от IP".
#define CmdIPStartConst		          (uint8_t)0x32u  //Команда “Начала воспроизведения”.
#define CmdIPAbortConst		          (uint8_t)0x3fu  //Прервать выполнение команды.

#define CMDresultACK			          (uint8_t)0xccu	//Команда выполнена
#define CMDresultNACK		            (uint8_t)0xaau	//Сбой

#define CmdIPStatusFlags_IDLE	      (uint8_t)0x00u
#define CmdIPStatusFlags_BUSY	      (uint8_t)0x01u
#define CmdIPStatusFlags_PLAY	      (uint8_t)0x02u
#define CmdIPStatusFlags_SDPresent	(uint8_t)0x10u	//SD подключено
#define CmdIPStatusFlags_LANPresent	(uint8_t)0x20u	//LAN подключен
#define CmdIPStatusFlags_SYSErr		  (uint8_t)0x80u	//Внутренняя ошибка
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Структура ответа платы расширения зон на запрос от ЦП.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef union {
	struct{
    //----------------------------------------
    uint8_t   SinhroByte1;    //0-й байт  - Синхробайт.
    uint8_t   SinhroByte2;    //1-й байт  - Синхробайт.
		uint8_t   Type;           //2-й байт  - Тип модуля.
    uint8_t   Size;           //3-й байт  - Размер пакета.    
    uint8_t   Comand_Code;    //4-й байт  - Код команды.
    //--------------------
    uint8_t   StatusZone1;    //5-й байт
    uint8_t   StatusZone2;    //6-й байт
    uint16_t  DebugData1;     //7-8й байт
    uint16_t  DebugData2;     //9-10й байт
    //--------------------
    uint16_t  CRC16;          //два байта CRC.
    //----------------------------------------
		  }STR;
	uint8_t BLK[13];
}ZE_DataUNION_Type;
#pragma pack(pop)//вернули предыдущую настройку.

//Номер байта в пакете, передаваемые центральной платой (ЦП).
#define ZE_StatusZone1Point   5 
#define ZE_StatusZone2Point   6 
#define ZE_DebugData1         7
#define ZE_DebugData2         9
//Состояния линии.
#define ZE_LineBreak     1 //(1<<0)
#define ZE_LineAbove     2 //(1<<1)
#define ZE_LineNorm      3 //(1<<2)
#define ZE_LineBelow     4 //(1<<3)
#define ZE_LineShort     5 //(1<<4)
#define ZE_LineUndef     6 //(1<<5) //Состояние не определено - идет переходной процесс.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Буферa и переменные дла работы протакола.
#define RS485BufSizeMax  64

typedef union {
	struct{
    //--------------------
    uint8_t  SinhroByte1;   // Синхробайт.
    uint8_t  SinhroByte2;   // Синхробайт.
		uint8_t  Type;          // Тип модуля.
    uint8_t  Size;          // Размер пакета.
    uint8_t  CmdCode;   		// Код команды.
    //--------------------
    //Данные.
    uint8_t  Data[RS485BufSizeMax-5];         
    //--------------------
		  }STR;
	uint8_t BLK[RS485BufSizeMax];
}RS485BufUNION_Type;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Прикладной уровень. Общие функции.
void     RS485Init      (void);
void     RS485DE        (uint8_t de);//Управление микросхемой приемо-передатчика.
void     RS485TimeOutInc(void);      //Отсчет таймаута приема пакета.

uint16_t RS485FlagGet(void);         //Флаги состояний протокола.
void     RS485FlagSet(uint16_t flag);//
void     RS485FlagClr(uint16_t flag);//

void     RS485RxClrBuf(void);        //Сброс приемного буфера.
uint8_t  RS485RxPacket(uint8_t data);//Прием пакета.
uint8_t  RS485RxGetDataFromBuf(uint8_t p);//Чтение байта из приемного буфера.
RS485BufUNION_Type* RS485RxGetBufPtr(void);
uint32_t RS48RxGetTimeUTC(void);

//Ф-и для работы ЦП.
void RS485MasterLoop(void);            //Автомат состояний, определяющий порядок выдачи кодограмм.

void RS485TxStatusMB (MB_DataUNION_Type *TxBuf);
//void RS485TxRequestSB(SB_DataUNION_Type *TxBuf);


void RS485TxRequestIP(RS485BufUNION_Type *TxBuf);
void RS485TxRequestZE(RS485BufUNION_Type *TxBuf);

void RS485MB_SetPriority (uint8_t stat);   //Установка приоритета блока.
RS485BufUNION_Type* RS485MB_SetTxBuf(void);//Запись данных в передающий буфер.

void    SetAddress(uint8_t adr);
uint8_t GetAddress(void);
void    SetGroup(uint8_t grp);
uint8_t GetGroup(void);
void    SetPriority(uint8_t prt);
uint8_t GetPriority(void);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#endif /*_Rs485_H*/


