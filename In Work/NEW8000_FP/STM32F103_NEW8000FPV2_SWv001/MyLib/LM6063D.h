
#ifndef _LM6063D_H
#define _LM6063D_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"
#include "spi_ST.h"

//-----------------------------------------------------------------------------
//#define Lm6063LcdPort	   GPIOA

//Сигнал A1.
// A1=H - transferring the Display data.
// A1=L - transferring the Control data.
#define Lm6063LcdA1Hight   	(GPIOB->BSRR = GPIO_BSRR_BS5) 
#define Lm6063LcdA1Low    	(GPIOB->BSRR = GPIO_BSRR_BR5)

//Сигнал /RES.
// /RES=L - initialization is executed.
// /RES=H - normal running.
#define Lm6063LcdResHight   (GPIOB->BSRR = GPIO_BSRR_BS6) 
#define Lm6063LcdResLow    	(GPIOB->BSRR = GPIO_BSRR_BR6)

//Сигнал /CS.
// /CS=L - Enable access to LCD module.
// /CS=H - Disable access to LCD module.
#define Lm6063LcdCsHight   (GPIOB->BSRR = GPIO_BSRR_BS7) 
#define Lm6063LcdCsLow   	 (GPIOB->BSRR = GPIO_BSRR_BR7)

//Сигнал LED.
//#define Lm6063LcdLedOn     (GPIOB->BSRR = GPIO_BSRR_BS3) 
//#define Lm6063LcdLedOff    (GPIOB->BSRR = GPIO_BSRR_BR3)
//#define Lm6063LcdLedToggel (GPIOB->ODR  = GPIOB->ODR ^ GPIO_ODR_ODR3)

#define Lm6063Cmd     0
#define Lm6063Data    1

#define Lm6063_X_RES  128	 //разрешение экрана    
#define Lm6063_Y_RES  64

#define Lm6063_CACHE_SIZE  (Lm6063_X_RES*Lm6063_Y_RES)/8  //
//-----------------------------------------------------------------------------

void Lm6063LcdGpioInit(void);
void Lm6063LcdSendByte(unsigned char data, unsigned char cmd);
void Lm6063LcdUpdate  (void);
void Lm6063LcdFillingScreen(uint8_t byte);

void Lm6063LcdInit    (void);
void Lm6063LcdBmp     (unsigned char *puts);

//-----------------------------------------------------------------------------
#endif //_LM6063D_H



