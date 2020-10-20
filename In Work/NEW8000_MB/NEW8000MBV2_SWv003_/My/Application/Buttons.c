
#include "Buttons.h"

//************************************************************************************************
static FPButtons_t     FPButtonsStr;
static ButtonStates_t  ButtonStatesStr;
//************************************************************************************************
//************************************************************************************************
FPButtons_t* ButtonFP(void){
	
	return &FPButtonsStr;
}
//*****************************************************************************
void ButtonFP_Clr(void){
	
	FPButtonsStr.Control   = 0;
	FPButtonsStr.Algorithm = 0;
	FPButtonsStr.All       = 0;
}
//************************************************************************************************
//************************************************************************************************
void Button_Update(void){
	
				 uint32_t butInstantState;
	static uint32_t butOldState = 0;
	//--------------------	
	butInstantState = ButtonFP()->All;
	//------
	ButtonStatesStr.Instant   = butInstantState;
	ButtonStatesStr.isPress   = (butOldState ^ butInstantState) & butInstantState;
	ButtonStatesStr.isRelease = (~butInstantState) & butOldState;
	ButtonStatesStr.isToggle ^= ButtonStatesStr.isPress;
	//------	
	ButtonStatesStr.ready_f = 1; 
	butOldState = butInstantState;
}
//*****************************************************************************
ButtonStates_t* Button(void){

	return &ButtonStatesStr;
}
//*****************************************************************************
uint32_t Button_Instant(ButtonsEnum_t but){

	return ButtonStatesStr.Instant & but;
}
//*****************************************************************************
uint32_t Button_IsPress(ButtonsEnum_t but){

	return ButtonStatesStr.isPress & but;
}
//*****************************************************************************
uint32_t Button_IsRelease(ButtonsEnum_t but){

	return ButtonStatesStr.isRelease & but;
}
//*****************************************************************************
uint32_t Button_IsToggle(ButtonsEnum_t but){

	return ButtonStatesStr.isToggle & but;
}
//*****************************************************************************
void Button_ClrToggle(ButtonsEnum_t but){

	ButtonStatesStr.isToggle &= ~(but);
}
//*****************************************************************************
//Перемещение по меню ВВЕРХ и ВНИЗ
void Button_UpDown(uint16_t *param, uint16_t min, uint16_t max){
	
	static uint16_t longPressCount = 0;
	static uint16_t butOldState    = 0;
	//--------------------
	if(*param < min) *param = min;
	if(*param > max) *param = max;				
	//--------------------	
	//ВНИЗ
	if(Button_Instant(STARTbut))
		{
			if((*param) == min) return;	
			if(butOldState != STARTbut)
				{
					butOldState = STARTbut;
					(*param)--;
				}
			//Ускоренное редактирование параметра.
			else if(++longPressCount >= LONG_PRESS_CYCLE)
					{
						(*param)--;
						longPressCount = (LONG_PRESS_CYCLE - 5);
					}
		}
	//--------------------
	//ВВЕРХ
	else if(Button_Instant(STOPbut))
		{
			if((*param) == max) return;
			if(butOldState != STOPbut)
				{
					butOldState = STOPbut;
					(*param)++;
				}			
			//Ускоренное редактирование параметра.
			else if(++longPressCount >= LONG_PRESS_CYCLE)
					{
						(*param)++;
						longPressCount = (LONG_PRESS_CYCLE - 5);
					}
		}
	//--------------------
	else
		{
			butOldState    = 0;
			longPressCount = 0;
		}	
}
//*****************************************************************************
//Перемещение ВЛЕВО и ВПРАВО.
void Button_LeftRigth(uint16_t *param, uint16_t min, uint16_t max){
	
//	static uint16_t longPressCount = 0;
	static uint16_t butOldState    = 0;
  //--------------------
	//ВЛЕВО
	if(Button_Instant(ZUMMERbut))
		{
			if((*param) == min) return;	
			if(butOldState != ZUMMERbut)
				{
					butOldState = ZUMMERbut;
					(*param)--;
				}
			//Ускоренное редактирование параметра.
//			else if(++longPressCount >= LONG_PRESS_CYCLE)
//					{
//						(*param)--;
//						longPressCount = (LONG_PRESS_CYCLE - 5);
//					}
		}
	//--------------------
	//ВПРАВО
	else if(Button_Instant(TESTbut))
		{
			if((*param) == max) return;
			if(butOldState != TESTbut)
				{
					butOldState = TESTbut;
					(*param)++;
				}			
			//Ускоренное редактирование параметра.
//			else if(++longPressCount >= LONG_PRESS_CYCLE)
//					{
//						(*param)++;
//						longPressCount = (LONG_PRESS_CYCLE - 5);
//					}
		}
	//--------------------
	else
		{
			butOldState    = 0;
//			longPressCount = 0;
		}			
}
//************************************************************************************************
//************************************************************************************************

ButtonStates_t MicButtonStateStr; 

//*****************************************************************************
void ButtonForMic_Update(void){

				 uint32_t butInstantState;
	static uint32_t butOldState = 0;
	//--------------------	
	butInstantState = ButtonFP()->All;
	//------
	MicButtonStateStr.Instant   =  butInstantState;
	MicButtonStateStr.isPress   = (butOldState ^ butInstantState) & butInstantState;
	MicButtonStateStr.isRelease = (~butInstantState) & butOldState;
	MicButtonStateStr.isToggle ^= MicButtonStateStr.isPress;
	//------	
	butOldState = butInstantState;
}
//*****************************************************************************
ButtonStates_t* ButtonForMic(void){

	return &MicButtonStateStr;
}
//************************************************************************************************
//************************************************************************************************

ButtonStates_t ManualButtonStateStr; 

//*****************************************************************************
void ButtonForManual_Update(void){

				 uint32_t butInstantState;
	static uint32_t butOldState = 0;
	//--------------------	
	butInstantState = ButtonFP()->All;
	//------
	ManualButtonStateStr.Instant   =  butInstantState;
	ManualButtonStateStr.isPress   = (butOldState ^ butInstantState) & butInstantState;
	ManualButtonStateStr.isRelease = (~butInstantState) & butOldState;
	ManualButtonStateStr.isToggle ^= ManualButtonStateStr.isPress;
	//------	
	butOldState = butInstantState;
}
//*****************************************************************************
ButtonStates_t* ButtonForManual(void){

	return &ManualButtonStateStr;
}
//************************************************************************************************
//************************************************************************************************








































