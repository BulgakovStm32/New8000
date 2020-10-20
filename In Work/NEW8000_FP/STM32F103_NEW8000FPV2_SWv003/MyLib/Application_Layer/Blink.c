
#include "Blink.h"

//-----------------------------------------------------------------------------
static  volatile uint8_t BlinkReg = 0;
uint8_t volatile        *BlinkPtr = &BlinkReg;
//-----------------------------------------------------------------------------
//Процесс для мигания светодиодами.
void BlinkLoop(void){
	
	static uint16_t BlinkCount = 0;
  //--------------------	
	++BlinkCount;
	if((BlinkCount % 100)  == 0) BlinkReg ^= (1<<BlinkInterval100mS);
	if((BlinkCount % 250)  == 0) BlinkReg ^= (1<<BlinkInterval250mS);	
	if((BlinkCount % 500)  == 0) BlinkReg ^= (1<<BlinkInterval500mS);
	if((BlinkCount % 1000) == 0) 
		{
			BlinkCount = 0;
			BlinkReg  ^= (1<<BlinkInterval1000mS);
		}
}
//-----------------------------------------------------------------------------
uint16_t Blink(uint8_t interval){
	
	if(*BlinkPtr & (1 << interval)) return 0xFFFF;
	else											      return 0;
}
//-----------------------------------------------------------------------------


















