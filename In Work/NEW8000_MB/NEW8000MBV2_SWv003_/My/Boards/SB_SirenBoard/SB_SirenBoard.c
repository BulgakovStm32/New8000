
#include "SB_SirenBoard.h"

//*************************************************************************************************

static SBData_t	SirenDataStr;

//*************************************************************************************************
//*************************************************************************************************
void SirenBoard_Init(void){

	SirenDataStr.SirenPower       = 0;
	SirenDataStr.Siren1.LineState = SIREN_LINE_NORM;
	SirenDataStr.Siren2.LineState = SIREN_LINE_NORM;
	SirenDataStr.Siren3.LineState = SIREN_LINE_NORM;
}
//*****************************************************************************
void SirenBoard_SaveData(uint8_t *dataBuf){

	uint32_t count;
	uint8_t *destination = (uint8_t*)&SirenDataStr;
	//-----------
	for(count = 0; count < SB_DATA_SIZE; count++)
		{
			*(destination + count) = *(dataBuf + count);
		}
}
//*****************************************************************************
void SirenBoard_BuildingPackForRS485(uint8_t *dataBuf){

	SBData_t *dataForTx = (SBData_t*)dataBuf;
	//-----------	
	dataForTx->Siren1.OutState = SirenDataStr.Siren1.OutState;
	dataForTx->Siren2.OutState = SirenDataStr.Siren2.OutState;
	dataForTx->Siren3.OutState = SirenDataStr.Siren3.OutState;
}
//*****************************************************************************
void SirenBoard_ClrData(void){
	
	SirenDataStr.SirenPower = NO_CONNECTION_WITH_SB;
//	SirenDataStr.Siren1Status  = 0;
//	SirenDataStr.Siren2Status  = 0;
//	SirenDataStr.Siren3Status  = 0;			
}
//*****************************************************************************
SBData_t* SirenBoard(void){

	return &SirenDataStr;
}
//*****************************************************************************
void SirenBoard_Set(uint8_t siren, uint8_t cmd){
	
	switch(siren){
		//-----------
		case(SIREN_1): 
			SirenDataStr.Siren1.OutState = cmd;
		break;
		//-----------
		case(SIREN_2):
			SirenDataStr.Siren2.OutState = cmd;
		break;
		//-----------
		case(SIREN_3):
			SirenDataStr.Siren3.OutState = cmd;
		break;
		//-----------
		case(SIREN_ALL):
			SirenDataStr.Siren1.OutState = cmd;
			SirenDataStr.Siren2.OutState = cmd;
			SirenDataStr.Siren3.OutState = cmd;
		break;
		//-----------		
		default:
			SirenDataStr.Siren1.OutState = SIREN_OUT_OFF;
			SirenDataStr.Siren2.OutState = SIREN_OUT_OFF;
			SirenDataStr.Siren3.OutState = SIREN_OUT_OFF;		
		break;
		//-----------		
	}
}
//*************************************************************************************************
//*************************************************************************************************















