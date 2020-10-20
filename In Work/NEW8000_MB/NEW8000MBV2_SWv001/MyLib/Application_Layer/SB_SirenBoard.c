
#include "SB_SirenBoard.h"

//-----------------------------------------------------------------------------
SirenStatusSTR_Type SirenStatusSTR;
SB_DataUNION_Type   SirenCmdSTR;
//-----------------------------------------------------------------------------
void SirenSaveStatus(uint8_t *RxBuf){
	
	SirenStatusSTR.SirenPower 	= (uint16_t)((RxBuf[SB_SirenPowerPoint] & 0x00FF) |
																					 (RxBuf[SB_SirenPowerPoint+1] << 8) );
	
	SirenStatusSTR.Siren1Status = (uint16_t)((RxBuf[SB_Siren1StatusPoint] & 0x00FF) |
																					 (RxBuf[SB_Siren1StatusPoint+1] << 8) );
	
	SirenStatusSTR.Siren2Status = (uint16_t)((RxBuf[SB_Siren2StatusPoint] & 0x00FF) |
																					 (RxBuf[SB_Siren2StatusPoint+1] << 8) );
	
	SirenStatusSTR.Siren3Status = (uint16_t)((RxBuf[SB_Siren3StatusPoint] & 0x00FF) |
																					 (RxBuf[SB_Siren3StatusPoint+1] << 8) );
}
//-----------------------------------------------------------------------------
void SirenClrStatus(void){
	
	SirenStatusSTR.SirenPower 	= 0;
	SirenStatusSTR.Siren1Status = 0;
	SirenStatusSTR.Siren2Status = 0;
	SirenStatusSTR.Siren3Status = 0;
}
//-----------------------------------------------------------------------------
SirenStatusSTR_Type* SirenGetStatus(void){

	return &SirenStatusSTR;
}
//-----------------------------------------------------------------------------
void SirenGet(uint8_t cmd){


}
//-----------------------------------------------------------------------------
void SirenControl(uint8_t cmd){


}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
SB_DataUNION_Type* SirenRS485TxBuf(void){

  //-----------
  SirenCmdSTR.STR.SinhroByte1 = SinhroByte1Const;
  SirenCmdSTR.STR.SinhroByte2 = SinhroByte2Const;
	SirenCmdSTR.STR.Type        = SB_TypeConst;
  SirenCmdSTR.STR.Size        = 19;
	SirenCmdSTR.STR.CmdCode			= SB_CmdStatusConst;
	
	SirenCmdSTR.STR.CRC16	      = Get_Crc16(SirenCmdSTR.BLK, (SirenCmdSTR.STR.Size - 2));
	
	return &SirenCmdSTR;
	//-----------
}
//-----------------------------------------------------------------------------
