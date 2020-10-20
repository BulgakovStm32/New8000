
#ifndef _Jq6500_H
#define _Jq6500_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "uart_ST.h"

//-----------------------------------------------------------------------------
//Команды.
#define JqCmdNop         0
#define JqCmdChipReset   1
#define JqCmdPlay        2
#define JqCmdPause       3
#define JqCmdSleepMode   4
#define JqCmdNext        5
#define JqCmdPre         6
#define JqCmdVolMax      7
#define JqCmdVolUp       8
#define JqCmdVolDown     9
#define JqCmdLoop        10
#define JqCmdFolderSw    11

//Ответы от модуля 
#define JqNoResp         0
#define JqRespOK         1

//Управление сообщениями
#define	PlayMessage		 1
#define	StopMessage		 2
#define	ReplayMessage	 3
#define	PlayMessage1	 4
#define	PlayMessage2	 5
//-----------------------------------------------------------------------------
void JQInit  (void);
void JQTxCmd (uint8_t cmd, uint16_t value);
void JQReset (void);
void JQRxResp(uint8_t data);//неотлажено!!!


void VoiceMessage(uint8_t cmd);//Управление сообщениями.
//-----------------------------------------------------------------------------
#endif //_Jq6500_H


