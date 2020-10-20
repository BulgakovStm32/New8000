
#include "FP_FacePanel.h"

//********************************************************************************************
//********************************************************************************************
//Данные лицевой панели

FPDataSTR_Type FPDataSTR;
//-----------------------------------------------------------------------------
FPDataSTR_Type* FacePanel(void){

	return &FPDataSTR;
}
//********************************************************************************************
//********************************************************************************************
//Работа с журналом событий.

FPLogPackUNION_Type FPLogPackUNION;
//-----------------------------------------------------------------------------
void FacePanelGetLog(uint16_t numEvent){
	
	FPLogPackUNION.STR.SinhroByte1 = SinhroByte1Const;
	FPLogPackUNION.STR.SinhroByte2 = SinhroByte2Const;
	FPLogPackUNION.STR.Type				 = FP_TypeConst;
	FPLogPackUNION.STR.Size				 = (uint8_t)sizeof(FPLogPackUNION);
	FPLogPackUNION.STR.Cmd_Code		 = CmdFPGetLogConst;
	//----------
	//FPLogPackUNION.STR.TotalSaveEvents =  LogGetTotalEventsSave();//Общее количество событий в журнале
	//FPLogPackUNION.STR.NotReadEvents	 =  LogGetNotReadEvents();
	FPLogPackUNION.STR.OneEvent		     = *LogGetEvent(numEvent);
	//----------
	FPLogPackUNION.STR.CRC16 					 = Get_Crc16(FPLogPackUNION.BLK, (FPLogPackUNION.STR.Size - 2));
}
//-----------------------------------------------------------------------------
FPLogPackUNION_Type* FacePanelTxLogPtr(void){

	return &FPLogPackUNION;
}
//********************************************************************************************
//********************************************************************************************
//Работа с конфигурированием логических входов.

FPFireLineContextPackUNION_Type FireLineContextUNION;
//-----------------------------------------------------------------------------
void FacePanelGetPojLineContext(uint8_t input){
	
	FireLineContextUNION.STR.SinhroByte1 = SinhroByte1Const;
	FireLineContextUNION.STR.SinhroByte2 = SinhroByte2Const;
	FireLineContextUNION.STR.Type				 = FP_TypeConst;
	FireLineContextUNION.STR.Size				 = (uint8_t)sizeof(FireLineContextUNION);
	FireLineContextUNION.STR.Cmd_Code		 = CmdFPGetInputConfigConst;
	//----------
	FireLineContextUNION.STR.FireLineNumber				 = input;
	FireLineContextUNION.STR.FireLineType          = FireLine(input)->Type;
	FireLineContextUNION.STR.FireLineTimeOut			 = FireLine(input)->TimeOut;
	FireLineContextUNION.STR.FireLinePhysicalState = FireLine(input)->PhysicalState;
	FireLineContextUNION.STR.FireLineMeasValue		 = FireLine(input)->MeasValue;
	//----------
	FireLineContextUNION.STR.CRC16 = Get_Crc16(FireLineContextUNION.BLK, (FireLineContextUNION.STR.Size - 2));
}
//-----------------------------------------------------------------------------
FPFireLineContextPackUNION_Type* FacePanelTxPojLineContextPtr(void){

	return &FireLineContextUNION;
}
//********************************************************************************************
//********************************************************************************************
//Передача информации о микросхеме EEPROM в лицевую панель.

FPResponse_t FPResponse;
//-----------------------------------------------------------------------------
FPResponse_t* FacePanelGetEepInfo(void){
	
	FPResponse.SinhroByte1 = SinhroByte1Const;
	FPResponse.SinhroByte2 = SinhroByte2Const;
	FPResponse.Type				 = FP_TypeConst;
	FPResponse.Size				 = (uint8_t)(sizeof(FPResponse.Data.Eep) + 4);
	//----------
	FPResponse.Data.Eep.Cmd_Code = CmdFPGetEepromConst;
	
	FPResponse.Data.Eep.Info.StatReg    = 123;
	FPResponse.Data.Eep.Info.WriteCount = 4567;
	FPResponse.Data.Eep.Info.ReadCount  = 8901;
	
	FPResponse.Data.Eep.CRC16 = Get_Crc16((uint8_t*)&FPResponse, (FPResponse.Size - 2));
	//----------
	return &FPResponse;
}
//********************************************************************************************
//********************************************************************************************

















