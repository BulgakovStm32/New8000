
#ifndef _Siren_H
#define _Siren_H
//*****************************************************************************

#include "adc_ST.h"
#include "gpio_ST.h"

#include "Blink.h"
#include "SB_SirenBoard.h"
//--------------------
//FSM (машина состояний).
#include "SwTimers.h"
#include "FsmMessage.h"
//*****************************************************************************
//*****************************************************************************
#define SIREN_POWER  0 // ADC0
#define SIREN_1      1 // ADC1
#define SIREN_2      2 // ADC2
#define SIREN_3      3 // ADC3
#define SIREN_ALL  ((1<<SIREN_1)|(1<<SIREN_2)|(1<<SIREN_3))

#define SIREN_LINE_NUM	4
//Флаги.
//#define SirenMeasEndFlag   (1<<0)//Окончено определение состояния пожарных шлейфов
//***********************************************
#define SIREN_POWER_MIN	200u //минимальное напряжение питания оповещ-й.
														 //При напряжении ниже этого значения линии не будут подключаться к питанию

#define TIMEOUT_OUTPUT	50	 //Пауза для завершения переходного процесса на линии.
//***********************************************
//Контекст одного входа ШС.
#pragma pack(push, 2)//размер выравнивания в 1 байт
typedef	struct{
	//-------------
	uint16_t AdcMeasValue;
	uint8_t  LogicalState;
	uint8_t  LineState;
	//-------------
}SirenLine_t;
#pragma pack(pop)//вернули предыдущую настройку.
//***********************************************
//***********************************************
#define OUT_STATE_OFF			  0	//Выход отключен.
#define OUT_STATE_OFF_to_ON	1 //Переход из состояния ОТКЛ в состояние ВКЛ
#define OUT_STATE_MEAS_ON   2
#define OUT_STATE_ON			  3	//Выход активен.
#define OUT_STATE_ON_to_OFF	4 //Переход из состояния ВКЛ в состояние ОТКЛ
//***********************************************
//Режимы работы выхода.
#define SIREN_OUT_OFF			0	//Выход отключен.
#define SIREN_OUT_ON		 	1	//Выход активен.
#define SIREN_OUT_Blink1 	2	//Режим МИГАНИЕ-1. 500мС - ON, 500мС - OFF.
#define SIREN_OUT_Blink2 	3	//Режим МИГАНИЕ-2. 250мС - ON, 250мС - OFF.
//--------------------
//Состояния шлейфов.
#define SIREN_LINE_UNDEF  0
#define SIREN_LINE_NORM   1
#define SIREN_LINE_BREAK  2
#define SIREN_LINE_SHORT  3
//*****************************************************************************
//*****************************************************************************
void     Siren_Init    (void);			
void     Siren_MeasLoop(void);           //Измерение и усреднение по 16 выборкам для 5-ти каналов АЦП. Вызывается каждую 1мСек.
uint16_t Siren_GetMeas(uint8_t ch);	     //Получение измерения для канала.
uint8_t  Siren_LineStatus(uint8_t line);
void     Siren_ControlFromMB(uint8_t ch);//Управление выходами от ЦП.
void     Siren_OutputFSM(uint8_t siren, uint8_t cmd);
//*****************************************************************************
//*****************************************************************************
#endif /*_Siren_H*/



