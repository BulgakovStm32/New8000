
#ifndef _adc_ST_H
#define _adc_ST_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "gpio_ST.h"

//-----------------------------------------------------------------------------
#define AdcRef						3300UL //Опорное напряжение
#define AdcQuant					((AdcRef * 10000UL) / 4096UL)

#define AdcMeasQuantity   16
#define AdcMeasShiftDev   4

#define Poj1_AdcCh        0
#define Poj2_AdcCh        1
#define Poj3_AdcCh        2
#define ChS_AdcCh         3
#define ZU_AdcCh          4
#define LC_AdcCh          5

///Флаги.
#define MeasPojLineEndFlag   (1<<0)//Окончено определение состояния пожарных шлейфов
#define MeasLcEndFlag        (1<<1)//Окончено определение состояния линий Гр.
//--------------------
//Пожарные шлейфы.
#define Poj1Meas   Poj1_AdcCh
#define Poj2Meas   Poj2_AdcCh 
#define Poj3Meas   Poj3_AdcCh 
#define ChSMeas    ChS_AdcCh 
#define ZUMeas     ZU_AdcCh 

#define Poj1       Poj1_AdcCh
#define Poj2       Poj2_AdcCh 
#define Poj3       Poj3_AdcCh 
#define ChS        ChS_AdcCh 
#define ZU         ZU_AdcCh 

//#define Lc1Line               4
//#define Lc1Atten              5
//#define Lc2Line               6
//#define Lc2Atten              7
#define AllLineAlrms            8
//--------------------
//Линии Гр.
#define Lc1LineValue            0
#define Lc1AttenValue           1
#define Lc2LineValue            2
#define Lc2AttenValue           3

#define Lc1LineOldValue         4
#define Lc1AttenOldValue        5
#define Lc2LineOldValue         6
#define Lc2AttenOldValue        7

#define LcOptoReleyOnTimeOut    2    //Пауза в мС перед включение оптореле какалов LC.
#define LcMeasPeriod            2000 //Период переключения между каналами измерения LC.
//--------------------
//Пожарные шлейфы.
#define NormSost          1//(1<<0)
#define NormOpenSost      2//(1<<1)
#define NormCloseSost     3//(1<<2)
#define ShortLineSost     4//(1<<3)
#define BreakLineSost     5//(1<<4)
//-----------------------------------------------------------------------------
//Аппатартный уровень.
void     AdcInit(void);
uint16_t AdcGetMeas(uint8_t channel);
//------------------------
//Прикладной уровень.
uint16_t MeasFlagGet(void);         //Флаги состояний.
void     MeasFlagSet(uint16_t flag);//
void     MeasFlagClr(uint16_t flag);//

void     PojLineMeasLoop (void);
uint8_t  PojLineStatus   (uint16_t meas);

uint8_t  PojLineGetStatus(uint8_t line);
uint16_t PojLineGetMeas  (uint8_t ch);
uint16_t BatLevel        (void);

void     LcMeasLoop  (void);
uint8_t  LcGetAvrMeas(void);
uint16_t LcGetValue  (uint8_t line);
//-----------------------------------------------------------------------------
#endif /*_adc_ST_HH*/
