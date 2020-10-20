
#ifndef _spi_ST_H
#define _spi_ST_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "gpio_ST.h"

//-----------------------------------------------------------------------------
#define Spi2SckHiht 	(GPIOB->BSRR = GPIO_BSRR_BS13) 
#define Spi2SckLow 	  (GPIOB->BSRR = GPIO_BSRR_BR13) 

#define Spi2RxOnlyMode 0 //Output disabled (receive-only mode) 
#define Spi2TxOnlyMode 1 //Output enabled  (transmit-only mode)
//-----------------------------------------------------------------------------
void    Spi2Init     (void);
void    Spi2BiDirMode(uint8_t mode);
void    Spi2TxData   (uint8_t data);
uint8_t Spi2RxData   (void);

//-----------------------------------------------------------------------------
#endif /*_spi_ST_H*/


