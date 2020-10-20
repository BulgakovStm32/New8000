
#include "MB_MotherBoard.h"

//************************************************************************************************
//Хранение состояния линий
static uint8_t  Lc1LineTemp = 0;
static uint8_t  Lc1AttTemp  = 0;
static uint8_t  Lc2LineTemp = 0;
static uint8_t  Lc2AttTemp  = 0;
//***************************************
static MBData_t  MotherBoardDataStr;
static SpParam_t SpParamStr;
//************************************************************************************************
//************************************************************************************************
uint8_t SpLine_GetOutStateForTxt(uint8_t out){

  if(out == Line1) return MotherBoardDataStr.Sp1.LineState;
  if(out == Att1)  return MotherBoardDataStr.Sp2.LineState;
  if(out == Line2) return MotherBoardDataStr.Sp3.LineState;
  if(out == Att2)  return MotherBoardDataStr.Sp4.LineState;
									 return LineUndef;
}
//-----------------------------------------------------------------------------
//
uint8_t SpLine_GetOutStateForLed(uint8_t out){
  
  if(out == Line1) return Lc1LineTemp;
  if(out == Att1)  return Lc1AttTemp;
  if(out == Line2) return Lc2LineTemp;
  if(out == Att2)  return Lc2AttTemp;
									 return LineUndef;
}
//-----------------------------------------------------------------------------
uint8_t SpLine_GetOutState(uint8_t out){

  if(out == Zone1) return MotherBoardDataStr.Sp1.OutState;
  if(out == Zone2) return MotherBoardDataStr.Sp2.OutState;
  if(out == Zone3) return MotherBoardDataStr.Sp3.OutState;
  if(out == Zone4) return MotherBoardDataStr.Sp4.OutState;
  if(out == Zone5) return MotherBoardDataStr.Sp5.OutState;
  if(out == Zone6) return MotherBoardDataStr.Sp6.OutState;
  return ActionDefect;
}
//-----------------------------------------------------------------------------
uint8_t SpLine_CompareAllLinesWith(uint8_t state){
	
	if((MotherBoardDataStr.Sp1.OutState == state) ||
		 (MotherBoardDataStr.Sp2.OutState == state) ||
		 (MotherBoardDataStr.Sp3.OutState == state) ||
		 (MotherBoardDataStr.Sp4.OutState == state) ||
		 (MotherBoardDataStr.Sp5.OutState == state) ||	
		 (MotherBoardDataStr.Sp6.OutState == state)) return 1;
	else return 0;
}
//-----------------------------------------------------------------------------
SpParam_t* SpLine_Param(void){

	return &SpParamStr;
}
//************************************************************************************************
//************************************************************************************************
uint8_t FireLine_GetLineState(uint8_t in){

  if(in == Poj1) return MotherBoardDataStr.Fr1.LineState;
  if(in == Poj2) return MotherBoardDataStr.Fr2.LineState;
  if(in == Poj3) return MotherBoardDataStr.Fr3.LineState;
  if(in == ChS)  return MotherBoardDataStr.FrChS.LineState;
								 return FR_LINE_BREAK;	
}
//-------------------------------------------------------
uint8_t FireLine_GetInState(uint8_t in){

  if(in == Poj1) return MotherBoardDataStr.Fr1.OutState;
  if(in == Poj2) return MotherBoardDataStr.Fr2.OutState;
  if(in == Poj3) return MotherBoardDataStr.Fr3.OutState;
  if(in == ChS)  return MotherBoardDataStr.FrChS.OutState;
								 return FR_LINE_BREAK;
}
//-------------------------------------------------------
uint8_t FireLine_CompareAllLinesWith(uint8_t state){
	
	if((MotherBoardDataStr.Fr1.OutState == state) ||
		 (MotherBoardDataStr.Fr2.OutState == state) ||
		 (MotherBoardDataStr.Fr3.OutState == state)) return 1;
	else return 0;
}
//************************************************************************************************
//************************************************************************************************
void MotherBoard_SaveData(uint8_t *dataBuf){
	
	uint32_t i;
	uint8_t* destination = (uint8_t*)&MotherBoardDataStr;
  //--------------------
	for(i = 0; i < MOTHER_BOARD_DATA_SIZE; i++)
		{
			*(destination + i) =  *(dataBuf + i);
		}
	//--------------------
	if(SpParamStr.ConfigFlag)return;
	
  if(MotherBoardDataStr.Sp1.LineState != LineUndef) Lc1LineTemp = MotherBoardDataStr.Sp1.LineState;  
  if(MotherBoardDataStr.Sp2.LineState != LineUndef) Lc1AttTemp  = MotherBoardDataStr.Sp2.LineState;
  if(MotherBoardDataStr.Sp3.LineState != LineUndef) Lc2LineTemp = MotherBoardDataStr.Sp3.LineState;
  if(MotherBoardDataStr.Sp4.LineState != LineUndef) Lc2AttTemp  = MotherBoardDataStr.Sp4.LineState;
	//------------------
	if(MotherBoardDataStr.Sp1.LineState != LineOffControl) SpParamStr.Control |= (1 << Line1);
	else SpParamStr.Control &= ~(1 << Line1);	
	
	if(MotherBoardDataStr.Sp2.LineState != LineOffControl) SpParamStr.Control |= (1 << Att1);
	else SpParamStr.Control &= ~(1 << Att1);
	
	if(MotherBoardDataStr.Sp3.LineState != LineOffControl) SpParamStr.Control |= (1 << Line2);
	else SpParamStr.Control &= ~(1 << Line2);
	
	if(MotherBoardDataStr.Sp4.LineState != LineOffControl) SpParamStr.Control |= (1 << Att2);
	else SpParamStr.Control &= ~(1 << Att2);
	//------------------	
	SpParamStr.Deviation = MotherBoardDataStr.SpLineDeviation;
}
//*****************************************************************************
MBData_t* MotherBoard(void){

	return &MotherBoardDataStr;
}
//*****************************************************************************



