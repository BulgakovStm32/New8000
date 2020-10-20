
#ifndef _FsmMessages_H
#define _FsmMessages_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

//-----------------------------------------------------------------------------
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct {
	uint8_t Msg;
	void   *ParamPtr;
}FsmMessageSTR_Type;
#pragma pack(pop)//вернули предыдущую настройку.
//-----------------------------------------
//#define USE_BROADCAST_MESSAGES

#define MAX_MESSAGES 			4

//Идентификаторы сообщений.
#define MSG_LCD					0
#define MSG_LED					1
#define MSG_UART				2
#define MSG_BUTTON			3


#ifdef USE_BROADCAST_MESSAGES
	#define MAX_BROADCAST_MESSAGES 	4

	#define B_MSG_1					0
	#define B_MSG_2					1
	#define B_MSG_3					2
	#define B_MSG_4					3
#endif
//-----------------------------------------------------------------------------
void     FsmMessages_Init         (void);
void     FsmMessages_Process      (void);
void     FsmMessages_Send         (uint8_t Msg);
void     FsmMessages_SendWithParam(uint8_t Msg, uint8_t *ParamPtr);
uint8_t  FsmMessages_Get          (uint8_t Msg);
uint8_t *FsmMessages_GetWithParam (uint8_t Msg);
void 		 FsmMessages_SendBroadcast(uint8_t Msg);
uint8_t  FsmMessages_GetBroadcast (uint8_t Msg);
//-----------------------------------------------------------------------------
#endif /*_FsmMessages_H*/

