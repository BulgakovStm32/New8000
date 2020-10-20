
#ifndef _Buttons_H
#define _Buttons_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "sm1628.h"
//-----------------------------------------------------------------------------
//Номер бита кнопки в структуре.
#define PUSK_BUTTONbit      24  //22
#define RESET_BUTTONbit     19  //18
#define START_BUTTONbit     16  //15
#define STOP_BUTTONbit      11  //12
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
//---------------------------------
//CONTROL_BUTTONS
#define TEST_BUTTON   	(1<<6)
#define ZUMMER_BUTTON 	(1<<5)
#define STOP_BUTTON 		(1<<4)
#define START_BUTTON  	(1<<3)
#define RESET_BUTTON  	(1<<2)
#define PUSK_BUTTON   	(1<<1)

//PRESET_BUTTONS
#define PRES1_BUTTON	  (1<<0)  
#define PRES2_BUTTON	  (1<<1) 
#define PRES3_BUTTON	  (1<<2)  
#define PRES4_BUTTON	  (1<<3)  
#define PRES5_BUTTON	  (1<<4)
#define PRES6_BUTTON	  (1<<5)
#define PRES7_BUTTON	  (1<<6)
#define PRES8_BUTTON	  (1<<7)

#define ALL_BUTTONS	     0xFF
//--------------------
typedef enum 
{
	ALG1but = 0,
	ALG2but,
	ALG3but,
	ALG4but,
	ALG5but,
	ALG6but,
	ALG7but,
	ALG8but,
	NotBut ,
  PUSKbut,
  RESETbut,
	STARTbut,	
	STOPbut,
	ZUMMERbut,
	TESTbut,
	AllButtons = 0xFFFF,
}ButtonEnumType;
//--------------------
#define CONTROL_BUTTONS	 2 //3
#define ALG_BUTTONS	     4 //6
//--------------------
#define LongPressCycleConst 50//90//100
//-----------------------------------------------------------------------------
uint8_t  ButtonCheck     (void);
uint8_t  ButtonGetInstant(uint8_t but);
uint16_t ButtonInstant   (ButtonEnumType but);

//uint16_t ButtonPress(ButtonEnumType but);
uint16_t ButtonToggle   (ButtonEnumType but);
uint16_t ButtonGetToggle(ButtonEnumType but);
void     ButtonClrToggle(ButtonEnumType but);

void ButtonUpDown(uint16_t *param, uint16_t min, uint16_t max);
void ButtonLeftRigth(uint16_t *param, uint16_t min, uint16_t max);

//-----------------------------------------------------------------------------
#endif //_Buttons_H


