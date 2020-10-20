
#ifndef _adc_ST_H
#define _adc_ST_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "gpio_ST.h"

//-----------------------------------------------------------------------------
#define Poj1_AdcCh        0
#define Poj2_AdcCh        1
#define Poj3_AdcCh        2
#define ChS_AdcCh         3
#define LC_AdcCh          4

#define AdcMeasQuantity   16
#define AdcMeasShiftDev   4

#define AdcAllChConvertionEndFlag           (1<<0)
#define AdcAllChLCConvertionEndFlag         (1<<1)
#define AdcAverageMeasForLineLoopBusyFlag   (1<<2)
#define AdcAverMeasForLineLoopReadyFlag     (1<<3)
//--------------------

#define Poj1                Poj1_AdcCh
#define Poj2                Poj2_AdcCh 
#define Poj3                Poj3_AdcCh 
#define ChS                 ChS_AdcCh 
//#define Lc1Line                 4
//#define Lc1Atten                5
//#define Lc2Line                 6
//#define Lc2Atten                7
#define AllLineAlrms            8

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

#define NormSost          (1<<0)
#define NormOpenSost      (1<<1)
#define NormCloseSost     (1<<2)
#define ShortLineSost     (1<<3)
#define BreakLineSost     (1<<4)
//-----------------------------------------------------------------------------
void     AdcInit(void);
void     AdcAverageMeasForPojLineLoop(void);

uint16_t AdcGetChMeas (uint8_t ch);
uint16_t AdcGetLcMeas (uint8_t ch);

uint16_t AdcGetFlagReg (void);
void     AdcClereFlag  (uint16_t flag);

uint8_t  AdcAverageMeasForLc(void);
void     AdcLcMeasLoop      (void);

uint8_t  PojLineGetState(uint8_t ch);
//-----------------------------------------------------------------------------
#endif /*_adc_ST_HH*/
