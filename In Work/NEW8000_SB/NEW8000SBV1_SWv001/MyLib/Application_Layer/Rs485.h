
#ifndef _Rs485_H
#define _Rs485_H
//******************************************************************************************

#include "uart_ST.h"
#include "dma_ST.h"
#include "crc16.h"

#include "SB_SirenBoard.h"
//******************************************************************************************
//******************************************************************************************
//Параметры для работы протокола.
#define RS485_BAUD_RATE		 			250000U//115200U
#define RS485_PROTOCOL_TIMEOUT	1000U
//--------------------
#define RS485_SINCHRO_BYTE_1 		0x72U 	
#define RS485_SINCHRO_BYTE_2    0xA5U 	
//--------------------
#define APBCLK 			48000000U
#define USART_BRR   ((APBCLK + RS485_BAUD_RATE / 2) / RS485_BAUD_RATE)
//--------------------
//Управление микросхемой драйвера RS485.
#define RS485DirectTX()	(GPIOB->BSRR = GPIO_BSRR_BS_1) 
#define RS485DirectRX()	(GPIOB->BSRR = GPIO_BSRR_BR_1)
#define RS485ReDeToggel	(GPIOB->ODR  = GPIOB->ODR ^ GPIO_ODR_1)
//********************************************************
//***********Буфер для работы протокола.******************
#define RS485_BUF_HEAD_SIZE		5  //
#define RS485_BUF_DATA_SIZE		89 //
#define RS485_BUF_CRC_SIZE		2  //
#define RS485_PACK_MAX_SIZE		(RS485_BUF_HEAD_SIZE + RS485_BUF_DATA_SIZE + RS485_BUF_CRC_SIZE)  //
//*********************************
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef union{ 
	struct{
	//--------------------
	//Заголовок пакета.	
	uint8_t SinhroByte1;// Синхробайт.
	uint8_t SinhroByte2;// Синхробайт.
	uint8_t Type;			  // Тип модуля.
	uint8_t Size;       // Размер пакета.   
	uint8_t CmdCode;    // Код команды.
	//--------------------
	uint8_t	Data[RS485_BUF_DATA_SIZE + RS485_BUF_CRC_SIZE];
	//--------------------
	}Str;
	uint8_t Buf[RS485_BUF_HEAD_SIZE + RS485_BUF_DATA_SIZE + RS485_BUF_CRC_SIZE];	
}RS485Buf_t;
#pragma pack(pop)//вернули предыдущую настройку.
//********************************************************
//***********Флаги.***************************************
typedef struct{
	uint8_t TIMEOUT_ERR_f :1;
	uint8_t SB_CMD_SET_f  :1;
	uint8_t :5;
}RS485Flags_t;
#define FlagClr	0
#define FlagSet	1
//******************************************************************************************
//******************************************************************************************
//Прикладной уровень. Общие функции.
void          RS485_Init(void);
void          RS485_TimeOutLoop(void);//Отсчет таймаута приема пакета.
RS485Flags_t* RS485_Flags(void);     

RS485Buf_t*   RS485_RxBuf(void);			    

void 		      RS485_TxStatusSB(void);
//******************************************************************************************
//******************************************************************************************
#endif /*_Rs485_H*/








