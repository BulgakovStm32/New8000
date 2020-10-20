
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
#include "adc_ST.h"
#include "rtc_ST.h"
#include "crc16.h"
//--------------------
//OS.
#include "RTOS.h"
//--------------------
//FSM (машина состояний).
#include "SwTimers.h"
#include "FsmMessage.h"
//--------------------
//AppLayer.
//#include "Rs485.h"
#include "LineCheck.h"
#include "PojLine.h"
#include "Relay.h"
#include "Time.h"
#include "Eep.h"
#include "Jq6500.h"
#include "Amplifier.h"
#include "Blink.h"
#include "Logging.h"
#include "Faults.h"
#include "Config.h"
//--------------------
//Boards.
#include "MB_MotherBoard.h"
#include "FP_FacePanel.h"
#include "SB_SirenBoard.h"
//-----------------------------------------------------------------------------
//Режим работы блока.
//#define	Manual       1
//#define	Mic          2
//#define	AutoPoj1     3
//#define	AutoPoj2     4
//#define	AutoPoj3     5
//#define	AutoChS      6

//#define PRIORITY_6	    6
//#define PRIORITY_5	    5
//#define PRIORITY_4	    4
//#define PRIORITY_3	    3
//#define PRIORITY_2	    2
//#define PRIORITY_1	    1
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
#endif  //_main_H
