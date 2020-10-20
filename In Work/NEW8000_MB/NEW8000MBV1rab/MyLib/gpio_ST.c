
#include "gpio_ST.h"

//-----------------------------------------------------------------------------
//Инициализация переферии.
void GpioInit (void){
  
  //Включаем тактирование порта A, B и модуля альтернативных функций.
  RCC->APB2ENR |= (RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN);
	//Отключение JTAG-D от порта PA15, отладка через SWD активна.
  AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE; 
  //--------------------
  //Настройка вывода MCO(PA8) для вывода на нее системной частоты SYSCLK.
  //GPIOA->CRH &= ~GPIO_CRH_CNF8;

  //GPIOA->CRH |= GPIO_CRH_CNF8_1; 	 //PA8 -выход, альтернативный режим push-pull.
  //GPIOA->CRH |= GPIO_CRH_MODE8;	   //PA8 -выход, тактирование 50МГц.
  
  //RCC->CFGR |= RCC_CFGR_MCO_SYSCLK;//Подключение к выводу PA8 системную частоту.
  //RCC->CFGR |= RCC_CFGR_MCO_HSI;   //Подключение к выводу PA8 частоту HSI.
  //RCC->CFGR |= RCC_CFGR_MCO_HSE;      //Подключение к выводу PA8 частоту HSE.
  //RCC->CFGR |= RCC_CFGR_MCO_PLL;   //Подключение к выводу PA8 частоту PLL/2.  
  //--------------------
	//Двухцветный светодио LC1_SOST.
	//PB1 - LC1_SOST_Red.
	//PB2 - LC1_SOST_Green.
	GPIOB->CRL &= ~GPIO_CRL_CNF1; //PB1(LC1_SOST_Red) - выход, режим - push-pull.
  GPIOB->CRL |= GPIO_CRL_MODE1; //PB1(LC1_SOST_Red) - тактирование 50МГц. 
	GPIOB->CRL &= ~GPIO_CRL_CNF2; //PB2(LC1_SOST_Green) - выход, режим - push-pull.
  GPIOB->CRL |= GPIO_CRL_MODE2; //PB2(LC1_SOST_Green) - тактирование 50МГц. 	
  //--------------------
	//Двухцветный светодио LC2_SOST.
	//PA2 - LC2_SOST_Red.
	//PB0 - LC2_SOST_Green.
	GPIOA->CRL &= ~GPIO_CRL_CNF2; //PA2(LC2_SOST_Red) - выход, режим - push-pull.
  GPIOA->CRL |= GPIO_CRL_MODE2; //PA2(LC2_SOST_Red) - тактирование 50МГц. 
	GPIOB->CRL &= ~GPIO_CRL_CNF0; //PB0(LC2_SOST_Green) - выход, режим - push-pull.
  GPIOB->CRL |= GPIO_CRL_MODE0; //PB0(LC2_SOST_Green) - тактирование 50МГц. 
	//--------------------
  //Оптореле LC. 
  //PB3 - OptSP2Att.
  //PB4 - OptSP2Line.
  //PB5 - OptSP1Line.
  //PB6 - OptSP1Att.
 	GPIOB->CRL &= ~( GPIO_CRL_CNF3 | 
                   GPIO_CRL_CNF4 |
                   GPIO_CRL_CNF5 |
                   GPIO_CRL_CNF6 );//выход, режим - push-pull.
                   
  GPIOB->CRL |= ( GPIO_CRL_MODE3 |
                  GPIO_CRL_MODE4 |
                  GPIO_CRL_MODE5 |
                  GPIO_CRL_MODE6 );//тактирование 50МГц. 
	//--------------------
  //Управление мультиплексором.
  //PB7 - MuxS0.
  //PB8 - MuxS1.
  //PB9 - MuxS2. 
  //выход, режим - open-drain.
  GPIOB->CRL &= ~GPIO_CRL_CNF7;
  GPIOB->CRH &= ~(GPIO_CRH_CNF8 | GPIO_CRH_CNF9);

  GPIOB->CRL |= ( GPIO_CRL_MODE7 );                 //тактирование 50МГц.   
  GPIOB->CRH |= ( GPIO_CRH_MODE8 | GPIO_CRH_MODE9 );//тактирование 50МГц. 
  //Выходы в Z-состояние.
  //GPIOB->ODR = (GPIOB->ODR | ((uint32_t)GPIO_ODR_ODR7 | (uint32_t)GPIO_ODR_ODR8 | (uint32_t)GPIO_ODR_ODR9 ) );
	//--------------------
	//Управление сдвиговыми регистрами. PA11(STPIC_CS), PA15(STPIC_CLR).
	GPIOA->CRH &= ~(GPIO_CRH_CNF11 | GPIO_CRH_CNF15);	

	GPIOA->CRH |= GPIO_CRH_MODE11;	//PA11(STPIC_CS)  - выход, режим - push-pull.
																	//PA11(STPIC_CS)  - тактирование 50МГц.		
	GPIOA->CRH |= GPIO_CRH_MODE15;	//PA15(STPIC_CLR) - выход, режим - push-pull.
																	//PA15(STPIC_CLR) - тактирование 50МГц.		

//Управление EEPROM. PA8(EEP_HOLD), PB11(EEP_WP), PB12(EEP_CS).
//	GPIOA->CRH &= ~(GPIO_CRH_CNF8);	
//	GPIOB->CRH &= ~(GPIO_CRH_CNF11 | GPIO_CRH_CNF12);
//	
//	GPIOA->CRH |= GPIO_CRH_MODE8;	//PA8(EEP_HOLD)  - выход, режим - push-pull.
//																//PA8(EEP_HOLD)  - тактирование 50МГц.		
//	GPIOB->CRH |= GPIO_CRH_MODE11;//PB11(EEP_WP) - выход, режим - push-pull.
//																//PB11(EEP_WP) - тактирование 50МГц.			
//	GPIOB->CRH |= GPIO_CRH_MODE12;//PB12(SPI2_NSS/EPP_CS) - выход, режим - push-pull.
																//PB12(SPI2_NSS/EPP_CS) - тактирование 50МГц. 	
	//--------------------	
}
//-----------------------------------------------------------------------------
