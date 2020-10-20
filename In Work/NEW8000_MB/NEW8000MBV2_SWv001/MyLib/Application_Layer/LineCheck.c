
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
    uint16_t LcDeviation;               //Отклонение в мВ от установившегося значения для фиксации перех-го проц-са на линии.
    uint16_t MeasTemp;
    uint16_t mScout;
    uint16_t Chanel; 
    //--------------------
    }STR;
  uint16_t BLK[12]; //uint16_t BLK[22];
}LcStateUNION_Type;   
//-----------------------------------------------
LcStateUNION_Type        LcStateUNION; //
static volatile uint8_t  BlinkReg  = 0;//
static volatile uint16_t LcFlagRef = 0;//Флаговый регистр.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void LcInit(void){

  LcStateUNION.STR.Lc1LineSteadyState  = 0; //LineUndef;
  LcStateUNION.STR.Lc1AttenSteadyState = 0; //LineUndef;
  LcStateUNION.STR.Lc2LineSteadyState  = 0; //LineUndef;
  LcStateUNION.STR.Lc2AttenSteadyState = 0; //LineUndef;
  LcStateUNION.STR.LcDeviation         = LcDeviationConst;
}
//-----------------------------------------------------------------------------
//Установка отклонения в мВ от установившегося значения для фиксации перех-го проц-са на линии.
void LcSetDeviation(uint16_t dev){

  __disable_irq();
  LcStateUNION.STR.LcDeviation = dev;
  __enable_irq();
}
//-----------------------------------------------------------------------------
uint16_t LcGetDeviation(void){
  
  return LcStateUNION.STR.LcDeviation;
}
//-----------------------------------------------------------------------------
//Получение установившегося значения после переходного процесса. 
//Вызывается каждую 1мСек.
void LcSteadyStateLoop(void){
  
//  static uint16_t Count = 0;
  //--------------------
  //Мигающая индикация.
//  if(++Count >= 250)
//    {
//      Count = 0;
//      BlinkReg ^= 0xFF;
//    }
  //------------------------------------------------------
  //Переходной процесс на линии Line Зоны_1.
  if((LcGetValue(Lc1LineValue) > (LcStateUNION.STR.Lc1LineSteadyState + LcStateUNION.STR.LcDeviation)) || //LcDeviationConst)) ||
     (LcGetValue(Lc1LineValue) < (LcStateUNION.STR.Lc1LineSteadyState - LcStateUNION.STR.LcDeviation)))   //LcDeviationConst)))
    {
      //------
      //Мигающая индикация переходного процесса.     
      if((LcGetValue(Lc1LineValue) >= (LcGetValue(Lc1LineOldValue) + LcStateUNION.STR.LcDeviation)) || //LcDeviationEndTransitionProcessConst)) ||
         (LcGetValue(Lc1LineValue) <= (LcGetValue(Lc1LineOldValue) - LcStateUNION.STR.LcDeviation)))   //LcDeviationEndTransitionProcessConst)))
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
          LcStateUNION.STR.Lc1LineSteadyState = LcGetValue(Lc1LineValue);
          //Сохранение опорных величин линий Гр. дя LC.
          if(!(LcFlagRef & Lc1SaveRefLineFlag)) LcStateUNION.STR.Lc1LineStateAfterPowerOn = LcGetValue(Lc1LineValue);
          LcFlagRef |= Lc1SaveRefLineFlag;
          
        }
      //------
    }
   else
    {
        LC1SostGreenLedOff;          
        LcStateUNION.STR.Lc1LineSteadyState = LcGetValue(Lc1LineValue); 
    }
  //------------------------------------------------------
  //Переходной процесс на линии Atten Зоны_1.
  if((LcGetValue(Lc1AttenValue) > (LcStateUNION.STR.Lc1AttenSteadyState + LcStateUNION.STR.LcDeviation)) || //LcDeviationConst)) ||
     (LcGetValue(Lc1AttenValue) < (LcStateUNION.STR.Lc1AttenSteadyState - LcStateUNION.STR.LcDeviation)))   //LcDeviationConst)))
    {
      //------
      //Мигающая индикация переходного процесса.
      if((LcGetValue(Lc1AttenValue) > (LcGetValue(Lc1AttenOldValue) + LcStateUNION.STR.LcDeviation)) || //LcDeviationEndTransitionProcessConst)) ||
         (LcGetValue(Lc1AttenValue) < (LcGetValue(Lc1AttenOldValue) - LcStateUNION.STR.LcDeviation)))   //LcDeviationEndTransitionProcessConst)))
        {
          if(++LcStateUNION.STR.mScout > LcLedBlinkPperiodConst)
            {
              LcStateUNION.STR.mScout = 0;
              LcStateUNION.STR.Lc1AttenSteadyState = LineUndef;
              LC1SostRedLedToggel;
            }   
        }
      //------
      //Если переходной процесс окончен то фиксация установившегося значения и выход. 
      else
        {  
          LC1SostRedLedOff;
          LcStateUNION.STR.Lc1AttenSteadyState = LcGetValue(Lc1AttenValue); 
          //Сохранение опорных величин линий Гр. дя LC.
          if(!(LcFlagRef & Lc1SaveRefAttFlag)) LcStateUNION.STR.Lc1AttenStateAfterPowerOn = LcGetValue(Lc1AttenValue); 
          LcFlagRef |= Lc1SaveRefAttFlag;
        }
      //------
    }   
   else
    {
        LC1SostRedLedOff;         
        LcStateUNION.STR.Lc1AttenSteadyState = LcGetValue(Lc1AttenValue); 
    } 
  //------------------------------------------------------
  //Переходной процесс на линии Line Зоны_2.
  if((LcGetValue(Lc2LineValue) > (LcStateUNION.STR.Lc2LineSteadyState + LcStateUNION.STR.LcDeviation)) || //LcDeviationConst)) ||
     (LcGetValue(Lc2LineValue) < (LcStateUNION.STR.Lc2LineSteadyState - LcStateUNION.STR.LcDeviation)))   //LcDeviationConst)))
    {
      //------
      //Мигающая индикация переходного процесса.     
      if((LcGetValue(Lc2LineValue) >= (LcGetValue(Lc2LineOldValue) + LcStateUNION.STR.LcDeviation)) || //LcDeviationEndTransitionProcessConst)) ||
         (LcGetValue(Lc2LineValue) <= (LcGetValue(Lc2LineOldValue) - LcStateUNION.STR.LcDeviation)))   //LcDeviationEndTransitionProcessConst)))
        {
          if(++LcStateUNION.STR.mScout > LcLedBlinkPperiodConst)
            {
              LcStateUNION.STR.mScout = 0;
              LcStateUNION.STR.Lc2LineSteadyState = LineUndef;
              LC2SostGreenLedToggel;
            }   
        }
      //------
      //Если переходной процесс окончен то фиксация установившегося значения и выход. 
      else
        { 
          LC2SostGreenLedOff;          
          LcStateUNION.STR.Lc2LineSteadyState = LcGetValue(Lc2LineValue);
          //Сохранение опорных величин линий Гр. дя LC.
          if(!(LcFlagRef & Lc2SaveRefLineFlag)) LcStateUNION.STR.Lc2LineStateAfterPowerOn = LcGetValue(Lc2LineValue);
          LcFlagRef |= Lc2SaveRefLineFlag;
          
        }
      //------
    }
   else
    {
        LC2SostGreenLedOff;          
        LcStateUNION.STR.Lc2LineSteadyState = LcGetValue(Lc2LineValue); 
    }
  //------------------------------------------------------
  //Переходной процесс на линии Atten Зоны_2.
  if((LcGetValue(Lc2AttenValue) > (LcStateUNION.STR.Lc2AttenSteadyState + LcStateUNION.STR.LcDeviation)) || //LcDeviationConst)) ||
     (LcGetValue(Lc2AttenValue) < (LcStateUNION.STR.Lc2AttenSteadyState - LcStateUNION.STR.LcDeviation)))   //LcDeviationConst)))
    {
      //------
      //Мигающая индикация переходного процесса.
      if((LcGetValue(Lc2AttenValue) > (LcGetValue(Lc2AttenOldValue) + LcStateUNION.STR.LcDeviation)) || //LcDeviationEndTransitionProcessConst)) ||
         (LcGetValue(Lc2AttenValue) < (LcGetValue(Lc2AttenOldValue) - LcStateUNION.STR.LcDeviation)))   //LcDeviationEndTransitionProcessConst)))
        {
          if(++LcStateUNION.STR.mScout > LcLedBlinkPperiodConst)
            {
              LcStateUNION.STR.mScout = 0;
              LcStateUNION.STR.Lc2AttenSteadyState = LineUndef;
              LC2SostRedLedToggel;
            }   
        }
      //------
      //Если переходной процесс окончен то фиксация установившегося значения и выход. 
      else
        {  
          LC2SostRedLedOff;
          LcStateUNION.STR.Lc2AttenSteadyState = LcGetValue(Lc2AttenValue); 
          //Сохранение опорных величин линий Гр. дя LC.
          if(!(LcFlagRef & Lc2SaveRefAttFlag)) LcStateUNION.STR.Lc2AttenStateAfterPowerOn = LcGetValue(Lc2AttenValue); 
          LcFlagRef |= Lc2SaveRefAttFlag;
        }
      //------
    }   
   else
    {
        LC2SostRedLedOff;       
        LcStateUNION.STR.Lc2AttenSteadyState = LcGetValue(Lc2AttenValue); 
    } 
  //------------------------------------------------------    
}
//-----------------------------------------------------------------------------
//Получение измерения для канала.
uint16_t LcGetMeas(uint8_t ch){
 
  return LcStateUNION.BLK[ch];
}
//-----------------------------------------------------------------------------
//Сохранение опорных величин линий Гр. дя LC.
uint8_t LcSaveRef(uint8_t ch){
  
  //Если идет переходной процесс то не сохранаем опрные величины и выходим.
  if((ch > Lc2Atten) || (LcGetStatus(ch) == LineUndef)) return 0;
  //Сохранение опорных величин.
  LcStateUNION.BLK[ch] = LcStateUNION.BLK[ch + 4];
  return 1;

//  if(ch == Lc1Line)  LcStateUNION.STR.Lc1LineStateAfterPowerOn  = LcStateUNION.STR.Lc1LineSteadyState;
//  if(ch == Lc1Atten) LcStateUNION.STR.Lc1AttenStateAfterPowerOn = LcStateUNION.STR.Lc1AttenSteadyState;
//  if(ch == Lc2Line)  LcStateUNION.STR.Lc2LineStateAfterPowerOn  = LcStateUNION.STR.Lc2LineSteadyState;
//  if(ch == Lc2Atten) LcStateUNION.STR.Lc2AttenStateAfterPowerOn = LcStateUNION.STR.Lc2AttenSteadyState;
}  
//-----------------------------------------------------------------------------
//Определение состояния линии Гр.
uint8_t LcGetStatus(uint8_t ch){

  //Переходной процесс на линии.
  if(LcStateUNION.BLK[ch + 4] == LineUndef)             return LineUndef;
  //КЗ на линии.
  if(LcStateUNION.BLK[ch + 4] < LineShorThresholdConst) return LineShort;
  //Обрыв линии.    
  if(LcStateUNION.BLK[ch + 4] > LineBreakThresholdConst)return LineBreak;
  //Выше нормы.  
  if(LcStateUNION.BLK[ch + 4] > (LcStateUNION.BLK[ch] + LcErrorThresholdConst))return LineOver;
  //Ниже нормы.
  if(LcStateUNION.BLK[ch + 4] < (LcStateUNION.BLK[ch] - LcErrorThresholdConst))return LineUnder;
  //Линия в норме.
  return LineNorm;  
}
//-----------------------------------------------------------------------------
//Получение установившихся состояний линий Гр. 
uint8_t LcGetDefStatus(uint8_t ch){

  static uint8_t TempState[4] = {0, 0, 0, 0};
  //--------------------
  if(LcGetStatus(ch) == LineUndef) return TempState[ch];
  TempState[ch] = LcGetStatus(ch);
  return TempState[ch];
}
//-----------------------------------------------------------------------------



