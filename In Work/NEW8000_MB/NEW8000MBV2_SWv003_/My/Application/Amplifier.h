
#ifndef _Amplifier_H
#define _Amplifier_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "gpio_ST.h"

//-----------------------------------------------------------------------------
//Управление и диагностика УМЗЧ. 
//PA_Prot - PA0.
//PA_On   - PA1.
#define PaProtGPOI    GpioAIDR //GPIOA 
#define PaProtPIN     GPIO_IDR_IDR0  

#define PaOnGPOI      GPIOA
#define PaOn         (PaOnGPOI->BSRR = GPIO_BSRR_BS1)
#define PaOff        (PaOnGPOI->BSRR = GPIO_BSRR_BR1)
//--------------------
#define PAMP_OFF  1
#define PAMP_ON   2
#define PAMP_PROT 3
//-----------------------------------------------------------------------------
void    PAmp_Init   (void);
void    PAmp_Control(uint8_t state);
uint8_t PAmp_GetStat(void);
//-----------------------------------------------------------------------------
#endif /*_заготовка_H*/


