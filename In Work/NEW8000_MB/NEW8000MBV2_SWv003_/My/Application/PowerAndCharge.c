
#include "PowerAndCharge.h"

//*****************************************************************************
static PowerSTR_t PowerSTR;
//*****************************************************************************
//Получение состояния батареи.
static uint8_t Bat_StateCalc(void){

	uint16_t tempBatMeas = PowerSTR.BatMeas;
  //--------------------
	//контроль отключен.
	if(PowerSTR.Check.bit.Bat == 0) return BAT_CONTROL_OFF; 
	//--------------------
  if( tempBatMeas >  2720)                           return BAT_CHARGE_END; 
  if((tempBatMeas >  2100) && (tempBatMeas <= 2720)) return BAT_BLINK;
  if((tempBatMeas >  1850) && (tempBatMeas <= 2100)) return BAT_ATTENTION;
	if( tempBatMeas <  1500)                           return BAT_NOT_CONNECT; 
  if( tempBatMeas <= 1850)                           return BAT_DEEP; 
  return BAT_NOT_CONNECT;   
}
//*****************************************************************************
void PowerAndCharge_Init(void){

  //Включаем тактирование порта A.
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  //DC_Ok    - PA3 - вход - после сброса настроен как вход.
  //DC_Fault - PA4 - вход - после сброса настроен как вход.
  //AC_Ok    - PA6 - вход - после сброса настроен как вход.

  //ZU_On - PA2 - выход.
  ZuOnGPIO->CRL &= ~GPIO_CRL_CNF2; //PA2 - выход, режим - push-pull.
  ZuOnGPIO->CRL |=  GPIO_CRL_MODE2;//PA2 - тактирование 50МГц.
	//--------------------
	PowerSTR.Check.Byte = POWER_ALL_CHECK_ON;
	PowerSTR.ACState    = POWER_AC_OK;
}
//*****************************************************************************
//Получение состояния основного ввода питания 220В и напряжения на АКБ.
//Вызывается каждые 5мс.
void PowerAndCharge_Loop(void){

  static uint16_t acCheckCount = 0;
	static uint32_t BatMeasTemp  = 0;
  static uint16_t BatMeasCount = 0;
  //--------------------
	//Получение усредненного напряжения на АКБ в мВ.
	BatMeasTemp += Adc_GetMeas(BatAdcCh);
  if(++BatMeasCount == BAT_MEAS_NUM)
    {
			PowerSTR.BatMeas  = (uint16_t)(BatMeasTemp >> BAT_MEAS_SHIFT);
			PowerSTR.BatMeas  = (uint16_t)(((PowerSTR.BatMeas * 11) + 5)/10);
			PowerSTR.BatState = Bat_StateCalc();
      BatMeasTemp  = 0;
      BatMeasCount = 0;
    }	
  //--------------------
	//Получение состояния инвертора.
	if( (Gpio_GetState(PowerGPOI) & DcOkPIN) &&
     !(Gpio_GetState(PowerGPOI) & DcFaultPIN)) PowerSTR.DCState = POWER_DC_OK;

  if(!(Gpio_GetState(PowerGPOI) & DcOkPIN) &&
      (Gpio_GetState(PowerGPOI) & DcFaultPIN)) PowerSTR.DCState = POWER_DC_FAULT;
  //--------------------		
	//Получение состояния основного ввода питания 220В.
  if(!(AcGPOI->IDR & AcPIN))
    {
      acCheckCount = 0;
			PowerSTR.ACState = POWER_AC_OK;
    }
	if(++acCheckCount >= (TIMEOUT_AC_CHECK_mS / 5))// делим на 5 т.к. эта ф-я вызывается каждые 5 мс.
    {
      acCheckCount = 0;
      PowerSTR.ACState = POWER_AC_NOT_CONNECT;
    }
	//--------------------
	//контроль отключен.
	if(PowerSTR.Check.bit.MainPower == 0) PowerSTR.ACState = POWER_CONTROL_OFF;
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
//*************************************************************************************************
//*************************************************************************************************

















