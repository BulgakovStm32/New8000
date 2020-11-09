
#ifndef _LcdMenu_H
#define _LcdMenu_H
//******************************************************************************************

#include "sys_ST.h"

//--------------------
//AppLayer.
#include "Rtc.h"
#include "Buttons.h"
#include "Led.h"
#include "Faults.h"
//--------------------
//Boards.
#include "SB_SirenBoard.h"
//******************************************************************************************
//******************************************************************************************
#define PAGE_NUM   6  //Количество страниц меню.
#define ITEM_NUM   6  //Количество пунктов на странице меню.
//Нумерация страниц.
#define Page1   0 
#define Page2   1
#define Page3   2
#define Page4   3
#define Page5   4
#define Page6   5
#define Page7   6
#define Page8   7
//Номера строк. Первая строка в верху дисплея.
#define String1   0 
#define String2   1 
#define String3   2 
#define String4   3 
#define String5   4 
#define String6   5 
#define String7   6 
#define String8   7
//Номера символов в строке. Первый символ слева.
#define Char1     1 
#define Char2     2
#define Char3     3
#define Char4     4
#define Char5     5
#define Char6     6
#define Char7     7
#define Char8     8
#define Char9     9
#define Char10    10
#define Char11    11
#define Char12    12
#define Char13    13
#define Char14    14
#define Char15    15
//******************************
//Структура пунктов на странице меню.
typedef struct{
	//---------
	const char *Name;// 
	const char *Text;// 
	uint16_t    Var; //
	//---------
}Item_t;
//******************************
//Структура одной страницы меню.
typedef	struct{
	//---------
	const char*  Name;  //Имя страницы меню.
	Item_t Item1;	
	Item_t Item2;
	Item_t Item3;
	Item_t Item4;
	Item_t Item5;
	Item_t Item6;
	//---------
}Page_t;
//******************************
//Структура переменных для работы с меню.
typedef struct{
  //---------
  uint16_t Page; //С какой страницей работаем.
  uint16_t Item; //С какой строкой страницы идет работа.
  //---------
}Index_t;
//******************************
typedef struct{
	
	Index_t Index;
	Page_t 	Page[PAGE_NUM];
  //---------
}Menu_t;
//******************************************************************************************
//******************************************************************************************
void  	 Menu_Init(void);
Page_t*  Menu(uint8_t page);
Index_t* Menu_IndexReg(void);
void     Menu_IndexRegCleare(void);

void  Menu_Logo       (void);
void  Menu_ProgressBar(void);

void Menu_AddressUpdate    (uint8_t position, uint8_t data, uint8_t blink);
void Menu_GroupUpdate      (uint8_t position, uint8_t data, uint8_t blink);
void Menu_TimeUpdate       (uint8_t position, uint16_t data, uint8_t blink, TimeSTR_Type* time);
void Menu_CalendarUpdate   (uint8_t position, uint16_t date, uint8_t blink, TimeSTR_Type* Data);
void Menu_InputConfigUpdate(uint8_t numStr, uint8_t digit, FireLine_t* ptr);
void Menu_Update(Page_t *menuPage, uint8_t mode, uint8_t pointStr);


void Print_Testmode  (void);
void Print_ErrConnect(void);
void Print_MicState  (void);
void Print_PowerStateFromMB(void);
void Print_PowerState(void);
void Print_SpLineState(uint8_t line);
void Print_SirenSate(uint8_t line);
void Print_FireLineState(uint8_t line);


char* Text_GetBatState    (uint8_t state);
char* Text_GetAcPowerState(uint8_t state);
char* Text_GetDcPowerState(uint8_t state);
char* Text_GetAmpState    (uint8_t state);
char* Text_GetSpLineState (uint8_t line);
//**********************************************
typedef enum{
	OTHER = 0,
	TEST,
	CONFIGURATION,
	CONTROL,
	LOCK,
	FAULT,
	FIRE,	
	PUSK,
}DisplayMode_t;

void Display_Header(const char* txt);
void Display_Mic(void);
void Display_PuskButtonActivation(void);
void Display_Fire(void);
void Display_Faults(uint32_t faults);
void Display_Manual(void);
void Display_AddressGroupAndMicState(void);
void Display_Test(void);
//******************************************************************************************
//******************************************************************************************
#endif //_MenuLcd12864_H


