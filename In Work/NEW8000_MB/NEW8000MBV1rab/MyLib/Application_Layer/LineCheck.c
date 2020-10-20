
#include "LineCheck.h"

//-----------------------------------------------------------------------------
typedef union{
  struct{
    //--------------------
    //Состояния линий после включения питания.
    uint16_t Lc1LineStateAfterPowerOn;  //
    uint16_t Lc1AttenStateAfterPowerOn; //
    uint16_t Lc2LineStateAfterPowerOn;  //
    uint16_t Lc2AttenStateAfterPowerOn; //   
    //Установившееся значения на линиях Гр. после переходных процессов.
    uint16_t Lc1LineSteadyState;        //
    uint16_t Lc1AttenSteadyState;       //
    uint16_t Lc2LineSteadyState;        //
    uint16_t Lc2AttenSteadyState;       //
    //Рабочие переменные.
    uint16_t MeasTemp;
    uint16_t mScout;
    uint16_t Chanel; 
    //--------------------
    }STR;
  uint16_t BLK[22];
}__LcState_TypeDef;   
//-----------------------------------------------
__LcState_TypeDef   LcStateUNION; 

//uint16_t LcErrorThreshold = 0;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void LcInit(void){

  LcStateUNION.STR.Lc1LineSteadyState  = 0; //LineUndef;
  LcStateUNION.STR.Lc1AttenSteadyState = 0; //LineUndef;
  LcStateUNION.STR.Lc2LineSteadyState  = 0; //LineUndef;
  LcStateUNION.STR.Lc2AttenSteadyState = 0; //LineUndef;
}
//-----------------------------------------------------------------------------
//Сохранение опорных величин линий Гр. дя LC.
void LcSaveRefState(void){
  
  while(LcStateUNION.STR.Lc1LineSteadyState == 0){}
  //while(LcStateUNION.STR.Lc1LineSteadyState == LineUndef){}
  LcStateUNION.STR.Lc1LineStateAfterPowerOn = LcStateUNION.STR.Lc1LineSteadyState;
    
  while(LcStateUNION.STR.Lc1AttenSteadyState == 0){}
  //while(LcStateUNION.STR.Lc1AttenSteadyState == LineUndef){}
  LcStateUNION.STR.Lc1AttenStateAfterPowerOn = LcStateUNION.STR.Lc1AttenSteadyState;  

//  while(LcStateUNION.STR.Lc2LineSteadyState == 0){}
//  LcStateUNION.STR.Lc2LineStateAfterPowerOn = LcStateUNION.STR.Lc2LineSteadyState;
//    
//  while(LcStateUNION.STR.Lc2AttenSteadyState == 0){}
//  LcStateUNION.STR.Lc2AttenStateAfterPowerOn = LcStateUNION.STR.Lc2AttenSteadyState;      
}  
//-----------------------------------------------------------------------------
//Получение установившегося значения после переходного процесса. 
//Вызывается каждую 1мСек в SysTic.
void LcGetSteadyState(void){
  
  //------------------------------------------------------
  //Переходной процесс на линии Line Зоны_1.
  if((AdcGetLcMeas(Lc1LineValue) > (LcStateUNION.STR.Lc1LineSteadyState + LcDeviationConst)) ||
     (AdcGetLcMeas(Lc1LineValue) < (LcStateUNION.STR.Lc1LineSteadyState - LcDeviationConst)))
    {
      //------
      //Мигающая индикация переходного процесса.     
      if((AdcGetLcMeas(Lc1LineValue) >= (AdcGetLcMeas(Lc1LineOldValue) + LcDeviationEndTransitionProcessConst)) ||
         (AdcGetLcMeas(Lc1LineValue) <= (AdcGetLcMeas(Lc1LineOldValue) - LcDeviationEndTransitionProcessConst)))
        {
          if(++LcStateUNION.STR.mScout > LcLedBlinkPperiodConst)
            {
              LcStateUNION.STR.mScout = 0;
              LcStateUNION.STR.Lc1LineSteadyState = LineUndef;
              LC1SostGreenLedToggel;
            }   
        }
      //------
      //Если переходной процесс окончен то фиксация установившегося значения и выход. 
      else
        { 
          LC1SostGreenLedOff;          
          LcStateUNION.STR.Lc1LineSteadyState = AdcGetLcMeas(Lc1LineValue);  
        }
      //------
    }
   else
    {
        LC1SostGreenLedOff;          
        LcStateUNION.STR.Lc1LineSteadyState = AdcGetLcMeas(Lc1LineValue); 
    }
  //------------------------------------------------------
  //Переходной процесс на линии Atten Зоны_1.
  if((AdcGetLcMeas(Lc1AttenValue) > (LcStateUNION.STR.Lc1AttenSteadyState + LcDeviationConst)) ||
     (AdcGetLcMeas(Lc1AttenValue) < (LcStateUNION.STR.Lc1AttenSteadyState - LcDeviationConst)))
    {
      //------
      //Мигающая индикация переходного процесса.
      if((AdcGetLcMeas(Lc1AttenValue) > (AdcGetLcMeas(Lc1AttenOldValue) + LcDeviationEndTransitionProcessConst)) ||
         (AdcGetLcMeas(Lc1AttenValue) < (AdcGetLcMeas(Lc1AttenOldValue) - LcDeviationEndTransitionProcessConst)))
        {
          if(++LcStateUNION.STR.mScout > LcLedBlinkPperiodConst)
            {
              LcStateUNION.STR.mScout = 0;
              LcStateUNION.STR.Lc1AttenSteadyState = LineUndef;
              LC2SostGreenLedToggel;
            }   
        }
      //------
      //Если переходной процесс окончен то фиксация установившегося значения и выход. 
      else
        {  
          LC2SostGreenLedOff;
          LcStateUNION.STR.Lc1AttenSteadyState = AdcGetLcMeas(Lc1AttenValue);  
        }
      //------
    }   
   else
    {
        LC2SostGreenLedOff;          
        LcStateUNION.STR.Lc1AttenSteadyState = AdcGetLcMeas(Lc1AttenValue); 
    } 
  //------------------------------------------------------
  //Переходной процесс на линии Line Зоны_2.
    
  //------------------------------------------------------
  //Переходной процесс на линии Atten Зоны_2.
    
  //------------------------------------------------------    
}
//-----------------------------------------------------------------------------
//Получение установившегося значения после переходного процесса. Вызывается каждую 1мСек в SysTic.
//void LcGetSteadyStateLine(void){ 
//  
//  //--------------------
//  switch(LcStateUNION.STR.Chanel)
//    {
//      //------------------------------------------------------
//      case(Lc1LineValue):
//        //------
//        //Если нет переходного процесса, то переключение на слудующий канал и выход.
//        if( (GetLcChannelVolue(Lc1LineValue) < (LcStateUNION.STR.Lc1LineSteadyState + LcDeviationConst)) &&
//            (GetLcChannelVolue(Lc1LineValue) > (LcStateUNION.STR.Lc1LineSteadyState - LcDeviationConst)))
//          {
//            LcStateUNION.STR.Chanel = Lc1AttenValue;
//            break;
//          }
//        //------
//        //Мигающая индикация переходного процесса.
//        if( (GetLcChannelVolue(Lc1LineValue) > (GetLcChannelVolue(Lc1LineOldValue) + LcDeviationEndTransitionProcessConst)) ||
//            (GetLcChannelVolue(Lc1LineValue) < (GetLcChannelVolue(Lc1LineOldValue) - LcDeviationEndTransitionProcessConst)))
//            {
//              if(++LcStateUNION.STR.mScout > LcLedBlinkPperiodConst)
//                {
//                  LcStateUNION.STR.mScout = 0;
//                  LC1SostGreenLedToggel;
//                }  
//              break;  
//            }
//        //------
//        //Если переходной процесс окончен то фиксация установившегося значения и выход.   
//        LcStateUNION.STR.Lc1LineSteadyState = GetLcChannelVolue(Lc1LineValue);
//        
//        LcStateUNION.STR.Chanel = Lc1AttenValue;
//        //------       
//      break;
//      //------------------------------------------------------
//      case(Lc1AttenValue): 
//        //------
//        //Если нет переходного процесса, то переключение на слудующий канал и выход.
//        if( (GetLcChannelVolue(Lc1AttenValue) < (LcStateUNION.STR.Lc1AttenSteadyState + LcDeviationConst)) &&
//            (GetLcChannelVolue(Lc1AttenValue) > (LcStateUNION.STR.Lc1AttenSteadyState - LcDeviationConst)))
//          {
//            LcStateUNION.STR.Chanel = Lc1LineValue;
//            break;
//          }
//        //------
//        //Мигающая индикация переходного процесса.
//        if( (GetLcChannelVolue(Lc1AttenValue) > (GetLcChannelVolue(Lc1AttenOldValue) + LcDeviationEndTransitionProcessConst)) ||
//            (GetLcChannelVolue(Lc1AttenValue) < (GetLcChannelVolue(Lc1AttenOldValue) - LcDeviationEndTransitionProcessConst))) 
//          {
//            if(++LcStateUNION.STR.mScout > LcLedBlinkPperiodConst)
//              {
//                LcStateUNION.STR.mScout = 0;
//                LC2SostGreenLedToggel;
//              }  
//            break;  
//          }
//        //------
//        //Если переходной процесс окончен то фиксация установившегося значения и выход.   
//        LcStateUNION.STR.Lc1AttenSteadyState = GetLcChannelVolue(Lc1AttenValue);
//        
//        LcStateUNION.STR.Chanel = Lc1LineValue;
//        //------       
//      break;
//      //------------------------------------------------------
//          
//      //------------------------------------------------------
//          
//      //------------------------------------------------------   
//    }
//  //-------------------- 
//}  
//-----------------------------------------------------------------------------
uint16_t LcGetStateMeas(uint8_t ch){
 
  return LcStateUNION.BLK[ch];
}
//-----------------------------------------------------------------------------
//Определение состояния линии Гр.
uint8_t LcGetState(uint8_t ch){

  //Переходной процесс на линии.
  if(LcStateUNION.BLK[ch + 4] == LineUndef)             return LineUndef;
  //КЗ на линии.
  if(LcStateUNION.BLK[ch + 4] < LineShorThresholdConst) return LineShort;
  //Обрыв линии.    
  if(LcStateUNION.BLK[ch + 4] > LineBreakThresholdConst)return LineBreak;
  //Выше нормы.  
  if(LcStateUNION.BLK[ch + 4] > (LcStateUNION.BLK[ch] + LcErrorThresholdConst))return LineAbove;
  //Ниже нормы.
  if(LcStateUNION.BLK[ch + 4] < (LcStateUNION.BLK[ch] - LcErrorThresholdConst))return LineBelow;
  //Линия в норме.
  return LineNorm;  
}
//-----------------------------------------------------------------------------
