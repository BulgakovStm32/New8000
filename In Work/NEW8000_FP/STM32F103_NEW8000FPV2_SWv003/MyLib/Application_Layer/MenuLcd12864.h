
#ifndef _MenuLcd12864_H
#define _MenuLcd12864_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "sys_ST.h"
#include "lcd12864.h"
//--------------------
//AppLayer.
#include "Rtc.h"
#include "Buttons.h"
#include "Led.h"
//--------------------
//Boards.
//#include "MB_MotherBoard.h"
#include "SB_SirenBoard.h"

//Отладка 
//#include "MyDebug.h"
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Структура пунктов на странице меню.
typedef struct{
		//---------
    const char *StringName[6];// 
    const char *StringText[6];// 
    uint16_t    StringVar[6]; //
		//---------
}PageItemsSTR_Type;
//--------------------
//Структура одной страницы меню.
typedef	struct{
		//---------
		const char*       NamePage;  //Имя страницы меню.
		PageItemsSTR_Type PageItems; //Структура пунктов на странице меню.
		//---------
}MenuPageSTR_Type;
//--------------------
//Структура переменных для работы с меню.
typedef struct{
  //---------
  uint16_t  IndexMenuPage;//С какой страницей работаем.
  uint16_t  IndexPageItem;//С какой строкой страницы идет работа.
  //---------
}IndexRegMenuSTR_Type;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#define MaxMenuPages  7  //Количество страниц меню.
#define MaxPageItems  6  //Количество пунктов на странице меню.

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
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
typedef struct{
	
	const char *EventID;
	const char *LcControlOn;	
	const char *LcControlOff;

}RusTextSTR_Type;	


//Указатели на русские надписи.
#define RusText_SelfTest_Pointer          1
#define RusText_TESTMode_Pointer          2
#define RusText_LockModeUnit_Pointer      3
#define RusText_ControlModeUnit_Pointer   4  
#define RusText_CofigModeUnit_Pointer     5  
#define RusText_MicNotConnect_Pointer     6
#define RusText_MicConnect_Pointer        7
#define RusText_MicFault_Pointer          8
#define RusText_MicActive_Pointer         9
#define RusText_Address_Pointer           10
#define RusText_Group_Pointer             11
#define RusText_Time_Pointer              12
#define RusText_Date_Pointer              13
#define RusText_ItemMenu_Pointer          14
#define RusText_SaveParam_Pointer         15
#define RusText_ErrorConnectMB_Pointer    16
#define RusText_PageMenuZone_Pointer      17
#define RusText_PojLine_Pointer           18
#define RusText_Zone1_Pointer             19
#define RusText_Zone2_Pointer             20
#define RusText_Zone3_Pointer             21
#define RusText_Zone4_Pointer             22
#define RusText_Zone5_Pointer             23
#define RusText_Zone6_Pointer             24
#define RusText_StatusPoj1_Pointer        25
#define RusText_StatusPoj2_Pointer        26
#define RusText_StatusPoj3_Pointer        27
#define RusText_StatusGOChS_Pointer       28
#define RusText_StatusPOWER_Pointer       29
#define RusText_StatusPA_Pointer          30
#define RusText_Active_Pointer            31
#define RusText_Disactive_Pointer         32
#define RusText_StateNorm_Pointer         33
#define RusText_StateAboveNorm_Pointer    34
#define RusText_StateBelowNorm_Pointer    35
#define RusText_StateBreak_Pointer        36
#define RusText_StateShort_Pointer        37
#define RusText_StateUndef_Pointer        38
#define RusText_LineNotWork_Pointer       39
#define RusText_Mic_Pointer               40
#define RusText_ManualControl_Pointer     41
#define RusText_BatLevel_Pointer          42
#define RusText_MainPower_Pointer         43
#define RusText_PowerPage_Pointer         44
#define RusText_PAProt_Pointer            45
#define RusText_Preset1_Pointer           46
#define RusText_LcDeviation_Pointer       47

#define RusText_PageMenuOpoveshatel_Pointer  48
#define RusText_Opoveshatel1_Pointer  		   49
#define RusText_Opoveshatel2_Pointer  		   50
#define RusText_Opoveshatel3_Pointer  		   51
#define RusText_PitanieOpovesh_Pointer 		   52
#define RusText_NoConnectionSB_Pointer 		   53

//-----------------------------------------------------------------------------
void  MenuInit       (void);
void  MenuLogo       (void);
void  MenuProgressBar(void);
char* MenuGetRusText(uint16_t pointer);

RusTextSTR_Type* RusText(void);

MenuPageSTR_Type*     MenuPage          (uint8_t page);
IndexRegMenuSTR_Type* MenuIndexReg      (void);
void                  MenuIndexRegCleare(void);

MenuPageSTR_Type* MenuBackgroundPage(void);
void              MenuBackgroundPageDisplay(MenuPageSTR_Type *menuPage);

void              MenuTextDisplay(uint8_t ptr1, char* txt, uint8_t str);

void MenuAddressUpdate    (uint8_t position, uint8_t data, uint8_t blink);
void MenuGroupUpdate      (uint8_t position, uint8_t data, uint8_t blink);
void MenuTimeUpdate       (uint8_t position, uint16_t data, uint8_t blink, TimeSTR_Type* time);
void MenuCalendarUpdate   (uint8_t position, uint16_t date, uint8_t blink, TimeSTR_Type* Data);
void MenuInputConfigUpdate(uint8_t numStr, uint8_t digit, FireLine_t* ptr);

void MenuUpdate (MenuPageSTR_Type *menuPage, uint8_t mode, uint8_t pointStr);


void  PrintTestmode  (void);
void  PrintErrConnect(void);
void  PrintMicState  (MenuPageSTR_Type *page);
void  PrintPowerState(void);

char* TextGetBatState  (uint8_t batState);
char* TextGetPowerState(uint8_t  mainPowerState);
char* TextGetAmpState  (uint8_t  ampState);
char* TextGetSirenSate (uint16_t sirenState);

char* MenuGetTextStatLc     (uint8_t line);
char* MenuGetTextStatPojLine(uint8_t line);
//-----------------------------------------------------------------------------
#endif //_MenuLcd12864_H


