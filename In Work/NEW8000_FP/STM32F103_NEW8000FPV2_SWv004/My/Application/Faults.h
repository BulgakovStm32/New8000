
#ifndef _Faults_H
#define _Faults_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "gpio_ST.h"
//--------------------
//Boards.
#include "MB_MotherBoard.h"
#include "SB_SirenBoard.h"
//-----------------------------------------------------------------------------
//#pragma pack(push, 1)
typedef struct{
	uint32_t Instant;
	uint32_t Rise;
  uint32_t Fall;
}Faults_t;
//#pragma pack(pop)
//-------------------------------
//Неисправности
#define FaultPoj1_Flag     (1<<0) 
#define FaultPoj2_Flag     (1<<1) 
#define FaultPoj3_Flag     (1<<2) 
#define FaultChS_Flag      (1<<3) 
#define FaultLc1Line_Flag  (1<<4) 
#define FaultLc1Att_Flag   (1<<5) 
#define FaultLc2Line_Flag  (1<<6) 
#define FaultLc2Att_Flag   (1<<7) 
#define FaultACNo_Flag     (1<<8)
#define FaultDCFault_Flag  (1<<9)  
#define FaultBat_Flag      (1<<10)
#define FaultPA_Flag       (1<<11)
#define FaultMic_Flag      (1<<12)
#define FaultConnect_Flag  (1<<13)
#define FaultSiren1_Flag   (1<<14)
#define FaultSiren2_Flag   (1<<15)
#define FaultSiren3_Flag   (1<<16)


#define FAULTS_MASK  ((FaultPoj1_Flag)\
										 |(FaultPoj2_Flag)\
										 |(FaultPoj3_Flag)\
										 |(FaultChS_Flag)\
										 |(FaultLc1Line_Flag)\
										 |(FaultLc1Att_Flag)\
										 |(FaultLc2Line_Flag)\
										 |(FaultLc2Att_Flag)\
										 |(FaultSiren1_Flag)\
										 |(FaultSiren2_Flag)\
										 |(FaultSiren3_Flag))

//-----------------------------------------------------------------------------
void      Faults_Loop(void);
uint8_t   Faults_CompareAllLinesWith(void);
Faults_t* Faults(void);
//-----------------------------------------------------------------------------
#endif /*_Faults_H*/


