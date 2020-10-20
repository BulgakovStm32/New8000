
#include "Buttons.h"

//************************************************************************************************
SM1628Buttons_t	 ButtonFacePanelStr;

#pragma pack(push, 1)//размер выравнивания в 1 байт
struct {
	uint8_t  Algorithm;
	uint8_t  Control;
	uint16_t All;
}ButtonsSTR;
#pragma pack(pop)//вернули предыдущую настройку.

static ButtonStates_t ButtonStatesStr;
//************************************************************************************************
//************************************************************************************************
//сортировка битов кнопок
static void ButtonSorting(void){
		
	uint8_t i;
  //--------------------
	ButtonsSTR.Algorithm = 0;
  ButtonsSTR.Control   = 0;	
	for (i = 0; i < 4; i++)
		{
      //Сортировка битов кнопок АЛГОРИТМЫ ОПОВЕЩЕНИЯ.
			ButtonsSTR.Algorithm |= ((((SM1628Buttons()->BLK[i] & 0x02)<<6) | ((SM1628Buttons()->BLK[i] & 0x10)<<2)) >> (2*i));
      //Сортировка битов кнопок управлени.
      ButtonsSTR.Control   |= ((((SM1628Buttons()->BLK[i] & 0x01)<<7) | ((SM1628Buttons()->BLK[i] & 0x08)<<3)) >> (2*i));
    }
	ButtonsSTR.All = (ButtonsSTR.Control << 8) | ButtonsSTR.Algorithm;
}
//************************************************************************************************
//************************************************************************************************
ButtonStates_t* Button(void){

	return &ButtonStatesStr;
}
//*****************************************************************************
uint8_t Button_CheckLoop(void){
	
				 uint32_t butInstantState = 0;
	static uint32_t butOldState     = 0;
	//--------------------	
	//После чтения состояния кнопок производится сортировка битов кнопок
	if(Sm1628ReadButton()) 
		{
			ButtonSorting();
			butInstantState = ButtonsSTR.All;
			//------
			ButtonStatesStr.Instant   = butInstantState;
			ButtonStatesStr.isPress   = (butOldState ^ butInstantState) & butInstantState;
			ButtonStatesStr.isRelease = (~butInstantState) & butOldState;
			ButtonStatesStr.isToggle ^= ButtonStatesStr.isPress;
			
			ButtonStatesStr.ready_f   = 1; 
			
			butOldState = butInstantState;
			return 1;
		}
	return 0;
}
//*****************************************************************************
uint16_t Button_GetAlg(void){

	return ButtonsSTR.Algorithm;
}
//*****************************************************************************
uint16_t Button_GetControl(void){

	return ButtonsSTR.Control;
}
//*****************************************************************************
uint32_t Button_Instant(ButtonsEnum_t but){

	return ButtonStatesStr.Instant & (1 << (uint32_t)but);
}
//*****************************************************************************
uint32_t Button_IsPress(ButtonsEnum_t but){

	return ButtonStatesStr.isPress & (1 << (uint32_t)but);
}
//*****************************************************************************
uint32_t Button_IsRelease(ButtonsEnum_t but){

	return ButtonStatesStr.isRelease & (1 << (uint32_t)but);
}
//*****************************************************************************
uint32_t Button_IsToggle(ButtonsEnum_t but){

	return ButtonStatesStr.isToggle & (1 << (uint32_t)but);
}
//*****************************************************************************
void Button_ClrToggle(ButtonsEnum_t but){

	ButtonStatesStr.isToggle &= ~(1 << (uint32_t)but);
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
//************************************************************************************************








