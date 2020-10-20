
#include "Blink.h"

//-----------------------------------------------------------------------------

static volatile uint16_t BlinkReg = 0;

//-----------------------------------------------------------------------------
//Процесс для мигания светодиодами.
void BlinkLoop(void){
	
	static uint16_t BlinkCount = 0;
  //--------------------
	++BlinkCount;
	if((BlinkCount % 50)   == 0) BlinkReg ^= (1 << Interval50mS);
	if((BlinkCount % 100)  == 0) BlinkReg ^= (1 << Interval100mS);
	if((BlinkCount % 250)  == 0) BlinkReg ^= (1 << Interval250mS);	
	if((BlinkCount % 500)  == 0) BlinkReg ^= (1 << Interval500mS);
	if((BlinkCount % 1000) == 0)
		{
			BlinkReg  ^= (1<<Interval1S);
			BlinkCount = 0;
		}
}
//-----------------------------------------------------------------------------
uint8_t Blink(enum BlinkIntervalEnum interval){
	
	return(BlinkReg & (1 << interval));
}
//-----------------------------------------------------------------------------
