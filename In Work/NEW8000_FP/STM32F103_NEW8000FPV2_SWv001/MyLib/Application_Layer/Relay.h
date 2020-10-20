
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
//Ф-ии для работы с зонами.
#define	Zone1   (1<<0)
#define	Zone2   (1<<1)		  
#define	Zone3   (1<<2) 
#define	Zone4   (1<<3)
#define	Zone5   (1<<4) 
#define	Zone6   (1<<5)
#define	ZoneAll (Zone1 | Zone2 | Zone3 | Zone4 | Zone5 | Zone6)

#define	On   1 
#define	Off  0
   
//Состояние зон.
#define	ZoneMask		  0x0f
#define	ZoneOff		   (0<<0) //зона отключена.
#define	ZoneManualOn (1<<0) //зона активирована в ручную, нажатием кнопки ПРЕСЕТ1-ПРЕСЕТ8.
#define	ZoneConsolOn (2<<0) //зона активирована консолью.
#define	ZoneMicOn    (3<<0) //зона активирована приоритетным микрофоном МИКР.
#define	ZoneStartOn  (4<<0) //зона активирована кнопкой ПУСК.
#define	ZonePoj1On	 (5<<0) //зона активирована пожарным шлейфом Пож.1.
#define	ZonePoj2On	 (6<<0) //зона активирована пожарным шлейфом Пож.2.
#define	ZonePoj3On	 (7<<0) //зона активирована пожарным шлейфом Пож.3. 
#define	ZoneChSOn	   (8<<0) //зона активирована пожарным шлейфом ГОиЧС.
#define	ZoneIPOn	   (9<<0) //зона активирована платой IP.
#define	ZoneAutoOn	 (10<<0)//зона активирована атоматикой. - нужно подумать, возможно это состояние не нужно.
#define	ZoneAutoOff	 (11<<0)//Автоматика отключена - прогизводится нажатием кнопки СБРОС ОПОВЕЩ.  
#define	ZoneDefect   (12<<0)//
//-----------------------------------------------------------------------------
void RelayOrLedLoop(void);
void RelayOrLedOn (uint16_t relay);
void RelayOrLedOff(uint16_t relay);

void    ZoneFpSet(uint8_t* p);
uint8_t ZoneFpGet(uint8_t ch);
//-----------------------------------------------------------------------------
#endif /*_заготовка_H*/

