
#ifndef _Rs485_H
#define _Rs485_H
//-----------------------------------------------------------------------------

#include "uart_ST.h"
#include "dma_ST.h"
#include "crc16.h"

//--------------------
//Boards.
#include "MB_MotherBoard.h"
#include "FP_FacePanel.h"
#include "SB_SirenBoard.h"
//*******************************************************************************************************
//***************************Материнская плата.**********************************************************
//Структура данных ЦП.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
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
}MBData_t;
#pragma pack(pop)//вернули предыдущую настройку.
//*******************************************************************************************************
//***************************Лицевая панелью*************************************************************
//Структура ответа лицевой панели на запрос от ЦП.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef	struct{
	//--------------------
	uint8_t  KeyAndMicState; //5-й байт  - Положением электронного замка и состояние микрофона.
	uint8_t  Address;        //6-й байт  - Адресс блока.        
	uint8_t  Group;          //7-й байт  - Группа блока.  
	uint8_t  Control_Buttons;//8-й байт  - Состояние кнопок управления.
	uint8_t  Alg_Buttons;    //9-й байт  - Состояние кнопок АЛГОРИТМЫ ОПОВЕЩЕНИЯ.
	uint32_t TimeUTC;        //10-й байт - Время в формате UTC.
	//--------------------
}FPData_t;
#pragma pack(pop)//вернули предыдущую настройку.
//*******************************************************************************************************
//***************************Плата управление оповещателями.*********************************************
//Структура ответа платы управления оповещателями на запрос от ЦП.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef	struct{
	//--------------------
	uint16_t SirenPowerMeas;//Напряжение питания оповещателей, в мВ.
	uint16_t Siren1Meas;    //Напряжение на оповещателе 1, в мВ.
	uint16_t Siren2Meas;    //Напряжение на оповещателе 2, в мВ.
	uint16_t Siren3Meas;    //Напряжение на оповещателе 3, в мВ.
	uint8_t  Siren1Status;	//
	uint8_t  Siren2Status;
	uint8_t  Siren3Status;
	//--------------------
}SBData_t;
#pragma pack(pop)//вернули предыдущую настройку.
//*******************************************************************************************************
//***************************Плата IP.*******************************************************************
//Структура данных платы IP.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
	//--------------------
	uint8_t   CmdCode;    //4-й байт  - Код команды.
	uint8_t   Reserv;     //5-й байт  - резерв.
	//--------------------
	union
		{
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
	uint16_t  CRC16;       //два байта CRC.
	//----------------------------------------
}IPData_t;
#pragma pack(pop)//вернули предыдущую настройку.
//----------------------------------------
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
//*******************************************************************************************************
//***************************Плата расширения зон.*******************************************************
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
	//-----------
	uint8_t   StatusZone1;    
	uint8_t   StatusZone2;    
	uint16_t  DebugData1;     
	uint16_t  DebugData2;     
	//-----------
}ZEData_t;
#pragma pack(pop)//вернули предыдущую настройку
//----------------------------------------
//Команды платы расширения зон(zone expander - ZE).
#define ZE_TypeConst             0x03 // Тип модуля - плата расширения зон.

