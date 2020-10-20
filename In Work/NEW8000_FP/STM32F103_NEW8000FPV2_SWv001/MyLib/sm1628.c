

#include "sm1628.h"

//-----------------------------------------------------------------------------

IndicatorSTR_TypeDef	  IndicatorsFacePanelStr;
ButtonSTR_TypeDef		    ButtonFacePanelStr;
ButtonStateSTR_TypeDef	ButtonStateStr;

//-----------------------------------------------------------------------------
//static void Sm1628Pause(uint16_t pause){

//  while(pause != 0){--pause;}
//}
//-----------------------------------------------------------------------------
static void Sm1628SendByte(uint8_t byte){

	Spi2TxData(byte);
}
//-----------------------------------------------------------------------------
static void Sm1628SendCmd(uint8_t cmd){
  
	//Spi2BidirectionalMode(Spi2TransmitOnlyMode);
	CsSm1628Low;
  //Sm1628Pause(Sm1628PauseConst);
	Spi2TxData(cmd);
	CsSm1628Hiht;
  //Sm1628Pause(Sm1628PauseConst);
}
//-----------------------------------------------------------------------------
//void Sm1628Init(void){

//	uint8_t i;
//  //--------------------
    //Очистка буфера.
//  for(i=0; i<QUANTITY_FACE_PANEL_BYTE; i++)
//    {
//      IndicatorsFacePanelStr.BLK[i] = 0;
//    }
//  Sm1628SendCmd(CMD4_DISPLAY_OFF);
//}
//-----------------------------------------------------------------------------
//Передача буфера в SM1628 - управление светодиодами.
void Sm1628SendDataBuf(void){
	
	uint8_t i = 0;
  //--------------------
 	Sm1628SendCmd(CMD2_WRITE);
 	CsSm1628Low;
 	//Sm1628Pause(Sm1628PauseConst);
 	Sm1628SendByte(CMD3_ADDRESS_00);
 	//Sm1628Pause(Sm1628PauseConst);
 	//--------------------
 	//передеча массива байт в лицевую панель.
  for (i = 0; i < QUANTITY_FACE_PANEL_BYTE; i++)
    {
      Sm1628SendByte(IndicatorsFacePanelStr.BLK[i]);
    }
 	//--------------------
 	//Sm1628Pause(Sm1628PauseConst);
 	CsSm1628Hiht;
  //Sm1628Pause(Sm1628PauseConst);
 	Sm1628SendCmd(CMD1_7DIGITS_10SEG);	
	Sm1628SendCmd(CMD4_DISPLAY_ON | DEFAULT_BRIGHT_DISPLAY);
}
//-----------------------------------------------------------------------------
//Считывание данных из Sm1628 - считывание состояния кнопок.
static void Sm1628ReadData(ButtonSTR_TypeDef *sostBut){

	uint8_t i = 0;
	//--------------------
  CsSm1628Low;
  //Sm1628Pause(25);
  Sm1628SendByte(CMD2_READ);    //Команда на чтение
	Spi2BiDirMode(Spi2RxOnlyMode);//Receive-only mode.
	//--------------------
 	//Чтение байт из лицевую панель.
	for (i = 0; i < QUANTITY_READ_BYTE; i++)
		{
			sostBut->BLK[i] = Spi2RxData();
		}
	//--------------------    
  Spi2BiDirMode(Spi2TxOnlyMode);//Transmit-only mode.
  //Sm1628Pause(10);
	CsSm1628Hiht;
}
//-----------------------------------------------------------------------------
//Считываение состояния кнопок и мажоритарное определение состояния кнопок.
//Вызывается в прерывании по таймеру1 каждые 10 мСек. 
uint8_t Sm1628ReadButton(void){
 
 	uint8_t i = 0;
 	static uint8_t cycle = 0;
 	static ButtonSTR_TypeDef SostBut[3];
 	//-------------------------------
 	if (cycle < 3)
 		{
 			Sm1628ReadData(&SostBut[cycle]);//Считывание данных из Sm1628.
			//Обработка проверочных бит. 
			//Если один из проверочных бит будет равен 1 значит 
      //некорректно вычитаны данные из pt6964.
			if((SostBut[cycle].BLK[0] & (CHECKBIT1 | CHECKBIT2)) ||
				 (SostBut[cycle].BLK[1] & (CHECKBIT1 | CHECKBIT2)) ||
				 (SostBut[cycle].BLK[2] & (CHECKBIT1 | CHECKBIT2)) ||
				 (SostBut[cycle].BLK[3] & (CHECKBIT1 | CHECKBIT2)) ||
				 (SostBut[cycle].BLK[4] & (CHECKBIT1 | CHECKBIT2)))
				{
					 cycle = 0; 
				}
			else cycle++;
 			return 0;
 		} 
 	//-------------------------------
 	else
 		{
 			cycle = 0;
 			//Мажоритарное определение состояния кнопок.
  			for (i = 0; i < 5; i++)
  				{
  					ButtonFacePanelStr.BLK[i] = ((SostBut[0].BLK[i] & SostBut[1].BLK[i]) |
                                         (SostBut[1].BLK[i] & SostBut[2].BLK[i]) |
                                         (SostBut[0].BLK[i] & SostBut[2].BLK[i]));
  				}
       //Сортировка
 		}
 	//-------------------------------
  return 1;
}
//-----------------------------------------------------------------------------
//Сортировка состояния кнопок.
uint8_t SortingButtons(uint8_t button){
  
	uint8_t i = 0;
	uint8_t PresetButtonSostTemp = 0;
  uint8_t ControlButtonSostTemp = 0;
  //--------------------
	for (i = 0; i < 4; i++)
		{
      //Сортировка битов кнопок PRESETx.
			PresetButtonSostTemp  |= ((((ButtonFacePanelStr.BLK[i] & 0x02)<<6) | ((ButtonFacePanelStr.BLK[i] & 0x10)<<2)) >> (2*i));
      //Сортировка битов кнопок управлени.
      ControlButtonSostTemp |= ((((ButtonFacePanelStr.BLK[i] & 0x01)<<7) | ((ButtonFacePanelStr.BLK[i] & 0x08)<<3)) >> (2*i));
    }
  //--------------------
  if(button == PRESET_BUTTONS) return PresetButtonSostTemp;
  if(button == CONTROL_BUTTONS)return ControlButtonSostTemp;
  return 0;
}
//-----------------------------------------------------------------------------
























