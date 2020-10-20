
#ifndef _gpio_ST_H
#define _gpio_ST_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

//-----------------------------------------------------------------------------
#define Gpio_mSConst   50//10

#define GpioAIDR       1
#define GpioBIDR       2
#define GpioCIDR       3 

//JQ6500/
#define JqK1Hight     (GPIOC->BSRR = GPIO_BSRR_BS9) 
#define JqK1Low    	  (GPIOC->BSRR = GPIO_BSRR_BR9) 

#define JqK2Hight     (GPIOA->BSRR = GPIO_BSRR_BS8) 
#define JqK2Low    	  (GPIOA->BSRR = GPIO_BSRR_BR8) 
//Двухцветный светодио LC1_SOST. PB1 - LC1_SOST_Red. PB2 - LC1_SOST_Green.
#define LC1SostRedLedOn()     	(GPIOB->BSRR = GPIO_BSRR_BS1) 
#define LC1SostRedLedOff()    	(GPIOB->BSRR = GPIO_BSRR_BR1) 
#define LC1SostRedLedToggel() 	(GPIOB->ODR  = GPIOB->ODR ^ GPIO_ODR_ODR1)

#define LC1SostGreenLedOn()     (GPIOB->BSRR = GPIO_BSRR_BS2) 
#define LC1SostGreenLedOff()    (GPIOB->BSRR = GPIO_BSRR_BR2) 
#define LC1SostGreenLedToggel() (GPIOB->ODR  = GPIOB->ODR ^ GPIO_ODR_ODR2)

//Двухцветный светодио LC2_SOST. PA2 - LC2_SOST_Red. PB0 - LC2_SOST_Green.
#define LC2SostRedLedOn()     	(GPIOA->BSRR = GPIO_BSRR_BS7) 
#define LC2SostRedLedOff()    	(GPIOA->BSRR = GPIO_BSRR_BR7) 
#define LC2SostRedLedToggel() 	(GPIOA->ODR  = GPIOA->ODR ^ GPIO_ODR_ODR7)

#define LC2SostGreenLedOn()     (GPIOB->BSRR = GPIO_BSRR_BS0) 
#define LC2SostGreenLedOff()    (GPIOB->BSRR = GPIO_BSRR_BR0) 
#define LC2SostGreenLedToggel() (GPIOB->ODR  = GPIOB->ODR ^ GPIO_ODR_ODR0)
 
//Оптореле LC. 
//PC10 - OptSP2Att.
//PC11 - OptSP2Line.
//PC12 - OptSP1Line.
//PD2  - OptSP1Att.
#define OptSP1AttOn()     (GPIOD->BSRR = GPIO_BSRR_BS2)
#define OptSP1AttOff()    (GPIOD->BSRR = GPIO_BSRR_BR2)
#define OptSP1AttToggel() (GPIOD->ODR  = (GPIOD->ODR ^ GPIO_ODR_ODR2))

#define OptSP1LineOn()     (GPIOC->BSRR = GPIO_BSRR_BS12)
#define OptSP1LineOff()    (GPIOC->BSRR = GPIO_BSRR_BR12)
#define OptSP1LineToggel() (GPIOC->ODR  = (GPIOC->ODR ^ GPIO_ODR_ODR12))

#define OptSP2LineOn()     (GPIOC->BSRR = GPIO_BSRR_BS11)
#define OptSP2LineOff()    (GPIOC->BSRR = GPIO_BSRR_BR11)
#define OptSP2LineToggel() (GPIOC->ODR  = (GPIOC->ODR ^ GPIO_ODR_ODR11))

#define OptSP2AttOn()      (GPIOC->BSRR = GPIO_BSRR_BS10) 
#define OptSP2AttOff()     (GPIOC->BSRR = GPIO_BSRR_BR10) 
#define OptSP2AttToggel()  (GPIOC->ODR  = (GPIOC->ODR ^ GPIO_ODR_ODR10))

//#define OptSPAllOff      (GPIOB->ODR  = (GPIOB->ODR & ~( GPIO_ODR_ODR3 | GPIO_ODR_ODR4 | GPIO_ODR_ODR5 | GPIO_ODR_ODR6 )))
//Управление мультиплексором DA5 - аудиосигнал приоритетов. 
#define MuxEHight        (GPIOB->BSRR = GPIO_BSRR_BS3) 
#define MuxELow    	     (GPIOB->BSRR = GPIO_BSRR_BR3) 

#define MuxS0Hight       (GPIOB->BSRR = GPIO_BSRR_BS7) 
#define MuxS0Low    	   (GPIOB->BSRR = GPIO_BSRR_BR7)  

#define MuxS1Hight       (GPIOB->BSRR = GPIO_BSRR_BS8) 
#define MuxS1Low    	   (GPIOB->BSRR = GPIO_BSRR_BR8)  

#define MuxS2Hight       (GPIOB->BSRR = GPIO_BSRR_BS9) 
#define MuxS2Low    	   (GPIOB->BSRR = GPIO_BSRR_BR9)        

//Управление мультиплексором DA7 - аудиосигнал платы мультимедия.
#define Mux1S0Hight      (GPIOB->BSRR = GPIO_BSRR_BS4) 
#define Mux1S0Low    	   (GPIOB->BSRR = GPIO_BSRR_BR4)  

#define Mux1S1Hight      (GPIOB->BSRR = GPIO_BSRR_BS5) 
#define Mux1S1Low    	   (GPIOB->BSRR = GPIO_BSRR_BR5)  

#define Mux1S2Hight      (GPIOB->BSRR = GPIO_BSRR_BS6) 
#define Mux1S2Low    	   (GPIOB->BSRR = GPIO_BSRR_BR6) 
                                           
//Управление сдвиговыми регистрами. PC7(STPIC_CS), PC8(STPIC_CLR).
#define StpicCSHight     (GPIOC->BSRR = GPIO_BSRR_BS7) 
#define StpicCSLow    	 (GPIOC->BSRR = GPIO_BSRR_BR7) 

#define StpicCLRHight    (GPIOC->BSRR = GPIO_BSRR_BS8) 
#define StpicCLRLow    	 (GPIOC->BSRR = GPIO_BSRR_BR8) 
//-----------------------------------------------------------------------------
void     GpioInit     (void);
void     GpioCheckLoop(void);
uint16_t GpioGetState (uint8_t port);

//-----------------------------------------------------------------------------
#endif /*_gpio_ST_H*/


