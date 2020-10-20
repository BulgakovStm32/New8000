
#ifndef _main_H
#define _main_H
//*****************************************************************************
//Драйвера работы с переферией.
#include "sys_ST.h"
#include "gpio_ST.h"
#include "sys_tick_ST.h"
#include "adc_ST.h"
//--------------------
//Планировщик задач.
#include "Scheduler.h"
//--------------------
//FSM (машина состояний).
#include "SwTimers.h"
#include "FsmMessage.h"
//--------------------
//Приложения.
#include "Time.h"
#include "Faults.h"
#include "Blink.h"

#include "PojLine.h"
#include "LineCheck.h"
#include "Amplifier.h"
#include "AnalogSwitch.h"
#include "Jq6500.h"
#include "Rs485.h"
#include "Config.h"
#include "Zummer.h"
#include "Logging.h"
#include "Relay.h"
//--------------------
//Boards.
#include "MB_MotherBoard.h"
#include "FP_FacePanel.h"
#include "SB_SirenBoard.h"
//*****************************************************************************
//*****************************************************************************
#define TEST_TIME	10000u

//*****************************************************************************
//*****************************************************************************
#endif  //_main_H
