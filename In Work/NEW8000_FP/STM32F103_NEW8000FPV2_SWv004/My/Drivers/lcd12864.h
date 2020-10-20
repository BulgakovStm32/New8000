
#ifndef _lcd12864_H
#define _lcd12864_H

//-----------------------------------------------------------------------------

#include "stm32f10x.h"

//-----------------------------------------------------------------------------
//Сигналы DB0-DB7.
#define Lcd12864DataPort	  GPIOA
#define Lcd12864DataSet(x)	Lcd12864DataPort->ODR = (Lcd12864DataPort->ODR & 0xFF00) | (uint16_t)(x)
//Сигнал LED.
#define Lcd12864LedOn     	(GPIOB->BSRR = GPIO_BSRR_BS11) 
#define Lcd12864LedOff    	(GPIOB->BSRR = GPIO_BSRR_BR11) 
#define Lcd12864LedToggel 	(GPIOB->ODR  = GPIOB->ODR ^ GPIO_ODR_ODR11)
//Сигнал D/I.
#define Lcd12864SetData    	(GPIOB->BSRR = GPIO_BSRR_BS2)
#define Lcd12864SetCmd     	(GPIOB->BSRR = GPIO_BSRR_BR2) 
//Сигнал R/W.
#define Lcd12864Read     		(GPIOB->BSRR = GPIO_BSRR_BS1) 
#define Lcd12864Write    		(GPIOB->BSRR = GPIO_BSRR_BR1)
//Сигнал E.
#define Lcd12864EHight     	(GPIOB->BSRR = GPIO_BSRR_BS10) 
#define Lcd12864ELow    		(GPIOB->BSRR = GPIO_BSRR_BR10)
//Сигнал CS1.
#define Lcd12864CS1On     	(GPIOC->BSRR = GPIO_BSRR_BS4) 
#define Lcd12864CS1Off   		(GPIOC->BSRR = GPIO_BSRR_BR4)
//Сигнал CS2.
#define Lcd12864CS2On     	(GPIOC->BSRR = GPIO_BSRR_BS5) 
#define Lcd12864CS2Off   		(GPIOC->BSRR = GPIO_BSRR_BR5)

#define Lcd12864CS1()     Lcd12864CS2Off; Lcd12864CS1On
#define Lcd12864CS2()     Lcd12864CS1Off; Lcd12864CS2On

#define Lcd12864CS12()    Lcd12864CS1On; Lcd12864CS2On
#define Lcd12864CS12Off() Lcd12864CS1Off; Lcd12864CS2Off

//Сигнал /RES.
#define Lcd12864ResHiht  		(GPIOB->BSRR = GPIO_BSRR_BS0) 
#define Lcd12864ResLow   		(GPIOB->BSRR = GPIO_BSRR_BR0)
//---------------------
//Биты регистра STATUS.
#define LCD12864StatReg_BUSYbit   (1<<7)
#define LCD12864StatReg_ON_OFFbit (1<<5)
#define LCD12864StatReg_RESETbit  (1<<4)
//---------------------
#define LCD12864PauseConst 20 //

#define LCD12864_CMD    0
#define LCD12864_DATA   1

#define Off 0
#define On  1
//-----------------------------------------------------------------------------
#define LCD12864_X_RES       128	 //разрешение экрана    
#define LCD12864_Y_RES       64
#define LCD12864_CACHE_SIZE  ((LCD12864_X_RES * LCD12864_Y_RES) / 8)//Видео буфера.

#define	LCD12864_X0	0xb8  // 
#define	LCD12864_Y0	0x40  //
//-----------------------------------------------------------------------------
void     Lcd12864_Send(uint8_t data, uint8_t type);
void     Lcd12864_Init     (void);
void     Lcd12864_BackLight(uint8_t state);	
uint8_t* Lcd12864_VideoBuf (void);
uint8_t  Lcd12864_Update   (void);
void     Lcd12864_UpdateAll(void);
//-----------------------------------------------------------------------------
#endif //_lcd12864_H


