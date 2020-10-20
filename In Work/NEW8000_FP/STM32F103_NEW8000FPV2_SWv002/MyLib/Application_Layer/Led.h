
#ifndef _Led_H
#define _Led_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"
#include "spi_ST.h"
#include "sm1628.h"

#include "LineCheck.h"
#include "Relay.h"
#include "MenuLcd12864.h"
//-----------------------------------------------------------------------------
//Управление светодиодами.
#define LedOff        0
#define LedOn         1 //(1 << 0)
#define GreenColor	  1 //(1 << 0)
#define RedColor      2 //(1 << 1)
#define YellowColor   3 //(1 << 2)

#define LedAllON      0xFF
#define LedAllOFF     0
//Номер бита светодиода в структуре.
#define PRESET8_LED             0
//#define PRESET8_RED_LED         0
//#define PRESET8_GREEN_LED       1

#define PRESET7_LED             2
//#define PRESET7_RED_LED         2
//#define PRESET7_GREEN_LED       3

#define PRESET6_LED             4
//#define PRESET6_RED_LED         4
//#define PRESET6_GREEN_LED       5

#define PRESET5_LED             6
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

#define PRESET4_LED             32
//#define PRESET4_RED_LED         32
//#define PRESET4_GREEN_LED       33

#define PRESET3_LED             34
//#define PRESET3_RED_LED         34
//#define PRESET3_GREEN_LED       35

#define PRESET2_LED             36
//#define PRESET2_RED_LED         36
//#define PRESET2_GREEN_LED       37

#define PRESET1_LED             38
//#define PRESET1_RED_LED         38
//#define PRESET1_GREEN_LED       39

#define TEST_LED                48
#define ZUMMER_LED              49
#define RESET_LED               50
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
//-----------------------------------------------------------------------------
void LedInit         (void);
void LedUpdate       (void);
void LedSetAll       (uint8_t state);
void LedControl      (uint8_t ledBit, uint8_t state);

void LedStart        (uint8_t state);
void LedPoj          (uint8_t state);
void LedCommunication(uint8_t state);
void LedPower        (uint8_t state);
void LedBattery      (uint8_t state);
void LedAmplifier    (uint8_t state);

void LedPresetControl(uint8_t led, uint8_t line, uint8_t zone, uint16_t blink);

//-----------------------------------------------------------------------------
#endif //_Led_H







