
#ifndef _gpio_ST_H
#define _gpio_ST_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

//-----------------------------------------------------------------------------
//Замок
#define KeyGPIO           GPIOC
#define Key1Pin           GPIO_IDR_IDR1 
#define Key2Pin           GPIO_IDR_IDR0 
#define Key3Pin           GPIO_IDR_IDR15 

#define	Key1              (KeyGPIO->IDR & Key1Pin)
#define	Key2              (KeyGPIO->IDR & Key2Pin)
#define	Key3              (KeyGPIO->IDR & Key3Pin)
//Положение ключа - Режима работы блока.
#define NO_KEY						 0
#define KEY_LOCK_STATE	   1
#define KEY_CONTROL_STATE  2
#define KEY_CONFIG_STATE	 3
//--------------------
//Светодиод кнопки "ПОЖАР"
#define PojLedOn     	    (GPIOC->BSRR = GPIO_BSRR_BS3) 
#define PojLedOff         (GPIOC->BSRR = GPIO_BSRR_BR3)
#define PojLedToggel    	(GPIOC->ODR  = GPIOC->ODR ^ GPIO_ODR_ODR3)
//--------------------
//Микрофон.
#define MicGPIO           GPIOA
#define MicRxPin          GPIO_IDR_IDR8 
#define MicTxPin          GPIO_IDR_IDR11 
#define MicOkPin          GPIO_IDR_IDR12
//Состояния микрофона.
#define MIC_CONNECTED 	1 //микрофон подключен
#define MIC_ACTIVE  		2 //на микрофоне нажата тангента, можно говорить.
#define MIC_NOT_CONNECT	3 //микрофон не подключен
#define MIC_FAULT       4 //капсуль микрофона неисправен.  
//-----------------------------------------------------------------------------
void    GpioInit (void);
uint8_t GpioCheck(void); 

uint8_t KeyGetState(void);
//uint8_t MicGetState(void);

uint8_t MicState(void);
//-----------------------------------------------------------------------------
#endif /*_gpio_ST_H*/


