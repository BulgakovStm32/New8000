
#include "Amplifier.h"

//*****************************************************************************
static uint8_t AmpStatReg = 0;
//*****************************************************************************
//*****************************************************************************
void PAmp_Init(void){

  //Включаем тактирование порта A.
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  //PA0 - PA_Prot - после сброса настроен как вход.
  //PA1 - PA_On. - выход.
  PaOnGPOI->CRL &= ~GPIO_CRL_CNF1; //PC9 - выход, режим - push-pull.
  PaOnGPOI->CRL |=  GPIO_CRL_MODE1;//PC9 - тактирование 50МГц.  
}
//*****************************************************************************
void PAmp_Control(uint8_t state){
  
  //Если УМЗЧ в защите то отключаем УМЗЧ.
  if(!(Gpio_GetState(PaProtGPOI) & PaProtPIN))
    {
      PaOff;
      AmpStatReg = PAMP_PROT;
      return;
    }
  //--------------------
  AmpStatReg = state;
  if(state == PAMP_ON) PaOn;
  if(state == PAMP_OFF)PaOff; 
}
//*****************************************************************************
//Получение стояния УМЗЧ.
uint8_t PAmp_GetStat(void){

  return AmpStatReg;

//  if(!(GpioGetState(PaProtGPOI) & PaProtPIN)) return AmpProt;
//  else                                        return AmpStatReg;
}
//*****************************************************************************
//*****************************************************************************





