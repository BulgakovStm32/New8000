
#include "SwTimers.h"

//-----------------------------------------------------------------------------
static volatile uint16_t SwTimers[MaxSwTimersConst];
static volatile uint16_t GSwTimers[MaxSwTimersConst];
static volatile uint8_t  GSwTimersState[MaxSwTimersConst];
//-----------------------------------------------------------------------------
void SwTimersLoop(void){
	
	uint8_t i;
	//--------------------
	for(i=0; i < MaxSwTimersConst; i++)
		{
			SwTimers[i]++;
			if(GSwTimersState[i] == SwTimerRUN) GSwTimers[i]++;
		}	
}
//-----------------------------------------------------------------------------
void SwTimersInit(void){
	
	uint8_t i;
	//--------------------
	for(i=0; i < MaxSwTimersConst; i++) 
		{
			SwTimers[i]       = 0;
			GSwTimers[i]      = 0;
			GSwTimersState[i] = SwTimerSTOP;
		}
}
//-----------------------------------------------------------------------------
uint16_t SwTimerGet(uint8_t timer){
	
	return SwTimers[timer];
}
//-----------------------------------------------------------------------------
void SwTimerClr(uint8_t timer){
	
	SwTimers[timer] = 0;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
uint16_t GlobSwTimerGet(uint8_t timer){
	
	return GSwTimers[timer];
}
//-----------------------------------------------------------------------------
void GlobSwTimerStop(uint8_t timer){
	
	GSwTimersState[timer] = SwTimerSTOP;
}
//-----------------------------------------------------------------------------
void GlobSwTimerStart(uint8_t timer){
	
	if(GSwTimersState[timer] == SwTimerSTOP)
		{
			GSwTimers[timer] = 0;
			GSwTimersState[timer] = SwTimerRUN;
		}
}
//-----------------------------------------------------------------------------
void GlobSwTimerPause(uint8_t timer){
	
	if(GSwTimersState[timer] == SwTimerRUN)
		{
			GSwTimersState[timer] = SwTimerPAUSE;
		}
}
//-----------------------------------------------------------------------------
void GlobSwTimerContinue(uint8_t timer){
	
	if(GSwTimersState[timer] == SwTimerPAUSE)
		{
			GSwTimersState[timer] = SwTimerRUN;
		}
}
//-----------------------------------------------------------------------------
