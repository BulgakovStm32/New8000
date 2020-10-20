
#ifndef _Relay_H
#define _Relay_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "gpio_ST.h"
#include "spi_ST.h"

//-----------------------------------------------------------------------------
#define	Led1		         (1<<0)
#define	Led2		         (1<<1)
#define	Led3		         (1<<2)
#define	Led4		         (1<<3)
#define	Led5		         (1<<4)
#define	RelSp1OnOff      (1<<5)
#define	RelSp2OnOff      (1<<6)
#define	Zummer           (1<<7)
#define	RelSTART		     (1<<8)
#define	RelFaultGenerel  (1<<9)
#define	RelFaultPower		 (1<<10)
#define	RelPoj3		       (1<<11)
#define	RelSp1AttLin		 (1<<12)
#define	RelSp2AttLin		 (1<<13)
#define	RelPoj		       (1<<14)
#define	RelGO		         (1<<15)

#define	AllOff		       (0xFFFF)
//-----------------------------------------------------------------------------
void RelayOrLedLoop(void);

void RelayOrLedOn (uint16_t relay);
void RelayOrLedOff(uint16_t relay);
//-----------------------------------------------------------------------------
#endif /*_заготовка_H*/

