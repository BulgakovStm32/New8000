
#ifndef _dma_ST_H
#define _dma_ST_H
//-----------------------------------------------------------------------------

//#include "stm32f10x.h"
#include "stm32f0xx.h"

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void DMA1_Ch2_InitForTX(USART_TypeDef *usart);
void DMA1_Ch2_StartTX  (uint8_t *buf, uint16_t size);
//-----------------------------------------------------------------------------
#endif /*_dma_ST_H*/


