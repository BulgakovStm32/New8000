
#ifndef _Buttons_H
#define _Buttons_H
//*****************************************************************************

#include "FP_FacePanel.h"

//*****************************************************************************
#define ALG1_BUTTON	  (1<<0)  
#define ALG2_BUTTON	  (1<<1) 
#define ALG3_BUTTON	  (1<<2)  
#define ALG4_BUTTON	  (1<<3)  
#define ALG5_BUTTON	  (1<<4)
#define ALG6_BUTTON	  (1<<5)
#define ALG7_BUTTON	  (1<<6)
#define ALG8_BUTTON	  (1<<7)

#define PUSK_BUTTON   (1<<9)
#define RESET_BUTTON  (1<<10)
#define START_BUTTON  (1<<11)
#define STOP_BUTTON 	(1<<12)
#define ZUMMER_BUTTON (1<<13)
#define TEST_BUTTON   (1<<14)

#define ALL_BUTTONS	     0xFF
//--------------------
//#define CONTROL_BUTTONS	 2 //3
//#define ALG_BUTTONS	     4 //6
//--------------------
#define LONG_PRESS_CYCLE 50//90//100
//*************************************************************
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct {
	uint8_t  Algorithm;
	uint8_t  Control;
	uint16_t All;
}FPButtons_t;
#pragma pack(pop)//вернули предыдущую настройку.
//*************************************************************
typedef struct{
	//-----
	uint32_t Instant;
	uint32_t isPress;
	uint32_t isRelease;
	uint32_t isToggle;
	//-----
	uint32_t ready_f :1;
	uint32_t :31;
	//-----
}ButtonStates_t;
//*************************************************************
typedef enum{
	ALG1but 	= (1<<0),
	ALG2but 	= (1<<1),
	ALG3but 	= (1<<2),
	ALG4but 	= (1<<3),
	ALG5but 	= (1<<4),
	ALG6but 	= (1<<5),
	ALG7but 	= (1<<6),
	ALG8but 	= (1<<7),
	NotBut,
  PUSKbut   = (1<<9),
  RESETbut  = (1<<10),
	STARTbut  = (1<<11),	
	STOPbut   = (1<<12),
	ZUMMERbut = (1<<13),
	TESTbut   = (1<<14),
	AllButtons = 0xFFFF,
}ButtonsEnum_t;
//*****************************************************************************
//*****************************************************************************
FPButtons_t* ButtonFP(void);
void         ButtonFP_Clr(void);

void            Button_Update(void);
ButtonStates_t* Button(void);

uint32_t Button_Instant  (ButtonsEnum_t but);
uint32_t Button_IsPress  (ButtonsEnum_t but);
uint32_t Button_IsRelease(ButtonsEnum_t but);
uint32_t Button_IsToggle (ButtonsEnum_t but);
void     Button_ClrToggle(ButtonsEnum_t but);

void Button_UpDown   (uint16_t *param, uint16_t min, uint16_t max);
void Button_LeftRigth(uint16_t *param, uint16_t min, uint16_t max);

void            ButtonForMic_Update(void);
ButtonStates_t* ButtonForMic(void);

void            ButtonForManual_Update(void);
ButtonStates_t* ButtonForManual(void);
//*****************************************************************************
//*****************************************************************************
#endif // _Buttons_H