#define CmdZEStatusConst         0x21 //Команда "Выдача статуса ZE".
#define CmdZEControlConst        0x22 //Команда "Управление ZE".
//----------------------------------------
//Состояния линии.
#define ZE_LineBreak     1 //(1<<0)
#define ZE_LineAbove     2 //(1<<1)
#define ZE_LineNorm      3 //(1<<2)
#define ZE_LineBelow     4 //(1<<3)
#define ZE_LineShort     5 //(1<<4)
#define ZE_LineUndef     6 //(1<<5) //Состояние не определено - идет переходной процесс.
//*******************************************************************************************************
//***************************Структуры и праметры для работы протокола.**********************************
#define RS485_BUF_SIZE         64
#define RS485_BUF_HEADER_SIZE  5
//буфер для работы протокола.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
	//Заголовок пакета
	uint8_t SinhroByte1;//Синхробайт.
	uint8_t SinhroByte2;//Синхробайт.
	uint8_t Type;       //Тип модуля.
	uint8_t Size;       //Размер пакета.
	uint8_t CmdCode;    //Код команды.	
	//Данные.
	union
		{
			uint8_t Buf[RS485_BUF_SIZE];//буфер.
			//----------
			struct
				{
					FPData_t Status;//данные FP.
					uint16_t CRC16;
				}FP;
			//----------
			struct
				{
					SBData_t Status;//данные FP.
					uint16_t CRC16;
				}SB;
			//----------
		}Data;
	//-----------
}RS485BufUnion_t;
#pragma pack(pop)//вернули предыдущую настройку.
//*********************************************************************************
//*********************************************************************************
//-----------------------------------------------------------------------------
//Счетчика для отсчета таймаутов ответа от плат.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
	//-----------
	uint16_t CountTimeOutProtocol;
	uint8_t  CountTimeOutAckFP;
	uint8_t  CountTimeOutAckSB;
	uint8_t  CountTimeOutAckIP;
	uint8_t  CountTimeOutAckZE;
	//-----------
}RS485TimeOut_t;
#pragma pack(pop)//вернули предыдущую настройку.
//*******************************************************************************************************
//*******************************************************************************************************
//Настраиваемые параметры для работы протокола.
#define RS485Baudrate 		 	115200UL
#define RS485RequestPeriod 	25 		 			 //Период между запросов протокола, в мС.
#define RS485NAckTimeOut	 	250 				 //Таймаут приема пакета от плат, в мС.
typedef RS485BufUnion_t     RS485RxBuf_t;//тип буфер приёма.
//--------------------
//APB1 тактируется 36МГц.
#define APBCLK 							36000000UL//72000000UL
#define USART3BRR_Const   	((APBCLK + RS485Baudrate / 2) / RS485Baudrate)
#define RS485DirectTX()			(GPIOC->BSRR = GPIO_BSRR_BS6)//Управление микросхемой драйвера RS485. 
#define RS485DirectRX()  		(GPIOC->BSRR = GPIO_BSRR_BR6)
//--------------------
#define SinhroByte1Const 	 	0x72 	// Значение синхробайта.
#define SinhroByte2Const  	0xA5 	// Значение синхробайта.
//--------------------
//
#define SEND_MB_STATUS_STEP     0
#define REQUEST_SB_STATUS_STEP	1
#define REQUEST_IP_STATUS_STEP	2
#define REQUEST_ZE_STATUS_STEP	3
//--------------------
//Флаги.
typedef struct{
	
	uint16_t TimeOutFlag   :1;
	uint16_t MBNewDataFlag :1;
	uint16_t FPNewDataFlag :1;
	uint16_t ZENewDataFlag :1;
	uint16_t IPNewDataFlag :1;
	uint16_t SBNewDataFlag :1;
	
	uint16_t FPNAckFlag :1;
	uint16_t SBNAckFlag :1;
	uint16_t IPNAckFlag :1;
	uint16_t ZENAckFlag :1;
	uint16_t :6;
	
}RS485Flags_t;
#define FlagClr	0
#define FlagSet	1
//*********************************************************************************
//*********************************************************************************
//Прикладной уровень. Общие функции.
void 		      RS485Init       (void);
void          RS485TimeOutInc (void);//Отсчет таймаута приема пакета.
RS485Flags_t* RS485Flags      (void);
void					RS485DirectionTX(void);//Микросхема на передачу
void					RS485DirectionRX(void);//Микросхема на прием

//Ф-и для работы с приемом данных.
uint8_t       RS485RxPacket(uint8_t data);//Прием пакета.
void          RS485RxClrBuf(void);        //Сброс приемного буфера.
RS485RxBuf_t* RS485RxBuf(void);				   //Указатель на приемный буфер.

//Ф-и для работы с передачей данных.
void RS485StartTx(uint8_t* buf, uint16_t size);

//Ф-и для работы ЦП.
void RS485MasterLoop(void);//Автомат состояний, определяющий порядок выдачи кодограмм.

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#endif /*_Rs485_H*/


