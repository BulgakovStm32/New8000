
#include "Zummer.h"

//-----------------------------------------------------------------------------
static uint8_t  ZemmerReg    = 0;
static uint16_t IntervalBeep = 0;
static uint8_t  NumberBeep   = 0;
//-----------------------------------------------------------------------------
void Zummer_Init(void){
	
	Tim1Init();
}
//-----------------------------------------------------------------------------
void Zummer(uint8_t st){

  if(st == ZummerOn) TIM1->BDTR |=  TIM_BDTR_MOE;
  else               TIM1->BDTR &= ~TIM_BDTR_MOE;
}
//-----------------------------------------------------------------------------
void Zummer_Fault(uint32_t fault){

  static uint32_t faultOld = 0;
  //--------------------
  //Нет неисправностей - гасим светодиод и выходим.
  if(fault == 0)
    {
      Zummer(ZummerOff);
      faultOld = 0;
    }
  //--------------------
  //Есть неисправность. 
  else if((faultOld ^ fault) & fault)//Если обнаружена новая неисправность.
    {
			Button_ClrToggle(ZUMMERbut);//сброс состояния кнопки ОТКЛ.ЗВУК.
      Zummer(ZummerOn);           //Активация зуммера.
      faultOld = fault;           //Сохранение состояния.
    }
  else 
    {
      if(Button_IsToggle(ZUMMERbut))//Если нажали кнопу ОТКЛ.ЗВУК., то
        {
          Zummer(ZummerOff);//Отключение зуммера
          faultOld = fault; //Сохранение состояния.
        }
    }
  //--------------------
}
//-----------------------------------------------------------------------------
void Zummer_Loop(void){

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
void Zummer_Beep(uint8_t num, uint16_t interval){

  //Zummer(ZummerOff);
  ZemmerReg    = 0;
  if(!num && !interval) return;
  IntervalBeep = interval;
  NumberBeep   = num;
  ZemmerReg    = ZummerStartBeep;
}
//-----------------------------------------------------------------------------
//Отключение звуковой индикации.
void Zummer_BeepOff(void){

  Zummer(ZummerOff);
  ZemmerReg    = 0;
  IntervalBeep = 0;
  NumberBeep   = 0;
}
//-----------------------------------------------------------------------------











