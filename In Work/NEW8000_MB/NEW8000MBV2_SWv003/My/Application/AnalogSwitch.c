
#include "AnalogSwitch.h"

//*****************************************************************************
//*****************************************************************************
void AnalogSwitch_Init(void){
	
  //Управление мультиплексором DA5 - аудиосигнал приоритетов.
  //PB3 - Mux_E.
  //PB7 - Mux_S0.
  //PB8 - Mux_S1.
  //PB9 - Mux_S2. 
  GPIOB->CRL &= ~(GPIO_CRL_CNF3 |  //PB3 - выход, режим - push-pull.
                  GPIO_CRL_CNF7);  //PB7 - выход, режим - push-pull.

  GPIOB->CRL |=  (GPIO_CRL_MODE3 | //PB3 - тактирование 50МГц. 
                  GPIO_CRL_MODE7); //PB7 - тактирование 50МГц. 

  GPIOB->CRH &= ~(GPIO_CRH_CNF8 |  //PB8 - выход, режим - push-pull.
                  GPIO_CRH_CNF9 ); //PB9 - выход, режим - push-pull.

  GPIOB->CRH |=  (GPIO_CRH_MODE8 | //PB8 - тактирование 50МГц. 
                  GPIO_CRH_MODE9); //PB9 - тактирование 50МГц. 
  MuxEHight;
  MuxS0Hight;
  MuxS1Hight;
  MuxS2Hight;
	//--------------------
  //Управление мультиплексором DA7 - аудиосигнал платы мультимедия.
  //PB4 - Mux1_S0.
  //PB5 - Mux1_S1.
  //PB6 - Mux1_S2. 
  GPIOB->CRL &= ~(GPIO_CRL_CNF4 |  //PB4 - выход, режим - push-pull.
                  GPIO_CRL_CNF5 |  //PB5 - выход, режим - push-pull.
                  GPIO_CRL_CNF6);  //PB6 - выход, режим - push-pull.

  GPIOB->CRL |=  (GPIO_CRL_MODE4 | //PB4 - тактирование 50МГц. 
                  GPIO_CRL_MODE5 | //PB5 - тактирование 50МГц. 
                  GPIO_CRL_MODE6); //PB6 - тактирование 50МГц. 
  Mux1S0Hight;
  Mux1S1Hight;
  Mux1S2Hight; 
	//--------------------
}
//*****************************************************************************
//Включение выходов мультиплексора.
void AnalogSwitch_Activate(void){

	MuxELow;                
}
//*****************************************************************************
//Выключение выходов мультиплексора.
void AnalogSwitch_Deactivate(void){

	MuxEHight;                
}
//*****************************************************************************
//Управление приоритетным мультиплексором.
void AnalogSwitch_Prior_Set(AnSwitchPriorityEnum_t ch){
  
  switch(ch)
    {
      //-----------
      case(MIC):
        MuxS0Low;
        MuxS1Low;
        MuxS2Low;
      break;
      //-----------
      case(MULTIMEDIA):
        MuxS0Hight;
        MuxS1Low;
        MuxS2Low;        
      break;
      //-----------
      case(LIN_IN_1):
        MuxS0Low;
        MuxS1Hight;
        MuxS2Low;
      break;
      //----------- 
      case(LIN_IN_2):
        MuxS0Hight;
        MuxS1Hight;
        MuxS2Low;
      break;
      //-----------
      case(PRIOR_5):
        MuxS0Low;
        MuxS1Low;
        MuxS2Hight;                                            
      break;
      //-----------
      case(VOICE_MESSAGE):
        MuxS0Hight;
        MuxS1Low;
        MuxS2Hight;
      break;
      //-----------
      case(GND):
        MuxS0Hight;
        MuxS1Hight;
        MuxS2Hight;  
      break;
      //-----------
    }
  //--------------------
}
//*****************************************************************************
//*****************************************************************************



