
#ifndef _timers_ST_H
#define _timers_ST_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "gpio_ST.h"
#include "Buttons.h"

//-----------------------------------------------------------------------------
#define ZummerOn        1
#define ZummerOff       2
#define ZummerStartBeep 3
//-----------------------------------------------------------------------------
void Tim1Init(void);

void Zummer       (uint8_t st);
void ZummerFault  (uint32_t fault);
void ZummerLoop   (void);
void ZummerBeep   (uint8_t num, uint16_t interval);
void ZummerBeepOff(void);
//-----------------------------------------------------------------------------
#endif //_timers_ST_H









