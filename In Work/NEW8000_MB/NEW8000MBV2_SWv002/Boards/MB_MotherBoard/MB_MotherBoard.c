
#include "MB_MotherBoard.h"

//-----------------------------------------------------------------------------
MBDataUNION_Type	MotherBoardTxSTR;

static volatile uint8_t Address  = 1;
static volatile uint8_t Group    = 1;
static volatile uint8_t Priority = 0;

//-----------------------------------------------------------------------------
MBDataUNION_Type* MotherBoardRS485TxBuf(void){
	
	MotherBoardTxSTR.STR.SinhroByte1 = SinhroByte1Const;
  MotherBoardTxSTR.STR.SinhroByte2 = SinhroByte2Const;
	MotherBoardTxSTR.STR.Type        = MB_TypeConst;
  MotherBoardTxSTR.STR.Size        = (uint8_t)sizeof(MotherBoardTxSTR.BLK);//40; //Размер всего пакета с CRC.
  MotherBoardTxSTR.STR.CmdCode 		 = CmdMBStatusConst;
  //-----------
  MotherBoardTxSTR.STR.Addres      = Address;
  MotherBoardTxSTR.STR.Group       = Group;
  MotherBoardTxSTR.STR.Priority    = Priority;//Данный байт устанавливается через RS485MB_SetPriority(uint8_t stat)

	MotherBoardTxSTR.STR.StatusZone1 = (SpeakerLine(SpLine1)->LcStatus  | SpeakerLine(SpLine1)->Action);
  MotherBoardTxSTR.STR.StatusZone2 = (SpeakerLine(SpAtten1)->LcStatus | SpeakerLine(SpAtten1)->Action);
  MotherBoardTxSTR.STR.StatusZone3 = (SpeakerLine(SpLine2)->LcStatus  | SpeakerLine(SpLine2)->Action);
  MotherBoardTxSTR.STR.StatusZone4 = (SpeakerLine(SpAtten2)->LcStatus | SpeakerLine(SpAtten2)->Action);
  MotherBoardTxSTR.STR.StatusZone5 = 0;//ZoneGetStatus(Zone5);
  MotherBoardTxSTR.STR.StatusZone6 = 0;//ZoneGetStatus(Zone6);

	MotherBoardTxSTR.STR.StatusPoj1  = (FireLine(Poj1)->FaultState | FireLine(Poj1)->LogicalState);
  MotherBoardTxSTR.STR.StatusPoj2  = (FireLine(Poj2)->FaultState | FireLine(Poj2)->LogicalState);
  MotherBoardTxSTR.STR.StatusPoj3  = (FireLine(Poj3)->FaultState | FireLine(Poj3)->LogicalState);
  MotherBoardTxSTR.STR.StatusGOChS = (FireLine(ChS)->FaultState  | FireLine(ChS)->LogicalState);
	//-----------
	//Питание блока.
  MotherBoardTxSTR.STR.StatusPOWER = (Power()->BatState | Power()->PowerState); 
  MotherBoardTxSTR.STR.StatusPA    = AmpGetStat();
  MotherBoardTxSTR.STR.TimeUTC     = TimeUTCGet();
	//-----------
  MotherBoardTxSTR.STR.DebugData1       = Power()->BatMeas;
  //MotherBoardTxSTR.STR.DebugData1   = (uint16_t)(LcGetValue(4)*5.62)/10;
  //MotherBoardTxSTR.STR.DebugData2   = (uint16_t)(LcGetValue(5)*5.62)/10;
  MotherBoardTxSTR.STR.LcDeviationVolue = *SpeakerLineDeviation();
	//-----------
	//Журнал событий.
	MotherBoardTxSTR.STR.TotalEvents   = LogGetTotalEventsSave();//Всего записей в журнале. 
	MotherBoardTxSTR.STR.NotReadEvents = LogGetNotReadEvents();  //Непрочитанных записей в журнеле.
	//-----------
	//Состояние оповещателей.
	MotherBoardTxSTR.STR.SirenPower   = SirenGetData()->STR.SirenPowerMeas;
	MotherBoardTxSTR.STR.Siren1Status = SirenGetData()->STR.Siren1Status; //SirenGetData()->STR.Siren1Meas;
	MotherBoardTxSTR.STR.Siren2Status = SirenGetData()->STR.Siren2Status; //SirenGetData()->STR.Siren2Meas;
	MotherBoardTxSTR.STR.Siren3Status = SirenGetData()->STR.Siren3Status; //SirenGetData()->STR.Siren3Meas;
  //-----------
	MotherBoardTxSTR.STR.CRC16	      = Get_Crc16(MotherBoardTxSTR.BLK, (MotherBoardTxSTR.STR.Size - 2));
  //-----------
	return &MotherBoardTxSTR;
}
//-----------------------------------------------------------------------------
void SetAddress(uint8_t adr){

  Address = adr;
}
uint8_t GetAddress(void){

  return Address;
}
//-----------------------------------------------------------------------------
void SetGroup(uint8_t grp){

  Group = grp;
}
uint8_t GetGroup(void){

  return Group;
}
//-----------------------------------------------------------------------------
void SetPriority(uint8_t prt){

  Priority = prt;
}
uint8_t GetPriority(void){

  return Priority;
}
//-----------------------------------------------------------------------------
