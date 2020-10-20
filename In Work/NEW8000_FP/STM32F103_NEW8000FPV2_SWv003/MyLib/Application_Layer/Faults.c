
#include "Faults.h"

//-----------------------------------------------------------------------------
static Faults_t FaultsRegStr;
//-----------------------------------------------------------------------------
//Обновление светодиодов.
void FaultsLoop(void){
	
				 uint32_t faultsReg    = 0;
	static uint32_t faultsOldReg = 0;
	//--------------------
	//FaultRegSTR.Rise = 0;
  //FaultRegSTR.Fall = 0;
  //--------------------
  //Неисправность "ПИТАНИЕ"
       if(PowerDevice()->MainPower <= PowerDCOk)    faultsReg &= ~(FaultACNo_Flag | FaultDCFault_Flag);//Питание в норме.
  else if(PowerDevice()->MainPower == PowerACNo)    faultsReg |= FaultACNo_Flag;   					  //Отсутствует основное питание.
  else if(PowerDevice()->MainPower == PowerDCFault) faultsReg |= FaultDCFault_Flag;					  //Неисправен инвертор.
  //--------------------
  //Неисправность "АКБ" 
  if(PowerDevice()->Bat >= BatAttention) faultsReg |=  FaultBat_Flag;
  else                                   faultsReg &= ~FaultBat_Flag;
  //--------------------
  //Неисправность "УМЗЧ" 
  if(PowerDevice()->Amp == AmpProt) faultsReg |=  FaultPA_Flag;
  else  												    faultsReg &= ~FaultPA_Flag; 
  //--------------------
  //Неисправность "НЕИСП.Л.О."
  if(LcGetForLed(Line1) >= LineBreak) faultsReg |=  FaultLc1Line_Flag;
  else                                faultsReg &= ~FaultLc1Line_Flag;
  
	if(LcGetForLed(Att1)  >= LineBreak) faultsReg |=  FaultLc1Att_Flag;
  else                                faultsReg &= ~FaultLc1Att_Flag;

  if(LcGetForLed(Line2) >= LineBreak) faultsReg |=  FaultLc2Line_Flag;
  else                                faultsReg &= ~FaultLc2Line_Flag;
  
	if(LcGetForLed(Att2)  >= LineBreak) faultsReg |=  FaultLc2Att_Flag;
  else                                faultsReg &= ~FaultLc2Att_Flag;
  //--------------------
  //Неисправность "НЕИСП.П.Ш." 
  if(FireLineGetPhysState(Poj1) >= FireLineShort) faultsReg |=  FaultPoj1_Flag;
  else                                 		 				faultsReg &= ~FaultPoj1_Flag;
  
	if(FireLineGetPhysState(Poj2) >= FireLineShort) faultsReg |=  FaultPoj2_Flag;
  else                                 		 				faultsReg &= ~FaultPoj2_Flag;
  
	if(FireLineGetPhysState(Poj3) >= FireLineShort) faultsReg |=  FaultPoj3_Flag;
  else                                 		 				faultsReg &= ~FaultPoj3_Flag;
  
	if(FireLineGetPhysState(ChS) >= FireLineShort)  faultsReg |=  FaultChS_Flag;
  else                                 		 				faultsReg &= ~FaultChS_Flag;
  //--------------------- 
  //Неисправность МИК.Пр.
  if(MicGetState() >= MicNotConnectedConst) faultsReg |=  FaultMic_Flag;
  else                                      faultsReg &= ~FaultMic_Flag;
  //---------------------
	FaultsRegStr.Instant = faultsReg;														  //Фиксвция действующих неисправностей
	FaultsRegStr.Rise    = (faultsOldReg ^ faultsReg) & faultsReg;//Фиксация установки флага.
	FaultsRegStr.Fall    = (~faultsReg) & faultsOldReg;					  //Фиксация сброса флага.
	faultsOldReg         = faultsReg;
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
Faults_t* Faults(void){

	return &FaultsRegStr;
}
//-----------------------------------------------------------------------------



























