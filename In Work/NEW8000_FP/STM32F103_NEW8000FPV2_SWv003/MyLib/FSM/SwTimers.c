
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
			if(GSwTimersState[i] == TIMER_RUNNING) GSwTimers[i]++;
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
			GSwTimersState[i] = TIMER_STOPPED;
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
	
	GSwTimersState[timer] = TIMER_STOPPED;
}
//-----------------------------------------------------------------------------
void GlobSwTimerStart(uint8_t timer){
	
	if(GSwTimersState[timer] == TIMER_STOPPED)
		{
			GSwTimers[timer] = 0;
			GSwTimersState[timer] = TIMER_RUNNING;
		}
}
//-----------------------------------------------------------------------------
void GlobSwTimerPause(uint8_t timer){
	
	if(GSwTimersState[timer] == TIMER_RUNNING)
		{
			GSwTimersState[timer] = TIMER_PAUSED;
		}
}
//-----------------------------------------------------------------------------
void GlobSwTimerContinue(uint8_t timer){
	
	if(GSwTimersState[timer] == TIMER_PAUSED)
		{
			GSwTimersState[timer] = TIMER_RUNNING;
		}
}
//-----------------------------------------------------------------------------
