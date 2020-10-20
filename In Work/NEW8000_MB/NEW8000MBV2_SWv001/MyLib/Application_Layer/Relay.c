
#include "Relay.h"

//-----------------------------------------------------------------------------

static uint16_t StpicSostReg = 0;
static uint8_t  ZoneStatArray[6];
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
void RelayOrLedToggle(uint16_t relay){
  
  __disable_irq();	//Глобальный запрет прерываний.
  StpicSostReg ^= relay;
  __enable_irq();	//Глобальное разрешение прерываний. 
} 
//************************************************************************************************** 
//**************************************************************************************************

//uint8_t ZoneStatArray[6];
//-----------------------------------------------------------------------------
//Активированть/деактивировать зону.
void ZoneSet(uint8_t zn, uint8_t state){

  //ZoneStatArray[zn-1] = sost;//Сохранение состояния зоны
  //--------------------
  if(zn & Zone1)
    {
      if(state != ZoneOff)
        {
          RelayOrLedOn(RelSp1OnOff | RelSp1AttLin);
          ZoneStatArray[0] = state;  //Сохранение состояния зоны
        }
      else
        {
          RelayOrLedOff(RelSp1OnOff | RelSp1AttLin);
          ZoneStatArray[0] = ZoneOff; //Сохранение состояния зоны          
        }
    }
  //--------------------
  if(zn & Zone2)
    {
      if(state != ZoneOff)
        {
          RelayOrLedOn(RelSp2OnOff | RelSp2AttLin);
          ZoneStatArray[1] = state;//Сохранение состояния зоны
        }
      else
        {
          RelayOrLedOff(RelSp2OnOff | RelSp2AttLin);
          ZoneStatArray[1] = ZoneOff;//Сохранение состояния зоны         
        }
    }
  //--------------------
//  switch(zn){
//      //-----------
//      case(Zone1):
//       if(sost == ZoneStatOn) RelayOrLedOn (RelSp1OnOff | RelSp1AttLin);
//       else                   RelayOrLedOff(RelSp1OnOff | RelSp1AttLin); 
//      break;
//      //-----------
//      case(Zone2):
//       if(sost == ZoneStatOn) RelayOrLedOn (RelSp2OnOff | RelSp2AttLin);
//       else                   RelayOrLedOff(RelSp2OnOff | RelSp2AttLin); 
//      break;
//      //-----------
//      case(Zone3):
//             
//      break;
//      //-----------
//      case(Zone4):
//       
//      break;
//      //-----------
//      case(Zone5):
//        
//      break;
//      //-----------
//      case(Zone6):
//        
//      break;
//      //-----------
//      case(ZoneAll):
//             
//      break;
//      //-----------
//      default:
//      break;
//      //-----------
//    }    
  //--------------------
}
//-----------------------------------------------------------------------------
//Получение состояни зоны.
uint8_t ZoneGetStatus(uint8_t zn){

  uint8_t temp;
  //--------------------
  __disable_irq();//Глобальный запрет прерываний.
  temp = ZoneStatArray[zn-1];
  __enable_irq();	//Глобальное разрешение прерываний. 
  return temp; 
}
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------





//************************************************************************************************** 
//************************************************************************************************** 



