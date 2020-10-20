
#ifndef _gpio_ST_H
#define _gpio_ST_H
//-----------------------------------------------------------------------------

//#include "stm32f10x.h"
#include "stm32f0xx.h"

//-----------------------------------------------------------------------------
//Cветодиод ACT. Порт PA4.
#define ACTLedOn()     	(GPIOA->BSRR = GPIO_BSRR_BS_4) 
#define ACTLedOff()    	(GPIOA->BSRR = GPIO_BSRR_BR_4) 
#define ACTLedToggel() 	(GPIOA->ODR  = GPIOA->ODR ^ GPIO_ODR_4)

//Управление реле. 
//РелОП1 - PA7
//РелОП2 - PA6
//РелОП3 - PA5
#define RelSiren1On()     	(GPIOA->BSRR = GPIO_BSRR_BS_7) 
#define RelSiren1Off()    	(GPIOA->BSRR = GPIO_BSRR_BR_7) 
#define RelSiren1Toggel() 	(GPIOA->ODR  = GPIOA->ODR ^ GPIO_ODR_7)
#define RelSiren1OutState()	(GPIOA->IDR  = GPIOA->IDR & GPIO_IDR_7)

#define RelSiren2On()     	(GPIOA->BSRR = GPIO_BSRR_BS_6) 
#define RelSiren2Off()    	(GPIOA->BSRR = GPIO_BSRR_BR_6) 
#define RelSiren2Toggel() 	(GPIOA->ODR  = GPIOA->ODR ^ GPIO_ODR_6)
#define RelSiren2OutState()	(GPIOA->IDR  = GPIOA->IDR & GPIO_IDR_6)

#define RelSiren3On()     	(GPIOA->BSRR = GPIO_BSRR_BS_5) 
#define RelSiren3Off()    	(GPIOA->BSRR = GPIO_BSRR_BR_5) 
#define RelSiren3Toggel() 	(GPIOA->ODR  = GPIOA->ODR ^ GPIO_ODR_5)
#define RelSiren3OutState()	(GPIOA->IDR  = GPIOA->IDR & GPIO_IDR_5)
//-----------------------------------------------------------------------------
void Gpio_Init(void);
void Led_ACT_Set(uint8_t state);
//-----------------------------------------------------------------------------
#endif /*_gpio_ST_H*/


