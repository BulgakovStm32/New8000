
#include "FP_FacePanel.h"

//************************************************************************************************

static FPData_t FPDataStr;

//************************************************************************************************
//************************************************************************************************
void FacePanelSaveData(uint8_t *dataBuf){
	
	FPData_t    *fp  = (FPData_t*)dataBuf;
	FPButtons_t *but = ButtonFP();
	//-----------
	FPDataStr.Mic			= fp->Mic;
	FPDataStr.Key			= fp->Key;	
	FPDataStr.Address = fp->Address;
	FPDataStr.Group		= fp->Group;
  FPDataStr.TimeUTC = fp->TimeUTC;
	//-----------	
	but->Algorithm = fp->Alg_Buttons;
	but->Control   = fp->Control_Buttons;
	but->All       = (fp->Control_Buttons << 8) | (fp->Alg_Buttons);
	Button_Update();
}
//*****************************************************************************
FPData_t* FacePanel(void){
	
	return &FPDataStr;
}
//************************************************************************************************
//************************************************************************************************

















