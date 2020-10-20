
#include "SB_SirenBoard.h"

//-----------------------------------------------------------------------------
Siren_t  		 SirenDataUNION;
SBDataPack_t SirenCmdSTR;
//-----------------------------------------------------------------------------
void SirenSaveData(uint8_t *Buf){
	
	uint8_t i = 0;
	//--------------------
	while(i < 11) 
		{
			SirenDataUNION.BLK[i] = Buf[(i + 5)];
			i++;
		}
}
//-----------------------------------------------------------------------------
void SirenClrData(void){
	
	uint8_t i = 0;
	//--------------------
	while(i < 11) 
		{
			SirenDataUNION.BLK[i] = 0;
			i++;
		}
	SirenDataUNION.STR.SirenPowerMeas = SB_NoConnection;
}
//-----------------------------------------------------------------------------
Siren_t* SirenGetData(void){

	return &SirenDataUNION;
}
//-----------------------------------------------------------------------------
void SirenGet(uint8_t cmd){


}
//-----------------------------------------------------------------------------
void SirenHeaderTxBuf(void){

  SirenCmdSTR.STR.SinhroByte1 = SinhroByte1Const;
  SirenCmdSTR.STR.SinhroByte2 = SinhroByte2Const;
	SirenCmdSTR.STR.Type        = SB_TypeConst;
}
//-----------------------------------------------------------------------------
void SirenStatusRequest(void){

	SirenHeaderTxBuf();
  SirenCmdSTR.STR.Size    = 18;
	SirenCmdSTR.STR.CmdCode = SB_CmdStatusConst;
	SirenCmdSTR.STR.CRC16   = Get_Crc16(SirenCmdSTR.BLK, (SirenCmdSTR.STR.Size - 2));
}
//-----------------------------------------------------------------------------
void SirenSet(uint8_t siren, uint8_t cmd){
	
  SirenHeaderTxBuf();
  SirenCmdSTR.STR.Size      = 18;
	SirenCmdSTR.STR.CmdCode   = SB_CmdControlConst;
	//-------------------------
	switch(siren){
		//-----------
		case(Siren1): 
			SirenCmdSTR.STR.Siren1Status = (uint16_t)cmd;
		break;
		//-----------
		case(Siren2):
			SirenCmdSTR.STR.Siren2Status = (uint16_t)cmd;
		break;
		//-----------
		case(Siren3):
			SirenCmdSTR.STR.Siren3Status = (uint16_t)cmd;
		break;
		//-----------
		case(SirenAll):
			SirenCmdSTR.STR.Siren1Status = (uint16_t)cmd;
			SirenCmdSTR.STR.Siren2Status = (uint16_t)cmd;
			SirenCmdSTR.STR.Siren3Status = (uint16_t)cmd;		
		break;
		//-----------		
		default:
			//SirenCmdSTR.STR.CmdCode	     = SB_CmdStatusConst;
			SirenCmdSTR.STR.Siren1Status = SirenOutOFF;
			SirenCmdSTR.STR.Siren2Status = SirenOutOFF;
			SirenCmdSTR.STR.Siren3Status = SirenOutOFF;		
		break;
		//-----------		
	}
	//-------------------------
	SirenCmdSTR.STR.CRC16 = Get_Crc16(SirenCmdSTR.BLK, (SirenCmdSTR.STR.Size - 2));
}
//-----------------------------------------------------------------------------
SBDataPack_t* SirenRS485TxBuf(void){

	return &SirenCmdSTR;
}
//-----------------------------------------------------------------------------
