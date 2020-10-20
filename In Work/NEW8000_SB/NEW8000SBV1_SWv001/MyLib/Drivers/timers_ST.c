
#include "timers_ST.h"

//-----------------------------------------------------------------------------

static uint8_t  ZemmerReg    = 0;
static uint16_t IntervalBeep = 0;
static uint8_t  NumberBeep   = 0;

//-----------------------------------------------------------------------------
void Tim1Init(void){

  //Тактирование  GPIOA , TIM1, альтернативных функций порта
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_TIM1EN | RCC_APB2ENR_AFIOEN;
        
  //PA11 push-pull
	GPIOA->CRH &= ~GPIO_CRH_CNF11;
	GPIOA->CRH |= GPIO_CRH_CNF11_1;

	GPIOA->CRH	&= ~GPIO_CRH_MODE11;
	GPIOA->CRH	|= GPIO_CRH_MODE11_1;
	
	TIM1->PSC  = 72; //делитель
  TIM1->ARR  = 288;//значение перезагрузки. 288 значение для частоты ~3400Гц.
	TIM1->CCR4 = 28; //144;//коэф. заполнения. 144 это 50%
	//настроим на выход канал 4, 
	TIM1->CCER |= TIM_CCER_CC4E;//активный уровень высокий  
  //TIM1->CCER |= TIM_CCER_CC4P;//активный уровень низкий 
	//разрешим использовать выводы таймера как выходы
	//TIM1->BDTR |= TIM_BDTR_MOE;
	//PWM mode 1, прямой ШИМ 4 канал
  TIM1->CCMR2 = TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1; 
  //если надо настроить первый канал, это можно сделать так
  //TIM1->CCMR1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;
	TIM1->CR1 &= ~TIM_CR1_DIR;//считаем вверх
	TIM1->CR1 &= ~TIM_CR1_CMS;//выравнивание по фронту, Fast PWM
	TIM1->CR1 |=  TIM_CR1_CEN;//включаем счётчик
}
//-----------------------------------------------------------------------------
void Zummer(uint8_t st){

  if(st == ZummerOn) TIM1->BDTR |=  TIM_BDTR_MOE;
  else               TIM1->BDTR &= ~TIM_BDTR_MOE;
}
//-----------------------------------------------------------------------------
void DisactivSound(uint16_t faultReg){

  static uint16_t FaultState = 0;
  //--------------------
  //Нет неисправностей - гасим светодиод и выходим.
  if(faultReg == 0)
    {
      Zummer(ZummerOff);
      FaultState = 0;
    }
  //--------------------
  //Есть неисправность. 
  else if((FaultState ^ faultReg) & faultReg)//Если обнаружена новая неисправность.
    {
      ButClrToggleState(CONTROL_BUTTONS, ZUMMER_BUTTON);//сброс состояния кнопки ОТКЛ.ЗВУК.
      Zummer(ZummerOn);                                 //Активация зуммера.
      FaultState = faultReg;                            //Сохранение состояния.
    }
  else 
    {
      if(ButGetToggleState(CONTROL_BUTTONS) & ZUMMER_BUTTON)//Если нажали кнопу ОТКЛ.ЗВУК., то
        {
          Zummer(ZummerOff);                                //Отключение зуммера
          FaultState = faultReg;                            //Сохранение состояния.
        }
    }
  //--------------------
}
//-----------------------------------------------------------------------------
void ZummerLoop(void){

  static uint16_t mScount = 0;
  static uint16_t State   = 0;
  //--------------------
  if(ZemmerReg == ZummerStartBeep)
    {
       switch(State){
          //-----------
          case(0):
            Zummer(ZummerOn);
            if(++mScount >= IntervalBeep)
              {
                mScount = 0;
                State   = 1;
              }
          break;
          //-----------
          case(1):
            Zummer(ZummerOff);
            if(++mScount >= IntervalBeep)
              {
                mScount = 0;
                if(--NumberBeep == 0) ZemmerReg = 0;
                else                  State     = 0;                            
              }
          break;
          //-----------
        }
      //--------------------
    }
  else 
    {
      //Zummer(ZummerOff);
      mScount = 0;
      State   = 0;
    }
}
//-----------------------------------------------------------------------------
//Звуковая индикация.
void ZummerBeep(uint8_t num, uint16_t interval){

  //Zummer(ZummerOff);
  ZemmerReg    = 0;
  if(!num && !interval) return;
  IntervalBeep = interval;
  NumberBeep   = num;
  ZemmerReg    = ZummerStartBeep;
}
//-----------------------------------------------------------------------------
//Отключение звуковой индикации.
void ZummerBeepOff(void){

  Zummer(ZummerOff);
  ZemmerReg    = 0;
  IntervalBeep = 0;
  NumberBeep   = 0;
}
//*****************************************************************************
//*****************************************************************************
//Прерываени системного таймера TIM1.
void TIM1_Handler(void){
  	
  //--------------------
  //ZummerToggel;
  //--------------------   
}
//*****************************************************************************
//*****************************************************************************


