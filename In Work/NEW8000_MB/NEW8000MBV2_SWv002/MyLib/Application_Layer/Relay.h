
#ifndef _Relay_H
#define _Relay_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "gpio_ST.h"
#include "spi_ST.h"

#include "LineCheck.h"
//-----------------------------------------------------------------------------
#define	LedRS485Act	    (1<<3)
#define	RelSp1OnOff     (1<<4)
#define	RelSp2OnOff     (1<<5)
#define	RelSp1AttLin		(1<<6)
#define	RelSp2AttLin		(1<<7)
#define	RelPusk		      (1<<8)
#define	RelFaultGeneral (1<<9)
#define	RelFaultPower	  (1<<10)
#define	RelPoj3		      (1<<11)
#define	RelPoj		      (1<<14)
#define	RelChS		      (1<<15)

#define	RelAllOff		    (0xFFFF)
//--------------------
#define Off	0
#define On	1
//-----------------------------------------------------------------------------
void RelayUpdate(void);

void SpeakerRelays(uint8_t state, uint8_t relay);
void OutputRelays (uint8_t state, uint16_t relay);
void LedRs485Act  (uint8_t state);
//-----------------------------------------------------------------------------

#endif /*_заготовка_H*/























