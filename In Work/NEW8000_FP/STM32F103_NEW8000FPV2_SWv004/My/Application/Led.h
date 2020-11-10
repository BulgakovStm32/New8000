
#ifndef _Led_H
#define _Led_H
//******************************************************************************************
#include "spi_ST.h"
#include "sm1628.h"
//********************
//AppLayer.
#include "Blink.h"
//********************
//Boards.
#include "MB_MotherBoard.h"
#include "SB_SirenBoard.h"
//******************************************************************************************
//******************************************************************************************
//Управление светодиодами.
#define LedOff        0
#define LedOn         1 //(1 << 0)
#define GreenColor	  1 //(1 << 0)
#define RedColor      2 //(1 << 1)
#define YellowColor   3 //(1 << 2)

#define LedAllOn      0xFF
#define LedAllOff     0
//Номер бита светодиода в структуре.
#define Alg8Led             0
//#define PRESET8_RED_LED         0
//#define PRESET8_GREEN_LED       1

#define Alg7Led             2
//#define PRESET7_RED_LED         2
//#define PRESET7_GREEN_LED       3

#define Alg6Led             4
//#define PRESET6_RED_LED         4
//#define PRESET6_GREEN_LED       5

#define Alg5Led             6
//#define PRESET5_RED_LED         6
//#define PRESET5_GREEN_LED       7

#define Communication_GREEN_LED 16
#define Communication_RED_LED   17
#define Amplifier_GREEN_LED     18
#define Amplifier_RED_LED       19
#define Battery_GREEN_LED       20
#define Battery_RED_LED         21
#define Power_GREEN_LED         22
#define Power_RED_LED           23

#define Alg4Led             32
//#define PRESET4_RED_LED         32
//#define PRESET4_GREEN_LED       33

#define Alg3Led             34
//#define PRESET3_RED_LED         34
//#define PRESET3_GREEN_LED       35

#define Alg2Led             36
//#define PRESET2_RED_LED         36
//#define PRESET2_GREEN_LED       37

#define Alg1Led             38
//#define PRESET1_RED_LED         38
//#define PRESET1_GREEN_LED       39

#define TEST_LED                48
#define ZUMMER_LED              49
#define ALERT_LED               50
#define START_LED               51
#define FIRE_LED                52
#define MIC_GREEN_LED           53
#define MIC_RED_LED             54

#define	GENERAL_FAULT_LED       55
#define ALERT_LINES_FAUL_LED    56
#define FIRE_LINES_FAUL_LED     57

#define	Communication_LED	      16
#define	Amplifier_LED		        18
#define	Battery_LED		          20
#define	Power_LED			          22
#define	MIC_LED                 53  
//******************************************************************************************
//******************************************************************************************
void Led_Update (void);
void Led_SetAll (uint8_t state);
void Led_Control(uint8_t ledBit, uint8_t state);

void Led_Mic   (uint8_t state);
void Led_Pusk  (uint8_t state);
void Led_Poj   (uint8_t state);
void Led_Alert (uint8_t state);
void Led_Zummer(uint8_t state);

void Led_Link (uint8_t state);
void Led_Power(PowerSTR_t* power);
void Led_Bat  (uint8_t batState);
void Led_Amp  (uint8_t ampState);

void Led_Algorithm(uint8_t led, uint8_t line, uint8_t spLine, BlinkIntervalEnum_t blinkPeriod);
//******************************************************************************************
//******************************************************************************************
#endif //_Led_H







