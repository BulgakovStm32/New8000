
#ifndef _uart_ST_H
#define _uart_ST_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "crc16.h"

#include "adc_ST.h"
#include "rtc_ST.h"

#include "LineCheck.h"
#include "Relay.h"
#include "Eep.h"
#include "Buttons.h"

//-----------------------------------------------------------------------------
//Управление светодиодом ACT. 
#define RS485ActLedOn     (GPIOB->BSRR = GPIO_BSRR_BS10) 
#define RS485ActLedOff    (GPIOB->BSRR = GPIO_BSRR_BR10) 
#define RS485ActLedToggel (GPIOB->ODR  = GPIOB->ODR ^ GPIO_ODR_ODR10)
//--------------------
//Управление микросхемой драйвера RS485.
#define RS485DEOn         (GPIOA->BSRR = GPIO_BSRR_BS12) 
#define RS485REOn         (GPIOA->BSRR = GPIO_BSRR_BR12)

#define DE    1
#define RE    0
//--------------------
//Флаги.
#define RS485_MBNewDataFlag (1<<0)//
#define RS485_MBErrorFlag   (1<<1)//
#define RS485_FPNewDataFlag (1<<2)//
#define RS485_FPErrorFlag   (1<<3)//
#define RS485_TimeOutFlag   (1<<4)//

//--------------------
#define RS485ProtocolLoopPeriod 25 //Период запросов протокола, в мС.

#define SinhroByte1Const        0x72 // Значение синхробайта.
#define SinhroByte2Const        0xA5 // Значение синхробайта.
//--------------------
//Номер байта в шапке пакета.
#define SinhroByte1Point        0   
#define SinhroByte2Point        1  
#define SizePoint               2 // Размер пакета.
#define TypePoint               3 // Тип модуля.
#define Comand_CodePoint        4 // Код команды.
//--------------------
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
#define CRC16_HPoint            24
#define CRC16_LPoint            25 
//-----------------------------------------------------------------------------
#define MB_DefultAddressConst      0x01 // Значение адреса материнской платы по умолчанию.
#define MB_TypeConst               0x01 // Тип модуля - материнская плата.

#define FP_DefultAddressConst      0x00 // Значение адреса лицевой панели по умолчанию.
#define FP_TypeConst               0x02 // Тип модуля - лицевая панель.


#define CmdBroadcastRequestConst   0xFF // Широковещательный запрос.
//--------------------
//Команды MB.
#define CmdFromMBStatusConst     0xA0 // Команда "Выдача статуса от MB".

#define PRIORITY_6	    6
#define PRIORITY_5	    5
#define PRIORITY_4	    4
#define PRIORITY_3	    3
#define PRIORITY_2	    2
#define PRIORITY_1	    1
//--------------------
//Команды FP.
#define CmdFromFPStatusConst     0x11 //Команда "Выдача статуса от FP".
#define CmdFromFPSetAddressConst 0x12 //Команда установки адрес блока
#define CmdFromFPSetGroupConst   0x13 //Команда устновки группы блока 
#define CmdFromFPSetTimeConst    0x14 //Команда установики времени 
#define CmdFromFPTetsZummerConst 0x15 //Команда влючение тестирования зуммера ЦП
                         
//--------------------
//Команды IP-платы.
#define CmdRetStatIPConst        0xA2 // Команда "Выдача статуса от IP".

//--------------------
//#define TimeUTCPointer 20
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Структура данны от ЦП.
//#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef union {
	struct{
    //----------------------------------------
    uint8_t  SinhroByte1;   // Синхробайт.
    uint8_t  SinhroByte2;   // Синхробайт.
    uint8_t  Size;          // Размер пакета.   
    uint8_t  Type;
    uint8_t  CommandCode;    // Код команды.
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
    //uint16_t Debug;
    //--------------------
    //uint16_t CRC16;
    uint8_t CRC16_H;  // Конрольная сумма.
    uint8_t CRC16_L;
    //----------------------------------------
		  }STR;
	uint8_t BLK[26];
}MBDataUNION_TypeDef;
//#pragma pack(pop)//вернули предыдущую настройку.
//-----------------------------------------------------------------------------
//Структура ответа лицевой панели на запрос от ЦП.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef union {
	struct{
    //----------------------------------------
    uint8_t   SinhroByte1;    //0-й байт  - Синхробайт.
    uint8_t   SinhroByte2;    //1-й байт  - Синхробайт.
    uint8_t   Size;           //2-й байт  - Размер пакета.    
    uint8_t   Type;           //3-й байт  - Тип модуля.
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
}FPDataUNION_TypeDef;
#pragma pack(pop)//вернули предыдущую настройку.

//Номер байта в пакете, передаваемые центральной платой (ЦП).
#define FP_KeyAndMicStatePoint   5 
#define FP_AddresPoint           6 
#define FP_GroupPoint            7
#define FP_ControlButtonsPoint   8
#define FP_PresetButtonsPoint    9
#define FP_TimeUTCPoint          10 //10 - 13 байты
#define FP_CRC16_HPoint          14
#define FP_CRC16_LPoint          15 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Аппатартный уровень.
void  Uart1Init  (void);
void  Uart1StarTx(uint8_t *TxBuf, uint8_t size);
//void USART1_IRQHandler(void);       //Прерывание от USART1.
//------------------------
//Прикладной уровень. Общие функции.
void     RS485DE        (uint8_t de);//Управление микросхемой приемо-передатчика.
void     RS485TimeOutInc(void);      //Отсчет таймаута приема пакета.

uint16_t RS485FlagGet(void);         //Флаги состояний протокола.
void     RS485FlagSet(uint16_t flag);//
void     RS485FlagClr(uint16_t flag);//

void     RS485RxClrBuf(void);        //Сброс приемного буфера.
uint8_t  RS485RxPacket(uint8_t data);//Прием пакета.
uint8_t  RS485RxGetDataFromBuf(uint8_t p);//Чтение байта из приемного буфера.

//Ф-и для работы ЦП.
void     RS485MB_ProtocolLoop(void);//Автомат состояний, определяющий порядок выдачи кодограмм.

//Ф-и для работы FP.
volatile FPDataUNION_TypeDef* RS485FP_SetParam        (void);
void                          RS485FP_TxStarResponse  (void);
uint32_t                      RS485FP_GetTimeUTCFromMB(void);

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

















