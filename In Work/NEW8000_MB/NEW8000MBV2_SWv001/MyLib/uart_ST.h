
#ifndef _uart_ST_H
#define _uart_ST_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Аппатартный уровень.
void Uart1Init  (void);
void Uart3Init  (void);
void Uart1StarTx(uint8_t *TxBuf, uint8_t size);
void Uart3StarTx(uint8_t *TxBuf, uint8_t size);

//void USART1_IRQHandler(void);//Прерывание от USART1.
//void USART3_IRQHandler(void);//Прерывание от USART3.
//------------------------
//Работа с DMA.
void DMA1Ch4InitForTX(USART_TypeDef *ptr);
void DMA1Ch4StartTX  (uint8_t *buf, uint16_t size);

//Работа с DMA. Не отлажено!!!
void    DMA1Ch5InitForRX (void);
void    StartDMA1Ch5ForRX(uint8_t *BuffAdr, uint16_t LengthBufer);
uint8_t GetStateDMA1Ch4  (void);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#endif 

















