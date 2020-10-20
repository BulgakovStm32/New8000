
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

#define LinePoj1                Poj1_AdcCh
#define LinePoj2                Poj2_AdcCh 
#define LinePoj3                Poj3_AdcCh 
#define LineChS                 ChS_AdcCh 
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


#define LcOptoReleyOnTimeOut    1   //Пауза в мС перед включение оптореле какалов LC.
#define LcMeasPeriod            2000 
//--------------------
//#define NormSost          (1<<0)
//#define NormOpenSost      (1<<1)
//#define NormCloseSost     (1<<2)
//#define ShortLineSost     (1<<3)
//#define BreakLineSost     (1<<4)

//#define NormSost          1//(1<<0)
//#define NormOpenSost      2//(1<<1)
//#define NormCloseSost     3//(1<<2)
//#define ShortLineSost     4//(1<<3)
//#define BreakLineSost     5//(1<<4)
//-----------------------------------------------------------------------------
void      AdcInit(void);
void      AdcAverageMeasForLineLoop(void);

uint8_t   AdcAverageMeasForLc(void);

void      AdcLcMeasLoop(void);

uint16_t  GetAdcChannelVolue(uint8_t channel);
uint16_t  GetLcChannelVolue(uint8_t channel);
uint16_t  GetAdcFlagReg(void);
void      ClereAdcFlag(uint16_t flag);
uint8_t   GetLineSost(uint8_t channel);
//-----------------------------------------------------------------------------
#endif /*_adc_ST_HH*/
