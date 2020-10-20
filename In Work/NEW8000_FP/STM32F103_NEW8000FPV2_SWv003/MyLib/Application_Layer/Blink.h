
#ifndef _Blink_H
#define _Blink_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"
//#include "stm32f0xx.h"

//-----------------------------------------------------------------------------

#define BlinkInterval100mS  0 //(uint16_t)100  //
#define BlinkInterval250mS  1 //(uint16_t)250  //
#define BlinkInterval500mS  2 //(uint16_t)500  //
#define BlinkInterval1000mS 3 //(uint16_t)1000 //

//-----------------------------------------------------------------------------
void     BlinkLoop(void);
uint16_t Blink(uint8_t interval);
//-----------------------------------------------------------------------------
#endif /*_Blink_H*/


