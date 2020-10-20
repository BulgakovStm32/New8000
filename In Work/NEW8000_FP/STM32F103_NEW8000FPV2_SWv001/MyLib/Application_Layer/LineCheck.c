
#include "LineCheck.h"

//-----------------------------------------------------------------------------
static uint8_t  Lc1Line = 0; //Хранение состояния линий
static uint8_t  Lc1Att  = 0;
static uint8_t  Lc2Line = 0;
static uint8_t  Lc2Att  = 0;

static uint8_t  Lc1LineTemp = 0;
static uint8_t  Lc1AttTemp  = 0;
static uint8_t  Lc2LineTemp = 0;
static uint8_t  Lc2AttTemp  = 0;
static uint16_t LcDeviationVolueTemp = 0;
//Состояния линий пожарных шлейфов.
static uint8_t Poj1Line = 0;
static uint8_t Poj2Line = 0;
static uint8_t Poj3Line = 0;
static uint8_t ChSLine  = 0;
//Состояния основного и резервного питания.
static uint8_t Power    = 0;
//Состояние батареи.
static uint8_t Bat      = 0;
//Состояние УМЗЧ.
static uint8_t Amp      = 0;
//-----------------------------------------------------------------------------
//
void LcFpSet(uint8_t* p){

  Lc1Line = (*p & LineMask);
  if(Lc1Line != LineUndef) Lc1LineTemp = Lc1Line;  

  Lc1Att  = (*(p+1) & LineMask);
  if(Lc1Att != LineUndef)  Lc1AttTemp = Lc1Att;

  Lc2Line = (*(p+2) & LineMask);
  if(Lc2Line != LineUndef) Lc2LineTemp = Lc2Line;

  Lc2Att  = (*(p+3) & LineMask);
  if(Lc2Att != LineUndef)  Lc2AttTemp = Lc2Att;
}
//-----------------------------------------------------------------------------
//
uint8_t LcFpGetForTxt(uint8_t ch){

  if(ch == Line1) return Lc1Line;
  if(ch == Att1)  return Lc1Att;
  if(ch == Line2) return Lc2Line;
  if(ch == Att2)  return Lc2Att;
  return LineUndef;
}
//-----------------------------------------------------------------------------
//
uint8_t LcFpGetForLed(uint8_t ch){
  
  if(ch == Line1) return Lc1LineTemp;
  if(ch == Att1)  return Lc1AttTemp;
  if(ch == Line2) return Lc2LineTemp;
  if(ch == Att2)  return Lc2AttTemp;
  return LineUndef;
}
//-----------------------------------------------------------------------------
void LcDeviationSet(uint8_t* p){

  LcDeviationVolueTemp = (uint16_t)(((*(p+1))<<8) | (*p));
}
//-----------------------------------------------------------------------------
uint16_t LcDeviationGet(void){

  return LcDeviationVolueTemp;
}
//-----------------------------------------------------------------------------
//
void PojLineFpSet(uint8_t* p){

  Poj1Line = *p;
  Poj2Line = *(p+1);
  Poj3Line = *(p+2);
  ChSLine  = *(p+3);
}
//-----------------------------------------------------------------------------
//
uint8_t PojLineFpGet(uint8_t ch){

  if(ch == Poj1) return Poj1Line;
  if(ch == Poj2) return Poj2Line;
  if(ch == Poj3) return Poj3Line;
  if(ch == ChS)  return ChSLine;
  return BreakLineSost;
}
//-----------------------------------------------------------------------------
//
void PowerAndBatFpSet(uint8_t* p){

  Power = (*p)   & PowerMask;
  Bat   = (*p+1) & BatMask;
}
//-----------------------------------------------------------------------------
uint8_t PowerFpGet(void){
  
  return Power;
}
//-----------------------------------------------------------------------------
uint8_t BatFpGet(void){
  
  return Bat;
}
//-----------------------------------------------------------------------------
//
void AmpFpSet(uint8_t* p){

  Amp = *p;
}
//-----------------------------------------------------------------------------
uint8_t AmpFpGet(void){
  
  return Amp;
}
//-----------------------------------------------------------------------------























