
#ifndef _PojLine_H
#define _PojLine_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "adc_ST.h"
//--------------------
//AppLayer.
#include "Rs485.h"
//--------------------
//FSM (машина состояний).
#include "SwTimers.h"
#include "FsmMessage.h"
//-----------------------------------------------------------------------------
//Контекст одного входа ШС.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef	struct{
	//-------------
					 uint8_t  FaultState;
					 uint8_t  LogicalState;  
					 uint8_t  Type;
	         uint8_t  TimeOut;
	volatile uint8_t  PhysicalState;
	volatile uint16_t MeasValue;
	//-------------
}FireLineContextSTR_Type;
#pragma pack(pop)//вернули предыдущую настройку.
//-------------------------------------------------------
//Линии шлейфов пожарной сигнализации
#define Poj1   								0
#define Poj2   								1
#define Poj3   								2
#define ChS    								3
//количество входов ШС.
#define NumberOfFireLines			4
//Время в сек. перехода из ВНИМАНИЕ в ПОЖАР
#define	InputTimeOutMin 			0
#define	InputTimeOutMax 			99
//Режим работы входа.Поле Type контекста входа.
#define	InputOff              0 // вход откл. и не контролируется; 
#define	InputOnControlOnClose	1 // вход вкл. и контролируется, фиксация замыкания СК;
#define	InputOnControlOnOpen  2 // вход вкл. и контролируется, фиксация размыкания СК;

//#define	InputOnControlOff     1 // вход вкл. и не контролируется;
//#define	InputOnControlOnClose	2 // вход вкл. и контролируется, фиксация замыкания СК;
//#define	InputOnControlOnOpen  3 // вход вкл. и контролируется, фиксация размыкания СК;
//#define	InputOnControlOnClOp  4 // вход вкл. и контролируется, фиксация замыкания и размыкания СК;
//Логические состояния Пожарных шлейфов.
#define FireLineLogicMask			0x0F
#define	FireLineCONTROLOFF  	0 //
#define	FireLineUNDERCONTROL  1 //
#define	FireLineATTENTION 		2 //«ВНИМАНИЕ» – зафиксировано срабатывание одного извещателя
#define	FireLineFIRE      		3 //«ПОЖАР»    – вход переходит в это состояние через в случае удержания сработки в течении TimeOut. TimeOut задается в настройках входа.
//Физические состояния Пожарных шлейфов.
#define FireLinePhysMask			0xF0
#define FireLineNorm 			   (0<<4)
#define FireLineUndef 			 (1<<4)
#define FireLineNormOpen  	 (2<<4)
#define FireLineNormClose 	 (3<<4)
#define FireLineShort 			 (4<<4)
#define FireLineBreak     	 (5<<4)
//----------------------------------------
//Флаги
//typedef struct{
//	uint8_t MeasEndFlag :1;
//	uint8_t :7;
//}PojLineFlagsSTR_Type;
//#define FlagClr	0
//#define FlagSet	1
//-----------------------------------------------------------------------------
void 									FireLineInit    (void);
void                  FireLineMeasLoop(void);
//PojLineFlagsSTR_Type* FireLineFlags   (void);
uint16_t 							FireLineGetValue(uint8_t line);
uint8_t 		   			  FireLineGetPhysicalState(uint8_t line);
uint8_t 							FireLineGetLogicState(uint8_t line);

void                     FireLineLogicFSM(uint8_t input);
void                     FireLineFaultFSM(uint8_t input);
FireLineContextSTR_Type* FireLine        (uint8_t input);
//-----------------------------------------------------------------------------
#endif /*_PojLine_H*/



