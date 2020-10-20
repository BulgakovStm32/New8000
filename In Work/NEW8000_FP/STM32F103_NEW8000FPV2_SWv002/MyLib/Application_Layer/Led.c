
#include "Led.h"

//-----------------------------------------------------------------------------

extern IndicatorSTR_TypeDef	  IndicatorsFacePanelStr;

//-----------------------------------------------------------------------------
void LedInit(void){

  //Spi2Init();
  LedSetAll(0xff);
  LedUpdate();
}
//-----------------------------------------------------------------------------
void LedUpdate(void){

  Sm1628SendDataBuf();
}
//-----------------------------------------------------------------------------
void LedSetAll(uint8_t state){

  IndicatorsFacePanelStr.BLK[0] = state;
  IndicatorsFacePanelStr.BLK[1] = state;
  IndicatorsFacePanelStr.BLK[2] = state;
  IndicatorsFacePanelStr.BLK[3] = state;
  IndicatorsFacePanelStr.BLK[4] = state;
  IndicatorsFacePanelStr.BLK[5] = state;
  IndicatorsFacePanelStr.BLK[6] = state;
  IndicatorsFacePanelStr.BLK[7] = state;
//  IndicatorsFacePanelStr.BLK[8] = 0xFF;
//  IndicatorsFacePanelStr.BLK[9] = 0xFF;
//  IndicatorsFacePanelStr.BLK[10] = 0xFF;
}
//-----------------------------------------------------------------------------
void LedControl(uint8_t ledBit, uint8_t state){

	uint8_t offset = 0;
  uint8_t bit;
  //--------------------
  if(ledBit >= 8)offset = ledBit/8;
  bit = (ledBit - (offset*8));
	
  //--------------------
  //Это проверка нужна для корректного управления одноцветными светодиодами.
  if((ledBit != TEST_LED)   && 
     (ledBit != ZUMMER_LED) &&
     (ledBit != RESET_LED)  &&
     (ledBit != START_LED)  &&
     (ledBit != FIRE_LED))
    {
      //Гашение обоих светодиодов.
      IndicatorsFacePanelStr.BLK[offset] &= ~(3<<bit);
    }
  //--------------------
  if(state == GreenColor)
    {
      IndicatorsFacePanelStr.BLK[offset] |= (1<<bit);
    }
  else if(state == RedColor)
    {
      IndicatorsFacePanelStr.BLK[offset] |= (2<<bit);
    }
  else if(state == YellowColor)
    {
      IndicatorsFacePanelStr.BLK[offset] |= (3<<bit);
    }
  else
    {
      //Гашение обоих светодиодов.
      IndicatorsFacePanelStr.BLK[offset] &= ~(3<<bit);
    }
}
//-----------------------------------------------------------------------------
void LedStart(uint8_t state){

  if(state == LedOn) IndicatorsFacePanelStr.bit.START_LED_bit = 1;
  else               IndicatorsFacePanelStr.bit.START_LED_bit = 0;
}
//-----------------------------------------------------------------------------
void LedPoj(uint8_t state){

  if(state == LedOn) IndicatorsFacePanelStr.bit.FIRE_LED_bit = 1;
  else               IndicatorsFacePanelStr.bit.FIRE_LED_bit = 0;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void LedCommunication(uint8_t state){

  //Гашение обоих светодиодов.
  IndicatorsFacePanelStr.bit.Communication_GREEN_LED_bit = 0;
  IndicatorsFacePanelStr.bit.Communication_RED_LED_bit   = 0;
  //--------------------
  if(state == GreenColor)
    {
      IndicatorsFacePanelStr.bit.Communication_GREEN_LED_bit = 1;
    }
  else if(state == RedColor)
    {
      IndicatorsFacePanelStr.bit.Communication_RED_LED_bit = 1;
    }
  else if(state == YellowColor)
    {
      IndicatorsFacePanelStr.bit.Communication_GREEN_LED_bit = 1;
      IndicatorsFacePanelStr.bit.Communication_RED_LED_bit   = 1;
    }
}
//-----------------------------------------------------------------------------
void LedPower(uint8_t state){

  //Гашение обоих светодиодов.
  IndicatorsFacePanelStr.bit.Power_GREEN_LED_bit = 0;
  IndicatorsFacePanelStr.bit.Power_RED_LED_bit   = 0;
  //--------------------
  if(state == GreenColor)
    {
      IndicatorsFacePanelStr.bit.Power_GREEN_LED_bit = 1;
    }
  else if(state == RedColor)
    {
      IndicatorsFacePanelStr.bit.Power_RED_LED_bit = 1;
    }
  else if(state == YellowColor)
    {
      IndicatorsFacePanelStr.bit.Power_GREEN_LED_bit = 1;
      IndicatorsFacePanelStr.bit.Power_RED_LED_bit   = 1;
    }
}
//-----------------------------------------------------------------------------
void LedBattery(uint8_t state){

  //Гашение обоих светодиодов.
  IndicatorsFacePanelStr.bit.Battery_GREEN_LED_bit = 0;
  IndicatorsFacePanelStr.bit.Battery_RED_LED_bit   = 0;
  //--------------------
  if(state == GreenColor)
    {
      IndicatorsFacePanelStr.bit.Battery_GREEN_LED_bit = 1;
    }
  else if(state == RedColor)
    {
      IndicatorsFacePanelStr.bit.Battery_RED_LED_bit = 1;
    }
  else if(state == YellowColor)
    {
      IndicatorsFacePanelStr.bit.Battery_GREEN_LED_bit = 1;
      IndicatorsFacePanelStr.bit.Battery_RED_LED_bit   = 1;
    }
}
//-----------------------------------------------------------------------------
void LedAmplifier(uint8_t state){

  //Гашение обоих светодиодов.
  IndicatorsFacePanelStr.bit.Amplifier_GREEN_LED_bit = 0;
  IndicatorsFacePanelStr.bit.Amplifier_RED_LED_bit   = 0;
  //--------------------
  if(state == GreenColor)
    {
      IndicatorsFacePanelStr.bit.Amplifier_GREEN_LED_bit = 1;
    }
  else if(state == RedColor)
    {
      IndicatorsFacePanelStr.bit.Amplifier_RED_LED_bit = 1;
    }
  else if(state == YellowColor)
    {
      IndicatorsFacePanelStr.bit.Amplifier_GREEN_LED_bit = 1;
      IndicatorsFacePanelStr.bit.Amplifier_RED_LED_bit   = 1;
    }
}
//-----------------------------------------------------------------------------
//
void LedPresetControl(uint8_t led, uint8_t line, uint8_t zone, uint16_t blink){

  uint8_t RusTextPointer = 0;
  uint8_t StrPointer     = 0;
  //--------------------
  if(zone & Zone1)
    {
      RusTextPointer = RusText_Zone1_Pointer;
      StrPointer     = String5;
    }
  if(zone & Zone2)
    {
      RusTextPointer = RusText_Zone2_Pointer;
      StrPointer     = String6;
    }
  if(zone & Zone3)
    {
      RusTextPointer = RusText_Zone3_Pointer;
      StrPointer     = String7;
    }
  if(zone & Zone4)
    {
      RusTextPointer = RusText_Zone4_Pointer;
      StrPointer     = String8;
    }
  //--------------------
  //Неисправность АКБ.
  if(AmpFpGet() == AmpProt)
    {
      MenuTextDisplay(RusText_StatusPA_Pointer, MenuGetRusText(RusText_Active_Pointer), StrPointer);
    }
  //--------------------
  //Зона неисправна.
  if(LcFpGetForLed(line) >= LineBreak)
    {
      //Активация зоны микрофоном или пож.шлейфом+неисправность - мигающий желтый+статичный красный..
      if(ZoneFpGet(zone) >= ZoneMicOn)//((ZoneFpGet(zone) == ZoneMicOn) || (ZoneFpGet(zone) == ZoneAutoOn))
        {
          LedControl(led, RedColor | (YellowColor & blink));
        } 
      //Зона активирована в ручную+неисправность - мигающий желтый+статичный зеленый.
      else if(ZoneFpGet(zone) == ZoneManualOn)
            {
              LedControl(led, GreenColor | (YellowColor & blink));
            }
      //Зоана не активирована - Мигающий оражевый.
      else LedControl(led, (YellowColor & blink));
    }
  //Зона не активна - гасим светодиод.          
  else if(ZoneFpGet(zone) == ZoneOff) LedControl(led, LedOff);
  //Ручная активация зоны - зеленый цвет светодиода.
  else if(ZoneFpGet(zone) == ZoneManualOn)
        {
          LedControl(led, GreenColor);
          MenuTextDisplay(RusTextPointer, MenuGetRusText(RusText_Active_Pointer), StrPointer);
          //Вывод надписи кто активировал зону.
          LcdString(11, StrPointer+1);
          LcdChr('-');
          LcdOutStr(MenuGetRusText(RusText_ManualControl_Pointer));
          LcdBinToDec(StrPointer-3, 1);
        }
  //Активация кнопкой "ПУСК".
//  else if(ZoneFpGet(zone) == ZoneStartOn)
//        { 
//          LedStart(LedOn);
//        }
  //Активация зоны микрофоном или пож.шлейфом - красный цвет светодиода.
  else if(ZoneFpGet(zone) >= ZoneMicOn)//((ZoneFpGet(zone) == ZoneMicOn) || (ZoneFpGet(zone) > ZoneMicOn))
        {
          MenuTextDisplay(RusTextPointer, MenuGetRusText(RusText_Active_Pointer), StrPointer);
          //Вывод надписи кто активировал зону.
          LcdString(11, StrPointer+1);
          LcdChr('-');
          if(ZoneFpGet(zone) == ZoneMicOn)  LcdOutStr(MenuGetRusText(RusText_Mic_Pointer));        //Зону активировал микрофон
          if(ZoneFpGet(zone) == ZoneStartOn)LcdOutStr(MenuGetRusText(RusText_Preset1_Pointer));    //Зона активирована кнопкой "ПУСК".
          if(ZoneFpGet(zone) == ZonePoj1On) LcdOutStr(MenuGetRusText(RusText_StatusPoj1_Pointer)); //Зону активировал пожарный шлейф.
          if(ZoneFpGet(zone) == ZonePoj2On) LcdOutStr(MenuGetRusText(RusText_StatusPoj2_Pointer)); //Зону активировал пожарный шлейф.
          if(ZoneFpGet(zone) == ZonePoj3On) LcdOutStr(MenuGetRusText(RusText_StatusPoj3_Pointer)); //Зону активировал пожарный шлейф.
          if(ZoneFpGet(zone) == ZoneChSOn)  LcdOutStr(MenuGetRusText(RusText_StatusGOChS_Pointer));//Зону активировал пожарный шлейф.
     
          LedStart(LedOn);
          LedPoj(LedOn);
          LedControl(led, RedColor);    
        }
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------











