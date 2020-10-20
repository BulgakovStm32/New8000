
#include "PojLine.h"

//-----------------------------------------------------------------------------
//PojLineFlagsSTR_Type     PojLineFlagsSTR;      //Флаги состояний
FireLineContextSTR_Type	 FireLineContextArr[4];//Всего 4 входа.
//-----------------------------------------------------------------------------
void FireLineInit(void){

	uint8_t i;
	//--------------------
	for(i=0; i < 4; i++)
		{
			FireLineContextArr[i].FaultState    = FireLineNorm;
			FireLineContextArr[i].LogicalState  = FireLineCONTROLOFF;
			FireLineContextArr[i].Type				  = InputOnControlOnClose;
			FireLineContextArr[i].TimeOut       = 5;
			FireLineContextArr[i].PhysicalState = 0;
			FireLineContextArr[i].MeasValue     = 0;
		}
}
//-----------------------------------------------------------------------------
//Определение состояния линии пожарных шлейфов.
static uint8_t FireLineCalсStatus(uint16_t meas){
  
       if((meas >= 200) && (meas < 800))   return FireLineNormOpen;
  else if((meas >= 800) && (meas <= 2000)) return FireLineNormClose;
  else if(meas < 200) 										 return FireLineBreak;//FireLineShort;
  else if(meas > 2000) 										 return FireLineShort;//FireLineBreak;
                                           return FireLineUndef;  
}
//-----------------------------------------------------------------------------
//Измерение и усреднение по 16 выборкам для 5-ти каналов АЦП.
void FireLineMeasLoop(void){
  
  static uint32_t MeasTemp  = 0;
  static uint8_t  MeasCount = 0;
  static uint8_t  Channel   = 0; 
  //--------------------
  MeasTemp += (uint32_t)AdcGetMeas(Channel);
  if(++MeasCount == AdcMeasQuantity)
    {      
			//Сохранение измеренного и усредненного сначения 
			FireLineContextArr[Channel].MeasValue     = (uint16_t)(MeasTemp >> AdcMeasShiftDev);
			//Обновление состояний пожарных шлейфов.
			FireLineContextArr[Channel].PhysicalState = FireLineCalсStatus(FireLineContextArr[Channel].MeasValue);
      MeasTemp  = 0;
      MeasCount = 0;
			if(++Channel >= NumberOfFireLines)Channel = 0;
    }
  //--------------------
}
//-----------------------------------------------------------------------------
//флаги состояний
//PojLineFlagsSTR_Type* FireLineFlags(void){

