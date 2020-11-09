
#include "Faults.h"

//-----------------------------------------------------------------------------
static Faults_t FaultsRegStr;
//-----------------------------------------------------------------------------
//Обновление светодиодов.
void Faults_Loop(void){
	
				 uint32_t faultsReg    = 0;
	static uint32_t faultsOldReg = 0;
  //--------------------
  //Неисправность МИК.Пр.
  if(MicState()== MIC_NOT_CONNECT || MicState()== MIC_FAULT) faultsReg |= FaultMic_Flag;
	//--------------------- 
  //Неисправность "ПИТАНИЕ"
  if(Power()->State.bits.AC == POWER_AC_FAULT) faultsReg |= FaultACNo_Flag;   //Отсутствует основное питание.
  if(Power()->State.bits.DC == POWER_DC_FAULT) faultsReg |= FaultDCFault_Flag;//Неисправен инвертор.
  //--------------------
  //Неисправность "АКБ" 
  if(Power()->State.bits.Bat >= BAT_ATTENTION) faultsReg |= FaultBat_Flag;
  //--------------------
  //Неисправность "УМЗЧ" 
  if(Amp_GetState() == PAMP_PROT) faultsReg |= FaultPA_Flag;
  //--------------------
  //Неисправность "НЕИСП.Л.О."
  if(SpLine_GetOutStateForLed(Line1) >= LineBreak) faultsReg |= FaultLc1Line_Flag;
	if(SpLine_GetOutStateForLed(Att1)  >= LineBreak) faultsReg |= FaultLc1Att_Flag;
  if(SpLine_GetOutStateForLed(Line2) >= LineBreak) faultsReg |= FaultLc2Line_Flag;
	if(SpLine_GetOutStateForLed(Att2)  >= LineBreak) faultsReg |= FaultLc2Att_Flag;
  //--------------------
  //Неисправность "НЕИСП.П.Ш." 
  if(FireLine_GetLineState(Poj1) >= FR_LINE_SHORT) faultsReg |= FaultPoj1_Flag;
	if(FireLine_GetLineState(Poj2) >= FR_LINE_SHORT) faultsReg |= FaultPoj2_Flag;
	if(FireLine_GetLineState(Poj3) >= FR_LINE_SHORT) faultsReg |= FaultPoj3_Flag;
	if(FireLine_GetLineState(ChS)  >= FR_LINE_SHORT) faultsReg |= FaultChS_Flag;
	//--------------------
  //Неисправность линий связи с табличками ВЫХОД.
  if(Siren()->Siren1.LineState >= SIREN_LINE_BREAK) faultsReg |= FaultSiren1_Flag;	
  if(Siren()->Siren2.LineState >= SIREN_LINE_BREAK) faultsReg |= FaultSiren2_Flag;	
  if(Siren()->Siren3.LineState >= SIREN_LINE_BREAK) faultsReg |= FaultSiren3_Flag;	
  //---------------------
	FaultsRegStr.Instant = faultsReg;														  //Фиксвция действующих неисправностей
	FaultsRegStr.Rise    = (faultsOldReg ^ faultsReg) & faultsReg;//Фиксация установки флага.
	FaultsRegStr.Fall    = (~faultsReg) & faultsOldReg;					  //Фиксация сброса флага.
	faultsOldReg         = faultsReg;
}
//-----------------------------------------------------------------------------
uint8_t Faults_CompareAllLinesWith(void){

	return 0;
}
//-----------------------------------------------------------------------------
Faults_t* Faults(void){

	return &FaultsRegStr;
}
//-----------------------------------------------------------------------------



























