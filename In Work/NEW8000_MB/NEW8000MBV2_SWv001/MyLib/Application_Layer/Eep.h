
#ifndef _Eep_H
#define _Eep_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "spi_ST.h"
//-----------------------------------------------------------------------------
//Управление EEPROM. PA8(EEP_HOLD), PB11(EEP_WP), PB12(EEP_CS).
#define EepHoldHight (GPIOA->BSRR = GPIO_BSRR_BS8) 
#define EepHoldLow   (GPIOA->BSRR = GPIO_BSRR_BR8) 

#define EepWPHight   (GPIOB->BSRR = GPIO_BSRR_BS11) 
#define EepWPLow     (GPIOB->BSRR = GPIO_BSRR_BR11)
#define EepWPToggel  (GPIOB->ODR  = GPIOB->ODR ^ GPIO_ODR_ODR11)

#define EepChipUnsel (GPIOB->BSRR = GPIO_BSRR_BS12) 
#define EepChipSel   (GPIOB->BSRR = GPIO_BSRR_BR12)
//-----------------------------------------------------------------------------
#define StatRegBitSRWD   (1 << 7) //Status Register Write Protect
#define StatRegBitBP1    (1 << 3) //Block Protect bits
#define StatRegBitBP0    (1 << 2)
#define StatRegBitWEL    (1 << 1) //Write Enable Latch bit
#define StatRegBitWIP    (1 << 0) //Write In Progress bit

#define EepCmdWRENConst   0x06//Write Enable
#define EepCmdWRDIConst   0x04//Write Disable

#define EepCmdRDSRConst   0x05//Read Status Register 
#define EepCmdWRSRConst   0x01//Write Status Register

#define EepCmdREADConst   0x03//Read from Memory Array
#define EepCmdWRITEConst  0x02//Write to Memory Array 

#define EepMemArrayAddrStartConst 0x0000
#define EepMemArrayAddrEndConst   0x3FFF
//-----------------------------------------------------------------------------
void     EepInit(void);
void     EepMemArrayCheck(void);
uint16_t EepGetNumberBadSectors(void);

void EepReadSR(void);

void EepWrite(uint8_t* data);


//-----------------------------------------------------------------------------
#endif 


