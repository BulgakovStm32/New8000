
#ifndef _Rs485_H
#define _Rs485_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "crc16.h"
#include "uart_ST.h"
//--------------------
//Boards.
#include "MB_MotherBoard.h"
#include "SB_SirenBoard.h"
#include "FP_FacePanel.h"
//-----------------------------------------------------------------------------
//Настраиваемые параметры для работы протокола.
#define RS485Baudrate		  115200UL
//APB2 тактируется 72МГц.
#define APBCLK 					  72000000UL
#define USARTxBRR_Const ((APBCLK + RS485Baudrate / 2) / RS485Baudrate)

#define RS485ProtocolTimeOut_Const	1000UL

//typedef MBDataUNION_Type	RS485RxBuf_Type;//Тип буфера Приема.
//typedef FPDataUNION_Type  RS485TxBuf_Type;//Тип буфера Передачи.

//typedef RS485BufUnion_t	RS485RxBuf_Type;//Тип буфера Приема.

typedef RS485BufUnion_t	RS485Buf_t; //Тип буфера для работы протокола.

//Управление микросхемой драйвера RS485.
#define RS485DEOn  (GPIOC->BSRR = GPIO_BSRR_BS14) 
#define RS485REOn  (GPIOC->BSRR = GPIO_BSRR_BR14) 

#define RS485DirectionTX() RS485DEOn
#define RS485DirectionRX() RS485REOn
//--------------------
#define SinhroByte1Const      0x72 // Значение синхробайта.
#define SinhroByte2Const      0xA5 // Значение синхробайта.
//--------------------
//Флаги.
typedef struct{
	
	uint8_t TimeOutFlag   :1;
	uint8_t MBNewDataFlag :1;
	uint8_t MBErrorFlag   :1;
	uint8_t :5;
	
}RS485Flags_t;

#define FlagClr	0
#define FlagSet	1
//-----------------------------------------------------------------------------
//Прикладной уровень. Общие функции.
void 		      RS485Init      (void);
void          RS485TimeOutInc(void); //Отсчет таймаута приема пакета.
RS485Flags_t* RS485Flags     (void);
uint8_t* 		  RS485Cmd       (void);

uint8_t  		RS485RxPacket(uint8_t data);//Прием пакета.
void     		RS485RxBufClr(void);        //Сброс приемного буфера.
RS485Buf_t* RS485RxBufPtr(void);        //Получение указателя на байт в вприемном буфере.

//RS485TxBuf_Type* RS485TxBufPtr       (void);
RS485Buf_t* RS485TxBufPtr(void);
void        RS485StartTxResponse(void);
//-----------------------------------------------------------------------------
#endif /*_Rs485_H*/

