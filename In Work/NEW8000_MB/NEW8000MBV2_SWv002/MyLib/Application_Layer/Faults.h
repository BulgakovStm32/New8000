
#ifndef _Faults_H
#define _Faults_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "gpio_ST.h"
//--------------------
//Boards.
#include "MB_MotherBoard.h"
#include "SB_SirenBoard.h"
#include "FP_FacePanel.h"
//-----------------------------------------------------------------------------
typedef struct{
	
	uint32_t Instant;//Фиксация действующих неисправностей.
	uint32_t Rise;	 //Фиксация установки флага.
  uint32_t Fall;   //Фиксация сброса флага.

}FaultRegSTR_Type;
//-------------------------------
//Неисправности
#define FireLine1Fault (1<<0) 
#define FireLine2Fault (1<<1) 
#define FireLine3Fault (1<<2) 
#define ChSLineFault   (1<<3) 
#define SpLine1Fault   (1<<4) 
#define SpAtten1Fault  (1<<5) 
#define SpLine2Fault   (1<<6) 
#define SpAtten2Fault  (1<<7) 
#define ACFault        (1<<8)
#define DCFault        (1<<9)  
#define BatFault       (1<<10)
#define PAFault        (1<<11)
#define MicFault       (1<<12)
#define ConnectFault   (1<<13)
#define Siren1Fault    (1<<14)
#define Siren2Fault    (1<<15)
#define Siren3Fault    (1<<16)

//-----------------------------------------------------------------------------
void              FaultsLoop(void);
FaultRegSTR_Type* Faults(void);

//-----------------------------------------------------------------------------
#endif /*_Faults_H*/


