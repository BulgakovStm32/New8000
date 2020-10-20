

#include "sm1628.h"

//-----------------------------------------------------------------------------
static SM1628Buttons_t  SM1628ButtonsStr;
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
void Sm1628SendLed(uint8_t* ledBuf){
	
 	Sm1628SendCmd(CMD2_WRITE);
 	Sm1628CsLow;
 	//Sm1628Pause(Sm1628PauseConst);
 	Sm1628SendByte(CMD3_ADDRESS_00);
 	//Sm1628Pause(Sm1628PauseConst);
 	//--------------------
 	//передеча массива байт в лицевую панель.
	for(uint8_t i = 0; i < LED_BUF_SIZE; i++)
    {
      Sm1628SendByte(*(ledBuf+i));
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
static void Sm1628ReadData(SM1628Buttons_t* sostBut){

	uint8_t i = 0;
	//--------------------
  Sm1628CsLow;
  //Sm1628Pause(Sm1628PauseConst);
  Sm1628SendByte(CMD2_READ);    //Команда на чтение
	Spi2BiDirMode(SPI_RX_ONLY_MODE);//Receive-only mode.
	//--------------------
 	//Чтение байт из лицевую панель.
	for (i = 0; i < QUANTITY_READ_BYTE; i++)
		{
			sostBut->BLK[i] = Spi2RxData();
		}
	//--------------------    
  Spi2BiDirMode(SPI_TX_ONLY_MODE);//Transmit-only mode.
  //Sm1628Pause(10);
	Sm1628CsHiht;
}
//-----------------------------------------------------------------------------
//Считываение состояния кнопок и мажоритарное определение состояния кнопок.
//Вызывается в прерывании по таймеру1 каждые 10 мСек. 
uint8_t Sm1628ReadButton(void){
 
 	uint8_t i = 0;
 	static uint8_t cycle = 0;
 	static SM1628Buttons_t stateBut[3];
 	//-------------------------------
	Sm1628ReadData(&stateBut[cycle]);//Считывание данных из Sm1628.
	//Обработка проверочных бит. 
	//Если один из проверочных бит будет равен 1 значит 
	//некорректно вычитаны данные из Sm1628.
	if((stateBut[cycle].BLK[0] & (CHECKBIT1 | CHECKBIT2)) ||
		 (stateBut[cycle].BLK[1] & (CHECKBIT1 | CHECKBIT2)) ||
		 (stateBut[cycle].BLK[2] & (CHECKBIT1 | CHECKBIT2)) ||
		 (stateBut[cycle].BLK[3] & (CHECKBIT1 | CHECKBIT2)) ||
		 (stateBut[cycle].BLK[4] & (CHECKBIT1 | CHECKBIT2)))
		{
			 cycle = 0; 
		}
	else cycle++;
 	//-------------------------------
	if(cycle == 3)	
		{
			cycle = 0;
			//Мажоритарное определение состояния кнопок.
			for (i = 0; i < 5; i++)
					{
						SM1628ButtonsStr.BLK[i] = ((stateBut[0].BLK[i] & stateBut[1].BLK[i]) |
																			 (stateBut[1].BLK[i] & stateBut[2].BLK[i]) |
																			 (stateBut[0].BLK[i] & stateBut[2].BLK[i]));
					}
			return 1;
		}
 	//-------------------------------
	return 0;
}
//-----------------------------------------------------------------------------
SM1628Buttons_t* SM1628Buttons(void){

	return &SM1628ButtonsStr;
}
//-----------------------------------------------------------------------------





