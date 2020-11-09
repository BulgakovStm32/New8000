
#include "Led.h"

//-----------------------------------------------------------------------------
static SM1628Leds_t	  LedStr;
//-----------------------------------------------------------------------------
void Led_Update(void){

  Sm1628SendLed((uint8_t*)&LedStr);
}
//-----------------------------------------------------------------------------
void Led_SetAll(uint8_t state){
	
	uint8_t* p = (uint8_t*)&LedStr;
	//-------------------
	for(uint8_t i = 0; i < LED_BUF_SIZE; i++) 
		{
			*(p+i) = state;
		}	
}
//-----------------------------------------------------------------------------
void Led_Control(uint8_t ledBit, uint8_t state){

	uint8_t offset;
  uint8_t bit;
	uint8_t* p = (uint8_t*)&LedStr;
  //--------------------
  if(ledBit >= 8) offset = ledBit / 8;
  bit = (ledBit - (offset*8));
  //--------------------
  //Это проверка нужна для корректного управления одноцветными светодиодами.
  if((ledBit != TEST_LED)   && 
     (ledBit != ZUMMER_LED) &&
     (ledBit != ALERT_LED)  &&
     (ledBit != START_LED)  &&
     (ledBit != FIRE_LED))
    {
      p[offset] &= ~(3<<bit);//Гашение обоих светодиодов.
    }
  //--------------------
			 if(state == GreenColor)  p[offset] |=  (1<<bit);
  else if(state == RedColor)    p[offset] |=  (2<<bit);
  else if(state == YellowColor) p[offset] |=  (3<<bit);
  else                          p[offset] &= ~(3<<bit);//Гашение обоих светодиодов.
}
//-----------------------------------------------------------------------------
void Led_Pusk(uint8_t state){

  if(state == LedOn) LedStr.START_LED_bit = 1;
  else               LedStr.START_LED_bit = 0;
}
//-----------------------------------------------------------------------------
void Led_Poj(uint8_t state){

  if(state == LedOn) LedStr.FIRE_LED_bit = 1;
  else               LedStr.FIRE_LED_bit = 0;
}
//-----------------------------------------------------------------------------
void Led_Alert(uint8_t state){
	
	if(state == LedOn) LedStr.ALERT_LED_bit = 1;
	else							 LedStr.ALERT_LED_bit = 0;
}
//-----------------------------------------------------------------------------
void Led_Zummer(uint8_t state){
	
	if(state == LedOn) LedStr.ZUMMER_LED_bit = 1;
	else							 LedStr.ZUMMER_LED_bit = 0;
}
//-----------------------------------------------------------------------------
void Led_Link(uint8_t state){

  //Гашение обоих светодиодов.
  LedStr.Communication_GREEN_LED_bit = 0;
  LedStr.Communication_RED_LED_bit   = 0;
  //--------------------
			 if(state == GreenColor)LedStr.Communication_GREEN_LED_bit = 1;
  else if(state == RedColor)  LedStr.Communication_RED_LED_bit = 1;
  else if(state == YellowColor)
    {
      LedStr.Communication_GREEN_LED_bit = 1;
      LedStr.Communication_RED_LED_bit   = 1;
    }
}
//-----------------------------------------------------------------------------
static void LedPower(uint8_t state){

  //Гашение обоих светодиодов.
  LedStr.Power_GREEN_LED_bit = 0;
	LedStr.Power_RED_LED_bit   = 0;
  //--------------------
			 if(state == GreenColor) LedStr.Power_GREEN_LED_bit = 1;
  else if(state == RedColor)   LedStr.Power_RED_LED_bit = 1;
  else if(state == YellowColor)
    {
			LedStr.Power_GREEN_LED_bit = 1;
      LedStr.Power_RED_LED_bit   = 1;
    }
}
//-----------------------------------------------------------------------------
void Led_Power(PowerSTR_t* power){
		
       if(power->State.bits.AC == POWER_AC_OK &&
				  power->State.bits.DC == POWER_DC_OK)    LedPower(GreenColor);//Питание в норме.
  else if(power->State.bits.AC == POWER_AC_FAULT) LedPower(RedColor);  //Отсутствует основное питание.
  else if(power->State.bits.DC == POWER_DC_FAULT) LedPower(YellowColor & Blink(INTERVAL_250_mS));//Неисправен инвертор.
}
//-----------------------------------------------------------------------------
static void LedBattery(uint8_t state){

  //Гашение обоих светодиодов.
  LedStr.Battery_GREEN_LED_bit = 0;
  LedStr.Battery_RED_LED_bit   = 0;
  //--------------------
       if(state == GreenColor) LedStr.Battery_GREEN_LED_bit = 1;
  else if(state == RedColor)   LedStr.Battery_RED_LED_bit = 1;
  else if(state == YellowColor)
    {
      LedStr.Battery_GREEN_LED_bit = 1;
      LedStr.Battery_RED_LED_bit   = 1;
    }
}
//-----------------------------------------------------------------------------
void Led_Bat(uint8_t state){
	
	if(state == BAT_CHECK_OFF)  LedBattery(LedOff);
  if(state == BAT_CHARGE_END) LedBattery(GreenColor);
  if(state == BAT_CHARGE)     LedBattery(GreenColor  & (uint8_t)Blink(INTERVAL_250_mS));
  if(state == BAT_ATTENTION)  LedBattery(YellowColor & (uint8_t)Blink(INTERVAL_250_mS));
  if(state >= BAT_DEEP)       LedBattery(RedColor);
}
//-----------------------------------------------------------------------------
static void LedAmp(uint8_t state){

  //Гашение обоих светодиодов.
  LedStr.Amplifier_GREEN_LED_bit = 0;
  LedStr.Amplifier_RED_LED_bit   = 0;
  //--------------------
		   if(state == GreenColor) LedStr.Amplifier_GREEN_LED_bit = 1;
  else if(state == RedColor)   LedStr.Amplifier_RED_LED_bit = 1;
  else if(state == YellowColor)
    {
      LedStr.Amplifier_GREEN_LED_bit = 1;
      LedStr.Amplifier_RED_LED_bit   = 1;
    }
}
//-----------------------------------------------------------------------------
void Led_Amp(uint8_t ampState){
	
  if(ampState == PAMP_PROT) LedAmp(YellowColor & Blink(INTERVAL_250_mS));
  else                      LedAmp(GreenColor);
}
//-----------------------------------------------------------------------------
//
void Led_Algorithm(uint8_t led, uint8_t line, uint8_t spLine, BlinkIntervalEnum_t blinkPeriod){

  uint8_t spState = SpLine_GetOutState(spLine);
	uint8_t blink   = (uint8_t)Blink(blinkPeriod);
  //--------------------
  //Зона неисправна.
  if(SpLine_GetOutStateForLed(line) >= LineBreak)
    {
      //Активация зоны микрофоном или пож.шлейфом+неисправность - мигающий желтый+статичный красный..
      if(spState >= ActionMicOn) Led_Control(led, RedColor | (YellowColor & blink));
      //Зона активирована в ручную+неисправность - мигающий желтый+статичный зеленый.
      else if(spState == ActionManualOn) Led_Control(led, GreenColor | (YellowColor & blink));
      //Зоана не активирована - Мигающий оражевый.
      else Led_Control(led, (YellowColor & blink));
    }
  //Зона не активна - гасим светодиод.          
  else if(spState == ActionOff)      Led_Control(led, LedOff);
  //Ручная активация зоны - зеленый цвет светодиода.
  else if(spState == ActionManualOn) Led_Control(led, GreenColor);
  //Активация зоны микрофоном или пож.шлейфом - красный цвет светодиода.
  else if(spState >= ActionMicOn)    Led_Control(led, RedColor);    
	//--------------------
	//Светодиод "ПУСК".
	if(spState == ActionPuskOn) Led_Pusk(LedOn);			
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------











