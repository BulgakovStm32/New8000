
#include "Rs485.h"

//***************************************************************************************************
static RS485Buf_t	        RS485RxBuf;    
static RS485Buf_t	        RS485TxBuf;
static RS485Flags_t       RS485Flags;
static volatile uint16_t  RS485TimeOutCounter = 0;
//***************************************************************************************************
//***************************************************************************************************
static void RS485_TxBufComplete(void){
	
	RS485DirectRX();//Микросхему на прием.
}
//*****************************************************************************
static void RS485_PackCrcCalc(void){
	
	uint8_t  packSizeForCrc;	
	//-----------
	packSizeForCrc = RS485TxBuf.Str.Size - RS485_BUF_CRC_SIZE;
	
	*(uint16_t*)&RS485TxBuf.Buf[packSizeForCrc] = Get_Crc16(RS485TxBuf.Buf, packSizeForCrc);
}
//*****************************************************************************
static void RS485_StartTxBuf(uint8_t *buf, uint8_t size){

  RS485DirectTX();    				//Микросхему на передачу.
	DMA1_Ch2_StartTX(buf, size);//
}
//*****************************************************************************
//Очистка "шапки" приемного буфера.
static void RS485_RxBufClrHead(void){

	RS485RxBuf.Str.SinhroByte1 = 0;
	RS485RxBuf.Str.SinhroByte2 = 0;
	RS485RxBuf.Str.Type				 = 0;
	RS485RxBuf.Str.Size			   = 0;
}
//*****************************************************************************
//Прием пакета.
static uint8_t RS485_RxPacket(uint8_t rxByte){
  
	RS485Buf_t *rxBuf = &RS485RxBuf;
  static uint8_t rxCounterByte = 0;
  //--------------------
  rxCounterByte++; 
  if(rxBuf->Str.Size > 0)
    {	
			if(rxBuf->Str.Size > RS485_PACK_MAX_SIZE)
				{
					RS485_RxBufClrHead();//Сброс приемного буфера.	
					return 0; 
				}
			//Прнятый байт в приемный буфер.
      rxBuf->Str.Data[rxCounterByte - RS485_BUF_HEAD_SIZE - 1] = rxByte;
			//Принято нужное количество байт.
      if(rxCounterByte >= rxBuf->Str.Size) 
        {	 	
					rxCounterByte = 0;
					//Проверка CRC.
					if(*(uint16_t*)&rxBuf->Buf[rxBuf->Str.Size - RS485_BUF_CRC_SIZE] ==
						    Get_Crc16(rxBuf->Buf, (rxBuf->Str.Size - RS485_BUF_CRC_SIZE)))
            {						
              //Смотрим, какая команда пришла.
              switch(rxBuf->Str.CmdCode){
								//-----------
								case(SB_CMD_SET):
									RS485Flags.SB_CMD_SET_f = FlagSet;
								break;
								//-----------
								default: //Неизвестная команда.
									RS485_RxBufClrHead(); //Сброс приемного буфера.	
								return 0;
								//-----------
							}
              //------------------------- 
							RS485_RxBufClrHead();		//Сброс приемного буфера.	
              RS485TimeOutCounter = 0;//Сброс счетчика таймаут
              return 1;
            }
          //-------------------------
          //CRC не совпало. 			
          RS485_RxBufClrHead();//Сброс приемного буфера.					
        }
      //-------------------------
    }
  //---------------------------------------------------
  else
    {		
      //Сохранение синхробайта 1.
					 if((rxBuf->Str.SinhroByte1 != RS485_SINCHRO_BYTE_1) &&
              (rxBuf->Str.SinhroByte2 != RS485_SINCHRO_BYTE_2) &&
              (rxBuf->Str.Type        != SB_TYPE)              &&
							(rxByte == RS485_SINCHRO_BYTE_1))
							{rxBuf->Str.SinhroByte1 = rxByte;}
      //Сохранение синхробайта 2.
		  else if((rxBuf->Str.SinhroByte1 == RS485_SINCHRO_BYTE_1) &&
              (rxBuf->Str.SinhroByte2 != RS485_SINCHRO_BYTE_2) &&
              (rxBuf->Str.Type        != SB_TYPE)              &&
						  (rxByte == RS485_SINCHRO_BYTE_2))
						  {rxBuf->Str.SinhroByte2 = rxByte;}
      //Сохранение типа.
      else if((rxBuf->Str.SinhroByte1 == RS485_SINCHRO_BYTE_1) &&
              (rxBuf->Str.SinhroByte2 == RS485_SINCHRO_BYTE_2) &&
              (rxBuf->Str.Type        != SB_TYPE)              &&
							(rxByte == SB_TYPE))
              {rxBuf->Str.Type = rxByte;}  
      //Сохранение размера.
      else if((rxBuf->Str.SinhroByte1 == RS485_SINCHRO_BYTE_1) &&
              (rxBuf->Str.SinhroByte2 == RS485_SINCHRO_BYTE_2) &&
              (rxBuf->Str.Type        == SB_TYPE)              &&
							(rxByte != 0))
              {rxBuf->Str.Size = rxByte;}  
      //Неправильная последовательность байт. 						
      else 
				{
					rxCounterByte = 0;	
					RS485_RxBufClrHead();
				}
      //-------------------------
    }
  //---------------------------------------------------
  return 0;
}
//***************************************************************************************************
//***************************************************************************************************
void RS485_Init(void){

  //DE_RE - PB1 - Управление микросхемой приемо-передатчика.
	GPIOB->MODER   |= GPIO_MODER_MODER1_0; 	//PB1 - выход, push-pull.
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR1;//High speed.
	GPIOB->PUPDR   &=~GPIO_PUPDR_PUPDR1;		//No pull-up, pull-down.
	//Инициализация USART. 
  Usart1_Init(USART_BRR);		
  Usart1_Handlers()->ReceiveByte         = RS485_RxPacket;
	Usart1_Handlers()->ReceiveByteBroken   = RS485_RxBufClrHead;
	Usart1_Handlers()->BufTransferComplete = RS485_TxBufComplete;
	//Инициализация DMA. TX USART1 работает с DMA1 Ch2.
	DMA1_Ch2_InitForTX(USART1);  			 
}
//*****************************************************************************
//Отсчет таймаута приема пакета.
//Если не сбрасывался счетчик в течении 1с значит небыло запросов/ответов на запросы
//что говорит об отсутсвии связи.
void RS485_TimeOutLoop(void){
  
	//Сброс счетчика происходит по приему запросов/ответов на запросы
  if(RS485TimeOutCounter <= RS485_PROTOCOL_TIMEOUT) 
		{
			 RS485TimeOutCounter++;
			 RS485Flags.TIMEOUT_ERR_f = FlagClr;
		}
  else RS485Flags.TIMEOUT_ERR_f = FlagSet;
}
//*****************************************************************************
RS485Flags_t* RS485_Flags(void){

	return &RS485Flags;
}
//*****************************************************************************
RS485Buf_t* RS485_RxBuf(void){
  
  return &RS485RxBuf;
}
//***************************************************************************************************
//***************************************************************************************************
void RS485_TxStatusSB(void){
	
	//Заголовок пакета.
  RS485TxBuf.Str.SinhroByte1 = RS485_SINCHRO_BYTE_1;
	RS485TxBuf.Str.SinhroByte2 = RS485_SINCHRO_BYTE_2;
	RS485TxBuf.Str.Type        = SB_TYPE;
  RS485TxBuf.Str.Size        = RS485_BUF_HEAD_SIZE + SB_DATA_SIZE + RS485_BUF_CRC_SIZE;
  RS485TxBuf.Str.CmdCode 		 = SB_CMD_SET;
 	//-----------
	//Данные пакета.
	SirenBoard_BuildingPackForRS485(RS485TxBuf.Str.Data);	
	//-----------
	RS485_PackCrcCalc();
	//-----------
	RS485_StartTxBuf(RS485TxBuf.Buf, RS485TxBuf.Str.Size);
}
//***************************************************************************************************
//***************************************************************************************************








