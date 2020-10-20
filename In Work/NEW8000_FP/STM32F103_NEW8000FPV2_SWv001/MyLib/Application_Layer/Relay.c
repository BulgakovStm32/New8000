
#include "Relay.h"

//-----------------------------------------------------------------------------

static uint16_t StpicSostReg = 0;

static uint8_t FpZone1 = 0; //Хранение состояния зон
static uint8_t FpZone2 = 0;
static uint8_t FpZone3 = 0;
static uint8_t FpZone4 = 0;
static uint8_t FpZone5 = 0;
static uint8_t FpZone6 = 0;




//-----------------------------------------------------------------------------
void RelayOrLedLoop(void){

  StpicCSHight;
  Spi2TxData(StpicSostReg);
  StpicCSLow;
}  
//-----------------------------------------------------------------------------
void RelayOrLedOn(uint16_t relay){
  
  __disable_irq();	//Глобальный запрет прерываний.
  StpicSostReg |= relay;
  __enable_irq();	//Глобальное разрешение прерываний.  
} 
//-----------------------------------------------------------------------------
void RelayOrLedOff(uint16_t relay){
  
  __disable_irq();	//Глобальный запрет прерываний.
  StpicSostReg &= ~relay;
  __enable_irq();	//Глобальное разрешение прерываний.  
} 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
void ZoneFpSet(uint8_t* p){

  FpZone1 = (*p & ZoneMask);
  FpZone2 = (*(p+1) & ZoneMask);
  FpZone3 = (*(p+2) & ZoneMask);
  FpZone4 = (*(p+3) & ZoneMask);
  FpZone5 = (*(p+4) & ZoneMask);
  FpZone6 = (*(p+5) & ZoneMask);
}
//-----------------------------------------------------------------------------
uint8_t ZoneFpGet(uint8_t ch){

  if(ch & Zone1) return FpZone1;
  if(ch & Zone2) return FpZone2;
  if(ch & Zone3) return FpZone3;
  if(ch & Zone4) return FpZone4;
  if(ch & Zone5) return FpZone5;
  if(ch & Zone6) return FpZone6;
  return ZoneDefect;
}
//-----------------------------------------------------------------------------






