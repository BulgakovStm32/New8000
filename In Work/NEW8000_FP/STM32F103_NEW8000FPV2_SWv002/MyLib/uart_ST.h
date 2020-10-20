
#ifndef _uart_ST_H
#define _uart_ST_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "crc16.h"
#include "sm1628.h"

#include "LineCheck.h"
#include "Relay.h"
#include "lcd12864.h"

#include "RTOS.h"
//-----------------------------------------------------------------------------
//Управление микросхемой драйвера RS485.
#define RS485DEOn  (GPIOC->BSRR = GPIO_BSRR_BS14) 
#define RS485REOn  (GPIOC->BSRR = GPIO_BSRR_BR14) 

#define Tx    1
#define Rx    2
//--------------------
//Флаги.
#define RS485_MBNewDataFlag  (1<<0)//
#define RS485_MBErrorFlag    (1<<1)//

#define RS485_TimeOutFlag    (1<<2)//

#define RS485_AllFlags        0xFFFF
//--------------------
#define SinhroByte1Const        0x72 // Значение синхробайта.
#define SinhroByte2Const        0xA5 // Значение синхробайта.
//--------------------
////Номер байта в шапке пакета.
//#define SinhroByte1Point        0   
//#define SinhroByte2Point        1  
//#define TypePoint               2 // Тип модуля.
//#define SizePoint               3 // Размер пакета.
//#define Comand_CodePoint        4 // Код команды.
////--------------------
////Номер байта в пакете, передаваемые центральной платой (ЦП).
//#define AddresPoint             5 
//#define GroupPoint              6
//#define PriorityPoint           7
//#define StatusZone1Point        8
//#define StatusZone2Point        9
//#define StatusZone3Point        10
//#define StatusZone4Point        11
//#define StatusZone5Point        12
//#define StatusZone6Point        13
//#define StatusPoj1Point         14
//#define StatusPoj2Point         15
//#define StatusPoj3Point         16
//#define StatusGOChSPoint        17
//#define StatusPOWERPoint        18
//#define StatusPAPoint           19
//#define TimeUTCPoint            20 //20 - 23 байт
//#define DebugData1LPoint        24 
//#define DebugData1HPoint        25
//#define DebugData2LPoint        26 
//#define DebugData2HPoint        27
//#define LcDeviationVolueLPoint  28
//#define LcDeviationVolueHPoint  29
//#define CRC16_HPoint            30
//#define CRC16_LPoint            31 
//-----------------------------------------------------------------------------
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
//--------------------
//Команды FP.
#define FP_DefultAddressConst    0x00 //Значение адреса лицевой панели по умолчанию.
#define FP_TypeConst             0x02 //Тип модуля - лицевая панель.

#define CmdFPStatusConst         0x11 //Команда "Выдача статуса от FP".
#define CmdFPSetAddressConst     0x12 //Команда установки адрес блока
#define CmdFPSetGroupConst       0x13 //Команда устновки группы блока 
#define CmdFPSetTimeConst        0x14 //Команда установики времени
#define CmdFPSetDataConst        0x15 //Команда установики даты
#define CmdFPTetsZummerConst     0x16 //Команда влючение тестирования зуммера ЦП
#define CmdFPLcSaveRefConst      0x17 //Команда сохранения опорных значений для Lc.
#define CmdFPLcSetDeviationConst 0x18 //Команда установки отклонения от установившегося значения для фиксации перех-го проц-са на линии.     

#define Debug1                   DebugData1LPoint  
#define Debug2                   DebugData2LPoint      
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
		//Состояния платы ZE.		
		
		//--------------------
		uint16_t CRC16;
    //----------------------------------------
		  }STR;
	uint8_t BLK[40];
}MB_DataUNION_Type;
#pragma pack(pop)//вернули предыдущую настройку.
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
    uint8_t   Command_Code;   //4-й байт  - Код команды.
    //--------------------
    uint8_t   KeyAndMicState; //5-й байт  - Положением электронного замка и состояние микрофона.
    uint8_t   Address;        //6-й байт  - Адресс блока.        
    uint8_t   Group;          //7-й байт  - Группа блока.  
    uint8_t   Control_Buttons;//8-й байт  - Состояние кнопок управления.
    uint8_t   Preset_Buttons; //9-й байт  - Состояние кнопок предустановок.
    uint32_t  TimeUTC;        //10-й байт - Время в формате UTC.
    //--------------------
    uint16_t  CRC16;          //два байта CRC.
    //----------------------------------------
		  }STR;
	uint8_t BLK[16];
}FPDataUNION_Type;
#pragma pack(pop)//вернули предыдущую настройку.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Аппатартный уровень.
void  Uart1Init  (void);
void  Uart1StarTx(uint8_t *TxBuf, uint8_t size);
//void USART1_IRQHandler(void);       //Прерывание от USART1.
//------------------------
//Прикладной уровень. Общие функции.
void     RS485Direction (uint8_t dir);//Управление микросхемой приемо-передатчика.
void     RS485TimeOutInc(void);       //Отсчет таймаута приема пакета.

uint16_t RS485FlagGet(void);         //Флаги состояний протокола.
void     RS485FlagSet(uint16_t flag);//
void     RS485FlagClr(uint16_t flag);//

void     				   RS485RxClrBuf(void);        //Сброс приемного буфера.
uint8_t  				   RS485RxPacket(uint8_t data);//Прием пакета.
MB_DataUNION_Type* RS485RxBuf   (void);        //Получение указателя на байт в вприемном буфере.
uint8_t  				   RS485RxGetDataFromBuf(uint8_t p);//Чтение байта из приемного буфера.

//Ф-и для работы FP.
FPDataUNION_Type* RS485FP_TxBufSet       (void);
void              RS485FP_TxStartResponse(void);
void              RS485FP_TxEnd          (void);

//uint32_t  RS485FP_GetTimeUTCFromMB(void);
uint8_t   RS485FP_GetZoneFromMB   (uint8_t ch);
uint16_t  RS485FP_GetDebugFromMB  (uint8_t ch);


//------------------------
//Работа с DMA. Не отлажено!!!
void    DMA1Ch4InitForTX (void);
void    DMA1Ch5InitForRX (void);
void    StartDMA1Ch4ForTX(uint8_t *BuffAdr, uint16_t LengthBufer);
void    StartDMA1Ch5ForRX(uint8_t *BuffAdr, uint16_t LengthBufer);
uint8_t GetStateDMA1Ch4  (void);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#endif 

















