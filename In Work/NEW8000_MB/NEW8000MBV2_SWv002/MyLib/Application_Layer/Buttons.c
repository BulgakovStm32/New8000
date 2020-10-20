
#include "Buttons.h"

//-----------------------------------------------------------------------------
ButStateSTR_Type	ButStateStr;
static uint8_t    ButControl = 0;
static uint8_t    ButPreset  = 0;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void ButFPControl(uint8_t but){
    
  ButControl = but;
}
//-----------------------------------------------------------------------------
void ButFPPreset(uint8_t but){
    
  ButPreset = but;   
}
//-----------------------------------------------------------------------------
//Получение мгновенного состояния кнопки.
uint8_t ButGetInstantState(uint8_t but){
  
  if(but == CONTROL_BUTTONS) return ButControl;
  if(but == ALG_BUTTONS)     return ButPreset;
  return 0;
}
//-----------------------------------------------------------------------------
//Получение состояния кнопки.
uint8_t ButGetState(uint8_t but){

  if(but == CONTROL_BUTTONS) return ButStateStr.STR.ControlButOldSt;
  else                       return ButStateStr.STR.PresetButOldSt;
  //return 0;
}
//-----------------------------------------------------------------------------
uint8_t ButGetToggleState(uint8_t but){
	
	uint8_t	temp = 0;
	//------------------------------------------------------------
  if(but == CONTROL_BUTTONS) temp = ButControl;
  if(but == ALG_BUTTONS)     temp = ButPreset;
  //Анализ состояния кнопок.
	switch (temp)
		{
			//-------------------------------
			case 0:
				ButStateStr.BLK[but-2] = 0;
				//--------------------------
				//Блокировка выбора зон.
//				if (button == SPEAKER_BUTTON)
//					{
//						//ButtonStateStr.BLK[button-1] &= get_blocked_speaker_line();
//					}
				//--------------------------
			break;
			//-------------------------------
			default:		
				if(ButStateStr.BLK[but-2] != temp)
					{
						ButStateStr.BLK[but-2]  = temp;
						ButStateStr.BLK[but-1] ^= temp;
						//--------------------------
						//Блокировка выбора зон.
//						if (button == SPEAKER_BUTTON)
//							{
//								//ButtonStateStr.BLK[button-1] &= get_blocked_speaker_line();
//							}
						//--------------------------
					}	
			break;
			//-------------------------------
		}
	//-------------------------------------------------------------
	return ButStateStr.BLK[but-1];
}
//-----------------------------------------------------------------------------
//Сброс состояний кнопок.
void ButClrToggleState(uint8_t but, uint8_t clr_bit){

  if(but == CONTROL_BUTTONS)
    {
      ButStateStr.STR.ControlButSt &= ~(clr_bit);
    }
  else if(but == ALG_BUTTONS)
    {
      ButStateStr.STR.PresetButSt &= ~(clr_bit);
    }
}
//-----------------------------------------------------------------------------
//Состояние кнопок для работы с Мик.
uint8_t ButGetToggleForMic(void){

	       uint8_t tempBut;
	static uint8_t micButState = 0;
	static uint8_t oldButState = 0;	
	//--------------------
	tempBut = ButGetInstantState(ALG_BUTTONS);
	if(tempBut == 0) oldButState = 0;
	if(tempBut != oldButState)
		{
			oldButState  = tempBut;
			micButState ^= tempBut;
		}	
	return micButState;
}
//-----------------------------------------------------------------------------









