
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

#define ALL_BUTTON	     0xFF
//--------------------
#define CONTROL_BUTTONS	   2 //3
#define PRESET_BUTTONS	   4 //6
//-----------------------------------------------------------------------------
//Струтура хранения состояния кнопок.
typedef union {
	struct{
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



//Струтура хранения состояния кнопок.
typedef union {
	struct{
		//-----------
    uint8_t ControlBut;   	 //Мгновенное состояние.- обновляется при каждом опросе FP.
		uint8_t ControlButOld;   //Преведущее состояние 
		uint8_t ControlButToggle;//Тригерное состояние.
		//-----------
		uint8_t PresetBut;	    //Мгновенное состояние.- обновляется при каждом опросе FP.
		uint8_t PresetButOld;	  //Преведущее состояние 
		uint8_t PresetButToggle;//Тригерное состояние.
		//-----------
	}STR;
	unsigned char BLK[6];
}ButStateSTR2_Type;
//-----------------------------------------------------------------------------
void               ButSaveState(uint8_t* p);
ButStateSTR2_Type* ButGet      (void);


void    ButFPControl(uint8_t but);
void    ButFPPreset (uint8_t but);
void    ButFP       (uint8_t but);

uint8_t ButGetInstantState(uint8_t but);
uint8_t ButGetState       (uint8_t but);
uint8_t ButGetToggleState (uint8_t but);
void    ButClrToggleState (uint8_t but, uint8_t clr_bit);

//-----------------------------------------------------------------------------
#endif // _Buttons_H


