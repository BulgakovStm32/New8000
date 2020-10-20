
#ifndef _adc_ST_H
#define _adc_ST_H
//-----------------------------------------------------------------------------

//#include "stm32f10x.h"
#include "stm32f0xx.h"

//-----------------------------------------------------------------------------
//#define SirenPow_AdcCh    0 // ADC0
//#define Siren1_AdcCh      1 // ADC1
//#define Siren2_AdcCh      2 // ADC2
//#define Siren3_AdcCh      3 // ADC3

#define ADC_Vref				 (3300 * 1000) //Опорное напряжение.
#define ADC_RESOLUTION    12 			     //Native ADC Resolution, bits
#define ADC_QUANT        ((ADC_Vref + ((1 << ADC_RESOLUTION)/2)) / (1 << ADC_RESOLUTION))

#define ADC_MEAS_NUM		 16 //Количество измерений АЦП
#define ADC_MEAS_SHIFT   4  //Количество сдвигов для деления.
//-----------------------------------------------------------------------------
void     Adc_Init(void);					//
//uint32_t Adc_GetState(void);
uint16_t Adc_GetMeas(uint8_t ch);//Одно измерение АЦП.
void 		 Adc_Start(void);
void     Adc_Stop(void);
//-----------------------------------------------------------------------------
#endif /*_adc_ST_HH*/