//************************************************************************************************
//************************************************************************************************
//питание блока и УМЗЧ.
static PowerSTR_t PowerSTR;
//-----------------------------------------------------------------------------
PowerSTR_t* PowerDevice(void){

	return &PowerSTR;
}
//************************************************************************************************
//************************************************************************************************
//Работа с журналом событий.

static Log_t LogStr;
//-----------------------------------------------------------------------------
//Сохранение одного события.
void LogSaveEvent(uint8_t* dataBuf){
	
	Event_t *receivedEvent = (Event_t*)dataBuf;
  //--------------------
	LogStr.Event.TimeUtc = receivedEvent->TimeUtc;
	LogStr.Event.Number  = receivedEvent->Number;
	LogStr.Event.ID      = receivedEvent->ID;
	LogStr.Event.Param   = receivedEvent->Param;
}
//-----------------------------------------------------------------------------
Log_t* Log(void){

	return &LogStr;
}
//************************************************************************************************
//************************************************************************************************
//Работа с конфигурированием логических входов.
static FireLine_t	FireLineArr[4];//массив параметров логических входов.

//*****************************************************************************
void FireLine_Save(uint8_t *dataBuf){

	FireLine_t *rxFireLineData = (FireLine_t*)dataBuf;
	uint32_t num = rxFireLineData->Number;
  //--------------------	
	FireLineArr[num].Number        = rxFireLineData->Number;
	FireLineArr[num].Type      		 = rxFireLineData->Type;
	FireLineArr[num].TimeOut       = rxFireLineData->TimeOut;
	FireLineArr[num].PhysicalState = rxFireLineData->PhysicalState;
	FireLineArr[num].MeasValue     = rxFireLineData->MeasValue;			
}
//*****************************************************************************
FireLine_t* FireLine(uint32_t line){

	return &FireLineArr[line];
}
//************************************************************************************************
//************************************************************************************************





























