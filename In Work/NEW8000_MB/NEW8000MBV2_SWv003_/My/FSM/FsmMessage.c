
#include "FsmMessage.h"

//-----------------------------------------------------------------------------
volatile FsmMessageSTR_Type FsmMessageSTR[MAX_MESSAGES];
#ifdef USE_BROADCAST_MESSAGES
volatile uint8_t BroadcastMessages[MAX_BROADCAST_MESSAGES];
#endif
//-----------------------------------------------------------------------------
//начальная инициализация сообщений;
void FsmMessages_Init(void){
	
	uint8_t i;
	//--------------------
	for(i=0; i< MAX_MESSAGES; i++) FsmMessageSTR[i].Msg = 0;
#ifdef USE_BROADCAST_MESSAGES
	for(i=0; i<MAX_BROADCAST_MESSAGES; i++) BroadcastMessages[i] = 0;
#endif
}
//-----------------------------------------------------------------------------
//обработчик службы сообщений;
void FsmMessages_Process(void){
	
	uint8_t i;
	//--------------------
	for(i=0; i<MAX_MESSAGES; i++)
		{
			if (FsmMessageSTR[i].Msg == 1) FsmMessageSTR[i].Msg = 2;
			if (FsmMessageSTR[i].Msg == 2) FsmMessageSTR[i].Msg = 0;
		}
#ifdef USE_BROADCAST_MESSAGES
	for(i=0; i< MAX_BROADCAST_MESSAGES; i++)
		{
			if (BroadcastMessages[i] == 1) BroadcastMessages[i] = 2;
			if (BroadcastMessages[i] == 2) BroadcastMessages[i] = 0;
		}
#endif
}
//-----------------------------------------------------------------------------
//послать сообщение с параметром: адрес сообщения и указатель на параметр;
void FsmMessage_SendWithParam(uint8_t Msg, uint8_t *ParamPtr){
	
	if(FsmMessageSTR[Msg].Msg == 0)
		{
			FsmMessageSTR[Msg].Msg = 1;
			FsmMessageSTR[Msg].ParamPtr = ParamPtr;
		}
}
//-----------------------------------------------------------------------------
//послать сообщение без параметров: иногда параметры передавать не нужно, 
//сигналом является сам факт получения сообщения;
void FsmMessage_Send(uint8_t Msg){
	
	if(FsmMessageSTR[Msg].Msg == 0) FsmMessageSTR[Msg].Msg = 1;
}
//-----------------------------------------------------------------------------
//получить сообщение: возвращает True (1) если сообщение было послано, иначе False (0);
uint8_t FsmMessages_Get(uint8_t Msg){
	
	if(FsmMessageSTR[Msg].Msg != 0) //== 2)
		{
			FsmMessageSTR[Msg].Msg = 0;
			return 1;
		}
	return 0;
}
//-----------------------------------------------------------------------------
//возвращает указатель на параметр сообщения;
uint8_t *FsmMessages_GetWithParam(uint8_t Msg){
	
	return FsmMessageSTR[Msg].ParamPtr;
}
//-----------------------------------------------------------------------------
#ifdef USE_BROADCAST_MESSAGES
void FsmMessages_SendBroadcast(uint8_t Msg){
	
	if(BroadcastMessages[Msg] == 0) BroadcastMessages[Msg] = 1;
}
//-----------------------------------------------------------------------------
uint8_t FsmMessages_GetBroadcast(uint8_t Msg){
	
	if(BroadcastMessages[Msg] == 2) return 1;
	return 0;
}
#endif
//-----------------------------------------------------------------------------
