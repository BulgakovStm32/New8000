
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
//Режима работы блока.
#define KeyMaskConst           0x0F
#define UnitLockModeConst	    (1<<0)
#define UnitControlModeConst  (2<<0)
#define UnitCofigModeConst	  (3<<0)
#define UnitErrorConst        (4<<0)
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
#define MicMaskConst           0xF0
#define MicConnectedConst	    (1<<4)//микрофон подключен
#define MicActiveConst	      (2<<4)//на микрофоне нажата тангента, можно говорить.
#define MicNotConnectedConst	(3<<4)//микрофон не подключен
#define MicFaultConst         (4<<4)//капсуль микрофона неисправен.
//-----------------------------------------------------------------------------
void    GpioInit (void);
uint8_t GpioCheck(void); 

uint8_t KeyGetState(void);
uint8_t MicGetState(void);
//-----------------------------------------------------------------------------
#endif /*_gpio_ST_H*/


