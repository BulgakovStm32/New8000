
#include "Zummer.h"

//-----------------------------------------------------------------------------
static uint8_t  ZumStateReg  = 0;
static uint16_t IntervalBeep = 0;
static uint8_t  NumberBeep   = 0;
//*************************************************************************************************
//*************************************************************************************************
void Zummer_Init(void){
	
	Tim1Init();
}
//*******************************************************************
void Zummer_Control(uint8_t st){

//  if(st == ZUMM_ON) TIM1->BDTR |=  TIM_BDTR_MOE;
//  else              TIM1->BDTR &= ~TIM_BDTR_MOE;
}
//*******************************************************************
void Zummer_Fault(uint32_t faults){

  static uint32_t faultsOldState = 0;
  //--------------------
  //Нет неисправностей - гасим светодиод и выходим.
  if(faults == 0)
    {
			faultsOldState = 0;
      Zummer_Control(ZUMM_OFF);
    }
  //--------------------
  //Есть неисправность. 
  else if((faultsOldState ^ faults) & faults)//Если обнаружена новая неисправность.
    {
			faultsOldState = faults;    //Сохранение состояния.
			Button_ClrToggle(ZUMMERbut);//сброс состояния кнопки ОТКЛ.ЗВУК.
      Zummer_Control(ZUMM_ON);   //Активация зуммера.
			//Zummer_Beep(3, 100);
    }
  else 
    {
      if(Button_IsToggle(ZUMMERbut))//Если нажали кнопу ОТКЛ.ЗВУК., то
        {
          Zummer_Control(ZUMM_OFF);//Отключение зуммера
          faultsOldState = faults;  //Сохранение состояния.
        }
    }
  //--------------------
}
//*******************************************************************
void Zummer_Loop(void){

  static uint16_t mScount = 0;
  static uint16_t state   = 0;
  //--------------------
  if(ZumStateReg == ZUMM_START_BEEP)
    {
       switch(state){
          //-----------
          case(0):
            Zummer_Control(ZUMM_ON);
            if(++mScount >= IntervalBeep)
              {
                mScount = 0;
                state   = 1;
              }
          break;
          //-----------
          case(1):
            Zummer_Control(ZUMM_OFF);
            if(++mScount >= IntervalBeep)
              {
                mScount = 0;
                if(--NumberBeep == 0) ZumStateReg = 0;
                else state = 0;                            
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
      state   = 0;
    }
}
//*******************************************************************
//Звуковая индикация.
void Zummer_Beep(uint8_t num, uint16_t interval){

  //Zummer(ZummerOff);
  ZumStateReg = 0;
  if(!num && !interval) return;
  IntervalBeep = interval;
  NumberBeep   = num;
  ZumStateReg  = ZUMM_START_BEEP;
}
//*******************************************************************
//Отключение звуковой индикации.
void Zummer_BeepOff(void){

  Zummer_Control(ZUMM_OFF);
  ZumStateReg  = 0;
  IntervalBeep = 0;
  NumberBeep   = 0;
}
//*************************************************************************************************
//*************************************************************************************************











