
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

#define	MicRX             (MicGPIO->IDR & MicRxPin)
#define	MicTX             (MicGPIO->IDR & MicTxPin)
#define	MicOK             (MicGPIO->IDR & MicOkPin)
//Состояния микрофона.
#define MicMaskConst           0xF0
#define MicConnectedConst	    (1<<4)//микрофон подключен
#define MicActiveConst	      (2<<4)//на микрофоне нажата тангента, можно говорить.
#define MicNotConnectedConst	(3<<4)//микрофон не подключен
#define MicFaultConst         (4<<4)//капсуль микрофона неисправен.
//--------------------
//Двухцветный светодио LC1_SOST. PB1 - LC1_SOST_Red. PB2 - LC1_SOST_Green.
#define LC1SostRedLedOn     	(GPIOB->BSRR = GPIO_BSRR_BS1) 
#define LC1SostRedLedOff    	(GPIOB->BSRR = GPIO_BSRR_BR1) 
#define LC1SostRedLedToggel 	(GPIOB->ODR  = GPIOB->ODR ^ GPIO_ODR_ODR1)

#define LC1SostGreenLedOn     (GPIOB->BSRR = GPIO_BSRR_BS2) 
#define LC1SostGreenLedOff    (GPIOB->BSRR = GPIO_BSRR_BR2) 
#define LC1SostGreenLedToggel (GPIOB->ODR  = GPIOB->ODR ^ GPIO_ODR_ODR2)
//Двухцветный светодио LC2_SOST. PA2 - LC2_SOST_Red. PB0 - LC2_SOST_Green.
#define LC2SostRedLedOn     	(GPIOA->BSRR = GPIO_BSRR_BS2) 
#define LC2SostRedLedOff    	(GPIOA->BSRR = GPIO_BSRR_BR2) 
#define LC2SostRedLedToggel 	(GPIOA->ODR  = GPIOA->ODR ^ GPIO_ODR_ODR2)

#define LC2SostGreenLedOn     (GPIOB->BSRR = GPIO_BSRR_BS0) 
#define LC2SostGreenLedOff    (GPIOB->BSRR = GPIO_BSRR_BR0) 
#define LC2SostGreenLedToggel (GPIOB->ODR  = GPIOB->ODR ^ GPIO_ODR_ODR0)
//Оптореле LC. 
//PB3 - OptSP2Att.
//PB4 - OptSP2Line.
//PB5 - OptSP1Line.
//PB6 - OptSP1Att.
#define OptSP1AttOn     (GPIOB->BSRR = GPIO_BSRR_BS6)
#define OptSP1AttOff    (GPIOB->BSRR = GPIO_BSRR_BR6)
#define OptSP1AttToggel (GPIOB->ODR  = (GPIOB->ODR ^ GPIO_ODR_ODR6))

#define OptSP1LineOn     (GPIOB->BSRR = GPIO_BSRR_BS5)
#define OptSP1LineOff    (GPIOB->BSRR = GPIO_BSRR_BR5)
#define OptSP1LineToggel (GPIOB->ODR  = (GPIOB->ODR ^ GPIO_ODR_ODR5))

#define OptSP2AttOn   (GPIOB->BSRR = GPIO_BSRR_BS3) 
#define OptSP2AttOff  (GPIOB->BSRR = GPIO_BSRR_BR3) 

#define OptSP2LineOn  (GPIOB->BSRR = GPIO_BSRR_BS4)
#define OptSP2LineOff (GPIOB->BSRR = GPIO_BSRR_BR4)

#define OptSPAllOff   (GPIOB->ODR = (GPIOB->ODR & ~( GPIO_ODR_ODR3 | GPIO_ODR_ODR4 | GPIO_ODR_ODR5 | GPIO_ODR_ODR6 )))
                                                               
//Управление сдвиговыми регистрами. PA11(STPIC_CS), PA15(STPIC_CLR).
#define StpicCSHight    (GPIOA->BSRR = GPIO_BSRR_BS11) 
#define StpicCSLow    	(GPIOA->BSRR = GPIO_BSRR_BR11) 

#define StpicCLRHight   (GPIOA->BSRR = GPIO_BSRR_BS15) 
#define StpicCLRLow    	(GPIOA->BSRR = GPIO_BSRR_BR15) 
//Управление EEPROM. PA8(EEP_HOLD), PB11(EEP_WP), PB12(EEP_CS).
#define EppHoldHight   (GPIOA->BSRR = GPIO_BSRR_BS8) 
#define EppHoldRLow    (GPIOA->BSRR = GPIO_BSRR_BR8) 

#define EppWPHight   (GPIOB->BSRR = GPIO_BSRR_BS11) 
#define EppWPRLow    (GPIOB->BSRR = GPIO_BSRR_BR11)

#define EppCSHight   (GPIOB->BSRR = GPIO_BSRR_BS12) 
#define EppCSRLow    (GPIOB->BSRR = GPIO_BSRR_BR12)
//-----------------------------------------------------------------------------
void     GpioInit(void);
uint8_t  CheckGpioPins(void); 

uint8_t  GetKeyState(void);
uint16_t GetMicState(void);
//-----------------------------------------------------------------------------
#endif /*_gpio_ST_H*/


