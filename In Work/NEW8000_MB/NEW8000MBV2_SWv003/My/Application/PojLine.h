
#ifndef _PojLine_H
#define _PojLine_H
//********************************************************************************************

#include "stm32f10x.h"

#include "adc_ST.h"

//--------------------
//FSM (машина состояний).
#include "SwTimers.h"
#include "FsmMessage.h"
//********************************************************************************************
//********************************************************************************************
//********************************************************************************************
//Контекст одного входа ШС.
#pragma pack(push, 2)//размер выравнивания в 1 байт
typedef	struct{
	//-------------
					 uint16_t Number;
					 uint16_t FaultState;
					 uint16_t LogicalState;  
					 uint16_t Type;
	         uint16_t TimeOut;
	volatile uint16_t PhysicalState;
	volatile uint16_t MeasValue;
	//-------------
}FireLine_t;
#pragma pack(pop)//вернули предыдущую настройку.
#define FIRE_LINES_CONTEXT_SIZE	((uint8_t)sizeof(FireLine_t))
//*******************************************************
//Линии шлейфов пожарной сигнализации
#define POJ_1  0
#define POJ_2  1
#define POJ_3  2
#define ChS    3
//количество входов ШС.
#define FIRE_LINES_NUMBER		4
//Время в сек. перехода из ВНИМАНИЕ в ПОЖАР
#define	FIRE_LINES_TIMEOUT_MIN 	0
#define	FIRE_LINES_TIMEOUT_MAX	99
//Режим работы входа.Поле Type контекста входа.
#define	FIRE_LINES_TYPE_OFF             		0 // вход откл. и не контролируется; 
#define	FIRE_LINES_TYPE_ON_CONTROL_ON_CLOSE	1 // вход вкл. и контролируется, фиксация замыкания СК;
#define	FIRE_LINES_TYPE_ON_CONTROL_ON_OPEN  2 // вход вкл. и контролируется, фиксация размыкания СК;

//#define	InputOnControlOff     1 // вход вкл. и не контролируется;
//#define	InputOnControlOnClose	2 // вход вкл. и контролируется, фиксация замыкания СК;
//#define	InputOnControlOnOpen  3 // вход вкл. и контролируется, фиксация размыкания СК;
//#define	InputOnControlOnClOp  4 // вход вкл. и контролируется, фиксация замыкания и размыкания СК;
//Логические состояния Пожарных шлейфов.
#define FIRE_LINES_LOGIC_MASK		0x0F
#define	FIRE_LINES_CONTROL_OFF  0 //
#define	FIRE_LINES_UNDERCONTROL 1 //
#define	FIRE_LINES_ATTENTION 		2 //«ВНИМАНИЕ» – зафиксировано срабатывание одного извещателя
#define	FIRE_LINES_FIRE      		3 //«ПОЖАР»    – вход переходит в это состояние через в случае удержания сработки в течении TimeOut. TimeOut задается в настройках входа.
//Физические состояния Пожарных шлейфов.
#define FIRE_LINES_PHYS_MASK	  0xF0
#define FIRE_LINES_NORM 			 (0<<4)
#define FIRE_LINES_UNDEF 			 (1<<4)
#define FIRE_LINES_NORM_OPEN 	 (2<<4)
#define FIRE_LINES_NORM_CLOSE	 (3<<4)
#define FIRE_LINES_SHORT 			 (4<<4)
#define FIRE_LINES_BREAK     	 (5<<4)
//*******************************************************
#define	DEFAULT_FIRE_LINES_INPUT_TIMEOUT	3
#define	DEFAULT_FIRE_LINES_INPUT_TYPE			FIRE_LINES_TYPE_ON_CONTROL_ON_CLOSE
//*****************************************************************************
void 			FireLine_Init    (void);
void      FireLine_MeasLoop(void);
void			FireLine_FSMLoop (void);

FireLine_t* FireLine(uint8_t line);
uint8_t	    FireLine_GetPhysicalState(uint8_t line);
void        FireLine_GetContext(uint8_t *pBuf ,uint8_t numLine);
//********************************************************************************************
//********************************************************************************************
//********************************************************************************************
#endif /*_PojLine_H*/



