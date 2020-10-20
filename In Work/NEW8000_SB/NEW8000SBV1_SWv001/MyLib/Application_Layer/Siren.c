
#include "Siren.h"

//******************************************************************************************
static SirenLine_t SirenLineContextArr[4];//3 линии табло и 1 одна линия питания.
//******************************************************************************************
//******************************************************************************************
//Опеределение состояния линии при отключенном выходе.
static void Siren_GetOffLineStatus(uint8_t line){
	
//	uint16_t meas  =  SirenLineContextArr[line].AdcMeasValue;//Берем измерение для канала.;
//  uint8_t* state = &SirenLineContextArr[line].LineState;
//	//--------------------	
//	switch(*state){
//		//-----------
//		case(SIREN_LINE_NORM):
//			//КЗ линии.
//			if(meas < 35) *state = SIREN_LINE_SHORT;
//			//Обрыв линии
//			if(meas > 2500) *state = SIREN_LINE_BREAK;
//		return;
//		//-----------
//		case(SIREN_LINE_SHORT):
//		 //Линия в норме.
//		 if((meas > 50) && (meas <= 2500)) *state = SIREN_LINE_NORM;
//		 //Обрыв линии
//		 if(meas > 2500) *state = SIREN_LINE_BREAK;
//		  
//		return;
//		//-----------
//		case(SIREN_LINE_BREAK):
//		 //Линия в норме.
//		 if((meas > 50) && (meas <= 2500)) *state = SIREN_LINE_NORM;			
//		 //КЗ линии.
//		 if(meas < 45) *state = SIREN_LINE_SHORT;
//		return;			
//		//-----------
//		default: //Неизвестная команда.
//		break;	
//		//-----------
//	}	
//	//--------------------
	
	uint16_t meas;
	uint8_t	 stateTemp;
	//--------------------
  meas = SirenLineContextArr[line].AdcMeasValue;//Берем измерение для канала.

	     if(meas < 40)                     stateTemp = SIREN_LINE_SHORT;//КЗ линии.
	else if((meas >= 40) && (meas <= 2500))stateTemp = SIREN_LINE_NORM; //Линия в норме.
	else if(meas > 2500)                   stateTemp = SIREN_LINE_BREAK;//Обрыв линии

	SirenLineContextArr[line].LineState = stateTemp;
}
//*****************************************************************************
//Опеределение состояния линии при активированном выходе.
static void Siren_GetOnLineStatus(uint8_t line){
	
//	uint16_t meas  =  SirenLineContextArr[line].AdcMeasValue;//Берем измерение для канала.;
//  uint8_t* state = &SirenLineContextArr[line].LineState;
//	//--------------------	
//	switch(*state){
//		//-----------
//		case(SIREN_LINE_NORM):
//			//КЗ линии.
//			if(meas > 1700)*state = SIREN_LINE_SHORT;
//			//Обрыв линии
//			if(meas < 40)  *state = SIREN_LINE_BREAK;
//		return;
//		//-----------
//		case(SIREN_LINE_SHORT):
//		 //Линия в норме.
//		 if((meas > 50) && (meas <= 1700)) *state = SIREN_LINE_NORM;
//		 //Обрыв линии
//		 if(meas < 40) *state = SIREN_LINE_BREAK;
//		return;
//		//-----------
//		case(SIREN_LINE_BREAK):
//		 //Линия в норме.
//		 if((meas > 50) && (meas <= 1700)) *state = SIREN_LINE_NORM;			
//		 //КЗ линии.
//		 if(meas > 1600) *state = SIREN_LINE_SHORT;
//		return;			
//		//-----------
//		default: //Неизвестная команда.
//		break;	
//		//-----------
//	}	
//	//--------------------	
	
	
	
	
	
	uint16_t meas;
	uint8_t	 stateTemp;
	//--------------------	
  meas = SirenLineContextArr[line].AdcMeasValue;//Берем измерение для канала.
	
			 if(meas > 1600)                    stateTemp = SIREN_LINE_SHORT;//КЗ линии.
	else if((meas >= 30) && (meas <= 1600)) stateTemp = SIREN_LINE_NORM; //Линия в норме.
	else if(meas < 30)                      stateTemp = SIREN_LINE_BREAK;//Обрыв линии
	
	SirenLineContextArr[line].LineState = stateTemp;
}
//*****************************************************************************
//Активация реле оповещателя.
static void Siren_On(uint8_t line){
	
			 if(line == SIREN_1) RelSiren1On();
	else if(line == SIREN_2) RelSiren2On();
	else if(line == SIREN_3) RelSiren3On();
	else if(line == SIREN_ALL) 
		{
			RelSiren1On();
			RelSiren2On();
			RelSiren3On();
		}
}	
//*****************************************************************************
//Отключение реле оповещателя.
static void Siren_Off(uint8_t line){

			 if(line == SIREN_1) RelSiren1Off();
	else if(line == SIREN_2) RelSiren2Off();
	else if(line == SIREN_3) RelSiren3Off();
	else if(line == SIREN_ALL) 
		{
			RelSiren1Off();
			RelSiren2Off();
			RelSiren3Off();
		}
}
//******************************************************************************************
//******************************************************************************************
void Siren_Init(void){
	
	RCC->AHBENR  |=   RCC_AHBENR_GPIOAEN; //Включаем тактирование порта A. 
	GPIOA->MODER &= ~(GPIO_MODER_MODER0 | //PA0 - аналоговый вход.
									  GPIO_MODER_MODER1 | //PA1 - аналоговый вход.
									  GPIO_MODER_MODER2 | //PA2 - аналоговый вход.
									  GPIO_MODER_MODER3); //PA3 - аналоговый вход.	
	
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0 |	//PA0 - No pull-up, pull-down.
									  GPIO_PUPDR_PUPDR1 |	//PA1 - No pull-up, pull-down.
									  GPIO_PUPDR_PUPDR2 |	//PA2 - No pull-up, pull-down.
									  GPIO_PUPDR_PUPDR3);	//PA3 - No pull-up, pull-down.
	//--------------------
	Adc_Init();
	
	SirenLineContextArr[SIREN_1].LineState = SIREN_LINE_NORM;
	SirenLineContextArr[SIREN_2].LineState = SIREN_LINE_NORM;
	SirenLineContextArr[SIREN_3].LineState = SIREN_LINE_NORM;

}
//*****************************************************************************
//Измерение и усреднение.
void Siren_MeasLoop(void){
  
  static uint32_t measTemp  = 0;
  static uint8_t  measCount = 0;
  static uint8_t  channel   = 0;	
	//--------------------	
  measTemp += Adc_GetMeas(channel);//Получение одного измерения АЦП для канала 
  if(++measCount == ADC_MEAS_NUM)  //Расчет усредненного значения для одного канала.
    {   
			SirenLineContextArr[channel].AdcMeasValue = (uint16_t)(((measTemp * ADC_QUANT + 500) / 1000) >> ADC_MEAS_SHIFT);			
      measTemp  = 0;
      measCount = 0;
			if(++channel > (SIREN_LINE_NUM - 1)) channel = 0;
    }
  //--------------------
}
//*****************************************************************************
//Получение измеренных значений овещателей в мВ.
uint16_t Siren_GetMeas(uint8_t ch){
  
	if(ch == SIREN_POWER) return ((SirenLineContextArr[ch].AdcMeasValue * 11 + 5) / 10);
  else                  return   SirenLineContextArr[ch].AdcMeasValue;
}
//*****************************************************************************
uint8_t Siren_LineStatus(uint8_t line){

	return SirenLineContextArr[line].LineState;
}
//*****************************************************************************
void Siren_ControlFromMB(uint8_t ch){
	
	uint8_t cmd = DataFromMB(ch)->Out;
	//--------------------
	switch(cmd){
		//-----------
		case(SIREN_OUT_OFF):
			Siren_OutputFSM(ch, SIREN_OUT_OFF);
		return;
		//-----------
		case(SIREN_OUT_ON):
		  Siren_OutputFSM(ch, SIREN_OUT_ON);
		return;
		//-----------
		case(SIREN_OUT_Blink1):
			if(Blink(INTERVAL_500_mS)) Siren_OutputFSM(ch, SIREN_OUT_ON);
			else		                   Siren_OutputFSM(ch, SIREN_OUT_OFF);
		return;
		//-----------
		case(SIREN_OUT_Blink2):
			if(Blink(INTERVAL_250_mS)) Siren_OutputFSM(ch, SIREN_OUT_ON);
			else		                   Siren_OutputFSM(ch, SIREN_OUT_OFF);
		break;					
		//-----------
		default: //Неизвестная команда.
		break;	
		//-----------
	}
}
//*****************************************************************************
//Машина состояний для управление выходами.
void Siren_OutputFSM(uint8_t siren, uint8_t cmd){
	
	uint8_t *sirenState = &SirenLineContextArr[siren].LogicalState;
	//--------------------
	switch(*sirenState){
		//--------------------
		//Выход отключен.
		case(SIREN_OUT_OFF):
		  Siren_Off(siren);
			Siren_GetOffLineStatus(siren);
			//Если линия неисправна то выходим.
			if(Siren_LineStatus(siren) != SIREN_LINE_NORM) return;
			//Если пришла команда активации выхода то переход в состояние OUT_STATE_OFF_to_ON .
			if(cmd == SIREN_OUT_ON)
				{
					Adc_Stop();
					GlobSwTimer_Start(siren);//Запус отсчета таймаута
					*sirenState = OUT_STATE_OFF_to_ON;
				}	
		return;
		//--------------------
		//Переход из состояния ОТКЛ в состояние ВКЛ
		case(OUT_STATE_OFF_to_ON):
			//Если закончился отсчет таймаута
			if(GlobSwTimer_Get(siren) >= TIMEOUT_OUTPUT)
				{
					GlobSwTimer_Stop(siren);
					Siren_On(siren);
					GlobSwTimer_Start(siren);
					*sirenState = OUT_STATE_MEAS_ON;	
				}	
			//Если пришла команда отключения выхода 
			if(cmd == SIREN_OUT_OFF)
				{
					GlobSwTimer_Stop(siren);
					*sirenState = SIREN_OUT_OFF;
				}				
		return;
		//--------------------
		case(OUT_STATE_MEAS_ON):
			//Если закончился отсчет таймаута
			if(GlobSwTimer_Get(siren) >= TIMEOUT_OUTPUT)
				{
					GlobSwTimer_Stop(siren);
					Adc_Start();
					GlobSwTimer_Start(siren);
					*sirenState = OUT_STATE_ON;	
				}	
		return;				
		//--------------------
		//Выход активен.
		case(OUT_STATE_ON):
			//Если закончился отсчет таймаута
			if(GlobSwTimer_Get(siren) >= TIMEOUT_OUTPUT)
				{
					GlobSwTimer_Stop(siren);//останов отсчета таймаута.
					Siren_On(siren);
					Siren_GetOnLineStatus(siren);
				}	
			//Если линия неисправна или пришла команда отключения выхода
			if(Siren_LineStatus(siren) != SIREN_LINE_NORM || cmd == SIREN_OUT_OFF)
				{
					GlobSwTimer_Start(siren);         
					*sirenState = OUT_STATE_ON_to_OFF;
				}
		return;
		//--------------------
		//Переход из состояния ВКЛ в состояние ОТКЛ
		case(OUT_STATE_ON_to_OFF):
			if(GlobSwTimer_Get(siren) >= TIMEOUT_OUTPUT)
				{
					Adc_Start();	
					GlobSwTimer_Stop(siren);
					*sirenState = OUT_STATE_OFF;
				}
		return;					
		//--------------------
		default: 
		break;	
		//--------------------
	}
}
//******************************************************************************************
//******************************************************************************************








