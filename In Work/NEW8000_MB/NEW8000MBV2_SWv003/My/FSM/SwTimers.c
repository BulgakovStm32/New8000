
#include "SwTimers.h"

//-----------------------------------------------------------------------------
static volatile uint16_t SwTimers[MaxSwTimersConst];
static volatile uint16_t GSwTimers[MaxSwTimersConst];
static volatile uint8_t  GSwTimersState[MaxSwTimersConst];
//-----------------------------------------------------------------------------
void SwTimers_Loop(void){
	
	uint8_t i;
	//--------------------
	for(i=0; i < MaxSwTimersConst; i++)
		{
			SwTimers[i]++;
			if(GSwTimersState[i] == SwTimerRUN) GSwTimers[i]++;
		}	
}
//-----------------------------------------------------------------------------
void SwTimers_Init(void){
	
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
uint16_t SwTimers_Get(uint8_t timer){
	
	return SwTimers[timer];
}
//-----------------------------------------------------------------------------
void SwTimers_Clr(uint8_t timer){
	
	SwTimers[timer] = 0;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
uint16_t GlobSwTimer_Get(uint8_t timer){
	
	return GSwTimers[timer];
}
//-----------------------------------------------------------------------------
void GlobSwTimer_Stop(uint8_t timer){
	
	GSwTimersState[timer] = SwTimerSTOP;
}
//-----------------------------------------------------------------------------
void GlobSwTimer_Start(uint8_t timer){
	
	if(GSwTimersState[timer] == SwTimerSTOP)
		{
			GSwTimers[timer] = 0;
			GSwTimersState[timer] = SwTimerRUN;
		}
}
//-----------------------------------------------------------------------------
void GlobSwTimer_Pause(uint8_t timer){
	
	if(GSwTimersState[timer] == SwTimerRUN)
		{
			GSwTimersState[timer] = SwTimerPAUSE;
		}
}
//-----------------------------------------------------------------------------
void GlobSwTimer_Continue(uint8_t timer){
	
	if(GSwTimersState[timer] == SwTimerPAUSE)
		{
			GSwTimersState[timer] = SwTimerRUN;
		}
}
//-----------------------------------------------------------------------------
