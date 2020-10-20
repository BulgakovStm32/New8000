
#ifndef _Buttons_H
#define _Buttons_H
//-----------------------------------------------------------------------------

#include "uart_ST.h"

//-----------------------------------------------------------------------------
#define TEST_BUTTON     (1<<6)
#define ZUMMER_BUTTON   (1<<5)
#define ResFAULT_BUTTON (1<<4)
#define ResALERT_BUTTON (1<<3)
#define STOP_BUTTON     (1<<2)
#define START_BUTTON    (1<<1)

#define PRES1_BUTTON	  (1<<0)  
#define PRES2_BUTTON	  (1<<1) 
#define PRES3_BUTTON	  (1<<2)  
#define PRES4_BUTTON	  (1<<3)  
#define PRES5_BUTTON	  (1<<4)
#define PRES6_BUTTON	  (1<<5)
#define PRES7_BUTTON	  (1<<6)
#define PRES8_BUTTON	  (1<<7)
//--------------------
#define CONTROL_BUTTONS	    1
#define PRESET_BUTTONS	    2



//-----------------------------------------------------------------------------
uint8_t GetStateButtons(uint8_t button);

//-----------------------------------------------------------------------------
#endif // _Buttons_H


