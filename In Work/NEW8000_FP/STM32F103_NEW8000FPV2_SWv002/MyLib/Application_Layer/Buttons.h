
#ifndef _Buttons_H
#define _Buttons_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "sm1628.h"

//-----------------------------------------------------------------------------
//Номер бита кнопки в структуре.
#define START_BUTTONbit     24  //22
#define STOP_BUTTONbit      19  //18
#define ResALERT_BUTTONbit  16  //15
#define ResFAULT_BUTTONbit  11  //12
#define ZUMMER_BUTTONbit    8   //8
#define TEST_BUTTONbit      3   //4

#define PRESET1_BUTTONbit	  28  //26
#define PRESET2_BUTTONbit	  25  //23
#define PRESET3_BUTTONbit	  20  //19
#define PRESET4_BUTTONbit	  17  //16
#define PRESET5_BUTTONbit	  12
#define PRESET6_BUTTONbit	  9
#define PRESET7_BUTTONbit	  4	  //5
#define PRESET8_BUTTONbit	  1	  //2
//--------------------------------------
#define CONTROL_BUTTONS      2
#define PRESET_BUTTONS	     4

#define TESTbut     (1<<6)
#define ZUMMERbut   (1<<5)
#define ResFAULTbut (1<<4)
#define ResALERTbut (1<<3)
#define STOPbut     (1<<2)
#define STARTbut    (1<<1)

#define PRES1but	  (1<<0)  
#define PRES2but	  (1<<1) 
#define PRES3but	  (1<<2)  
#define PRES4but	  (1<<3)  
#define PRES5but	  (1<<4)
#define PRES6but	  (1<<5)
#define PRES7but	  (1<<6)
#define PRES8but	  (1<<7)

#define ALLbut	     0xFF
//-----------------------------------------------------------------------------
uint8_t ButGetInstantState(uint8_t but);
uint8_t ButGetToggleState (uint8_t but);
void    ButClrToggleState (uint8_t but, uint8_t clr_bit);
void    ButModParam1      (uint16_t *param, uint8_t min, uint8_t max);
void    ButModParam2      (uint16_t *param, uint8_t min, uint8_t max);

//-----------------------------------------------------------------------------
#endif //_Buttons_H


