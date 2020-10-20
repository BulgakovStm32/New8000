
#include "Rs485.h"

//***************************************************************************************************
static RS485Buf_t		 RS485RxBufStr;
static RS485Buf_t		 RS485TxBufStr;
static RS485Counts_t RS485CountsStr;
static RS485Flags_t	 RS485FlagsStr;
//***************************************************************************************************
//*********************Прикладной уровень. Общие функции.********************************************
//Очистка загаловка буфера.
static void RS485_RxBufClrHead(void){

	RS485Buf_t *rxBuf = &RS485RxBufStr;
  //--------------------	
	rxBuf->Str.SinhroByte1 = 0;
	rxBuf->Str.SinhroByte2 = 0;
	rxBuf->Str.Type  			 = 0;
	rxBuf->Str.Size				 = 0;
}
//*****************************************************************************
//Прием пакета.
static uint8_t RS485_RxPacket(volatile uint8_t rxByte){
	
	static uint32_t rxCountByte = 0;
	RS485Buf_t *rxBuf = &RS485RxBufStr;
  //--------------------
  rxCountByte++;
	if(rxBuf->Str.Size > 0)
    {
      rxBuf->Str.Data[rxCountByte - RS485_BUF_HEAD_SIZE - 1] = rxByte;//Прнятый байт в приемный буфер.
      //-------------------------
      if(rxCountByte >= rxBuf->Str.Size)//Принято нужное количество байт.
        {	  
					rxCountByte = 0;
					//Проверка CRC.
					if(*(uint16_t*)&rxBuf->Buf[rxBuf->Str.Size - RS485_BUF_CRC_SIZE] ==
						    Get_Crc16(rxBuf->Buf, (rxBuf->Str.Size - RS485_BUF_CRC_SIZE)))
						{  
              //Смотрим, от кого пришли данные.
              switch(rxBuf->Str.Type){
								//-----------
								case(FP_TYPE)://Данные от FP.
									RS485FlagsStr.FPNewData	= FLAG_SET;
								  RS485CountsStr.TimeOutAckFP = 0;
								break;
								//-----------
								case(SB_TYPE)://Данные от SB.
									RS485FlagsStr.SBNewData	= FLAG_SET;
									RS485CountsStr.TimeOutAckSB = 0;
								break;
								//-----------
								case(IP_TypeConst)://Данные от IP.
									RS485FlagsStr.IPNewData	= FLAG_SET;
									RS485CountsStr.TimeOutAckIP = 0;
								break;
								//-----------
								case(ZE_TypeConst)://Данные от ZE.
									RS485FlagsStr.ZENewData	= FLAG_SET;
									RS485CountsStr.TimeOutAckZE = 0;
								break;
								//-----------
								default://Неизвестный тип устройства.
									RS485_RxBufClrHead();//Сброс приемного буфера.				
								return 0;
								//-----------
                }
              //-------------------------
							RS485_RxBufClrHead(); 			
              RS485CountsStr.TimeOutAckProtocol = 0;//Сброс счетчика таймаут
              return 1;
            }
          //-------------------------					
          //CRC не совпало. 			
          RS485_RxBufClrHead(); 
        }
      //-------------------------
    }
  //---------------------------------------------------
  else
    {	
      //Сохранение синхробайта 1.
						if((rxBuf->Str.SinhroByte1 != RS485_SINCHRO_BYTE_1) &&
							 (rxBuf->Str.SinhroByte2 != RS485_SINCHRO_BYTE_2) &&
							 (rxBuf->Str.Type        == 0)              		  &&
							 (rxByte == RS485_SINCHRO_BYTE_1))
							 {rxBuf->Str.SinhroByte1 = rxByte;}
      //Сохранение синхробайта 2.
		   else if((rxBuf->Str.SinhroByte1 == RS485_SINCHRO_BYTE_1) &&
							 (rxBuf->Str.SinhroByte2 != RS485_SINCHRO_BYTE_2) &&
							 (rxBuf->Str.Type        == 0)              			&&
							 (rxByte == RS485_SINCHRO_BYTE_2))
							 {rxBuf->Str.SinhroByte2 = rxByte;}
      //Сохранение типа.
       else if((rxBuf->Str.SinhroByte1 == RS485_SINCHRO_BYTE_1) &&
							 (rxBuf->Str.SinhroByte2 == RS485_SINCHRO_BYTE_2) &&
							 (rxBuf->Str.Type        == 0)					    			&&
						   (rxByte != 0))
							 {rxBuf->Str.Type = rxByte;}
      //Сохранение размера.
       else if((rxBuf->Str.SinhroByte1 == RS485_SINCHRO_BYTE_1) &&
							 (rxBuf->Str.SinhroByte2 == RS485_SINCHRO_BYTE_2) &&
							 (rxBuf->Str.Type != 0) 		  										&&
							 (rxByte != 0))
							 {rxBuf->Str.Size = rxByte;}
			//Сохранение кода команды.	ЭТО НЕ РАБОТАЕТ!!!!! Проверл 10 раз!!!!!
			//Почему - непонятно!!! В byte почемуто значение 0!!!	
//      if( (rxBuf->Str.SinhroByte1 == RS485_SINCHRO_BYTE_1) &&
//					(rxBuf->Str.SinhroByte2 == RS485_SINCHRO_BYTE_2) &&
//					(rxBuf->Str.Type > 0) 		  										 &&
//					(rxBuf->Str.Size > 0)													 &&
//					(byte > 0))
//					{rxBuf->Str.CmdCode = byte;
//					return 0;}					 
      //Неправильная последовательность байт.  
      else
				{
					rxCountByte = 0;
					RS485_RxBufClrHead();
				}
    }
  //---------------------------------------------------
  return 0;
}
//*****************************************************************************
static RS485Buf_t* RS485_TxBuf(void){

	return &RS485TxBufStr;
}
//*****************************************************************************
static void RS485_PackCrcCalc(void){
	
	uint8_t	packSizeForCrc;	
	//-----------
	//CRC пакета.
	packSizeForCrc = RS485_TxBuf()->Str.Size - RS485_BUF_CRC_SIZE;
	*(uint16_t*)&RS485_TxBuf()->Buf[packSizeForCrc] = Get_Crc16(RS485_TxBuf()->Buf, packSizeForCrc);
}
//*****************************************************************************
static void RS485_TxBufComplete(void){
	
	RS485DirectRX();//Микросхему на прием.
	RS485FlagsStr.TxContinues = FLAG_CLEAR;
}
//*****************************************************************************
//Отсчет таймаута приема пакета.
//Если не сбрасывался счетчик в течении 1с значит небыло запросов/ответов на запросы
//что говорит об отсутсвии связи.
static void RS485_TimeOutInc(void){
  
	//--------------------
	//Сброс счетчика происходит по приему запросов/ответов на запросы
  if(RS485CountsStr.TimeOutAckProtocol <= 1000) 
		{
			 RS485CountsStr.TimeOutAckProtocol++;
			 RS485FlagsStr.NoConnect = FLAG_CLEAR;
		}
  else RS485FlagsStr.NoConnect = FLAG_SET;
	//--------------------
	//Таймаут приема пакета от лицевой панели.
	if(RS485CountsStr.TimeOutAckFP <= RS485_NACK_TIMEOUT) 
		{
			RS485CountsStr.TimeOutAckFP++;
			RS485FlagsStr.FPNAck = FLAG_CLEAR;	
		}
  else RS485FlagsStr.FPNAck = FLAG_SET;
	//--------------------
	//Таймаут приема пакета от SB.
	if(RS485CountsStr.TimeOutAckSB <= RS485_NACK_TIMEOUT) 
		{
			RS485CountsStr.TimeOutAckSB++;
			RS485FlagsStr.SBNAck = FLAG_CLEAR;	
		}
	//Нет связи с платой SB.	
  else 
		{
			RS485FlagsStr.SBNAck = FLAG_SET;
			RS485_SB_ErrorConnect();
		}
	//--------------------
	//Таймаут приема пакета от IP.
		
	//--------------------
	//Таймаут приема пакета от ZE.
		
	//--------------------
}
//***************************************************************************************************
//***************************************************************************************************
void RS485_Init(void){

  //DE/RE - PC6 - Управление микросхемой приемо-передатчика.
  RCC->APB2ENR |=   RCC_APB2ENR_IOPCEN;//Включение тактирование GPIOC.	
  GPIOC->CRL   &= ~(GPIO_CRL_CNF6);	   //PC6 - выход, режим - push-pull.
  GPIOC->CRL   |=   GPIO_CRL_MODE6;	   //PC6 - тактирование 50МГц.
	//Инициализация USART3.
	Uart3_Init(USART_BRR);
	Uart3_Handler()->ReceiveByte         = &RS485_RxPacket;
	Uart3_Handler()->ReceiveByteBroken   = &RS485_RxBufClrHead;
	Uart3_Handler()->BufTransferComplete = &RS485_TxBufComplete;
	//Инициализация DMA. TX USART3 работает с DMA1 Ch2.
	DMA1Ch2InitForTX(USART3);  			 
}
//-----------------------------------------------------------------------------
RS485Flags_t* RS485_Flags(void){

	return &RS485FlagsStr;
}
//***************************************************************************************************
//*****************************Ф-и для работы с приемом данных.**************************************
RS485Buf_t* RS485_RxBuf(void){

	return &RS485RxBufStr;
}
//***************************************************************************************************
//*****************************Ф-и для работы с передачей данных.**************************************
void RS485_StartBufTx(uint8_t* buf, uint16_t size){
	
	//Если еще не закончили передачу то выход.
	if(RS485FlagsStr.TxContinues)return;
	//--------------------
	RS485DirectTX();          //Микросхему на передачу.
	DMA1Ch2StartTX(buf, size);//
	RS485FlagsStr.TxContinues = FLAG_SET;
}
//***************************************************************************************************
//*****************************Ф-и для работы ЦП.****************************************************
//Автомат состояний, определяющий порядок выдачи кодограмм.
void RS485_MB_MasterLoop(void){
	
  static uint32_t step    = SEND_MB_STATUS;
  static uint32_t mScount = 0;
  //--------------------
	RS485_TimeOutInc();     									 //Отсчет таймаута приема пакета.
  if(++mScount < RS485_REQUEST_PERIOD)return;//Формирование паузы между запросами.
  mScount = 0;
  //--------------------
	//Автомат состояний, определяющий порядок выдачи кодограмм в цикле.
	switch(step){
		//-------------------------
		//Выдача статуса от MB.
		case(SEND_MB_STATUS):		
			RS485_MB_BuildAndTxPack();
			step = REQUEST_SB_STATUS;
		break;
		//-------------------------				
		//Запрос статуса к плате управления оповещателями.
		case(REQUEST_SB_STATUS):
			RS485_SB_BuildAndTxPack();
			step = REQUEST_IP_STATUS;
		break;
		//-------------------------
		//Запрос статуса к модулю IP.
		case(REQUEST_IP_STATUS):

			step = REQUEST_ZE_STATUS;
		break;
		//-------------------------
		//Запрос статуса к модулю рсширения зон.
		case(REQUEST_ZE_STATUS):

			step = REQUEST_IB_STATUS;
		break;
		//-------------------------				
		//Запрос статуса к плате интерфейсов.
		case(REQUEST_IB_STATUS):
			
			step = SEND_MB_STATUS;
		break;
		//-------------------------						
		}
	//------------------------------------------------------------------
}
//*****************************************************************************
void RS485_MB_BuildAndTxPack(void){

	RS485Buf_t *txBuf = &RS485TxBufStr;
	//-----------
	//Шапка пакета.
	txBuf->Str.SinhroByte1 = RS485_SINCHRO_BYTE_1;
	txBuf->Str.SinhroByte2 = RS485_SINCHRO_BYTE_2;
	txBuf->Str.Type        = MB_TYPE;
	txBuf->Str.Size        = (uint8_t)sizeof(MBData_t) + RS485_BUF_HEAD_SIZE + RS485_BUF_CRC_SIZE;
	txBuf->Str.CmdCode     = MB_CMD_SEND_STATUS;
	//-----------
	//Данные пакета.
	MotherBoard_BuildingPackForRS485(txBuf->Str.Data);
	//-----------
	RS485_PackCrcCalc();
	RS485_StartBufTx(txBuf->Buf, txBuf->Str.Size);
}
//***************************************************************************************************
//*****************************Ф-и для работы SB.****************************************************
void RS485_SB_ErrorConnect(void){

	SirenBoard_ClrData();
}
//*****************************************************************************
void RS485_SB_BuildAndTxPack(void){
	
	RS485Buf_t *txBuf = &RS485TxBufStr;
	//-----------
	//Шапка пакета.
	txBuf->Str.SinhroByte1 = RS485_SINCHRO_BYTE_1;
	txBuf->Str.SinhroByte2 = RS485_SINCHRO_BYTE_2;
	txBuf->Str.Type        = SB_TYPE;
	txBuf->Str.Size        = SB_DATA_SIZE + RS485_BUF_HEAD_SIZE + RS485_BUF_CRC_SIZE;
	txBuf->Str.CmdCode     = SB_CMD_SET;
	//-----------	
	//Данные пакета.
	SirenBoard_BuildingPackForRS485(txBuf->Str.Data);
	//-----------
	RS485_PackCrcCalc();
	RS485_StartBufTx(txBuf->Buf, txBuf->Str.Size);
}
//***************************************************************************************************
//*****************************Ф-и для работы FP.****************************************************
void RS485_FP_BuildAndTxEventPack(uint16_t numEvent){
	
	RS485Buf_t *txBuf = &RS485TxBufStr;	
	//----------
	//Шапка пакета.
	txBuf->Str.SinhroByte1 = RS485_SINCHRO_BYTE_1;
	txBuf->Str.SinhroByte2 = RS485_SINCHRO_BYTE_2;
	txBuf->Str.Type        = FP_TYPE;
	txBuf->Str.Size        = (uint8_t)sizeof(Event_t) + RS485_BUF_HEAD_SIZE + RS485_BUF_CRC_SIZE;
	txBuf->Str.CmdCode     = FP_CMD_GET_LOG;
	//-----------
	//Данные пакета. 
	Log_GetEvent(txBuf->Str.Data ,numEvent);	
	//-----------
	RS485_PackCrcCalc();
	RS485_StartBufTx(txBuf->Buf, txBuf->Str.Size);
}
//*****************************************************************************
void RS485_FP_BuildAndTxEepInfoPack(void){
	
	EepInfo_t  *eepInfo = (EepInfo_t*)&RS485TxBufStr.Str.Data;
	RS485Buf_t *txBuf   = &RS485TxBufStr;
	//----------
	//Шапка пакета.
	txBuf->Str.SinhroByte1 = RS485_SINCHRO_BYTE_1;
	txBuf->Str.SinhroByte2 = RS485_SINCHRO_BYTE_2;
	txBuf->Str.Type        = FP_TYPE;
	txBuf->Str.Size        = (uint8_t)sizeof(EepInfo_t) + RS485_BUF_HEAD_SIZE + RS485_BUF_CRC_SIZE;
	txBuf->Str.CmdCode     = FP_CMD_GET_EEPROM_INFO;
	//-----------
	//Данные пакета.
	eepInfo->BadSectorsCount = EepM95128_Info()->BadSectorsCount;
	eepInfo->WriteCount 		 = EepM95128_Info()->WriteCount;
	eepInfo->ReadCount  		 = EepM95128_Info()->ReadCount;
	//-----------
	RS485_PackCrcCalc();
	RS485_StartBufTx(txBuf->Buf, txBuf->Str.Size);
}
//*****************************************************************************
void RS485_FP_BuildAndTxFireLineContextPack(uint8_t numLine){
	
	RS485Buf_t *txBuf           = &RS485TxBufStr;
	//----------
	//Шапка пакета.
	txBuf->Str.SinhroByte1 = RS485_SINCHRO_BYTE_1;
	txBuf->Str.SinhroByte2 = RS485_SINCHRO_BYTE_2;
	txBuf->Str.Type        = FP_TYPE;
	txBuf->Str.Size        = FIRE_LINES_CONTEXT_SIZE + RS485_BUF_HEAD_SIZE + RS485_BUF_CRC_SIZE;
	txBuf->Str.CmdCode     = FP_CMD_GET_INPUT_CONFIG;
	//-----------
	//Данные пакета.
	FireLine_GetContext(txBuf->Str.Data, numLine);	
	//-----------
	RS485_PackCrcCalc();
	RS485_StartBufTx(txBuf->Buf, txBuf->Str.Size);
}
//***************************************************************************************************
//*****************************Ф-и для работы c ZE.**************************************************


//***************************************************************************************************
//*****************************Ф-и для работы IP.****************************************************


//***************************************************************************************************
//***************************************************************************************************


