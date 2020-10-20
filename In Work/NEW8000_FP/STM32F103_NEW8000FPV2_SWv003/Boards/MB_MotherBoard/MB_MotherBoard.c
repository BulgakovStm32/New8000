
#include "MB_MotherBoard.h"

//-----------------------------------------------------------------------------
//Флаги
static MBFlagsSTR_Type MBFlagsSTR;
//Постановка линий Гр. на контроль.	
//0-й бит - Линия 1.
//1-й бит - Атт. 1.
//2-й бит - Линия 2.
//3-й бит - Атт. 2.
static uint8_t  LcControlReg       = 0xFF;//Все линии Гр. на контроле.
static uint8_t  LcControlFromMBReg = 0; 
//Хранение состояния линий
static uint8_t  Lc1Line = 0; 
static uint8_t  Lc1Att  = 0;
static uint8_t  Lc2Line = 0;
static uint8_t  Lc2Att  = 0;

static uint8_t  Lc1LineTemp = 0;
static uint8_t  Lc1AttTemp  = 0;
static uint8_t  Lc2LineTemp = 0;
static uint8_t  Lc2AttTemp  = 0;
static uint16_t LcDeviationVolueTemp = 0;

//Хранение состояния зон
static uint8_t FpZone1 = 0; 
static uint8_t FpZone2 = 0;
static uint8_t FpZone3 = 0;
static uint8_t FpZone4 = 0;
static uint8_t FpZone5 = 0;
static uint8_t FpZone6 = 0;

