
#ifndef _main_H
#define _main_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "sys_ST.h"
#include "gpio_ST.h"
#include "spi_ST.h"
#include "sys_tick_ST.h"
#include "timers_ST.h"
#include "rtc_ST.h"
#include "crc16.h"

//#include "uart_ST.h"
//#include "lcd12864.h"
//#include "sm1628.h"
//--------------------
//OS.
#include "RTOS.h"
//--------------------
//AppLayer.
#include "Rs485.h"
#include "Rtc.h"
#include "MenuLcd12864.h"
#include "Buttons.h"
#include "Led.h"
#include "Blink.h"
#include "Faults.h"

#include "SwTimers.h"
#include "FsmMessage.h"
//--------------------
//Boards.
#include "MB_MotherBoard.h"
#include "SB_SirenBoard.h"
//-----------------------------------------------------------------------------
#endif  //_main_H
