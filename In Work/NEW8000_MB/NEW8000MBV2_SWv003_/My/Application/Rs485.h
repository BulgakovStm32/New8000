
#ifndef _Rs485_H
#define _Rs485_H

//*******************************************************************************************************
#include "uart_ST.h"
#include "dma_ST.h"
#include "crc16.h"
//*********************
//Boards.
#include "MB_MotherBoard.h"
#include "FP_FacePanel.h"
#include "SB_SirenBoard.h"
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
	uint16_t  CRC16;       
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
//*******************************************************************************************
//*******************************************************************************************
//*******************************************************************************************
//***********************Буфер для работы протокола.***************************************** 
#define RS485_BUF_HEAD_SIZE		5  //
#define RS485_BUF_DATA_SIZE		89 //
#define RS485_BUF_CRC_SIZE		2  //
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef union{ 
	struct{
	//--------------------
	//Заголовок пакета.	
	uint8_t  SinhroByte1;// Синхробайт.
	uint8_t  SinhroByte2;// Синхробайт.
	uint8_t  Type;			 // Тип модуля.
	uint8_t  Size;       // Размер пакета.   
	uint8_t  CmdCode;    // Код команды.
	//--------------------
	uint8_t	 Data[RS485_BUF_DATA_SIZE + RS485_BUF_CRC_SIZE];
	//--------------------
	}Str;
	uint8_t Buf[RS485_BUF_HEAD_SIZE + RS485_BUF_DATA_SIZE + RS485_BUF_CRC_SIZE];	
}RS485Buf_t;
#pragma pack(pop)//вернули предыдущую настройку.
//****************************************************
//Настраиваемые параметры для работы протокола.
#define RS485_BAUD_RATE		 		250000u
#define RS485_REQUEST_PERIOD 	20u  	 //Период между запросов протокола, в мС.
#define RS485_NACK_TIMEOUT 		1000u	 //Таймаут приема пакета от плат, в мС.
//--------------------
#define RS485_SINCHRO_BYTE_1  0x72u 	
#define RS485_SINCHRO_BYTE_2  0xA5u 	
//--------------------
//APB1 тактируется 36МГц.
#define APBCLK 					36000000ul 
#define USART_BRR     	((APBCLK + RS485_BAUD_RATE / 2) / RS485_BAUD_RATE)
//--------------------
//Управление микросхемой драйвера RS485.
#define RS485DirectTX()	(GPIOC->BSRR = GPIO_BSRR_BS6)//Управление микросхемой драйвера RS485. 
#define RS485DirectRX()	(GPIOC->BSRR = GPIO_BSRR_BR6)
//--------------------
#define SEND_MB_STATUS    0
#define REQUEST_SB_STATUS	1
#define REQUEST_IP_STATUS	2
#define REQUEST_ZE_STATUS	3
#define REQUEST_IB_STATUS	4 //interface board
//--------------------
//Флаги.
typedef struct{
	
	uint16_t TxContinues :1;
	uint16_t NoConnect   :1;
	uint16_t MBNewData   :1;
	uint16_t FPNewData   :1;
	uint16_t ZENewData   :1;
	uint16_t IPNewData   :1;
	uint16_t SBNewData   :1;
	
	uint16_t FPNAck :1;
	uint16_t SBNAck :1;
	uint16_t IPNAck :1;
	uint16_t ZENAck :1;
	uint16_t :5;
	
}RS485Flags_t;
#define FLAG_CLEAR	0
#define FLAG_SET  	1
//****************************************************
//Счетчики для отсчета таймаутов ответа от плат.
#pragma pack(push, 1)//размер выравнивания в 1 байт.
typedef struct{
	//-----------
	uint16_t TimeOutAckProtocol;
	uint16_t TimeOutAckFP;
	uint16_t TimeOutAckSB;
	uint16_t TimeOutAckIP;
	uint16_t TimeOutAckZE;
	//-----------
}RS485Counts_t;
#pragma pack(pop)//вернули предыдущую настройку.
//*******************************************************************************************
//*******************************************************************************************
//Прикладной уровень. Общие функции.
void 		      RS485_Init (void);
RS485Flags_t* RS485_Flags(void);

//Ф-и для работы с приемом данных.
RS485Buf_t* RS485_RxBuf(void);

//Ф-и для работы с передачей данных.
void RS485_StartBufTx(uint8_t* buf, uint16_t size);

//Ф-и для работы ЦП.
void RS485_MB_MasterLoop(void);//Автомат состояний, определяющий порядок выдачи кодограмм.
void RS485_MB_BuildAndTxPack(void);

//Ф-и для работы SB.
void RS485_SB_ErrorConnect(void);
void RS485_SB_BuildAndTxPack(void);

//Ф-и для работы FP.
void RS485_FP_BuildAndTxEventPack  (uint16_t numEvent);
void RS485_FP_BuildAndTxEepInfoPack(void);
void RS485_FP_BuildAndTxFireLineContextPack(uint8_t numLine);
//*******************************************************************************************
//*******************************************************************************************
#endif /*_Rs485_H*/










