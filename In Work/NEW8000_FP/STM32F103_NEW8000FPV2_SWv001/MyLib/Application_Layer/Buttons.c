
#include "Buttons.h"

//-----------------------------------------------------------------------------

extern ButtonSTR_TypeDef		  ButtonFacePanelStr;
extern ButtonStateSTR_TypeDef	ButtonStateStr;

//-----------------------------------------------------------------------------
//Получение мгновенного состояния кнопки.
uint8_t ButGetInstantState(uint8_t button){
		
  return SortingButtons(button);
}
//-----------------------------------------------------------------------------
uint8_t ButGetToggleState(uint8_t button){
	
	uint8_t	temp = 0;
	//--------------------
	//Анализ состояния кнопок.
	temp = SortingButtons(button);
	switch (temp){
			//--------------------
			case 0:
				ButtonStateStr.BLK[button-2] = 0;
			break;
			//--------------------
			default:		
				if(ButtonStateStr.BLK[button-2] != temp)
					{
						ButtonStateStr.BLK[button-2]  = temp;
						ButtonStateStr.BLK[button-1] ^= temp;
					}	
			break;
			//--------------------
		}
	//-------------------------------------------------------------
	return ButtonStateStr.BLK[button-1];
}
//-----------------------------------------------------------------------------
//Сброс состояний кнопок.
void ButClrToggleState(uint8_t button, uint8_t clear_bit){

  if(button == CONTROL_BUTTONS)
    {
      ButtonStateStr.STR.ControlButtonSost &= ~(clear_bit);
    }
  else if(button == PRESET_BUTTONS)
    {
      ButtonStateStr.STR.PresetButtonSost &= ~(clear_bit);
    }
}
//-----------------------------------------------------------------------------
void ButModParam1(uint16_t *param, uint8_t min, uint8_t max){
	
	static uint8_t TempState = 0;//Временное состояние кнопок. 
	//--------------------
//	if ((Timer0FlagReg & Timer1_LongPressSPButtomFlag) != 0)
//		{
//			Timer0FlagReg &= ~Timer1_LongPressSPButtomFlag;
//			ButtonTemporarySost = 0;
//		}
	//--------------------
	switch(SortingButtons(CONTROL_BUTTONS)){
			//--------------------
			case ResALERTbut:
				if((TempState & ResALERTbut) == 0)
					{
						TempState	|= ResALERTbut;
						//---------
						if ((*param) > min)(*param)--;//Проверка на минимум.
            //else(*param) = max;           //Закольцовывание редактирования параметра.
						//---------
					}
			break;
			//--------------------
			case ResFAULTbut:
				if((TempState & ResFAULTbut) == 0)
					{
						TempState	|= ResFAULTbut;
						//---------
						if ((*param) < max)(*param)++;//Проверка на максимум.
            //else(*param) = min;           //Закольцовывание редактирования параметра.
						//---------
					}
			break;
			//--------------------
			default:
				TempState = 0;
			//--------------------
		}	
}
//-----------------------------------------------------------------------------
void ButModParam2(uint16_t *param, uint8_t min, uint8_t max){
	
	static uint16_t TempState = 0;//Временное состояние кнопок. 
  //--------------------
//	if ((Timer0FlagReg & Timer1_LongPressSPButtomFlag) != 0)
//		{
//			Timer0FlagReg &= ~Timer1_LongPressSPButtomFlag;
//			ButtonTemporarySost = 0;
//		}
  //--------------------
	switch(SortingButtons(CONTROL_BUTTONS)){
			//--------------------
			case ZUMMERbut:
				if((TempState & ZUMMERbut) == 0)
					{
						TempState	|= ZUMMERbut;
						//---------
						if ((*param) > min)(*param)--;//Проверка на минимум.
            //else(*param) = max;           //Закольцовывание редактирования параметра.
						//---------
					}
			break;
			//--------------------
			case TESTbut:
				if((TempState & TESTbut) == 0)
					{
						TempState	|= TESTbut;
						//---------
						if ((*param) < max)(*param)++;//проверка на максимум.
            //else(*param) = min;           //Закольцовывание редактирования параметра.
						//---------
					}
			break;
			//--------------------
			default:
				TempState = 0;
			//--------------------
		}	
}
//-----------------------------------------------------------------------------








