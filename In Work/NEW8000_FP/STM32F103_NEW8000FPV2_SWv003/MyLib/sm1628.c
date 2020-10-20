

#include "sm1628.h"

//-----------------------------------------------------------------------------

static Sm1628ButtonSTR_Type  Sm1628ButtonSTR;

//-----------------------------------------------------------------------------
static void Sm1628Pause(uint16_t pause){

	while(pause--){};	
}
//-----------------------------------------------------------------------------
static void Sm1628SendByte(uint8_t byte){

	Spi2TxData(byte);
}
//-----------------------------------------------------------------------------
static void Sm1628SendCmd(uint8_t cmd){
  
	//Spi2BidirectionalMode(Spi2TransmitOnlyMode);
	Sm1628CsLow;
  Sm1628Pause(Sm1628PauseConst);
	Sm1628SendByte(cmd);
	Sm1628CsHiht;
  //Sm1628Pause(Sm1628PauseConst);
}
//-----------------------------------------------------------------------------
//Передача буфера в SM1628 - управление светодиодами.
void Sm1628SendLed(Sm1628LedSTR_Type* ledArray){
	
	uint8_t i;
  //--------------------
 	Sm1628SendCmd(CMD2_WRITE);
 	Sm1628CsLow;
 	//Sm1628Pause(Sm1628PauseConst);
 	Sm1628SendByte(CMD3_ADDRESS_00);
 	//Sm1628Pause(Sm1628PauseConst);
 	//--------------------
 	//передеча массива байт в лицевую панель.
  for (i = 0; i < QUANTITY_FACE_PANEL_BYTE; i++)
    {
      Sm1628SendByte(ledArray->BLK[i]);
    }
 	//--------------------
 	//Sm1628Pause(Sm1628PauseConst);
 	Sm1628CsHiht;
  //Sm1628Pause(Sm1628PauseConst);
 	Sm1628SendCmd(CMD1_7DIGITS_10SEG);	
	Sm1628SendCmd(CMD4_DISPLAY_ON | DEFAULT_BRIGHT_DISPLAY);
}
//-----------------------------------------------------------------------------
//Считывание данных из Sm1628 - считывание состояния кнопок.
static void Sm1628ReadData(Sm1628ButtonSTR_Type* sostBut){

	uint8_t i = 0;
	//--------------------
  Sm1628CsLow;
  //Sm1628Pause(Sm1628PauseConst);
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
	Sm1628CsHiht;
}
//-----------------------------------------------------------------------------
//Считываение состояния кнопок и мажоритарное определение состояния кнопок.
//Вызывается в прерывании по таймеру1 каждые 10 мСек. 
uint8_t Sm1628ReadButton(void){
 
 	uint8_t i = 0;
 	static uint8_t cycle = 0;
 	static Sm1628ButtonSTR_Type SostBut[3];
 	//-------------------------------
 	if(cycle < 3)
 		{
 			Sm1628ReadData(&SostBut[cycle]);//Считывание данных из Sm1628.
			//Обработка проверочных бит. 
			//Если один из проверочных бит будет равен 1 значит 
      //некорректно вычитаны данные из Sm1628.
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
	cycle = 0;
	//Мажоритарное определение состояния кнопок.
	for (i = 0; i < 5; i++)
		{
			Sm1628ButtonSTR.BLK[i] = ((SostBut[0].BLK[i] & SostBut[1].BLK[i]) |
																(SostBut[1].BLK[i] & SostBut[2].BLK[i]) |
																(SostBut[0].BLK[i] & SostBut[2].BLK[i]));
		}
 	//-------------------------------
  return 1;
}
//-----------------------------------------------------------------------------
Sm1628ButtonSTR_Type* Sm1628ButtonPtr(void){

	return &Sm1628ButtonSTR;
}
//-----------------------------------------------------------------------------





