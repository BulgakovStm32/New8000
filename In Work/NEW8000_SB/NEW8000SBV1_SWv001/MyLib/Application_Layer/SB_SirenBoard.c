
#include "SB_SirenBoard.h"
//***************************************************************************************************
static SBData_t SirenDataStr;
//***************************************************************************************************
//***************************************************************************************************
void SaveDataFromMB(uint8_t* dataBuf){

	SBData_t *dataFromMB = (SBData_t*)dataBuf;
	//--------------------
	SirenDataStr.Siren1 = dataFromMB->Siren1;
	SirenDataStr.Siren2 = dataFromMB->Siren2;
	SirenDataStr.Siren3 = dataFromMB->Siren3;	
}
//*****************************************************************
Line_t* DataFromMB(uint8_t line){

	if(line == SIREN_1) return &SirenDataStr.Siren1;
	if(line == SIREN_2) return &SirenDataStr.Siren2;
	if(line == SIREN_3) return &SirenDataStr.Siren3;
	return 0;
}
//*****************************************************************
void SirenBoard_BuildingPackForRS485(uint8_t *dataBuf){
	
	SBData_t *dataForTx = (SBData_t*)dataBuf;
	//--------------------
	dataForTx->PowerVol    = Siren_GetMeas(SIREN_POWER);
	dataForTx->Siren1.Line = Siren_LineStatus(SIREN_1);
	dataForTx->Siren2.Line = Siren_LineStatus(SIREN_2);
	dataForTx->Siren3.Line = Siren_LineStatus(SIREN_3);
}
//***************************************************************************************************
//***************************************************************************************************
