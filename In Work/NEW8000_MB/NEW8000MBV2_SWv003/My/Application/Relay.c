
#include "Relay.h"

//-----------------------------------------------------------------------------
static uint16_t	StpicReg = 0;
//-----------------------------------------------------------------------------
static void Pause(volatile uint32_t pause){

	while(pause--);
}
//-----------------------------------------------------------------------------
void Relay_Init(void){

	//Включаем тактирование порта C.
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	//Управление сдвиговыми регистрами. PC7(STPIC_CS), PC8(STPIC_CLR).
	GPIOC->CRL &= ~GPIO_CRL_CNF7;
	GPIOC->CRL |= GPIO_CRL_MODE7;//PC7(STPIC_CS) - выход, режим - push-pull.
															 //PC7(STPIC_CS) - тактирование 50МГц.
	GPIOC->CRH &= ~GPIO_CRH_CNF8;	
	GPIOC->CRH |= GPIO_CRH_MODE8;//PC8(STPIC_CLR)- выход, режим - push-pull.
															 //PC8(STPIC_CLR)- тактирование 50МГц.
	//--------------------	
	STPIC6C595CS_High();
	STPIC6C595CLR_High();

	Spi2_Init();
}
//-----------------------------------------------------------------------------
//Обновление реле.
void Relay_UpdateLoop(void){
	
	STPIC6C595CS_Low();
	Pause(5);
	
	Spi2_TxRxByte(StpicReg >> 8);
	Spi2_TxRxByte(StpicReg);
	
	STPIC6C595CS_High();
	
	//Такая последовательность тоже работает стабильно.
//	STPIC6C595CS_Low();
//	Pause(5);
//	STPIC6C595CS_High();
}  
//-----------------------------------------------------------------------------
//Управление реле линий Гр.
void SpeakerRelays(uint8_t state, uint8_t relay){

	switch(relay){
		//--------------------	
		case(SP_LINE_1):
			if(state == ON) StpicReg |=  RELAY_SP1_ON_OFF; 
			else            StpicReg &= ~RELAY_SP1_ON_OFF;
		break;
		//--------------------
		case(SP_LINE_2):
			if(state == ON) StpicReg |=  RELAY_SP2_ON_OFF;
			else            StpicReg &= ~RELAY_SP2_ON_OFF;
		break;
		//--------------------
		case(ATTEN_LINE_1):
			if(state == ON) StpicReg |=  RELAY_SP1_ATT_LIN;
			else            StpicReg &= ~RELAY_SP1_ATT_LIN;
		break;
		//--------------------
		case(ATTEN_LINE_2):
			if(state == ON) StpicReg |=  RELAY_SP2_ATT_LIN;
			else            StpicReg &= ~RELAY_SP2_ATT_LIN;
		break;
		//--------------------
//		case():
//		break;
//		//--------------------
//		case():
//		break;
//		//--------------------
//		case():
//		break;
		//--------------------
		default:
		break;
		//--------------------
		
	}
}
//-----------------------------------------------------------------------------
//Управление выходными реле.
void OutputRelays(uint8_t state, uint16_t relay){

	if(state) StpicReg |=  relay;
	else			StpicReg &= ~relay;
}
//-----------------------------------------------------------------------------
//Управление светодиодом RS485Act.
void LedRs485Act(uint8_t state){

	if(state) StpicReg |=  LED_RS485_ACT; 
	else      StpicReg &= ~LED_RS485_ACT;
}
//-----------------------------------------------------------------------------




















