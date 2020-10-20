
#ifndef _LineCheck_H
#define _LineCheck_H
//-----------------------------------------------------------------------------

#include "adc_ST.h"

//-----------------------------------------------------------------------------
#define Lc1LineAfterPowerOn   0
#define Lc1AttenAfterPowerOn  1
#define Lc2LineAfterPowerOn   2
#define Lc2AttenAfterPowerOn  3

#define Lc1LineSteady         4
#define Lc1AttenSteady        5
#define Lc2LineSteady         6
#define Lc2AttenSteady        7
//--------------------
#define LcDeviationConst                     1//5  //Отклонение в мВ от установившегося значения
                                                 //для фиксации переходного процесс на линии.
#define LcDeviationEndTransitionProcessConst 1//4

#define LcErrorThresholdConst           10//65
//--------------------
#define LcLedBlinkPperiodConst          150  //Интервал в мС изменения состояни сетодиода
                                             //во время переходного процесса.
//--------------------
#define Lc1Line       0
#define Lc1Atten      1
#define Lc2Line       2
#define Lc2Atten      3

#define LineShorThresholdConst  100  //Порого в мВ, ниже которого фиксируется КЗ на линии Гр.
#define LineBreakThresholdConst 3000 //Порого в мВ, выше которого фиксируется обрыв линии Гр.

#define LineBreak     1 //(1<<0)
#define LineAbove     2 //(1<<1)
#define LineNorm      3 //(1<<2)
#define LineBelow     4 //(1<<3)
#define LineShort     5 //(1<<4)
#define LineUndef     6 //(1<<5) //Состояние не определено - идет переходной процесс.
//-----------------------------------------------------------------------------
void     LcInit          (void);
void     LcSaveRefState  (void);
void     LcGetSteadyState(void);

uint16_t LcGetStateMeas  (uint8_t ch);
uint8_t  LcGetState      (uint8_t ch);
//-----------------------------------------------------------------------------
#endif /*_LineCheck_H*/



