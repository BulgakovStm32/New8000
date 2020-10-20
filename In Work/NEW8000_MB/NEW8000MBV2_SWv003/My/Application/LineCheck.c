
#include "LineCheck.h"

//*************************************************************************************************
static volatile uint16_t SpLineAvrMeas;        //усредненное измерение канала LC.
static SpLineParam_t     SpLineConfigParamStr; //
static SpLineContext_t   SpLineContextArray[4];//контекст для 4-х линий Гр.
//*************************************************************************************************
//*************************************************************************************************

static uint8_t SpeakerLine_GetAvrMeas(void){
  
  static uint16_t measTemp  = 0;
  static uint16_t measCount = 0;
  //--------------------
  measTemp += Adc_GetMeas(LcAdcCh);
  if(++measCount == AdcMeasQuantity)
    {
      SpLineAvrMeas = (uint16_t)(measTemp >> AdcMeasShiftDev);
      measTemp      = 0;
      measCount     = 0;
      return 1;
    }
  return 0;
}
//*****************************************************************************
//управление оптореле схемы контроля линии Гр.
static void SpeakerLine_OptoReley(uint8_t optoReley, uint8_t state){

	switch(optoReley){
		//--------------------
		case(SP_LINE_1):
			if(state)OptSP1LineOn();
			else     OptSP1LineOff(); 
		break;
		//--------------------
		case(ATTEN_LINE_1):
			if(state)OptSP1AttOn();
			else     OptSP1AttOff(); 
		break;
		//--------------------
		case(SP_LINE_2):
			if(state)OptSP2LineOn();
			else     OptSP2LineOff(); 
		break;
		//--------------------
		case(ATTEN_LINE_2):
			if(state)OptSP2AttOn();
			else     OptSP2AttOff(); 
		break;				
		//--------------------
		default:
		break;		
		//--------------------		
	}
}
//*****************************************************************************
static void SpeakerLine_Led(uint8_t led, uint8_t state){

	switch(led){
		//--------------------
		case(SP_LINE_1):
			if(state == OFF)    LC1SostGreenLedOff();
		  if(state == ON)     LC1SostGreenLedOn();
			if(state == TOGGLE) LC1SostGreenLedToggel(); 
		break;
		//--------------------
		case(ATTEN_LINE_1):
			if(state == OFF)    LC1SostRedLedOff();
		  if(state == ON)     LC1SostRedLedOn();
			if(state == TOGGLE) LC1SostRedLedToggel(); 
		break;
		//--------------------
		case(SP_LINE_2):
			if(state == OFF)    LC2SostGreenLedOff();
		  if(state == ON)     LC2SostGreenLedOn();
			if(state == TOGGLE) LC2SostGreenLedToggel();
		break;
		//--------------------
		case(ATTEN_LINE_2):
			if(state == OFF)    LC2SostRedLedOff();
		  if(state == ON)     LC2SostRedLedOn();
			if(state == TOGGLE) LC2SostRedLedToggel(); 
		break;				
		//--------------------
		default:
		break;		
		//--------------------		
	}
}
//*****************************************************************************
//Определение состояния линии Гр.
static void SpeakerLine_CalcLcStatus(uint8_t ch){
	
	uint16_t deviation      = SpLineConfigParamStr.Deviation;
	SpLineContext_t *spLine = SpeakerLine(ch);
	//--------------------	
	//Линии снята с контроля.
	if((SpLineConfigParamStr.Check & (1 << ch)) == OFF) 
		{
			spLine->LcStatus = SP_LINE_CHECK_OFF;
			return;
		}
	//Включение прибора.Это нужно что бы небыло фиксации КЗ при включении блока.
	if(SpeakerLine(ch)->FirstOnFlag == FLAG_CLEAR)
		{
			spLine->LcStatus = SP_LINE_NORM;
			return;
		}		
  //Переходной процесс на линии.
	if(spLine->SteadyValue == SP_LINE_UNDEF) 
		{
			spLine->LcStatus = SP_LINE_UNDEF;
			return;
		}
  //КЗ на линии.
	if(spLine->SteadyValue < SP_LINE_SHORT_TRESHOLD) 
		{
			spLine->LcStatus = SP_LINE_SHORT;
			return;
		}
  //Обрыв линии.    
	if(spLine->SteadyValue > SP_LINE_BREAK_TRESHOLD) 
		{
			spLine->LcStatus = SP_LINE_BREAK;
			return;
		}
  //Выше нормы.  
	if(spLine->SteadyValue > (spLine->RefValue + deviation)) 
		{
			spLine->LcStatus = SP_LINE_OVER;
			return;
		}
	//Ниже нормы.
	if(spLine->SteadyValue < (spLine->RefValue - deviation)) 
		{
			spLine->LcStatus = SP_LINE_UNDER;
			return;
		}
	//Линия в норме.
	spLine->LcStatus = SP_LINE_NORM;   
}
//*************************************************************************************************
//*************************************************************************************************