//Состояния линий пожарных шлейфов.
static uint8_t Poj1Line = 0;
static uint8_t Poj2Line = 0;
static uint8_t Poj3Line = 0;
static uint8_t ChSLine  = 0;
//************************************************************************************************
//************************************************************************************************
MBFlagsSTR_Type* MBFlags(void){

	return &MBFlagsSTR;
}
//************************************************************************************************
//************************************************************************************************
//Получение установленных на контроль линий Гр.
uint8_t  LcGetControlFromMB(void){
	
	if(Lc1Line != ControlOff) LcControlFromMBReg |=  (1 << Line1);
	else											LcControlFromMBReg &= ~(1 << Line1);
	if(Lc1Att  != ControlOff) LcControlFromMBReg |=  (1 << Att1);
	else											LcControlFromMBReg &= ~(1 << Att1);
	if(Lc2Line != ControlOff) LcControlFromMBReg |=  (1 << Line2);
	else											LcControlFromMBReg &= ~(1 << Line2);
	if(Lc2Att  != ControlOff) LcControlFromMBReg |=  (1 << Att2);
	else											LcControlFromMBReg &= ~(1 << Att2);
	LcControlReg = LcControlFromMBReg;
	return LcControlFromMBReg;
}
//-----------------------------------------------------------------------------
//Постановка линий Гр. на контроль.
//line    - линия, которую хотим поставить/снять на/с клнтроля.
//control - поставить на контрол - ControlOn; снять с контроля - ControlOff. 
void LcSetControl(uint8_t line, uint8_t control){
	
	if(control == ControlOn)  LcControlReg |=  (1 << line);
	if(control == ControlOff) LcControlReg &= ~(1 << line);	
}
//-----------------------------------------------------------------------------
//Получение поставленных на контроль линй Гр.
uint8_t LcGetControl(uint8_t line){
	
	if(line != AllLines) return (LcControlReg & (1 << line));
	else								 return LcControlReg;
}
//-----------------------------------------------------------------------------
void LcSave(uint8_t* p){

  Lc1Line = (*p & LineMask);
  if(Lc1Line != LineUndef) Lc1LineTemp = Lc1Line;  

  Lc1Att  = (*(p+1) & LineMask);
  if(Lc1Att != LineUndef)  Lc1AttTemp = Lc1Att;

  Lc2Line = (*(p+2) & LineMask);
  if(Lc2Line != LineUndef) Lc2LineTemp = Lc2Line;

  Lc2Att  = (*(p+3) & LineMask);
  if(Lc2Att != LineUndef)  Lc2AttTemp = Lc2Att;
}
//-----------------------------------------------------------------------------
//
uint8_t LcGetForTxt(uint8_t ch){

  if(ch == Line1) return Lc1Line;
  if(ch == Att1)  return Lc1Att;
  if(ch == Line2) return Lc2Line;
  if(ch == Att2)  return Lc2Att;
									return LineUndef;
}
//-----------------------------------------------------------------------------
//
uint8_t LcGetForLed(uint8_t ch){
  
  if(ch == Line1) return Lc1LineTemp;
  if(ch == Att1)  return Lc1AttTemp;
  if(ch == Line2) return Lc2LineTemp;
  if(ch == Att2)  return Lc2AttTemp;
									return LineUndef;
}
//-----------------------------------------------------------------------------
void LcDeviationSave(uint16_t* p){

  LcDeviationVolueTemp = *p;
}
//-----------------------------------------------------------------------------
uint16_t LcDeviationGet(void){

  return LcDeviationVolueTemp;
}
//************************************************************************************************
//************************************************************************************************
void ZoneSave(uint8_t* p){

  FpZone1 = (*p & ActionMask);
  FpZone2 = (*(p+1) & ActionMask);
  FpZone3 = (*(p+2) & ActionMask);
  FpZone4 = (*(p+3) & ActionMask);
  FpZone5 = (*(p+4) & ActionMask);
  FpZone6 = (*(p+5) & ActionMask);
}
//-----------------------------------------------------------------------------
uint8_t ZoneGet(uint8_t ch){

  if(ch == Zone1) return FpZone1;
  if(ch == Zone2) return FpZone2;
  if(ch == Zone3) return FpZone3;
  if(ch == Zone4) return FpZone4;
  if(ch == Zone5) return FpZone5;
  if(ch == Zone6) return FpZone6;
  return ActionDefect;
}
//************************************************************************************************
//************************************************************************************************
//
void FireLineSave(uint8_t* p){

  Poj1Line = *p;
  Poj2Line = *(p+1);
  Poj3Line = *(p+2);
  ChSLine  = *(p+3);
//	//получение обобщеного признака(флага) что есть переход в 
//	//состоние ВНИМАНИЕ хотябы на одном входе
//	if((((Poj1Line | Poj2Line | Poj3Line | ChSLine) & FireLineLogicMask) ^ FireLineATTENTION) == 0) MBFlagsSTR.FireLineAlert = FlagSet;  
//	else 																															 												  MBFlagsSTR.FireLineAlert = FlagClr;  
}
//-----------------------------------------------------------------------------
//
uint8_t FireLineGetPhysState(uint8_t ch){

  if(ch == Poj1) return Poj1Line & FireLinePhysMask;
  if(ch == Poj2) return Poj2Line & FireLinePhysMask;
  if(ch == Poj3) return Poj3Line & FireLinePhysMask;
  if(ch == ChS)  return ChSLine  & FireLinePhysMask;
								 return FireLineBreak;
}
//-----------------------------------------------------------------------------
//
uint8_t FireLineGetLogicState(uint8_t ch){

  if(ch == Poj1) return Poj1Line & FireLineLogicMask;
  if(ch == Poj2) return Poj2Line & FireLineLogicMask;
  if(ch == Poj3) return Poj3Line & FireLineLogicMask;
  if(ch == ChS)  return ChSLine & FireLineLogicMask;
								 return FireLineBreak;
}
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
Event_t 			EventStr;
LogWorkReg_t	LogWorkRegStr;
//-----------------------------------------------------------------------------
//Сохранение одного события.
void LogSaveEvent(uint8_t* dataBuf){
	
	Event_t *receivedEvent = (Event_t*)dataBuf;
  //--------------------
	EventStr.EventTimeUtc = receivedEvent->EventTimeUtc;
	EventStr.EventNumber  = receivedEvent->EventNumber;
	EventStr.EventID      = receivedEvent->EventID;
	EventStr.EventParam   = receivedEvent->EventParam;
}
//-----------------------------------------------------------------------------
Event_t* LogEvent(void){

	return &EventStr;
}
//-----------------------------------------------------------------------------
LogWorkReg_t* LogWorkReg(void){

	return &LogWorkRegStr;
}
//************************************************************************************************
//************************************************************************************************
//Работа с конфигурированием логических входов.
static FireLine_t	FireLineArr[4];//массив параметров логических входов.
//-----------------------------------------------------------------------------
FireLine_t* FireLine(uint8_t line){

	return &FireLineArr[line];
}
//************************************************************************************************
//************************************************************************************************
//Работа с микросхемой EEPROM, устанвленной на ЦП.
EepInfo_t EepInfo;

//-----------------------------------------------------------------------------
EepInfo_t* Eeprom(void){

	return &EepInfo;
}
//************************************************************************************************
//************************************************************************************************





























