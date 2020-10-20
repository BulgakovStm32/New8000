
#ifndef _uart_ST_H
#define _uart_ST_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

//--------------------
//AppLayer.
#include "Rs485.h"
//-----------------------------------------------------------------------------

//#define USARTxRXInteruptHandler RS485RxPacket


//-----------------------------------------------------------------------------
//Рабочие регистры.
typedef struct{
	//--------------------
	volatile uint16_t CounterByte;//Счетчик переданных байт. 
	volatile uint8_t  BufSize;    //Количество передаваемых байт.
	volatile uint8_t *BufPtr;     //Указатель на передающий буфер.
	//--------------------
}UsartWorkRegSTR_Type;
//-----------------------------------------------------------------------------
//Аппатартный уровень.
void Uart1Init  (void);
//void UsartStarTx(USART_TypeDef *usart, uint8_t *TxBuf, uint8_t size);

//void USART1_IRQHandler(void);       //Прерывание от USART1.
//------------------------
//Работа с DMA. Не отлажено!!!
void DMA1Ch4InitForTx(USART_TypeDef *usart);
void DMA1Ch4StartTx  (uint8_t *buf, uint16_t size);

//void    DMA1Ch5InitForRX (void);
//void    StartDMA1Ch5ForRX(uint8_t *BuffAdr, uint16_t LengthBufer);
//uint8_t GetStateDMA1Ch4  (void);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#endif 

















