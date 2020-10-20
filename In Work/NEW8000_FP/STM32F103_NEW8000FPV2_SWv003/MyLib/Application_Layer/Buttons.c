
#include "Buttons.h"

//-----------------------------------------------------------------------------

Sm1628ButtonSTR_Type	ButtonFacePanelStr;
static uint16_t	      ButtonState    = 0;
static uint16_t	      ButtonOldState = 0;

#pragma pack(push, 1)//размер выравнивания в 1 байт
struct {
	uint8_t  AlgorithmButtonsState;
	uint8_t  ControlButtonsState;
	uint16_t AllButtonsState;
}ButtonsSTR;
#pragma pack(pop)//вернули предыдущую настройку.
//-----------------------------------------------------------------------------
//сортировка битов кнопок
static void ButtonSorting(void){
		
	uint8_t i;
  //--------------------
	ButtonsSTR.AlgorithmButtonsState = 0;
  ButtonsSTR.ControlButtonsState   = 0;	
	for (i = 0; i < 4; i++)
		{
      //Сортировка битов кнопок АЛГОРИТМЫ ОПОВЕЩЕНИЯ.
			ButtonsSTR.AlgorithmButtonsState |= ((((Sm1628ButtonPtr()->BLK[i] & 0x02)<<6) | ((Sm1628ButtonPtr()->BLK[i] & 0x10)<<2)) >> (2*i));
      //Сортировка битов кнопок управлени.
      ButtonsSTR.ControlButtonsState   |= ((((Sm1628ButtonPtr()->BLK[i] & 0x01)<<7) | ((Sm1628ButtonPtr()->BLK[i] & 0x08)<<3)) >> (2*i));
    }
	ButtonsSTR.AllButtonsState =	(ButtonsSTR.ControlButtonsState << 8) | ButtonsSTR.AlgorithmButtonsState;
}
//-----------------------------------------------------------------------------
uint8_t ButtonCheck(void){
	
	__disable_irq();
	//После чтения состояния кнопок производится сортировка битов кнопок
	if(Sm1628ReadButton())
		{
			ButtonSorting();
			__enable_irq();	
			return 1;
		}
	__enable_irq();		
	return 0;
}
//-----------------------------------------------------------------------------
//Получение мгновенного состояния кнопки.
uint8_t ButtonGetInstant(uint8_t button){
		
//	uint8_t i;
//	uint8_t AlgButtonStateTemp     = 0;
//  uint8_t ControlButtonStateTemp = 0;
//  //--------------------
//	for (i = 0; i < 4; i++)
//		{
//      //Сортировка битов кнопок АЛГОРИТМЫ ОПОВЕЩЕНИЯ.
//			AlgButtonStateTemp     |= ((((Sm1628ButtonPtr()->BLK[i] & 0x02)<<6) | ((Sm1628ButtonPtr()->BLK[i] & 0x10)<<2)) >> (2*i));
//      //Сортировка битов кнопок управлени.
//      ControlButtonStateTemp |= ((((Sm1628ButtonPtr()->BLK[i] & 0x01)<<7) | ((Sm1628ButtonPtr()->BLK[i] & 0x08)<<3)) >> (2*i));
//    }
//	//--------------------	
//  if(button == PRESET_BUTTONS) return AlgButtonStateTemp;
//  if(button == CONTROL_BUTTONS)return ControlButtonStateTemp;
//  return 0;
	
  if(button == ALG_BUTTONS)    return ButtonsSTR.AlgorithmButtonsState;
  if(button == CONTROL_BUTTONS)return ButtonsSTR.ControlButtonsState;
  return 0;
}
//-----------------------------------------------------------------------------
//Получение мгновенного состояния одной кнопки.
uint16_t ButtonInstant(ButtonEnumType but){
		
  return (ButtonsSTR.AllButtonsState & (1 << (uint16_t)but));
}
//-----------------------------------------------------------------------------
//Нажатие кнопки. Не отлажено!!!
//uint16_t ButtonPress(ButtonEnumType but){

