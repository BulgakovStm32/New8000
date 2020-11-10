
#ifndef _Rs485_H
#define _Rs485_H
//******************************************************************************************

#include "stm32f10x.h"

#include "crc16.h"
#include "uart_ST.h"
#include "gpio_ST.h"
//--------------------
//Application.
#include "Buttons.h"
//--------------------
//Boards.
#include "MB_MotherBoard.h"
#include "FP_FacePanel.h"
//******************************************************************************************
//******************************************************************************************
//Параметры для работы протокола.
#define RS485_BAUD_RATE		 			250000UL//115200UL
#define RS485_PROTOCOL_TIMEOUT	1000UL
//--------------------
#define RS485_SINCHRO_BYTE_1 		0x72U 	
#define RS485_SINCHRO_BYTE_2    0xA5U 	
//--------------------
//UART3 тактируется от APB1(36МГц).
//UART1 тактируется от APB2(72МГц).
#define APB1CLK 						32000000UL 
#define APB2CLK 						72000000UL 
#define USART1_BRR   	      ((APB2CLK + RS485_BAUD_RATE / 2) / RS485_BAUD_RATE)
#define RS485DirectTX()			(GPIOC->BSRR = GPIO_BSRR_BS14)//Управление микросхемой драйвера RS485. 
#define RS485DirectRX()  		(GPIOC->BSRR = GPIO_BSRR_BR14)
//********************************************************
//***********Буфер для работы протокола.******************
#define RS485_BUF_HEAD_SIZE		5  //
#define RS485_BUF_DATA_SIZE		89 //
#define RS485_BUF_CRC_SIZE		2  //
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
	//Данные пакета.
	uint8_t	Data[RS485_BUF_DATA_SIZE + RS485_BUF_CRC_SIZE];
	//--------------------
	}Str;
	uint8_t Buf[RS485_BUF_HEAD_SIZE + RS485_BUF_DATA_SIZE + RS485_BUF_CRC_SIZE];	
}RS485Buf_t;
#pragma pack(pop)//вернули предыдущую настройку.
//********************************************************
//***********Флаги.***************************************
typedef struct{
	uint16_t TxContinues  :1;
	uint16_t TimeOutError :1;
	uint16_t MB_NewData   :1;
	uint16_t MB_Error     :1;
	uint16_t :12;
}RS485Flags_t;

#define FLAG_CLEAR	0
#define FLAG_SET	  1
//******************************************************************************************
//******************************************************************************************
//Прикладной уровень. Общие функции.
void 		      RS485_Init      (void);
void          RS485_TimeOutInc(void); //Отсчет таймаута приема пакета.
RS485Flags_t* RS485_Flags     (void);

uint8_t* 		RS485_Cmd(void);

RS485Buf_t* RS485_RxBuf(void);           						

RS485Buf_t* RS485_TxBuf(void);
void        RS485_StartTxResponse(void);

void RS485_TxDataToMB(void);
//******************************************************************************************
//******************************************************************************************
#endif /*_Rs485_H*/

