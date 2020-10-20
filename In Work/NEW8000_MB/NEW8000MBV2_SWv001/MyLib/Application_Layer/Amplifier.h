
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
#define AmpOff   1
#define AmpOn    2
#define AmpProt  3
//-----------------------------------------------------------------------------
void    AmpInit   (void);
void    AmpControl(uint8_t state);
uint8_t AmpGetStat(void);
//-----------------------------------------------------------------------------
#endif /*_заготовка_H*/


