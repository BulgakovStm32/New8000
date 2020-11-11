
#include "PowerAndCharge.h"

//*************************************************************************************************
static PowerSTR_t PowerSTR;
//*************************************************************************************************
//*************************************************************************************************
//Получение состояния батареи.
static uint8_t Bat_StateCalc(void){

	uint16_t tempBatMeas = PowerSTR.BatMeas;
  //--------------------
	//контроль отключен.
	if(PowerSTR.CheckConfig.bits.Bat == BAT_CHECK_OFF) return BAT_CHECK_OFF; 
	//--------------------
  if( tempBatMeas >  2720)                           return BAT_CHARGE_END; 
  if((tempBatMeas >  2100) && (tempBatMeas <= 2720)) return BAT_CHARGE;
  if((tempBatMeas >  1850) && (tempBatMeas <= 2100)) return BAT_ATTENTION;
	if( tempBatMeas <  1500)                           return BAT_NOT_CONNECT; 
  if( tempBatMeas <= 1850)                           return BAT_DEEP; 
  return BAT_NOT_CONNECT;   
}
//*************************************************************************************************
//*************************************************************************************************
void PowerAndCharge_Init(void){

  //Включаем тактирование порта A.
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  //DC_Ok    - PA3 - вход - после сброса настроен как вход.
  //DC_Fault - PA4 - вход - после сброса настроен как вход.
  //AC_Ok    - PA6 - вход - после сброса настроен как вход.

  //ZU_On - PA2 - выход.
  ZuOnGPIO->CRL &= ~GPIO_CRL_CNF2; //PA2 - выход, режим - push-pull.
  ZuOnGPIO->CRL |=  GPIO_CRL_MODE2;//PA2 - тактирование 50МГц.
}
//*****************************************************************************
//Получение состояния основного ввода питания 220В и напряжения на АКБ.
//Вызывается каждые 5мс.
void PowerAndCharge_Loop(void){

  static uint16_t acCheckCount = 0;
	static uint32_t batMeasTemp  = 0;
  static uint16_t batMeasCount = 0;
  //--------------------
	//Получение усредненного напряжения на АКБ в мВ.
	batMeasTemp += Adc_GetMeas(BatAdcCh);
  if(++batMeasCount == BAT_MEAS_NUM)
    {
			PowerSTR.BatMeas  = (uint16_t)(batMeasTemp >> BAT_MEAS_SHIFT);
			PowerSTR.BatMeas  = (uint16_t)(((PowerSTR.BatMeas * 11) + 5)/10);
			PowerSTR.State.bits.Bat = Bat_StateCalc();
      batMeasTemp  = 0;
      batMeasCount = 0;
    }	
  //--------------------
	//Получение состояния инвертора.
	uint16_t gpio = Gpio_GetState(PowerGPOI);
		
	if( (gpio & DcOkPIN) &&
     !(gpio & DcFaultPIN)) PowerSTR.State.bits.DC = POWER_DC_OK;

  if(!(gpio & DcOkPIN) &&
      (gpio & DcFaultPIN)) PowerSTR.State.bits.DC = POWER_DC_FAULT;
  //--------------------		
	//Получение состояния основного ввода питания 220В.
  if(!(AcGPOI->IDR & AcPIN))
    {
      acCheckCount = 0;
			PowerSTR.State.bits.AC = POWER_AC_OK;
    }
	if(++acCheckCount >= (TIMEOUT_AC_CHECK_mS / 5))// делим на 5 т.к. эта ф-я вызывается каждые 5 мс.
    {
      acCheckCount = 0;
      PowerSTR.State.bits.AC = POWER_AC_FAULT;
    }
	//--------------------
	//контроль отключен.
	if(PowerSTR.CheckConfig.bits.AC == POWER_AC_CHECK_OFF) PowerSTR.State.bits.AC = POWER_AC_CHECK_OFF;
}
//*****************************************************************************
PowerSTR_t* Power(void){

	return &PowerSTR;
}
//*************************************************************************************************
//*******************************Зарядное устройство.**********************************************
void Charger_Activate(void){

	ZuOn();
}
//*****************************************************************************
void Charger_Deactivate(void){

	ZuOff();
}
//*****************************************************************************
void Charger_FSMLoop(void){
	
	       uint8_t batState = PowerSTR.State.bits.Bat;
	static uint8_t state    = BAT_CONNECT;
  //--------------------
	switch(state){			
		//------------
		case(BAT_CONNECT):
			if(batState == BAT_ATTENTION ||
			   batState == BAT_CHARGE)   
				{
					state = BAT_CHARGE;
				}		
			if(batState == BAT_NOT_CONNECT) state = BAT_NOT_CONNECT;				
		break;
		//------------
		case(BAT_CHARGE):
			ZuOn();
			if(batState == BAT_CHARGE_END)  
				{
					state = BAT_CHARGE_END;
					SwTimers_Clr(0);
				}
		  if(batState == BAT_NOT_CONNECT) state = BAT_NOT_CONNECT;	  
		break;
		//------------
		case(BAT_CHARGE_END):
			ZuOff();
			if(SwTimers_Get(0) >= (5*SEC))
				{
					if(batState != BAT_CHARGE_END) state = BAT_CONNECT;
					else SwTimers_Clr(0);
				}
		break;
		//------------
		case(BAT_NOT_CONNECT):
			ZuOff();
			if(batState != BAT_NOT_CONNECT) state = BAT_CONNECT;	
		break;				
		//------------
		default:
			state = BAT_CONNECT;	
		break;		
		//------------
	}
}
//*************************************************************************************************
//*************************************************************************************************

















