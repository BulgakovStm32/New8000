
#include "PowerAndCharge.h"

//-----------------------------------------------------------------------------
static PowerSTR_t PowerSTR;
//-----------------------------------------------------------------------------
//Получение состояния батареи.
static uint8_t BatStateCalc(void){

	uint16_t TempBatValue = PowerSTR.BatMeas;
  //--------------------
	//контроль отключен.
	if(PowerSTR.Check.bit.Bat == 0) return BatControlOff; 
	//--------------------
  if( TempBatValue >  2720)                            return BatChargeEnd; 
  if((TempBatValue >  2100) && (TempBatValue <= 2720)) return BatBlink;
  if((TempBatValue >  1850) && (TempBatValue <  2100)) return BatAttention;
	if( TempBatValue <  1500)                            return BatNo; 
  if( TempBatValue <= 1850)                            return BatDeep; 
  return BatNo;   
}
//-----------------------------------------------------------------------------
void PowerAndChargeInit(void){

  //Включаем тактирование порта A.
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  //DC_Ok    - PA3 - вход - после сброса настроен как вход.
  //DC_Fault - PA4 - вход - после сброса настроен как вход.
  //AC_Ok    - PA6 - вход - после сброса настроен как вход.

  //ZU_On - PA2 - выход.
  ZuOnGPIO->CRL &= ~GPIO_CRL_CNF2; //PA2 - выход, режим - push-pull.
  ZuOnGPIO->CRL |=  GPIO_CRL_MODE2;//PA2 - тактирование 50МГц.
	//--------------------
	PowerSTR.Check.Byte = 0xFF;
}
//-----------------------------------------------------------------------------
//Получение состояния основного ввода питания 220В и напряжения на АКБ.
//Вызывается в прерывании каждую 1мс.
void PowerAndChargeLoop(void){

  static uint16_t ACmScount    = 0;
	static uint32_t BatMeasTemp  = 0;
  static uint16_t BatMeasCount = 0;
  //--------------------
	//Получение усредненного напряжения на АКБ в мВ.
	BatMeasTemp += AdcGetMeas(BatAdcCh);
  if(++BatMeasCount == BatAdcMeasQuantity)
    {
			PowerSTR.BatMeas  = (uint16_t)(BatMeasTemp >> BatAdcMeasShiftDev);
			PowerSTR.BatMeas  = (uint16_t)(((PowerSTR.BatMeas * 11) + 5)/10);
			PowerSTR.BatState = BatStateCalc();
      BatMeasTemp  = 0;
      BatMeasCount = 0;
    }	
  //--------------------	
	//Получение состояния основного ввода питания 220В.
  if(!(AcGPOI->IDR & AcPIN))
    {
      ACmScount = 0;
			PowerSTR.PowerState = PowerACOk;
    }
  if(++ACmScount >= ACmSecConst)
    {
      ACmScount = 0;
      PowerSTR.PowerState = PowerACNo;
    }
	//--------------------
	//контроль отключен.
	if(PowerSTR.Check.bit.MainPower == 0) PowerSTR.PowerState = PowerControlOff;
}
//-----------------------------------------------------------------------------
PowerSTR_t* Power(void){

	return &PowerSTR;
}
//-----------------------------------------------------------------------------
//Получение состояния инвертора.
//uint8_t PowerDCGetStatus(void){

//  if( (GpioGetState(PowerGPOI) & DcOkPIN) &&
//     !(GpioGetState(PowerGPOI) & DcFaultPIN)) return PowerDCOk;

//  if(!(GpioGetState(PowerGPOI) & DcOkPIN) &&
//      (GpioGetState(PowerGPOI) & DcFaultPIN)) return PowerDCFault;
//  return PowerDCFault;
//}
//-----------------------------------------------------------------------------

















