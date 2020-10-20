
#include "MB_MotherBoard.h"

//********************************************************************************************
//static MBData_t 		MBDataStr;
static MBWorkReg_t	MBWorkRegStr;
//********************************************************************************************
//********************************************************************************************
void MotherBoard_BuildingPackForRS485(uint8_t *dataBuf){
	
	MBData_t *dataForTx = (MBData_t*)dataBuf;
	//-----------
  dataForTx->Addres      = MBWorkRegStr.Address;
  dataForTx->Group       = MBWorkRegStr.Group;
  dataForTx->MB_State    = MBWorkRegStr.State;
	//-----------
	dataForTx->StatusZone1 = SpeakerLine(SP_LINE_1)->LcStatus ;//    | SpeakerLine(SP_LINE_1)->Action);
  dataForTx->StatusZone2 = (SpeakerLine(ATTEN_LINE_1)->LcStatus | SpeakerLine(ATTEN_LINE_1)->Action);
  dataForTx->StatusZone3 = (SpeakerLine(SP_LINE_2)->LcStatus    | SpeakerLine(SP_LINE_2)->Action);
  dataForTx->StatusZone4 = (SpeakerLine(ATTEN_LINE_2)->LcStatus | SpeakerLine(ATTEN_LINE_2)->Action);
  dataForTx->StatusZone5 = 0; //ZoneGetStatus(Zone5);
  dataForTx->StatusZone6 = 0; //ZoneGetStatus(Zone6);
	//-----------
	dataForTx->StatusPoj1  = (FireLine(POJ_1)->FaultState | FireLine(POJ_1)->LogicalState);
  dataForTx->StatusPoj2  = (FireLine(POJ_2)->FaultState | FireLine(POJ_2)->LogicalState);
  dataForTx->StatusPoj3  = (FireLine(POJ_3)->FaultState | FireLine(POJ_3)->LogicalState);
  dataForTx->StatusGOChS = (FireLine(ChS)->FaultState   | FireLine(ChS)->LogicalState);
	//-----------
	//Питание блока.
  dataForTx->StatusPOWER = (Power()->BatState | Power()->ACState | Power()->DCState); 
  dataForTx->StatusPA    = PAmp_GetStat();
  dataForTx->TimeUTC     = TimeUTC_Get();
	//-----------
  dataForTx->DebugData1       = Power()->BatMeas;
  dataForTx->DebugData2   		= 0x1234;//(uint16_t)(LcGetValue(5)*5.62)/10;
  dataForTx->LcDeviationVolue = SpeakerLine_Param()->Deviation;
	//-----------
	//Журнал событий.
	dataForTx->TotalEvents   = Log_Counts()->SavedEvents; //Всего записей в журнале. 
	dataForTx->NotReadEvents = Log_Counts()->UnreadEvents;//Непрочитанных записей в журнеле.
	//-----------
	//Состояние оповещателей.
	dataForTx->SirenPower   = SirenBoard()->SirenPowerVol;
	dataForTx->Siren1Status = SirenBoard()->Siren1Status; 
	dataForTx->Siren2Status = SirenBoard()->Siren2Status;
	dataForTx->Siren3Status = SirenBoard()->Siren3Status;
  //-----------
}
//*****************************************************************************
MBWorkReg_t* MotherBoard_WorkReg(void){

return &MBWorkRegStr;
}
//********************************************************************************************
//********************************************************************************************






