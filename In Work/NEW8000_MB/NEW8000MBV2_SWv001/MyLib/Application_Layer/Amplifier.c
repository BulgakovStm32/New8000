
#include "Amplifier.h"

//-----------------------------------------------------------------------------

static uint8_t AmpStatReg = 0;

//-----------------------------------------------------------------------------
void AmpInit(void){

  //Включаем тактирование порта A.
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  //PA0 - PA_Prot - после сброса настроен как вход.
  //PA1 - PA_On. - выход.
  PaOnGPOI->CRL &= ~GPIO_CRL_CNF1; //PC9 - выход, режим - push-pull.
  PaOnGPOI->CRL |=  GPIO_CRL_MODE1;//PC9 - тактирование 50МГц.  
}
//-----------------------------------------------------------------------------
void AmpControl(uint8_t state){
  
  //Если УМЗЧ в защите то отключаем УМЗЧ.
  if(!(GpioGetState(PaProtGPOI) & PaProtPIN))
    {
      PaOff;
      AmpStatReg = AmpProt;
      return;
    }
  //--------------------
  AmpStatReg = state;
  if(state == AmpOn) PaOn;
  if(state == AmpOff)PaOff; 
}
//-----------------------------------------------------------------------------
//Получение стояния УМЗЧ.
uint8_t AmpGetStat(void){

  return AmpStatReg;

//  if(!(GpioGetState(PaProtGPOI) & PaProtPIN)) return AmpProt;
//  else                                        return AmpStatReg;
}
//-----------------------------------------------------------------------------





