
#include "MB_MotherBoard.h"

//********************************************************************************************
static MBWorkReg_t	MBWorkRegStr;
//********************************************************************************************
//********************************************************************************************
void MotherBoard_BuildingPackForRS485(uint8_t *dataBuf){
	
	MBData_t *dataForTx = (MBData_t*)dataBuf;
  //--------------------
  dataForTx->TimeUTC  = TimeUTC_Get();
	//-----------	
  dataForTx->Addres   = MBWorkRegStr.Address;
  dataForTx->Group    = MBWorkRegStr.Group;
  dataForTx->MB_State = MBWorkRegStr.State;
	//-----------
	dataForTx->SpLineDeviation = SpeakerLine_Param()->Deviation;
	
	dataForTx->Sp1.LineState = SpeakerLine(SP_LINE_1)->LcStatus;
	dataForTx->Sp1.OutState  = SpeakerLine(SP_LINE_1)->IsActivated;
	
	dataForTx->Sp2.LineState = SpeakerLine(ATTEN_LINE_1)->LcStatus;
	dataForTx->Sp2.OutState  = SpeakerLine(ATTEN_LINE_1)->IsActivated;
	
	dataForTx->Sp3.LineState = SpeakerLine(SP_LINE_2)->LcStatus;
	dataForTx->Sp3.OutState  = SpeakerLine(SP_LINE_2)->IsActivated;
	
	dataForTx->Sp4.LineState = SpeakerLine(ATTEN_LINE_2)->LcStatus;
	dataForTx->Sp4.OutState  = SpeakerLine(ATTEN_LINE_2)->IsActivated;
	
	dataForTx->Sp5.LineState = 0;
	dataForTx->Sp5.OutState  = 0;

	dataForTx->Sp6.LineState = 0;
	dataForTx->Sp6.OutState  = 0;
	//-----------
	dataForTx->Fr1.LineState = FireLine(POJ_1)->FaultState;
	dataForTx->Fr1.OutState  = FireLine(POJ_1)->LogicalState;
	
	dataForTx->Fr2.LineState = FireLine(POJ_2)->FaultState;
	dataForTx->Fr2.OutState  = FireLine(POJ_2)->LogicalState;
	
	dataForTx->Fr3.LineState = FireLine(POJ_3)->FaultState;
	dataForTx->Fr3.OutState  = FireLine(POJ_3)->LogicalState;
	
	dataForTx->FrChS.LineState = FireLine(ChS)->FaultState;
	dataForTx->FrChS.OutState  = FireLine(ChS)->LogicalState;	
	//-----------
	//Питание блока.
  dataForTx->StatusPOWER = Power()->State.Byte; //(Power()->BatState | Power()->ACState | Power()->DCState); 
  dataForTx->StatusPA    = PAmp_GetStat();
  dataForTx->DebugData1  = Power()->BatMeas;
  dataForTx->DebugData2  = 0x1234;//(uint16_t)(LcGetValue(5)*5.62)/10;
	//-----------
	//Журнал событий.
	dataForTx->TotalEvents   = Log_Counts()->SavedEvents; //Всего записей в журнале. 
	dataForTx->NotReadEvents = Log_Counts()->UnreadEvents;//Непрочитанных записей в журнеле.
	//-----------
	//Состояние оповещателей.
	dataForTx->SirenPower = SirenBoard()->SirenPower;
	
	dataForTx->Siren1.OutState  = SirenBoard()->Siren1.OutState ; 
	dataForTx->Siren1.LineState = SirenBoard()->Siren1.LineState;
	
	dataForTx->Siren2.OutState  = SirenBoard()->Siren2.OutState ; 
	dataForTx->Siren2.LineState = SirenBoard()->Siren2.LineState;
	
	dataForTx->Siren3.OutState  = SirenBoard()->Siren3.OutState ; 
	dataForTx->Siren3.LineState = SirenBoard()->Siren3.LineState;
  //-----------
}
//*****************************************************************************
MBWorkReg_t* MotherBoard_WorkReg(void){

	return &MBWorkRegStr;
}
//********************************************************************************************
//********************************************************************************************






