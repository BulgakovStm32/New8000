
#include "Relay.h"

//-----------------------------------------------------------------------------

static uint16_t StpicSostReg = 0;

//-----------------------------------------------------------------------------
void RelayOrLedLoop(void){

  StpicCSHight;
  Spi2SendData(StpicSostReg);
  StpicCSLow;
}  
//-----------------------------------------------------------------------------
void RelayOrLedOn(uint16_t relay){
  
  __disable_irq();	//Глобальный запрет прерываний.
  
  StpicSostReg |= relay;
  
  __enable_irq();	//Глобальное разрешение прерываний. 
 
//  StpicCSHight;
//  Spi2SendData(StpicSostReg);
//  StpicCSLow;
} 
//-----------------------------------------------------------------------------
void RelayOrLedOff(uint16_t relay){
  
  __disable_irq();	//Глобальный запрет прерываний.
  
  StpicSostReg &= ~relay;

  __enable_irq();	//Глобальное разрешение прерываний. 
 
//  StpicCSHight;
//  Spi2SendData(StpicSostReg);
//  StpicCSLow;
} 
//-----------------------------------------------------------------------------


