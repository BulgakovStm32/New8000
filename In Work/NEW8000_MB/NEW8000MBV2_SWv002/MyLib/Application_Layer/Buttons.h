
#ifndef _Buttons_H
#define _Buttons_H
//-----------------------------------------------------------------------------

#include "uart_ST.h"

//-----------------------------------------------------------------------------
#define TEST_BUTTON   	(1<<6)
#define ZUMMER_BUTTON 	(1<<5)
#define STOP_BUTTON 		(1<<4)
#define START_BUTTON  	(1<<3)
#define RESET_BUTTON  	(1<<2)
#define PUSK_BUTTON   	(1<<1)

#define ALG1_BUTTON	  (1<<0)  
#define ALG2_BUTTON	  (1<<1) 
#define ALG3_BUTTON	  (1<<2)  
#define ALG4_BUTTON	  (1<<3)  
#define ALG5_BUTTON	  (1<<4)
#define ALG6_BUTTON	  (1<<5)
#define ALG7_BUTTON	  (1<<6)
#define ALG8_BUTTON	  (1<<7)

#define ALL_BUTTON	     0xFF
//--------------------
#define CONTROL_BUTTONS	2 //3
#define ALG_BUTTONS	    4 //6
//-----------------------------------------------------------------------------
//Струтура хранения состояния кнопок.
typedef union {
	struct{
		uint8_t ControlBut;
		uint8_t AlgBut;
		//-----------
		uint8_t ControlButOldSt; //Кнопки 
		uint8_t ControlButSt;  	 //
		//-----------
		uint8_t PresetButOldSt;	//Кнопки 
		uint8_t PresetButSt;	  //
		//-----------
	}STR;
	unsigned char BLK[4];
}ButStateSTR_Type;
//-----------------------------------------------------------------------------
void    ButFPControl(uint8_t but);
void    ButFPPreset (uint8_t but);
void    ButFP       (uint8_t but);

uint8_t ButGetInstantState(uint8_t but);
uint8_t ButGetState       (uint8_t but);
uint8_t ButGetToggleState (uint8_t but);
void    ButClrToggleState (uint8_t but, uint8_t clr_bit);


uint8_t ButGetToggleForMic(void);


//-----------------------------------------------------------------------------
#endif // _Buttons_H


