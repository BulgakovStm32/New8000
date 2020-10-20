
#ifndef _Jq6500_H
#define _Jq6500_H
//*****************************************************************************

#include "stm32f10x.h"

#include "uart_ST.h"

//*****************************************************************************
//*****************************************************************************
//Команды.
typedef enum{
	JQ_CMD_NOP = 0,
	JQ_CMD_CHIP_RESET,
	JQ_CMD_PLAY,
	JQ_CMD_PAUSE,
	JQ_CMD_SLEEP_MODE,
	JQ_CMD_NEXT,
	JQ_CMD_PRE,
	JQ_CMD_VOL_MAX,
	JQ_CMD_VOL_UP,
	JQ_CMD_VOL_DOWN,
	JQ_CMD_LOOP,
	JQ_CMD_FOLDER_SW,
}Jq6500CmdEnum_t;
//Ответы от модуля 
#define JqNoResp         0
#define JqRespOK         1
//************************************
//Управление сообщениями
typedef enum{
	MESSAGE_NOP = 0,
	PLAY_MESSAGE,
	STOP_MESSAGE,
	REPLAY_MESSAGE,
	PLAY_MESSAGE_1,
	PLAY_MESSAGE_2,
}VoiceMessageCmdEnum_t;
//*****************************************************************************
//*****************************************************************************
void VoiceMessage_Init(void);
void VoiceMessage_Cmd(VoiceMessageCmdEnum_t cmd);
void VoiceMessage_UpdateLoop(void);//Управление сообщениями.
//*****************************************************************************
//*****************************************************************************
#endif //_Jq6500_H


