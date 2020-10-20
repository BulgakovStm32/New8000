
#include "PowerAndCharge.h"

//-----------------------------------------------------------------------------

static volatile uint8_t PowerACStatReg = 0;

//-----------------------------------------------------------------------------
void PowerAndChargeInit(void){

  //Включаем тактирование порта A.
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  //DC_Ok    - PA3 - вход - после сброса настроен как вход.
  //DC_Fault - PA4 - вход - после сброса настроен как вход.
  //AC_Ok    - PA6 - вход - после сброса настроен как вход.

  //ZU_On    - PA2 - выход.
  ZuOnGPIO->CRL &= ~GPIO_CRL_CNF2; //PA2 - выход, режим - push-pull.
  ZuOnGPIO->CRL |=  GPIO_CRL_MODE2;//PA2 - тактирование 50МГц.
}
//-----------------------------------------------------------------------------
//Получение состояния основного ввода питания 220В.
void PowerGetACStatus(void){

  static uint16_t mScount = 0;
  //--------------------
  if(!(AcGPOI->IDR & AcPIN))
    {
      mScount = 0;
      PowerACStatReg = PowerACOk;
    }
  if(++mScount >= ACmSecConst)
    {
      mScount = 0;
      PowerACStatReg = PowerACNo;
    }
}
//-----------------------------------------------------------------------------
//Получение состояния питания.
uint8_t PowerGetStatus(void){

  return PowerACStatReg;
//  if(!(GpioGetState(PowerGPOI) & AcPIN)) return PowerACOk;
//                                         return PowerACNo;
}
//-----------------------------------------------------------------------------
//Получение состояния инвертора.
//uint8_t PowerDCGetStatus(void){

//  if( (GpioGetState(PowerGPOI) & DcOkPIN) &&
//     !(GpioGetState(PowerGPOI) & DcFaultPIN)) return PowerDCOk;

//  if(!(GpioGetState(PowerGPOI) & DcOkPIN) &&
//      (GpioGetState(PowerGPOI) & DcFaultPIN)) return PowerDCFault;
//  return PowerDCFault;
//}
//-----------------------------------------------------------------------------
//Получение состояния батареи.
uint8_t PowerBatGetStatus(void){

  if( BatLevel() >  2720)                          return BatChargeEnd; 
  if((BatLevel() >  2100) && (BatLevel() <= 2720)) return BatBlink;
  if((BatLevel() >  1850) && (BatLevel() <= 2100)) return BatAttention;
  if( BatLevel() <= 1850)                          return BatDeep; 
  return BatNo;   
}
//-----------------------------------------------------------------------------

















