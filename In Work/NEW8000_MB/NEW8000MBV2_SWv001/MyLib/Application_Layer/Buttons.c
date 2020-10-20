
#include "Buttons.h"

//-----------------------------------------------------------------------------
ButStateSTR_Type	ButStateStr;
ButStateSTR2_Type	ButStateSTR2;

static uint8_t    ButControl = 0;
static uint8_t    ButPreset  = 0;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Сохранение мгновенного состояния кнопок FP.
void ButSaveState(uint8_t* p){

  ButStateSTR2.STR.ControlBut = *p;
  ButStateSTR2.STR.PresetBut  = *(p+1);
}
//-----------------------------------------------------------------------------
//Получение состояний кнопок.
ButStateSTR2_Type* ButGet(void){
  
  return &ButStateSTR2;
}
//-----------------------------------------------------------------------------
uint8_t ButToggle(uint8_t but){
	
	uint8_t	temp = 0;
	//--------------------
  if(but == CONTROL_BUTTONS) temp = ButControl;
  if(but == PRESET_BUTTONS)  temp = ButPreset;
  //Анализ состояния кнопок.
  if(temp)
    {
      if(ButStateStr.BLK[but-2] != temp)
        {
          ButStateSTR2.BLK[but-2]  = temp;
					ButStateSTR2.BLK[but-1] ^= temp;
        }
    }
  else ButStateSTR2.BLK[but-2] = 0;

	return ButStateSTR2.BLK[but-1];
}
//-----------------------------------------------------------------------------








//-----------------------------------------------------------------------------
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
  if(but == PRESET_BUTTONS)  return ButPreset;
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
	//temp = SortingButtons(button);//Sm1628GetInstantStateButton(button);	
  if(but == CONTROL_BUTTONS) temp = ButControl;
  if(but == PRESET_BUTTONS)  temp = ButPreset;
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
  else if(but == PRESET_BUTTONS)
    {
      ButStateStr.STR.PresetButSt &= ~(clr_bit);
    }
}
//-----------------------------------------------------------------------------









