
#ifndef _Faults_H
#define _Faults_H
//*****************************************************************************

#include "stm32f10x.h"

//--------------------
//AppLayer.
#include "RS485.h"

//--------------------
//Boards.
#include "MB_MotherBoard.h"
#include "SB_SirenBoard.h"
#include "FP_FacePanel.h"
//*****************************************************************************
typedef struct{
	
	uint32_t Instant;//Фиксация действующих неисправностей.
	uint32_t Rise;	 //Фиксация установки флага.
	uint32_t Fall;   //Фиксация сброса флага.

}FaultReg_t;
//-------------------------------
//Неисправности
#define FireLine1FaultFlag (1<<0) 
#define FireLine2FaultFlag (1<<1) 
#define FireLine3FaultFlag (1<<2) 
#define ChSLineFaultFlag   (1<<3) 
#define SpLine1FaultFlag   (1<<4) 
#define SpAtten1FaultFlag  (1<<5) 
#define SpLine2FaultFlag   (1<<6) 
#define SpAtten2FaultFlag  (1<<7) 
#define ACFaultFlag        (1<<8)
#define DCFaultFlag        (1<<9)  
#define BatFaultFlag       (1<<10)
#define PAFaultFlag        (1<<11)
#define MicFaultFlag       (1<<12)
#define ConnectFaultFlag   (1<<13)
#define Siren1FaultFlag    (1<<14)
#define Siren2FaultFlag    (1<<15)
#define Siren3FaultFlag    (1<<16)

//*****************************************************************************
void        FaultsLoop(void);
FaultReg_t* Faults(void);
//*****************************************************************************
#endif /*_Faults_H*/


