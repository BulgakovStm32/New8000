
#include "Led.h"

//-----------------------------------------------------------------------------
static SM1628Leds_t	  LedStr;
//-----------------------------------------------------------------------------
void LedUpdate(void){

  Sm1628SendLed((uint8_t*)&LedStr);
}
//-----------------------------------------------------------------------------
void LedSetAll(uint8_t state){
	
	uint8_t* p = (uint8_t*)&LedStr;
	//-------------------
	for(uint8_t i = 0; i < LED_BUF_SIZE; i++) 
		{
			*(p+i) = state;
		}	
}
//-----------------------------------------------------------------------------
void LedControl(uint8_t ledBit, uint8_t state){

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
void LedPusk(uint8_t state){

  if(state == LedOn) LedStr.START_LED_bit = 1;
  else               LedStr.START_LED_bit = 0;
}
//-----------------------------------------------------------------------------
void LedPoj(uint8_t state){

  if(state == LedOn) LedStr.FIRE_LED_bit = 1;
  else               LedStr.FIRE_LED_bit = 0;
}
//-----------------------------------------------------------------------------
void LedAlert(uint8_t state){
	
	if(state == LedOn) LedStr.ALERT_LED_bit = 1;
	else							 LedStr.ALERT_LED_bit = 0;
}
//-----------------------------------------------------------------------------
void LedZummer(uint8_t state){
	
	if(state == LedOn) LedStr.ZUMMER_LED_bit = 1;
	else							 LedStr.ZUMMER_LED_bit = 0;
}
//-----------------------------------------------------------------------------
void LedCommunication(uint8_t state){

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
void LedPowerIndication(uint8_t powerState){
	
       if(powerState <= PowerDCOk)    LedPower(GreenColor);//Питание в норме.
  else if(powerState == PowerACNo)    LedPower(RedColor);  //Отсутствует основное питание.
  else if(powerState == PowerDCFault) LedPower(YellowColor & Blink(INTERVAL_250_mS));//Неисправен инвертор.
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
void LedBatteryIndication(uint8_t batState){
	
  if(batState == BatChargeEnd) LedBattery(GreenColor);
  if(batState == BatBlink)     LedBattery(GreenColor  & (uint8_t)Blink(INTERVAL_250_mS));
  if(batState == BatAttention) LedBattery(YellowColor & (uint8_t)Blink(INTERVAL_250_mS));
  if(batState >= BatDeep)      LedBattery(RedColor);
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
void LedAmpIndication(uint8_t ampState){
	
  if(ampState == AmpProt) LedAmp(YellowColor & Blink(INTERVAL_250_mS));
  else                    LedAmp(GreenColor);
}
//-----------------------------------------------------------------------------
//
void LedPresetControl(uint8_t led, uint8_t line, uint8_t spLine, BlinkIntervalEnum_t blinkPeriod){

  uint8_t spState = SpLine_GetOutState(spLine);
	uint8_t blink   = (uint8_t)Blink(blinkPeriod);
  //--------------------
  //Зона неисправна.
  if(SpLine_GetOutStateForLed(line) >= LineBreak)
    {
      //Активация зоны микрофоном или пож.шлейфом+неисправность - мигающий желтый+статичный красный..
      if(spState >= ActionMicOn) LedControl(led, RedColor | (YellowColor & blink));
      //Зона активирована в ручную+неисправность - мигающий желтый+статичный зеленый.
      else if(spState == ActionManualOn) LedControl(led, GreenColor | (YellowColor & blink));
      //Зоана не активирована - Мигающий оражевый.
      else LedControl(led, (YellowColor & blink));
    }
  //Зона не активна - гасим светодиод.          
  else if(spState == ActionOff) LedControl(led, LedOff);
  //Ручная активация зоны - зеленый цвет светодиода.
  else if(spState == ActionManualOn) LedControl(led, GreenColor);
  //Активация зоны микрофоном или пож.шлейфом - красный цвет светодиода.
  else if(spState >= ActionMicOn)
        {
					//Светодиоды "Алгоритмы оповещения".
          LedControl(led, RedColor);    
        }
	//--------------------
	//Светодиод "ПУСК".
	if(spState == ActionPuskOn) LedPusk(LedOn);			
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------