void SpeakerLine_Init(void){

  OptSP1LineOff();
	OptSP1AttOff();
  OptSP2LineOff();
  OptSP2AttOff();
	
	SpLineConfigParamStr.Check     = SP_LINE_ALL_OFF;//Все линии установленны на контроль.
	SpLineConfigParamStr.Deviation = SP_LINE_DEVIATION_DEFAULT;
}
//*****************************************************************************
void SpeakerLine_MeasLoop(void){
  
  static uint16_t mSCount   = 0;
	static uint8_t  spLineNum = 0;
	//--------------------
	mSCount++;
	if(mSCount == SP_LINE_OPTO_RELAYL_PERIOD) SpeakerLine_OptoReley(spLineNum, ON);
  if(mSCount >= SP_LINE_MEAS_PERIOD)
		{
			//Измерение напряжения.
			if(SpeakerLine_GetAvrMeas() == 0) return;              
			//Сохранение старого значения.
			SpLineContextArray[spLineNum].OldMeas = SpLineContextArray[spLineNum].Meas;
			//Сохранение нового значения.   
			SpLineContextArray[spLineNum].Meas = SpLineAvrMeas;
			//Отключение оптореле линии Гр.
			SpeakerLine_OptoReley(spLineNum, OFF);
			
			mSCount = 0;
			if(++spLineNum >= SP_LINE_NUMBER) spLineNum = 0;
		}  
	//--------------------	
}
//*****************************************************************************
//Получение установившегося значения после переходного процесса. 
//Вызывается каждую 1мСек.
void SpeakerLine_SteadyStateLoop(void){
  
	static uint8_t  spLineNum = 0;
	static uint16_t mScount   = 0; 
	uint16_t deviation        = SpLineConfigParamStr.Deviation;
	SpLineContext_t *spLine   = SpeakerLine(spLineNum);
	//--------------------
	//Переходной процесс на линии Гр.
  if((spLine->Meas > (spLine->SteadyValue + deviation)) || 
     (spLine->Meas < (spLine->SteadyValue - deviation)))   
    {
      //------
      //Мигающая индикация переходного процесса.     
      if((spLine->Meas >= (spLine->OldMeas + deviation)) || 
         (spLine->Meas <= (spLine->OldMeas - deviation)))   
        {
          if(++mScount >= (SP_LINE_LED_BLINK_PERIOD / 4))
            {
              mScount = 0;
							SpeakerLine_Led(spLineNum, TOGGLE);
							spLine->SteadyValue = SP_LINE_UNDEF;
							spLine->FirstOnFlag = FLAG_SET;
            }   
        }
      //------
      //Если переходной процесс окончен то фиксация установившегося значения и выход. 
      else
        {          
          SpeakerLine_Led(spLineNum,OFF);
					spLine->SteadyValue = spLine->Meas;
          //Сохранение опорных величин линий Гр. при первом включении..					
					if(spLine->FirstOnFlag == FLAG_SET && //если первый переходной процесс
						 spLine->RefValue    == 0)					//и не сохранено опорное значение
						{
							spLine->RefValue = spLine->Meas;
						}
        }
      //------
    }
	 //Переходного процесса нет на линии Гр.
   else
    {   				
      SpeakerLine_Led(spLineNum, OFF);
			spLine->SteadyValue = spLine->Meas;
    }
	//--------------------
	//Проход по всем линиям Гр.	
	if(++spLineNum >= SP_LINE_NUMBER) spLineNum = 0;
	//--------------------
}
//*****************************************************************************
//Настраиваемые параметры для работы контроля линии Гр.
SpLineParam_t* SpeakerLine_Param(void){

	return &SpLineConfigParamStr;
}
//*****************************************************************************
//Контекст линии Гр. Это основная функция для работы с линией Гр.
SpLineContext_t* SpeakerLine(uint8_t speakerLine){
	
	return &SpLineContextArray[speakerLine];
}
//*****************************************************************************
//Сохранение опорных величин линий Гр. дя LC.
uint8_t SpeakerLine_SaveRef(uint8_t ch){

	SpLineContext_t *spLine = SpeakerLine(ch);
  //--------------------  
  //Если идет переходной процесс то не сохранаем опорные величины и выходим.
  if((ch > (SP_LINE_NUMBER-1)) || (spLine->LcStatus == SP_LINE_UNDEF)) return 0;
  //Сохранение опорных величин.
	spLine->RefValue = spLine->SteadyValue;
  return 1;
}  
//*****************************************************************************
//Получение установившихся состояний линий Гр. 
uint8_t SpeakerLine_GetDefStatus(uint8_t ch){

  static uint8_t tempState[SP_LINE_NUMBER];
	SpLineContext_t *spLine = SpeakerLine(ch);
  //--------------------
	//Если линия снята с контроля
	if((SpLineConfigParamStr.Check & (1 << ch)) == OFF)
		{
			tempState[ch] = SP_LINE_NORM;
			return SP_LINE_NORM;
		}
	//Линия на контроле.
  if(spLine->LcStatus == SP_LINE_UNDEF) return tempState[ch];
  tempState[ch] = spLine->LcStatus;
  return tempState[ch];
}
//***************************************************************************** 
//*****************************************************************************
//Машина состояний для управления линией Гр.
static void SpeakerLine_FSM(uint8_t speakerLine){	
	
	SpLineContext_t *spLine = SpeakerLine(speakerLine);
  //--------------------
	//Определение состояния линии Гр.
	SpeakerLine_CalcLcStatus(speakerLine);
	//--------------------
	//Действия с линией Гр.
	switch(spLine->Action){
		//--------------------	
		case(ActionOff):
			SpeakerRelays(OFF, speakerLine);
		break;
		//--------------------	
		default://case(ActionManualOn):
			SpeakerRelays(ON, speakerLine);
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
//*****************************************************************************
void SpeakerLine_FSMLoop(void){
	
	static uint8_t speakerLine = 0;
	//--------------------
	SpeakerLine_FSM(speakerLine);
	if(++speakerLine >= SP_LINE_NUMBER) speakerLine = 0;
}
//*************************************************************************************************
//*************************************************************************************************






































