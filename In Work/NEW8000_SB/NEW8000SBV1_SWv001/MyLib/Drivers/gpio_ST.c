
#include "gpio_ST.h"

//***************************************************************************************************

//***************************************************************************************************
//***************************************************************************************************
//Инициализация переферии.
void Gpio_Init (void){
  
	RCC->AHBENR |= (RCC_AHBENR_GPIOAEN | //Включаем тактирование порта A
									RCC_AHBENR_GPIOBEN); //Включаем тактирование порта B
  //--------------------
	//Cветодиод ACT. Порт PA4.
	GPIOA->MODER   |= GPIO_MODER_MODER4_0; 	//PA4 - выход, push-pull.
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR4;//High speed.
	GPIOA->PUPDR   &=~GPIO_PUPDR_PUPDR4;		//No pull-up, pull-down.
	//--------------------
	//Управление реле. 
	//РелОП1 - PA7
	//РелОП2 - PA6
	//РелОП3 - PA5	
	GPIOA->MODER   |= (GPIO_MODER_MODER7_0 |  //PA7 - выход, push-pull.
										 GPIO_MODER_MODER6_0 |  //PA6 - выход, push-pull.
										 GPIO_MODER_MODER5_0);  //PA5 - выход, push-pull.
	GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEEDR7 |//High speed.
										 GPIO_OSPEEDR_OSPEEDR6 |
										 GPIO_OSPEEDR_OSPEEDR5);
	GPIOA->PUPDR   &=~(GPIO_PUPDR_PUPDR7 |		//No pull-up, pull-down.
										 GPIO_PUPDR_PUPDR6 |
										 GPIO_PUPDR_PUPDR5);
}
//****************************************************************************
void Led_ACT_Set(uint8_t state){

	if(state) ACTLedOn();
	else 			ACTLedOff();	
}
//***************************************************************************************************
//***************************************************************************************************

