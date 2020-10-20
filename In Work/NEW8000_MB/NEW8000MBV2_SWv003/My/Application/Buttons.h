
#ifndef _Buttons_H
#define _Buttons_H
//*****************************************************************************

#include "FP_FacePanel.h"

//*****************************************************************************
//Номер бита кнопки в структуре.
//#define PUSK_BUTTONbit      24  //22
//#define RESET_BUTTONbit     19  //18
//#define START_BUTTONbit     16  //15
//#define STOP_BUTTONbit      11  //12
//#define ZUMMER_BUTTONbit    8   //8
//#define TEST_BUTTONbit      3   //4

//#define PRESET1_BUTTONbit	  28  //26
//#define PRESET2_BUTTONbit	  25  //23
//#define PRESET3_BUTTONbit	  20  //19
//#define PRESET4_BUTTONbit	  17  //16
//#define PRESET5_BUTTONbit	  12
//#define PRESET6_BUTTONbit	  9
//#define PRESET7_BUTTONbit	  4	  //5
//#define PRESET8_BUTTONbit	  1	  //2
//---------------------------------
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
//#define ALL_BUTTONS	     0xFF
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
}ButtonsEnum_t;
//*****************************************************************************
//*****************************************************************************
FPButtons_t* ButtonFP(void);

void            Button_Update(void);
ButtonStates_t* Button(void);

uint32_t Button_Instant  (ButtonsEnum_t but);
uint32_t Button_IsPress  (ButtonsEnum_t but);
uint32_t Button_IsRelease(ButtonsEnum_t but);
uint32_t Button_IsToggle (ButtonsEnum_t but);
void     Button_ClrToggle(ButtonsEnum_t but);

void Button_UpDown   (uint16_t *param, uint16_t min, uint16_t max);
void Button_LeftRigth(uint16_t *param, uint16_t min, uint16_t max);
//*****************************************************************************
//*****************************************************************************
#endif // _Buttons_H


