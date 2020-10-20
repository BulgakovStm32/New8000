
#ifndef _AnalogSwitch_H
#define _AnalogSwitch_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

//-----------------------------------------------------------------------------
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
//--------------------------
typedef enum {
	MIC = 0,	
	MULTIMEDIA,
	LIN_IN_1,
	LIN_IN_2,
	PRIOR_5,
	VOICE_MESSAGE,
	GND,
}AnSwitchPriorityEnum_t;
//-----------------------------------------------------------------------------
void AnalogSwitch_Init(void);
void AnalogSwitch_Activate(void);
void AnalogSwitch_Deactivate(void);
void AnalogSwitch_Prior_Set(AnSwitchPriorityEnum_t ch);
//-----------------------------------------------------------------------------
#endif /*_AnalogSwitch_H*/


