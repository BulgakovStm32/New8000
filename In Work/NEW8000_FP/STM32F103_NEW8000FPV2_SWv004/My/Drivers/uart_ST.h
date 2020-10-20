
#ifndef _uart_ST_H
#define _uart_ST_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

//-----------------------------------------------------------------------------
//Указатели обработчиков событий
typedef struct{
	//--------
	uint8_t (*ReceiveByte)        (uint8_t byte);//Обработчик приема байта.
  void    (*ReceiveByteBroken)  (void);   		 //Обработчик битого байта.
	void    (*BufTransferComplete)(void);        //Окончание передачи буфера.
	//--------
}UxHandlers_t;
//-----------------------------------------------------------------------------
//Аппатартный уровень.
void 					Uart1Init  (uint16_t usartBrr);
void 				  Uart1ManagingRx(uint8_t rxState);
void 					Uart1StarTx(uint8_t *TxBuf, uint8_t size);
UxHandlers_t* Uart1Handler(void);
//------------------------
//Работа с DMA. Не отлажено!!!
void DMA1Ch4InitForTx(USART_TypeDef *usart);
void DMA1Ch4StartTx  (uint8_t *buf, uint32_t size);

//void    DMA1Ch5InitForRX (void);
//void    StartDMA1Ch5ForRX(uint8_t *BuffAdr, uint16_t LengthBufer);
//uint8_t GetStateDMA1Ch4  (void);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#endif 

















