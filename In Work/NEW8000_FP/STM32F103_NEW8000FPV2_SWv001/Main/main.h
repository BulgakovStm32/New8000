
#ifndef _main_H
#define _main_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "sys_ST.h"
#include "gpio_ST.h"
#include "spi_ST.h"
#include "uart_ST.h"
#include "sys_tick_ST.h"
#include "timers_ST.h"
#include "rtc_ST.h"
#include "crc16.h"

//#include "lcd12864.h"
//#include "sm1628.h"
//--------------------
//OS.
#include "RTOS.h"
//--------------------
//AppLayer.
#include "LineCheck.h"
#include "Relay.h"
#include "Rtc.h"
#include "MenuLcd12864.h"
#include "Buttons.h"
#include "Led.h"
//-----------------------------------------------------------------------------

#define ErrorFacePanel	7
#define PRIORITY_6	    6
#define PRIORITY_5	    5
#define PRIORITY_4	    4
#define PRIORITY_3	    3
#define PRIORITY_2	    2
#define PRIORITY_1	    1

//-----------------------------------------------------------------------------
#endif  //_main_H
