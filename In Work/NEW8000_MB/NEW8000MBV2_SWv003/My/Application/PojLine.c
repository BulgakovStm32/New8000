
#include "PojLine.h"

//********************************************************************************************
static FireLine_t	FireLineContextArr[4];//Всего 4 входа.
//********************************************************************************************
//********************************************************************************************
//********************************************************************************************
//Определение состояния линии пожарных шлейфов.
static uint8_t FireLine_CalсStatus(uint16_t meas){
  
       if((meas >= 200) && (meas < 800))   return FIRE_LINES_NORM_OPEN;
  else if((meas >= 800) && (meas <= 2000)) return FIRE_LINES_NORM_CLOSE;
  else if(meas < 200) 										 return FIRE_LINES_BREAK;
  else if(meas > 2000) 										 return FIRE_LINES_SHORT;
                                           return FIRE_LINES_UNDEF;  
}
//*****************************************************************************
//*******************Модуль работы с сухими контактами.************************
//Машина состояний для работы с логическими состоями входа ШС.
//вход: line - вход для которой хотим получит состояние.
static void FireLine_LogicFSM(uint8_t input){

					  uint16_t *FireLineState = &FireLineContextArr[input].LogicalState;  
						uint16_t *Type    			= &FireLineContextArr[input].Type;
					  uint16_t *TimeOut 			= &FireLineContextArr[input].TimeOut;
	volatile  uint16_t *PhysicalState = &FireLineContextArr[input].PhysicalState;   
	//--------------------
	switch(*FireLineState){
		//--------------------------
		//Состояние "СНЯТ С КОНТРОЛЯ"
		case(FIRE_LINES_CONTROL_OFF):
			if(*Type == FIRE_LINES_TYPE_ON_CONTROL_ON_CLOSE)//Если включили контроль линии
				{
					*FireLineState = FIRE_LINES_UNDERCONTROL;//Переход в состояние "ПОД КОНТРОЛЕМ".
				}
		break;		
		//--------------------------
		//Состояние "ПОД КОНТРОЛЕМ"
		case(FIRE_LINES_UNDERCONTROL):
			//Линия в состоянии "СРАБОТКА" 
			if(*PhysicalState == FIRE_LINES_NORM_CLOSE)                  
				{
					GlobSwTimer_Start(input);             //Запус отсчета таймаута
					*FireLineState = FIRE_LINES_ATTENTION;//Переход в состояние "ВНИМАНИЕ".
				}
			//Отключили контроль при конфигурировании 	
			if(*Type == FIRE_LINES_TYPE_OFF)
				{
					GlobSwTimer_Stop(input);                //останов отсчета таймаута.
					*FireLineState = FIRE_LINES_CONTROL_OFF;//переход в состояние "СНЯТ С КОНТРОЛЯ".
				}
		break;	
		//--------------------------
		//Состояние "ВНИМАНИЕ"
		case(FIRE_LINES_ATTENTION):
			if(*PhysicalState == FIRE_LINES_NORM_CLOSE)//Если линия все еще в состоянии "СРАБОТКА"
				{
					if(GlobSwTimer_Get(input) >= (*TimeOut * SEC))//и если закончился отсчет таймаута
						{
							GlobSwTimer_Stop(input);         //останов отсчета таймаута.
							*FireLineState = FIRE_LINES_FIRE;//переход в состояние "ПОЖАР".
						}		
				}
			else //Если линии была ДЕАКТИВирванна 
				{
					GlobSwTimer_Stop(input);                 //останов отсчета таймаута.
					*FireLineState = FIRE_LINES_UNDERCONTROL;//переход в сотояние "ПОД КОНТРОЛЕМ". 
				}
		break;
		//--------------------------	
		//Состояние "ПОЖАР"		
		case(FIRE_LINES_FIRE):
			if(*PhysicalState == FIRE_LINES_NORM_OPEN)//Если была ДЕАКТИВАЦИЯ линии, то переход в сотояние "ПОД КОНТРОЛЕМ". 
				{ 
					*FireLineState = FIRE_LINES_UNDERCONTROL;//Переход в состояние "ПОД КОНТРОЛЕМ".
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
//*****************************************************************************
//************Машина состояний для работы с неисправностями входа ШС.**********
//вход: line - вход для которой хотим получит состояние.
static void FireLine_FaultFSM(uint8_t input){
	
					 uint8_t swTimer      = input + SwTimerForPojLineFaultOffset;
					 uint16_t *faultState = &FireLineContextArr[input].FaultState;
	//uint16_t *type          = &FireLineContextArr[input].Type;//так больше маста занимает.  
	volatile uint16_t *physicalState = &FireLineContextArr[input].PhysicalState;  
	//--------------------
	//Отключен контроль ШС.
	if(FireLineContextArr[input].Type == FIRE_LINES_TYPE_OFF)
		{
			*faultState = FIRE_LINES_NORM;//Cостояние "НОРМА ШС".
			return;
		}
	//--------------------	
	//Определение состояния ШС.
	switch(*faultState){
		//--------------------------
		//Состояние "НОРМА ШС".	
		case(FIRE_LINES_NORM):
			if(*physicalState == FIRE_LINES_NORM_CLOSE) *faultState = FIRE_LINES_NORM_CLOSE;//Переход в состояние "Замкнут".
			if(*physicalState == FIRE_LINES_NORM_OPEN)  *faultState = FIRE_LINES_NORM_OPEN; //Переход в состояние "Разомкнут".				
			//На линии зафиксированна неисправность.
			if(*physicalState >= FIRE_LINES_SHORT)
				{
					GlobSwTimer_Start(swTimer);    //Запус отсчета таймаута.
					*faultState = FIRE_LINES_UNDEF;//Переход в состояние "НЕОПРЕДЕЛЕННО".
				}
		break;
		//-----------------------------------------------------		
		//Состояние "Разомкнут".
		case(FIRE_LINES_NORM_OPEN):
			if(*physicalState == FIRE_LINES_NORM_CLOSE) *faultState = FIRE_LINES_NORM_CLOSE;//Переход в состояние "Замкнут".
			//На линии зафиксированна неисправность.
			if(*physicalState >= FIRE_LINES_SHORT)
				{
					GlobSwTimer_Start(swTimer);    //Запус отсчета таймаута.
					*faultState = FIRE_LINES_UNDEF;//Переход в состояние "НЕОПРЕДЕЛЕННО".
				}				
		break;				
		//--------------------------
		//Состояние "Замкнут".
		case(FIRE_LINES_NORM_CLOSE):
			if(*physicalState == FIRE_LINES_NORM_OPEN) *faultState = FIRE_LINES_NORM_OPEN;//Переход в состояние "Разомкнут".
			//На линии зафиксированна неисправность.
			if(*physicalState >= FIRE_LINES_SHORT)
				{
					GlobSwTimer_Start(swTimer);    //Запус отсчета таймаута.
					*faultState = FIRE_LINES_UNDEF;//Переход в состояние "НЕОПРЕДЕЛЕННО".
				}					
		break;				
		//-----------------------------------------------------
		//Состояние "НЕОПРЕДЕЛЕННО".	
		case(FIRE_LINES_UNDEF):
			//Если на входе зафиксированна неисправность.
			if(*physicalState >= FIRE_LINES_SHORT)
				{
					if(GlobSwTimer_Get(swTimer) >= (3 * SEC))//закончился отсчет таймаута
						{
							GlobSwTimer_Stop(swTimer);                                           //останов отсчета таймаута.
							if(*physicalState == FIRE_LINES_SHORT)*faultState = FIRE_LINES_SHORT;//переход в состояние "КЗ ШС".
							if(*physicalState == FIRE_LINES_BREAK)*faultState = FIRE_LINES_BREAK;//переход в состояние "ОБРЫВ ШС".
						}		
				}
			//Вход в состоянии норма.
			else //if(GlobSwTimerGet(swTimer) >= (3 * SEC))
				{
					GlobSwTimer_Stop(swTimer);    //останов отсчета таймаута.
					*faultState = FIRE_LINES_NORM;//Переход в состояние "НОРМА ШС".	
				}
		break;
		//--------------------------
		//Состояние "КЗ ШС". 
		case(FIRE_LINES_SHORT):
			if(*physicalState != FIRE_LINES_SHORT)
				{
					//GlobSwTimerStart(swTimer);  //Запус отсчета таймаута.
					*faultState = FIRE_LINES_UNDEF;//переход в состояние "НЕОПРЕДЕЛЕННО".
				}							
		break;
		//--------------------------
		//Состояние "ОБРЫВ ШС".
		case(FIRE_LINES_BREAK):
			if(*physicalState != FIRE_LINES_BREAK)
				{
					//GlobSwTimerStart(swTimer);  //Запус отсчета таймаута.
					*faultState = FIRE_LINES_UNDEF;//переход в состояние "НЕОПРЕДЕЛЕННО".				
				}
		break;
		//-----------------------------------------------------
		default:
		break;
		//--------------------------
	}
	//--------------------
}
//********************************************************************************************
//********************************************************************************************
//********************************************************************************************
void FireLine_Init(void){

	uint8_t i;
	//--------------------
	for(i=0; i < 4; i++)
		{
			FireLineContextArr[i].Number        = i;
			FireLineContextArr[i].FaultState    = FIRE_LINES_NORM;
			FireLineContextArr[i].LogicalState  = FIRE_LINES_CONTROL_OFF;
			FireLineContextArr[i].PhysicalState = 0;
			FireLineContextArr[i].MeasValue     = 0;
			//Тип и таймаут устанавливаются при чтении конфигурации из флеш. 
			//FireLineContextArr[i].Type				  = DEFAULT_FIRE_LINES_INPUT_TYPE;
			//FireLineContextArr[i].TimeOut       = DEFAULT_FIRE_LINES_INPUT_TIMEOUT;
		}
}
//*****************************************************************************
//Измерение и усреднение по 16 выборкам для 5-ти каналов АЦП. Вызывается в прерывании 
//таймера каждую 1 мС.
void FireLine_MeasLoop(void){
  
  static uint32_t MeasTemp  = 0;
  static uint8_t  MeasCount = 0;
  static uint8_t  Channel   = 0; 
  //--------------------
  MeasTemp += Adc_GetMeas(Channel);
  if(++MeasCount >= AdcMeasQuantity)
    {      
			//Сохранение измеренного и усредненного сначения 
			FireLineContextArr[Channel].MeasValue     = (uint16_t)(MeasTemp >> AdcMeasShiftDev);
			//Обновление состояний пожарных шлейфов.
			FireLineContextArr[Channel].PhysicalState = FireLine_CalсStatus(FireLineContextArr[Channel].MeasValue);
      MeasTemp  = 0;
      MeasCount = 0;
			if(++Channel >= FIRE_LINES_NUMBER)Channel = 0;
    }
  //--------------------
}
//*****************************************************************************
void FireLine_FSMLoop(void){
	
	static uint8_t fireLine = 0;
	//--------------------
	FireLine_LogicFSM(fireLine);										 //определение логического состояния ШС.
	FireLine_FaultFSM(fireLine);										 //определение неисправности ШС.
	if(++fireLine >= FIRE_LINES_NUMBER) fireLine = 0;//Работа с 4-мя входами ШС.
}
//*****************************************************************************
FireLine_t* FireLine(uint8_t line){

	return &FireLineContextArr[line];
}
//*****************************************************************************
//Получение физического состояния пожарных линий.
uint8_t FireLine_GetPhysicalState(uint8_t line){

	//при отключенном контроле возвращаем неопределенное состояние
	if(FireLineContextArr[line].LogicalState == FIRE_LINES_CONTROL_OFF)return FIRE_LINES_UNDEF;
  return FireLineContextArr[line].PhysicalState;
}
//*****************************************************************************
void FireLine_GetContext(uint8_t *pBuf ,uint8_t numLine){

	FireLine_t* context = (FireLine_t*)pBuf;
	//--------------------
	context->Number        = FireLineContextArr[numLine].Number;
	context->Type          = FireLineContextArr[numLine].Type;
	context->TimeOut       = FireLineContextArr[numLine].TimeOut;
	context->PhysicalState = FireLineContextArr[numLine].PhysicalState;
	context->MeasValue     = FireLineContextArr[numLine].MeasValue;
}
//********************************************************************************************
//********************************************************************************************
//********************************************************************************************


























