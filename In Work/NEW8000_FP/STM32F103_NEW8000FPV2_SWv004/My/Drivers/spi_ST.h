
#ifndef _spi_ST_H
#define _spi_ST_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "gpio_ST.h"

//-----------------------------------------------------------------------------
#define Spi2SckHiht 	(GPIOB->BSRR = GPIO_BSRR_BS13) 
#define Spi2SckLow 	  (GPIOB->BSRR = GPIO_BSRR_BR13) 

#define SPI_RX_ONLY_MODE 0 //Output disabled (receive-only mode) 
#define SPI_TX_ONLY_MODE 1 //Output enabled  (transmit-only mode)

#define SPI_WAIT ((uint32_t)0x5000)
//-----------------------------------------------------------------------------
void    Spi2Init     (void);
void    Spi2BiDirMode(uint8_t mode);
void    Spi2TxData   (uint8_t data);
uint8_t Spi2RxData   (void);
//-----------------------------------------------------------------------------
#endif /*_spi_ST_H*/


