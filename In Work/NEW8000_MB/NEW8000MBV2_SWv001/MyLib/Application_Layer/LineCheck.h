
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
#define LcDeviationConst                     1   //Отклонение в мВ от установившегося значения
                                                 //для фиксации переходного процесс на линии.
#define LcDeviationEndTransitionProcessConst 1  

#define LcErrorThresholdConst                10  //

#define LcLedBlinkPperiodConst               100 //Интервал в мС изменения состояни сетодиода
                                                 //во время переходного процесса.
//--------------------
//Флаги.
#define Lc1SaveRefLineFlag    (1<<0)//
#define Lc1SaveRefAttFlag     (1<<1)
#define Lc2SaveRefLineFlag    (1<<2)
#define Lc2SaveRefAttFlag     (1<<3)
//--------------------
#define Lc1Line       0
#define Lc1Atten      1
#define Lc2Line       2
#define Lc2Atten      3

#define LineShorThresholdConst  100  //Порого в мВ, ниже которого фиксируется КЗ на линии Гр.
#define LineBreakThresholdConst 2650 //Порого в мВ, выше которого фиксируется обрыв линии Гр.

//Состояния линий Гр.
#define	LineMask		  0xf0
#define LineNorm     (1<<4) 
#define LineUndef    (2<<4)//Состояние не определено - идет переходной процесс.
#define LineBreak    (3<<4)
#define LineOver     (4<<4)
#define LineUnder    (5<<4)
#define LineShort    (6<<4)
//-----------------------------------------------------------------------------
void     LcInit           (void);
void     LcSetDeviation   (uint16_t dev);//
uint16_t LcGetDeviation   (void);
void     LcSteadyStateLoop(void);        //Получение установившегося значения после переходного процесса.
uint16_t LcGetMeas        (uint8_t ch);  //Получение измерения для канала.
uint8_t  LcSaveRef        (uint8_t ch);  //Сохранение опорных величин линий Гр. дя LC.
uint8_t  LcGetStatus      (uint8_t ch);  //
uint8_t  LcGetDefStatus   (uint8_t ch);  //
//-----------------------------------------------------------------------------
#endif /*_LineCheck_H*/



