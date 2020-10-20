
#ifndef _Zummer_H
#define _Zummer_H
//-----------------------------------------------------------------------------

#include "timers_ST.h"

#include "Buttons.h"
//-----------------------------------------------------------------------------
#define ZUMM_ON         1
#define ZUMM_OFF        2
#define ZUMM_START_BEEP 3
//-----------------------------------------------------------------------------
void Zummer_Init   (void);
void Zummer_Control(uint8_t st);
void Zummer_Fault  (uint32_t faults);
void Zummer_Loop   (void);
void Zummer_Beep   (uint8_t num, uint16_t interval);
void Zummer_BeepOff(void);
//-----------------------------------------------------------------------------
#endif /*_Zummer_H*/



