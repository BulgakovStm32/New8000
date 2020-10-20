
#include "SB_SirenBoard.h"

//*************************************************************************************************

static SBData_t	SirenDataStr;

//*************************************************************************************************
//*************************************************************************************************
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
//	dataForTx->SirenPowerVol = 0;
	dataForTx->Siren1Status  = SirenDataStr.Siren1Status;
	dataForTx->Siren2Status  = SirenDataStr.Siren2Status;
	dataForTx->Siren3Status  = SirenDataStr.Siren3Status;
}
//*****************************************************************************
void SirenBoard_ClrData(void){
	
	SirenDataStr.SirenPowerVol = NO_CONNECTION_WITH_SB;
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
			SirenDataStr.Siren1Status = (SirenDataStr.Siren1Status & SIREN_LINE_MASK) | cmd;
		break;
		//-----------
		case(SIREN_2):
			SirenDataStr.Siren2Status = (SirenDataStr.Siren2Status & SIREN_LINE_MASK) | cmd;
		break;
		//-----------
		case(SIREN_3):
			SirenDataStr.Siren3Status = (SirenDataStr.Siren3Status & SIREN_LINE_MASK) | cmd;
		break;
		//-----------
		case(SIREN_ALL):
			SirenDataStr.Siren1Status = (SirenDataStr.Siren1Status & SIREN_LINE_MASK) | cmd;
			SirenDataStr.Siren2Status = (SirenDataStr.Siren2Status & SIREN_LINE_MASK) | cmd;
			SirenDataStr.Siren3Status = (SirenDataStr.Siren3Status & SIREN_LINE_MASK) | cmd;		
		break;
		//-----------		
		default:
			SirenDataStr.Siren1Status = (SirenDataStr.Siren1Status & SIREN_LINE_MASK) | SIREN_OUT_OFF;
			SirenDataStr.Siren2Status = (SirenDataStr.Siren2Status & SIREN_LINE_MASK) | SIREN_OUT_OFF;
			SirenDataStr.Siren3Status = (SirenDataStr.Siren3Status & SIREN_LINE_MASK) | SIREN_OUT_OFF;		
		break;
		//-----------		
	}
}
//*************************************************************************************************
//*************************************************************************************************















