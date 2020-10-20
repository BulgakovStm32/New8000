
#include "Led.h"

//-----------------------------------------------------------------------------
static Sm1628LedSTR_Type	  LedFacePanelStr;
//-----------------------------------------------------------------------------
//void LedInit(void){

//  //Spi2Init();
//  LedSetAll(0xff);
//  LedUpdate();
//}
//-----------------------------------------------------------------------------
void LedUpdate(void){

  Sm1628SendLed(&LedFacePanelStr);
}
//-----------------------------------------------------------------------------
void LedSetAll(uint8_t state){
	
	uint8_t i;
	//--------------------
	for(i = 0; i < 9; i++)LedFacePanelStr.BLK[i] = state;
	
//  IndicatorsFacePanelStr.BLK[0] = state;
//  IndicatorsFacePanelStr.BLK[1] = state;
//  IndicatorsFacePanelStr.BLK[2] = state;
//  IndicatorsFacePanelStr.BLK[3] = state;
//  IndicatorsFacePanelStr.BLK[4] = state;
//  IndicatorsFacePanelStr.BLK[5] = state;
//  IndicatorsFacePanelStr.BLK[6] = state;
//  IndicatorsFacePanelStr.BLK[7] = state;
	
//  IndicatorsFacePanelStr.BLK[8] = 0xFF;
//  IndicatorsFacePanelStr.BLK[9] = 0xFF;
//  IndicatorsFacePanelStr.BLK[10] = 0xFF;
}
//-----------------------------------------------------------------------------
Sm1628LedSTR_Type* LedBits(void){

	return &LedFacePanelStr;
}
//-----------------------------------------------------------------------------
void LedControl(uint8_t ledBit, uint8_t state){

	uint8_t offset;
  uint8_t bit;
  //--------------------
  if(ledBit >= 8) offset = ledBit/8;
  bit = (ledBit - (offset*8));
  //--------------------
  //Это проверка нужна для корректного управления одноцветными светодиодами.
  if((ledBit != TEST_LED)   && 
     (ledBit != ZUMMER_LED) &&
     (ledBit != ALERT_LED)  &&
     (ledBit != START_LED)  &&
     (ledBit != FIRE_LED))
    {
      //Гашение обоих светодиодов.
      LedFacePanelStr.BLK[offset] &= ~(3<<bit);
    }
  //--------------------
			 if(state == GreenColor)  LedFacePanelStr.BLK[offset] |=  (1<<bit);
  else if(state == RedColor)    LedFacePanelStr.BLK[offset] |=  (2<<bit);
  else if(state == YellowColor) LedFacePanelStr.BLK[offset] |=  (3<<bit);
  else                          LedFacePanelStr.BLK[offset] &= ~(3<<bit);//Гашение обоих светодиодов.
}
//-----------------------------------------------------------------------------
void LedPusk(uint8_t state){

  if(state == LedOn) LedFacePanelStr.bit.START_LED_bit = 1;
  else               LedFacePanelStr.bit.START_LED_bit = 0;
}
//-----------------------------------------------------------------------------
void LedPoj(uint8_t state){

  if(state == LedOn) LedFacePanelStr.bit.FIRE_LED_bit = 1;
  else               LedFacePanelStr.bit.FIRE_LED_bit = 0;
}
//-----------------------------------------------------------------------------
void LedAlert(uint8_t state){
	
	if(state == LedOn) LedFacePanelStr.bit.ALERT_LED_bit = 1;
	else							 LedFacePanelStr.bit.ALERT_LED_bit = 0;
}
//-----------------------------------------------------------------------------
void LedZummer(uint8_t state){
	
	if(state == LedOn) LedFacePanelStr.bit.ZUMMER_LED_bit = 1;
	else							 LedFacePanelStr.bit.ZUMMER_LED_bit = 0;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void LedCommunication(uint8_t state){

  //Гашение обоих светодиодов.
  LedFacePanelStr.bit.Communication_GREEN_LED_bit = 0;
  LedFacePanelStr.bit.Communication_RED_LED_bit   = 0;
  //--------------------
			 if(state == GreenColor)LedFacePanelStr.bit.Communication_GREEN_LED_bit = 1;
  else if(state == RedColor)  LedFacePanelStr.bit.Communication_RED_LED_bit = 1;
  else if(state == YellowColor)
    {
      LedFacePanelStr.bit.Communication_GREEN_LED_bit = 1;
      LedFacePanelStr.bit.Communication_RED_LED_bit   = 1;
    }
}
//-----------------------------------------------------------------------------
static void LedPower(uint8_t state){

  //Гашение обоих светодиодов.
  LedFacePanelStr.bit.Power_GREEN_LED_bit = 0;
	LedFacePanelStr.bit.Power_RED_LED_bit   = 0;
  //--------------------
			 if(state == GreenColor) LedFacePanelStr.bit.Power_GREEN_LED_bit = 1;
  else if(state == RedColor)   LedFacePanelStr.bit.Power_RED_LED_bit = 1;
  else if(state == YellowColor)
    {
			LedFacePanelStr.bit.Power_GREEN_LED_bit = 1;
      LedFacePanelStr.bit.Power_RED_LED_bit   = 1;
    }
}
//-----------------------------------------------------------------------------
void LedPowerIndication(uint8_t powerState){
	
       if(powerState <= PowerDCOk)    LedPower(GreenColor);//Питание в норме.
  else if(powerState == PowerACNo)    LedPower(RedColor);  //Отсутствует основное питание.
  else if(powerState == PowerDCFault) LedPower(YellowColor & Blink(BlinkInterval250mS));//Неисправен инвертор.
}
//-----------------------------------------------------------------------------
static void LedBattery(uint8_t state){

  //Гашение обоих светодиодов.
  LedFacePanelStr.bit.Battery_GREEN_LED_bit = 0;
  LedFacePanelStr.bit.Battery_RED_LED_bit   = 0;
  //--------------------
  if(state == GreenColor)
    {
      LedFacePanelStr.bit.Battery_GREEN_LED_bit = 1;
    }
  else if(state == RedColor)
    {
      LedFacePanelStr.bit.Battery_RED_LED_bit = 1;
    }
  else if(state == YellowColor)
    {
      LedFacePanelStr.bit.Battery_GREEN_LED_bit = 1;
      LedFacePanelStr.bit.Battery_RED_LED_bit   = 1;
    }
}
//-----------------------------------------------------------------------------
void LedBatteryIndication(uint8_t batState){
	
  if(batState == BatChargeEnd) LedBattery(GreenColor);
  if(batState == BatBlink)     LedBattery(GreenColor  & (uint8_t)Blink(BlinkInterval250mS));
  if(batState == BatAttention) LedBattery(YellowColor & (uint8_t)Blink(BlinkInterval250mS));
  if(batState >= BatDeep)      LedBattery(RedColor);
}
//-----------------------------------------------------------------------------
static void LedAmp(uint8_t state){

  //Гашение обоих светодиодов.
  LedFacePanelStr.bit.Amplifier_GREEN_LED_bit = 0;
  LedFacePanelStr.bit.Amplifier_RED_LED_bit   = 0;
  //--------------------
  if(state == GreenColor)
    {
      LedFacePanelStr.bit.Amplifier_GREEN_LED_bit = 1;
    }
  else if(state == RedColor)
    {
      LedFacePanelStr.bit.Amplifier_RED_LED_bit = 1;
    }
  else if(state == YellowColor)
    {
      LedFacePanelStr.bit.Amplifier_GREEN_LED_bit = 1;
      LedFacePanelStr.bit.Amplifier_RED_LED_bit   = 1;
    }
}
//-----------------------------------------------------------------------------
void LedAmpIndication(uint8_t ampState){
	
  if(ampState == AmpProt) LedAmp(YellowColor & Blink(BlinkInterval250mS));
  else                    LedAmp(GreenColor);
}
//-----------------------------------------------------------------------------
//
void LedPresetControl(uint8_t led, uint8_t line, uint8_t zone, uint16_t blink){

  uint8_t RusTextPointer;
  uint8_t StrPointer    ;
  //--------------------
  if(zone == Zone1)
    {
      RusTextPointer = RusText_Zone1_Pointer;
      StrPointer     = String5;
    }
  if(zone == Zone2)
    {
      RusTextPointer = RusText_Zone2_Pointer;
      StrPointer     = String6;
    }
  if(zone == Zone3)
    {
      RusTextPointer = RusText_Zone3_Pointer;
      StrPointer     = String7;
    }
  if(zone == Zone4)
    {
      RusTextPointer = RusText_Zone4_Pointer;
      StrPointer     = String8;
    }
  //--------------------
  //Неисправность АКБ.
  if(PowerDevice()->Amp == AmpProt)
    {
      MenuTextDisplay(RusText_StatusPA_Pointer, MenuGetRusText(RusText_Active_Pointer), StrPointer);
    }
  //--------------------
  //Зона неисправна.
  if(LcGetForLed(line) >= LineBreak)
    {
      //Активация зоны микрофоном или пож.шлейфом+неисправность - мигающий желтый+статичный красный..
      if(ZoneGet(zone) >= ActionMicOn)//((ZoneFpGet(zone) == ZoneMicOn) || (ZoneFpGet(zone) == ZoneAutoOn))
        {
          LedControl(led, RedColor | (YellowColor & blink));
        } 
      //Зона активирована в ручную+неисправность - мигающий желтый+статичный зеленый.
      else if(ZoneGet(zone) == ActionManualOn)
            {
              LedControl(led, GreenColor | (YellowColor & blink));
            }
      //Зоана не активирована - Мигающий оражевый.
      else LedControl(led, (YellowColor & blink));
    }
	//--------------------
  //Зона не активна - гасим светодиод.          
  else if(ZoneGet(zone) == ActionOff) LedControl(led, LedOff);
	//--------------------
  //Ручная активация зоны - зеленый цвет светодиода.
  else if(ZoneGet(zone) == ActionManualOn)
        {
          LedControl(led, GreenColor);
					//Надпись "Акт."
          MenuTextDisplay(RusTextPointer, MenuGetRusText(RusText_Active_Pointer), StrPointer);
          //Вывод надписи кто активировал зону.
          LcdString(Char13, StrPointer+1);
          LcdChr('-');
          LcdOutStr(MenuGetRusText(RusText_ManualControl_Pointer));
          LcdBinToDec(StrPointer-3, 1);
        }
  //Активация кнопкой "ПУСК".
//  else if(ZoneFpGet(zone) == ZoneStartOn)
//        { 
//          LedStart(LedOn);
//        }			
	//--------------------
	//Мигающая индикация светодиода ПОЖАР при состоянии входа ВНИМАНИЕ
	//else if(PojLineGetLogicState(Poj1) == FireLineATTENTION) LedPoj(LedOn & blink);
	//--------------------
  //Активация зоны микрофоном или пож.шлейфом - красный цвет светодиода.
  else if(ZoneGet(zone) >= ActionMicOn)//((ZoneFpGet(zone) == ZoneMicOn) || (ZoneFpGet(zone) > ZoneMicOn))
        {
          MenuTextDisplay(RusTextPointer, MenuGetRusText(RusText_Active_Pointer), StrPointer);
          //Вывод надписи кто активировал зону.
          LcdString(Char13, StrPointer+1);
          LcdChr('-');
          if(ZoneGet(zone) == ActionMicOn) 		   	 LcdOutStr(MenuGetRusText(RusText_Mic_Pointer));        //Зону активировал микрофон
          if(ZoneGet(zone) == ActionPuskOn)			   LcdOutStr(MenuGetRusText(RusText_Preset1_Pointer));    //Зона активирована кнопкой "ПУСК".
          if(ZoneGet(zone) == ActionFireLine1On) 	 LcdOutStr(MenuGetRusText(RusText_StatusPoj1_Pointer)); //Зону активировал пожарный шлейф.
          if(ZoneGet(zone) == ActionFireLine2On) 	 LcdOutStr(MenuGetRusText(RusText_StatusPoj2_Pointer)); //Зону активировал пожарный шлейф.
          if(ZoneGet(zone) == ActionFireLine3On) 	 LcdOutStr(MenuGetRusText(RusText_StatusPoj3_Pointer)); //Зону активировал пожарный шлейф.
          if(ZoneGet(zone) == ActionFireLineChSOn) LcdOutStr(MenuGetRusText(RusText_StatusGOChS_Pointer));//Зону активировал пожарный шлейф.
      
					//Светодиод "ПОЖАР".
//					if((ZoneGet(zone) >= ZonePoj1On) && (ZoneGet(zone) <= ZonePoj3On))
//						{
//							LedPoj(LedOn);
//							LedPusk(LedOn);
//						}
					//Светодиод "ПУСК".
					if(ZoneGet(zone) == ActionPuskOn) LedPusk(LedOn);
					//Светодиоды "Алгоритмы оповещения".
          LedControl(led, RedColor);    
        }
	//--------------------
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------











