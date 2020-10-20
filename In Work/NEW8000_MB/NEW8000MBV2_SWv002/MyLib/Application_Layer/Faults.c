
#include "Faults.h"

//-----------------------------------------------------------------------------
static FaultRegSTR_Type FaultRegSTR;
//-----------------------------------------------------------------------------
//Анализ неисправностей блока.
void FaultsLoop(void){
	
				 volatile uint32_t faultReg    = 0;
	static volatile uint32_t faultOldReg = 0;
	//---------------------
//	FaultRegSTR.Rise = 0;
//  FaultRegSTR.Fall = 0;
  //---------------------  
//	if(Power()->PowerState == PowerACNo)       faultReg |= ACFault; //Отсутствует основное питание.
//	if(Power()->PowerState == PowerDCFault)    faultReg |= DCFault; //Неисправен инвертор.
//  if(Power()->BatState   >= BatAttention)    faultReg |= BatFault;//нет АКБ.
//  if(AmpGetStat()        == AmpProt)         faultReg |= PAFault; //УМЗЧ в защите.
//  if(FacePanel()->Mic >= MicNotConnectConst) faultReg |= MicFault;//МИК. не подключен или неисправен.
	//---------------------
  //Линии Гр.
//  if(SpeakerLineGetDefStatus(SpLine1)  >= LineBreak) faultReg |= SpLine1Fault;  
//	if(SpeakerLineGetDefStatus(SpAtten1) >= LineBreak) faultReg |= SpAtten1Fault;
//	if(SpeakerLineGetDefStatus(SpLine2)  >= LineBreak) faultReg |= SpLine2Fault;
//	if(SpeakerLineGetDefStatus(SpAtten2) >= LineBreak) faultReg |= SpAtten2Fault;
	//---------------------
  //Пожарные шлейфы.
//	if(FireLine(Poj1)->FaultState >= FireLineShort) faultReg |= FireLine1Fault;
//  if(FireLine(Poj2)->FaultState >= FireLineShort) faultReg |= FireLine2Fault;
//  if(FireLine(Poj3)->FaultState >= FireLineShort) faultReg |= FireLine3Fault;
//  if(FireLine(ChS)->FaultState  >= FireLineShort) faultReg |= ChSLineFault;
  //---------------------
	FaultRegSTR.Instant = faultReg;														//Фиксация действующих неисправностей.
	FaultRegSTR.Rise    = (faultOldReg ^ faultReg) & faultReg;//Фиксация установки флага.
	FaultRegSTR.Fall    = (~faultReg) & faultOldReg;					//Фиксация сброса флага.
	faultOldReg         = faultReg;
	//---------------------
	//ОТЛАДКА
//	faultReg = 0x0001;
//	
//	FaultRiseReg = (FaultOldReg ^ faultReg) & faultReg;
//	//FaultOldReg  = faultReg;
//	FaultFallReg = (~faultReg) & FaultOldReg;
//	FaultOldReg  = faultReg;	
//	
//	FaultRiseReg = 0;
//  FaultFallReg = 0;
//	
//	//
//	faultReg = 0x0100;
//	
//	FaultRiseReg = (FaultOldReg ^ faultReg) & faultReg;
//	//FaultOldReg  = faultReg;
//	FaultFallReg = (~faultReg) & FaultOldReg;
//	FaultOldReg  = faultReg;
//	
//	FaultRiseReg = 0;
//  FaultFallReg = 0;

//	//
//	faultReg = 0x0100;
//	
//	FaultRiseReg = (FaultOldReg ^ faultReg) & faultReg;
//	//FaultOldReg  = faultReg;
//	FaultFallReg = (~faultReg) & FaultOldReg;
//	FaultOldReg  = faultReg;
//	
//	FaultRiseReg = 0;
//  FaultFallReg = 0;
}
//-----------------------------------------------------------------------------
FaultRegSTR_Type* Faults(void){

	return &FaultRegSTR;
}
//-----------------------------------------------------------------------------
void FaultsFsm(void){

//	static uint32_t oldFaults = 0;
	//--------------------- 


}
//-----------------------------------------------------------------------------



























