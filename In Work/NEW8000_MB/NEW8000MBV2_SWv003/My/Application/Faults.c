
#include "Faults.h"

//*****************************************************************************
static FaultReg_t FaultRegStr;
//*****************************************************************************
//*****************************************************************************
//Анализ неисправностей блока.
void FaultsLoop(void){
	
				 uint32_t faultReg    = 0;
	static uint32_t faultOldReg = 0;
  //---------------------  
	if(Power()->ACState  != POWER_AC_OK) 	 faultReg |= ACFaultFlag; //Отсутствует основное питание.
	if(Power()->DCState  != POWER_DC_OK) 	 faultReg |= DCFaultFlag; //Неисправен инвертор.
  if(Power()->BatState >= BAT_ATTENTION) faultReg |= BatFaultFlag;//глубокий разряд или отсутствие АКБ.
  if(PAmp_GetStat()    == PAMP_PROT)   	 faultReg |= PAFaultFlag; //УМЗЧ в защите.
	if(RS485_Flags()->NoConnect)		 		   faultReg |= ConnectFaultFlag;
	//---------------------
  //Пожарные шлейфы.
	if(FireLine(POJ_1)->FaultState >= FIRE_LINES_SHORT) faultReg |= FireLine1FaultFlag;
  if(FireLine(POJ_2)->FaultState >= FIRE_LINES_SHORT) faultReg |= FireLine2FaultFlag;
  if(FireLine(POJ_3)->FaultState >= FIRE_LINES_SHORT) faultReg |= FireLine3FaultFlag;
  if(FireLine(ChS)->FaultState   >= FIRE_LINES_SHORT) faultReg |= ChSLineFaultFlag;
	//---------------------
  //Линии Гр.
  if(SpeakerLine_GetDefStatus(SP_LINE_1)    >= SP_LINE_BREAK) faultReg |= SpLine1FaultFlag;  
	if(SpeakerLine_GetDefStatus(ATTEN_LINE_1) >= SP_LINE_BREAK) faultReg |= SpAtten1FaultFlag;
	if(SpeakerLine_GetDefStatus(SP_LINE_2)    >= SP_LINE_BREAK) faultReg |= SpLine2FaultFlag;
	if(SpeakerLine_GetDefStatus(ATTEN_LINE_2) >= SP_LINE_BREAK) faultReg |= SpAtten2FaultFlag;
	//---------------------
  //Линии связи с табличками ВЫХОД.
	if((SirenBoard()->Siren1Status & SIREN_LINE_MASK) != SIREN_LINE_NORM)faultReg |= Siren1FaultFlag;  
	if((SirenBoard()->Siren2Status & SIREN_LINE_MASK) != SIREN_LINE_NORM)faultReg |= Siren2FaultFlag; 
	if((SirenBoard()->Siren3Status & SIREN_LINE_MASK) != SIREN_LINE_NORM)faultReg |= Siren3FaultFlag; 
  //---------------------
	FaultRegStr.Instant = faultReg;														//Фиксация действующих неисправностей.
	FaultRegStr.Rise    = (faultOldReg ^ faultReg) & faultReg;//Фиксация установки флага.
	FaultRegStr.Fall    = (~faultReg) & faultOldReg;					//Фиксация сброса флага.
	faultOldReg         = faultReg;
}
//*****************************************************************************
FaultReg_t* Faults(void){

	return &FaultRegStr;
}
//*****************************************************************************
//*****************************************************************************



























