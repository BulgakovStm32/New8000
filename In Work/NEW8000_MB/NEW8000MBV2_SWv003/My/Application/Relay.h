
#ifndef _Relay_H
#define _Relay_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "gpio_ST.h"
#include "spi_ST.h"

#include "LineCheck.h"
//-----------------------------------------------------------------------------
//Управление сдвиговыми регистрами. PC7(STPIC_CS), PC8(STPIC_CLR).
#define STPIC6C595CS_High() (GPIOC->BSRR = GPIO_BSRR_BS7) 
#define STPIC6C595CS_Low()  (GPIOC->BSRR = GPIO_BSRR_BR7) 

#define STPIC6C595CLR_High()(GPIOC->BSRR = GPIO_BSRR_BS8) 
#define STPIC6C595CLR_Low() (GPIOC->BSRR = GPIO_BSRR_BR8) 
//--------------------
#define	LED_RS485_ACT	    	(1<<3)
#define	RELAY_SP1_ON_OFF  	(1<<4)
#define	RELAY_SP2_ON_OFF  	(1<<5)
#define	RELAY_SP1_ATT_LIN		(1<<6)
#define	RELAY_SP2_ATT_LIN		(1<<7)
#define	RELAY_PUSK          (1<<8)
#define	RELAY_FAULT_GENERAL (1<<9)
#define	RELAY_FAULT_POWER	  (1<<10)
#define	RELAY_FIRE3		      (1<<11)
#define	RELAY_FIRE		      (1<<14)
#define	RELAY_GO_ChS	      (1<<15)
#define	RELAY_ALL   				(0xFFFF)
//--------------------
#define OFF	0
#define ON	1
//-----------------------------------------------------------------------------
void Relay_Init(void);
void Relay_UpdateLoop(void);

void SpeakerRelays(uint8_t state, uint8_t relay);
void OutputRelays (uint8_t state, uint16_t relay);
void LedRs485Act  (uint8_t state);
//-----------------------------------------------------------------------------

#endif /*_заготовка_H*/