//  return &PojLineFlagsSTR; 
//}
//-----------------------------------------------------------------------------
//Получение измеренных значений пожарных линий в мВ.
uint16_t FireLineGetValue(uint8_t line){
  
  return FireLineContextArr[line].MeasValue;
}
//-----------------------------------------------------------------------------
//Получение физического состояния пожарных линий.
uint8_t FireLineGetPhysicalState(uint8_t line){

	//при отключенном контроле возвращаем неопределенное состояние
	if(FireLineContextArr[line].LogicalState == FireLineCONTROLOFF)return FireLineUndef;
  return FireLineContextArr[line].PhysicalState;
}
//-----------------------------------------------------------------------------
//Получение логического состояния пожарных линий.
uint8_t FireLineGetLogicState(uint8_t line){

  return FireLineContextArr[line].LogicalState;
}
//***************************************************************************************************** 
//*****************************************************************************************************
////////////////////Модуль работы с сухими контактами./////////////////////////
//Машина состояний для работы с логическими состоями входа ШС.
//вход: line - вход для которой хотим получит состояние.
void FireLineLogicFSM(uint8_t input){

					  uint8_t *FireLineState = &FireLineContextArr[input].LogicalState;  
						uint8_t *Type    			 = &FireLineContextArr[input].Type;
					  uint8_t *TimeOut 			 = &FireLineContextArr[input].TimeOut;
	volatile  uint8_t *PhysicalState = &FireLineContextArr[input].PhysicalState;   
	//--------------------
	switch(*FireLineState){
		//--------------------------
		//Состояние "СНЯТ С КОНТРОЛЯ"
		case(FireLineCONTROLOFF):
			if(*Type == InputOnControlOnClose)//Если включили контроль линии
				{
					*FireLineState = FireLineUNDERCONTROL;//Переход в состояние "ПОД КОНТРОЛЕМ".
				}
		break;		
		//--------------------------
		//Состояние "ПОД КОНТРОЛЕМ"
		case(FireLineUNDERCONTROL):
			//Линия в состоянии "СРАБОТКА" 
			if(*PhysicalState == FireLineNormClose)                  
				{
					GlobSwTimerStart(input);           //Запус отсчета таймаута
					*FireLineState = FireLineATTENTION;//Переход в состояние "ВНИМАНИЕ".
				}
			//Отключили контроль при конфигурировании 	
			if(*Type == InputOff)
				{
					GlobSwTimerStop(input);             //останов отсчета таймаута.
					*FireLineState = FireLineCONTROLOFF;//переход в состояние "СНЯТ С КОНТРОЛЯ".
				}
		break;	
		//--------------------------
		//Состояние "ВНИМАНИЕ"
		case(FireLineATTENTION):
			if(*PhysicalState == FireLineNormClose)//Если линия все еще в состоянии "СРАБОТКА"
				{
					if(GlobSwTimerGet(input) >= (*TimeOut * SEC))//и если закончился отсчет таймаута
						{
							GlobSwTimerStop(input);       //останов отсчета таймаута.
							*FireLineState = FireLineFIRE;//переход в состояние "ПОЖАР".
						}		
				}
			else //Если линии была ДЕАКТИВирванна 
				{
					GlobSwTimerStop(input);               //останов отсчета таймаута.
					*FireLineState = FireLineUNDERCONTROL;//переход в сотояние "ПОД КОНТРОЛЕМ". 
				}
		break;
		//--------------------------	
		//Состояние "ПОЖАР"		
		case(FireLineFIRE):
			if(*PhysicalState == FireLineNormOpen)//Если была ДЕАКТИВАЦИЯ линии, то переход в сотояние "ПОД КОНТРОЛЕМ". 
				{ 
					*FireLineState = FireLineUNDERCONTROL;//Переход в состояние "ПОД КОНТРОЛЕМ".
				}
		break;
		//--------------------------	
		default:
		break;
		//--------------------------
	}
	//--------------------
	//return FireLineUndef;
}
//-----------------------------------------------------------------------------
//Машина состояний для работы с неисправностями входа ШС.
//вход: line - вход для которой хотим получит состояние.
void FireLineFaultFSM(uint8_t input){
	
					 uint8_t swTimer        = input + SwTimerForPojLineFaultOffset;
					 uint8_t *faultState    = &FireLineContextArr[input].FaultState;
	//uint8_t *type          = &FireLineContextArr[input].Type;//так больше маста занимает.  
	volatile uint8_t *physicalState = &FireLineContextArr[input].PhysicalState;  
	//--------------------
	//Отключен контроль ШС.
	if(FireLineContextArr[input].Type == InputOff)
		{
			*faultState = FireLineNorm;//Cостояние "НОРМА ШС".
			return;
		}
	//--------------------	
	//Определение состояния ШС.
	switch(*faultState){
		//--------------------------
		//Состояние "НОРМА ШС".	
		case(FireLineNorm):
			if(*physicalState == FireLineNormClose) *faultState = FireLineNormClose;//Переход в состояние "Замкнут".
			if(*physicalState == FireLineNormOpen)  *faultState = FireLineNormOpen; //Переход в состояние "Разомкнут".				
			//На линии зафиксированна неисправность.
			if(*physicalState >= FireLineShort)
				{
					GlobSwTimerStart(swTimer);  //Запус отсчета таймаута.
					*faultState = FireLineUndef;//Переход в состояние "НЕОПРЕДЕЛЕННО".
				}
		break;
		//-----------------------------------------------------		
		//Состояние "Разомкнут".
		case(FireLineNormOpen):
			if(*physicalState == FireLineNormClose) *faultState = FireLineNormClose;//Переход в состояние "Замкнут".
			//На линии зафиксированна неисправность.
			if(*physicalState >= FireLineShort)
				{
					GlobSwTimerStart(swTimer);  //Запус отсчета таймаута.
					*faultState = FireLineUndef;//Переход в состояние "НЕОПРЕДЕЛЕННО".
				}				
		break;				
		//--------------------------
		//Состояние "Замкнут".
		case(FireLineNormClose):
			if(*physicalState == FireLineNormOpen) *faultState = FireLineNormOpen;//Переход в состояние "Разомкнут".
			//На линии зафиксированна неисправность.
			if(*physicalState >= FireLineShort)
				{
					GlobSwTimerStart(swTimer);  //Запус отсчета таймаута.
					*faultState = FireLineUndef;//Переход в состояние "НЕОПРЕДЕЛЕННО".
				}					
		break;				
		//-----------------------------------------------------
		//Состояние "НЕОПРЕДЕЛЕННО".	
		case(FireLineUndef):
			//Если на входе зафиксированна неисправность.
			if(*physicalState >= FireLineShort)
				{
					if(GlobSwTimerGet(swTimer) >= (3 * SEC))//закончился отсчет таймаута
						{
							GlobSwTimerStop(swTimer);                                      //останов отсчета таймаута.
							if(*physicalState == FireLineShort)*faultState = FireLineShort;//переход в состояние "КЗ ШС".
							if(*physicalState == FireLineBreak)*faultState = FireLineBreak;//переход в состояние "ОБРЫВ ШС".
						}		
				}
			//Вход в состоянии норма.
			else //if(GlobSwTimerGet(swTimer) >= (3 * SEC))
				{
					GlobSwTimerStop(swTimer);  //останов отсчета таймаута.
					*faultState = FireLineNorm;//Переход в состояние "НОРМА ШС".	
				}
		break;
		//--------------------------
		//Состояние "КЗ ШС". 
		case(FireLineShort):
			if(*physicalState != FireLineShort)
				{
					//GlobSwTimerStart(swTimer);  //Запус отсчета таймаута.
					*faultState = FireLineUndef;//переход в состояние "НЕОПРЕДЕЛЕННО".
				}							
		break;
		//--------------------------
		//Состояние "ОБРЫВ ШС".
		case(FireLineBreak):
			if(*physicalState != FireLineBreak)
				{
					//GlobSwTimerStart(swTimer);  //Запус отсчета таймаута.
					*faultState = FireLineUndef;//переход в состояние "НЕОПРЕДЕЛЕННО".				
				}
		break;
		//-----------------------------------------------------
		default:
		break;
		//--------------------------
	}
	//--------------------
}
//-----------------------------------------------------------------------------
FireLineContextSTR_Type* FireLine(uint8_t input){

	return &FireLineContextArr[input];
}
//-----------------------------------------------------------------------------


