//	static ButtonEnumType butTemp = 0;
//	//--------------------
//	if(butTemp != ButtonInstant(but))
//		{
//			butTemp = ButtonInstant(but);
//			return 1;
//		}
//	else return 0;	
//}
//-----------------------------------------------------------------------------
//Тригерное состоняие кнопки. При каждом нажатии состояние кнопки инвертируется.
//Нажали-отпустили = 1
//Нажали-отпустили = 0
uint16_t ButtonToggle(ButtonEnumType but){
	
	if(ButtonsSTR.AllButtonsState == 0) ButtonOldState = 0;
	else
		{
			if(ButtonOldState != ButtonsSTR.AllButtonsState)
				{
					ButtonOldState = ButtonsSTR.AllButtonsState;
					ButtonState   ^= ButtonsSTR.AllButtonsState;
				}
		}
	//--------------------
	return (ButtonState & (1 << (uint16_t)but));	
}
//-----------------------------------------------------------------------------
//Получение тригерного состояния кнопки
uint16_t ButtonGetToggle(ButtonEnumType but){

	return (ButtonState & (1 << (uint16_t)but));
}
//-----------------------------------------------------------------------------
//Сброс состояний кнопок.
void ButtonClrToggle(ButtonEnumType but){

  ButtonState    						 &= ~(1 << (uint16_t)but);
//	ButtonOldState 					   &= ~(1 << (uint16_t)but);
//	ButtonsSTR.AllButtonsState &= ~(1 << (uint16_t)but);
}
//-----------------------------------------------------------------------------
//Перемещение по меню ВВЕРХ и ВНИЗ
void ButtonUpDown(uint16_t *param, uint16_t min, uint16_t max){
	
	static uint8_t  TempState      = 0;//Временное состояние кнопок.
	static uint16_t LongPressCount = 0;
	//--------------------
	if(*param < min) *param = min;
	if(*param > max) *param = max;				
	//--------------------	
	//ВНИЗ
	if(ButtonInstant(STARTbut))
	{
		if((*param) == min)return;			
		if(TempState != STARTbut)
			{
				TempState = STARTbut;
				(*param)--;
				
				//if ((*param) > min)(*param)--;//Проверка на минимум.
				//else(*param) = max;           //Закольцовывание редактирования параметра.
			}
		//Ускоренное редактирование параметра.
		else if(++LongPressCount >= LongPressCycleConst)
					{
						(*param)--;
						LongPressCount = (LongPressCycleConst - 5);
					}
	}
	//--------------------
	//ВВЕРХ
	else if(ButtonInstant(STOPbut))
	{
		if((*param) == max) return;
		if(TempState != STOPbut)
			{
				TempState	= STOPbut;
				(*param)++;
				
			 //if ((*param) < max)(*param)++;//Проверка на максимум.
			 //else(*param) = min;           //Закольцовывание редактирования параметра.
			}
		//Ускоренное редактирование параметра.
		else if(++LongPressCount >= LongPressCycleConst)
					{
						(*param)++;
						LongPressCount = (LongPressCycleConst - 5);
					}
	}
	//--------------------
	else
	{
		TempState      = 0;
		LongPressCount = 0;
	}
}
//-----------------------------------------------------------------------------
//Перемещение ВЛЕВО и ВПРАВО.
void ButtonLeftRigth(uint16_t *param, uint16_t min, uint16_t max){
	
	static uint16_t TempState = 0;//Временное состояние кнопок. 
  //--------------------
	//ВЛЕВО
	if(ButtonInstant(ZUMMERbut))
	{
		if((*param) == min)return;			
		if(TempState != ZUMMERbut)
			{
				TempState = ZUMMERbut;
				(*param)--;
				
				//if ((*param) > min)(*param)--;//Проверка на минимум.
				//else(*param) = max;           //Закольцовывание редактирования параметра.
			}
		//Ускоренное редактирование параметра.
//		else if(++LongPressCount >= LongPressCycleConst)
//					{
//						(*param)--;
//						LongPressCount = (LongPressCycleConst - 5);
//					}
	}
	//--------------------
	//ВПРАВО
	else if(ButtonInstant(TESTbut))
	{
		if((*param) == max) return;
		if(TempState != TESTbut)
			{
				TempState	= TESTbut;
				(*param)++;
				
			 //if ((*param) < max)(*param)++;//Проверка на максимум.
			 //else(*param) = min;           //Закольцовывание редактирования параметра.
			}
		//Ускоренное редактирование параметра.
//		else if(++LongPressCount >= LongPressCycleConst)
//					{
//						(*param)++;
//						LongPressCount = (LongPressCycleConst - 5);
//					}
	}
	//--------------------
	else
	{
		TempState      = 0;
//		LongPressCount = 0;
	}	
}
//-----------------------------------------------------------------------------








