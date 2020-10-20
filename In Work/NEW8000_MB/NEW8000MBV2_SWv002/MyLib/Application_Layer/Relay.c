
#include "Relay.h"

//-----------------------------------------------------------------------------
static uint16_t   StpicReg = 0;
//-----------------------------------------------------------------------------
//Обновление реле.
void RelayUpdate(void){

  StpicCSHight;
  Spi2SendData(StpicReg);
  StpicCSLow;
}  
//-----------------------------------------------------------------------------
//Управление реле линий Гр.
void SpeakerRelays(uint8_t state, uint8_t relay){

	switch(relay){
		//--------------------	
		case(SpLine1):
			if(state == On) StpicReg |=  RelSp1OnOff; 
			else            StpicReg &= ~RelSp1OnOff;
		break;
		//--------------------
		case(SpLine2):
			if(state == On) StpicReg |=  RelSp2OnOff;
			else            StpicReg &= ~RelSp2OnOff;
		break;
		//--------------------
		case(SpAtten1):
			if(state == On) StpicReg |=  RelSp1AttLin;
			else            StpicReg &= ~RelSp1AttLin;
		break;
		//--------------------
		case(SpAtten2):
			if(state == On) StpicReg |=  RelSp2AttLin;
			else            StpicReg &= ~RelSp2AttLin;
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

	if(state) StpicReg |=  LedRS485Act; 
	else      StpicReg &= ~LedRS485Act;
}
//-----------------------------------------------------------------------------




















