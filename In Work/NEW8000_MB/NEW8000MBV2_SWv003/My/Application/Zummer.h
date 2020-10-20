
#ifndef _Zummer_H
#define _Zummer_H
//-----------------------------------------------------------------------------

#include "timers_ST.h"

#include "Buttons.h"
//-----------------------------------------------------------------------------
#define ZummerOn        1
#define ZummerOff       2
#define ZummerStartBeep 3
//-----------------------------------------------------------------------------
void Zummer_Init   (void);
void Zummer        (uint8_t st);
void Zummer_Fault  (uint32_t fault);
void Zummer_Loop   (void);
void Zummer_Beep   (uint8_t num, uint16_t interval);
void Zummer_BeepOff(void);
//-----------------------------------------------------------------------------
#endif /*_Zummer_H*/



