
#include "SB_SirenBoard.h"
//-----------------------------------------------------------------------------
static SBData_t SirenStr;
//-----------------------------------------------------------------------------
void Siren_SaveData(uint8_t* buf){
	
	MBData_t* p = (MBData_t*)buf;
	//------------------------
	SirenStr.PowerVol = p->SirenPower;
	SirenStr.Siren1   = p->Siren1;
	SirenStr.Siren2   = p->Siren2;
	SirenStr.Siren3   = p->Siren3;	
}
//-----------------------------------------------------------------------------
SBData_t* Siren(void){
	
	return &SirenStr;
}
//-----------------------------------------------------------------------------
