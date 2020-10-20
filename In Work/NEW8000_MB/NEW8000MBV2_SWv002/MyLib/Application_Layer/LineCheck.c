
#include "LineCheck.h"

//-----------------------------------------------------------------------------
static volatile uint16_t          SpLineAvrMeas;        //усредненное измерение канала LC.
static volatile uint16_t          SpLineDeviation;      //Отклонение в мВ от установившегося значения для фиксации перех-го проц-са на линии.
static volatile uint8_t 	        SpLineControlReg;		  //
static SpeakerLineContextSTR_Type SpLineContextArray[4];//контекст для 4-х линий Гр.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
static uint8_t SpeakerLineGetAvrMeas(void){
  
  static uint16_t measTemp  = 0;
  static uint16_t measCount = 0;
  //--------------------
  measTemp += AdcGetMeas(LсAdcCh);
  if(++measCount == AdcMeasQuantity)
    {
      SpLineAvrMeas = (uint16_t)(measTemp >> AdcMeasShiftDev);
      measTemp      = 0;
      measCount     = 0;
      return 1;
    }
  return 0;
}
//-----------------------------------------------------------------------------
//управление оптореле схемы контроля линии Гр.
static void SpeakerLineOptoReley(uint8_t optoReley, uint8_t state){

	switch(optoReley){
		//--------------------
		case(SpLine1):
			if(state)OptSP1LineOn();
			else     OptSP1LineOff(); 
		break;
		//--------------------
		case(SpAtten1):
			if(state)OptSP1AttOn();
			else     OptSP1AttOff(); 
		break;
		//--------------------
		case(SpLine2):
			if(state)OptSP2LineOn();
			else     OptSP2LineOff(); 
		break;
		//--------------------
		case(SpAtten2):
			if(state)OptSP2AttOn();
			else     OptSP2AttOff(); 
		break;				
		//--------------------
		default:
		break;		
		//--------------------		
	}
}
//-----------------------------------------------------------------------------
static void SpeakerLineLed(uint8_t led, uint8_t state){

	switch(led){
		//--------------------
		case(SpLine1):
			if(state == Off)    LC1SostGreenLedOff();
		  if(state == On)     LC1SostGreenLedOn();
			if(state == Toggel) LC1SostGreenLedToggel(); 
		break;
		//--------------------
		case(SpAtten1):
			if(state == Off)    LC1SostRedLedOff();
		  if(state == On)     LC1SostRedLedOn();
			if(state == Toggel) LC1SostRedLedToggel(); 
		break;
		//--------------------
		case(SpLine2):
			if(state == Off)    LC2SostGreenLedOff();
		  if(state == On)     LC2SostGreenLedOn();
			if(state == Toggel) LC2SostGreenLedToggel();
		break;
		//--------------------
		case(SpAtten2):
			if(state == Off)    LC2SostRedLedOff();
		  if(state == On)     LC2SostRedLedOn();
			if(state == Toggel) LC2SostRedLedToggel(); 
		break;				
		//--------------------
		default:
		break;		
		//--------------------		
	}
}
//-----------------------------------------------------------------------------
//Определение состояния линии Гр.
static void SpeakerLineCalcLcStatus(uint8_t ch){
	
	uint16_t steadyValue = SpeakerLine(ch)->SteadyValue;
	uint16_t refValue    = SpeakerLine(ch)->RefValue;
	//--------------------
	//Линии снята с контроля.
	if((SpLineControlReg & (1 << ch)) == ControlOff) 
		{
			SpeakerLine(ch)->LcStatus = LineOffControl;
			return;
		}
  //Переходной процесс на линии.
	if(steadyValue == LineUndef) 
		{
			SpeakerLine(ch)->LcStatus = LineUndef;
			return;
		}
  //КЗ на линии.
	if(steadyValue < LineShorThresholdConst) 
		{
			SpeakerLine(ch)->LcStatus = LineShort;
			return;
		}
  //Обрыв линии.    
	if(steadyValue > LineBreakThresholdConst) 
		{
			SpeakerLine(ch)->LcStatus = LineBreak;
			return;
		}
  //Выше нормы.  
	if(steadyValue > (refValue + (SpLineDeviation + LcErrorThresholdConst))) 
		{
			SpeakerLine(ch)->LcStatus = LineOver;
			return;
		}
	//Ниже нормы.
	if(steadyValue < (refValue - (SpLineDeviation + LcErrorThresholdConst))) 
		{
			SpeakerLine(ch)->LcStatus = LineUnder;
			return;
		}
	//Линия в норме.
	SpeakerLine(ch)->LcStatus = LineNorm;   
}
//*********************************************************************************
//*********************************************************************************
void SpeakerLineInit(void){

  OptSP1LineOff();
	OptSP1AttOff();
  OptSP2LineOff();
  OptSP2AttOff();
	
	SpLineControlReg = SpAllLines;//Все линии установленны на контроль.
	SpLineDeviation  = LcDeviationDefaultConst;
}
//-----------------------------------------------------------------------------
void SpeakerLineMeasLoop(void){
  
  static uint16_t mSCount           = 0;
	static uint8_t  speakerLineNumber = 0;
	//--------------------
	mSCount++;
	if(mSCount == LcOptoReleyOnTimeOut) SpeakerLineOptoReley(speakerLineNumber, On);
  if(mSCount >= LcMeasPeriod)
		{
			//Измерение напряжения.
			if(SpeakerLineGetAvrMeas() == 0) return;              
			//Сохранение старого значения.
			SpLineContextArray[speakerLineNumber].OldMeas = SpLineContextArray[speakerLineNumber].Meas;
			//Сохранение нового значения.   
			SpLineContextArray[speakerLineNumber].Meas = SpLineAvrMeas;

			SpeakerLineOptoReley(speakerLineNumber, Off);//Отключение оптореле линии Гр.
			mSCount   = 0;
			if(++speakerLineNumber > (SpeakerLineNumbers - 1)) speakerLineNumber = 0;
		}  
	//--------------------	
}
//-----------------------------------------------------------------------------
//Получение установившегося значения после переходного процесса. 
//Вызывается каждую 1мСек.
void SpeakerLineSteadyStateLoop(void){
  
	static uint8_t  speakerLineNumber = 0;
	static uint16_t mScount           = 0; 
	//--------------------
	//Переходной процесс на линии Гр.
  if((SpeakerLine(speakerLineNumber)->Meas > (SpeakerLine(speakerLineNumber)->SteadyValue + SpLineDeviation)) || 
     (SpeakerLine(speakerLineNumber)->Meas < (SpeakerLine(speakerLineNumber)->SteadyValue - SpLineDeviation)))   
    {
      //------
      //Мигающая индикация переходного процесса.     
      if((SpeakerLine(speakerLineNumber)->Meas >= (SpeakerLine(speakerLineNumber)->OldMeas + SpLineDeviation)) || 
         (SpeakerLine(speakerLineNumber)->Meas <= (SpeakerLine(speakerLineNumber)->OldMeas - SpLineDeviation)))   
        {
          if(++mScount >= (LcLedBlinkPperiodConst / 4))
            {
              mScount = 0;
							SpeakerLine(speakerLineNumber)->SteadyValue = LineUndef;
							SpeakerLineLed(speakerLineNumber, Toggel);
            }   
        }
      //------
      //Если переходной процесс окончен то фиксация установившегося значения и выход. 
      else
        {          
          SpeakerLineLed(speakerLineNumber, Off);
					SpeakerLine(speakerLineNumber)->SteadyValue = SpeakerLine(speakerLineNumber)->Meas;
          //Сохранение опорных величин линий Гр. дя LC.					
					if(!(SpeakerLine(speakerLineNumber)->RefValueFlag) && (SpeakerLine(speakerLineNumber)->SteadyValue != 0))
						{
							SpeakerLine(speakerLineNumber)->RefValue     = SpeakerLine(speakerLineNumber)->Meas;
							SpeakerLine(speakerLineNumber)->RefValueFlag = FlagSet;
						}
        }
      //------
    }
	 //Переходного процесса нет на линии Гр.
   else
    {         
      SpeakerLineLed(speakerLineNumber, Off);
			SpeakerLine(speakerLineNumber)->SteadyValue = SpeakerLine(speakerLineNumber)->Meas;
    }
	//--------------------
	if(++speakerLineNumber > (SpeakerLineNumbers - 1)) speakerLineNumber = 0;
	//--------------------
}
//-----------------------------------------------------------------------------
//Линии Гр. установленные на контроль.
volatile uint8_t* SpeakerLineControl(void){

	return &SpLineControlReg;
}
//-----------------------------------------------------------------------------
//Установка отклонения в мВ от установившегося значения для фиксации перех-го проц-са на линии.
volatile uint16_t* SpeakerLineDeviation(void){

	return &SpLineDeviation;
}
//-----------------------------------------------------------------------------
//Контекст линии Гр. Это основная функция для работы с линией Гр.
SpeakerLineContextSTR_Type* SpeakerLine(uint8_t speakerLine){
	
	return &SpLineContextArray[speakerLine];
}
//-----------------------------------------------------------------------------
//Сохранение опорных величин линий Гр. дя LC.
uint8_t SpeakerLineSaveRef(uint8_t ch){
  
  //Если идет переходной процесс то не сохранаем опорные величины и выходим.
  if((ch > SpAtten2) || (SpeakerLine(ch)->LcStatus == LineUndef)) return 0;
  //Сохранение опорных величин.
	SpeakerLine(ch)->RefValue = SpeakerLine(ch)->SteadyValue;
  return 1;
}  
//-----------------------------------------------------------------------------
//Получение установившихся состояний линий Гр. 
uint8_t SpeakerLineGetDefStatus(uint8_t ch){

  static uint8_t TempState[4] = {0, 0, 0, 0};
  //--------------------
	//Если линия снята с контроля
	if((SpLineControlReg & (1 << ch)) == ControlOff)
		{
			TempState[ch] = LineNorm;
			return LineNorm;
		}
	//Линия на контроле.
  if(SpeakerLine(ch)->LcStatus == LineUndef) return TempState[ch];
  TempState[ch] = SpeakerLine(ch)->LcStatus;
  return TempState[ch];
}
//***************************************************************************************************** 
//*****************************************************************************************************
//Машина состояний для управления линией Гр.
void SpeakerLineFSM(uint8_t speakerLine){
	
	volatile uint8_t *speakerLineAction = &SpLineContextArray[speakerLine].Action;  
//	uint8_t *Type    			 = &FireLineContextArr[input].Type;
//	uint8_t *TimeOut 			 = &FireLineContextArr[input].TimeOut;
//	uint8_t *PhysicalState = &FireLineContextArr[input].PhysicalState;   
	//--------------------
	//Определение состояния линии Гр.
	SpeakerLineCalcLcStatus(speakerLine);
	//--------------------
	//Действия с линией Гр.
	switch(*speakerLineAction){
		//--------------------	
		case(ActionOff):
			SpeakerRelays(Off, speakerLine);
		break;
		//--------------------	
		default://case(ActionManualOn):
			SpeakerRelays(On, speakerLine);
		break;
		//--------------------	
//		case(ActionConsolOn):
//		break;
//		//--------------------	
//		case(ActionMicOn):
//		break;
//		//--------------------	
//		case(ActionPuskOn):
//		break;
//		//--------------------	
//		default:
//		break;
//		//--------------------			
	}
}
//***************************************************************************************************** 
//*****************************************************************************************************





































