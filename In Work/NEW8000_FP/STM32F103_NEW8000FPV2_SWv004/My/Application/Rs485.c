
#include "Rs485.h"

//***************************************************************************************************
static RS485Buf_t	        RS485RxBuf;    
static RS485Buf_t	        RS485TxBuf;
static RS485Flags_t       RS485FlagsStr;
static volatile uint16_t  RS485TimeOutCounter = 0;
//***************************************************************************************************
//***************************************************************************************************
static void RS485_TxBufComplete(void){
	
	RS485DirectRX();//Микросхему на прием.
	RS485FlagsStr.TxContinues = FLAG_CLEAR;
}
//*****************************************************************************
//CRC пакета.
static void RS485_PackCrcCalc(void){
	
	uint8_t  packSizeForCrc;	
	//-----------
	packSizeForCrc = RS485TxBuf.Str.Size - RS485_BUF_CRC_SIZE;
	
	*(uint16_t*)&RS485TxBuf.Buf[packSizeForCrc] = Get_Crc16(RS485TxBuf.Buf, packSizeForCrc);
}
//*****************************************************************************
 static void RS485_StartBufTx(uint8_t* buf, uint16_t size){
	 
	//Если еще не закончили передачу то выход.
	if(RS485FlagsStr.TxContinues == FLAG_SET)return;
	//--------------------
	RS485DirectTX();//Микросхему на передачу.
	DMA1Ch4StartTx(buf, size);
	RS485FlagsStr.TxContinues = FLAG_SET;		
}
//*****************************************************************************
//Сброс приемного буфера.
static void RS485_RxBufClrHead(void){

  RS485RxBuf.Str.SinhroByte1 = 0;
  RS485RxBuf.Str.SinhroByte2 = 0;
	RS485RxBuf.Str.Type		     = 0;
  RS485RxBuf.Str.Size		     = 0;
} 
//*****************************************************************************
//Прием пакета.
static uint8_t RS485_RxPacket(volatile uint8_t rxByte){
  
	static uint8_t rxCounterByte = 0;
	RS485Buf_t *rxBuf = &RS485RxBuf;
  //--------------------
  rxCounterByte++;
	if(rxBuf->Str.Size != 0)
    {
      rxBuf->Str.Data[rxCounterByte - RS485_BUF_HEAD_SIZE - 1] = rxByte;//Принятый байт в приемный буфер.
      //-------------------------
      if(rxCounterByte >= rxBuf->Str.Size)//Принято нужное количество байт.
        {	  
					rxCounterByte = 0;
					//Проверка CRC.
					if(*(uint16_t*)((uint8_t*)&rxBuf->Buf + rxBuf->Str.Size - 2) ==
						    Get_Crc16((uint8_t*)&rxBuf->Buf, (rxBuf->Str.Size - 2)))
            {  
              //Смотрим, от кого пришли данные.
              switch(rxBuf->Str.Type){
                  //-----------
                  case(MB_TYPE)://Данные ЦП.
										RS485FlagsStr.MB_NewData = FLAG_SET;
									break;
									//-----------
                  case(FP_TYPE)://Данные ЦП.
										RS485FlagsStr.MB_NewData = FLAG_SET;
									break;
                  //-----------								
                  default: //Неизвестный тип устройства.
                    RS485_RxBufClrHead(); 	
                  return 0;
                  //-----------
                }
              //------------------------- 
							RS485_RxBufClrHead();   
              RS485TimeOutCounter = 0;
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
              (rxBuf->Str.Type        == 0)                    && 
							(rxByte 								== RS485_SINCHRO_BYTE_1))
							{rxBuf->Str.SinhroByte1  = rxByte;}
      //Сохранение синхробайта 2.
		  else if((rxBuf->Str.SinhroByte1 == RS485_SINCHRO_BYTE_1) &&
              (rxBuf->Str.SinhroByte2 != RS485_SINCHRO_BYTE_2) &&
              (rxBuf->Str.Type        == 0)                    && 
						  (rxByte 								== RS485_SINCHRO_BYTE_2))
						  {rxBuf->Str.SinhroByte2  = rxByte;}
      //Сохранение типа.
      else if((rxBuf->Str.SinhroByte1 == RS485_SINCHRO_BYTE_1) &&
              (rxBuf->Str.SinhroByte2 == RS485_SINCHRO_BYTE_2) &&
              (rxBuf->Str.Type        == 0)                    && 
							(rxByte != 0))
              {rxBuf->Str.Type = rxByte;}
      //Сохранение размера.
      else if((rxBuf->Str.SinhroByte1 == RS485_SINCHRO_BYTE_1) &&
              (rxBuf->Str.SinhroByte2 == RS485_SINCHRO_BYTE_2) &&
              (rxBuf->Str.Type        != 0)                    && 
							(rxByte != 0))
              {rxBuf->Str.Size = rxByte;} 
			//Сохранение кода команды.	ЭТО НЕ РАБОТАЕТ!!!!! Проверил 10 раз!!!!!
			//Почему - непонятно!!! В rxByte почему-то значение 0!!!	
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

	//DE_RE - PC14 - Управление микросхемой приемо-передатчика.
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;//Включение тактирование GPIOC.	
	GPIOC->CRH &= ~(GPIO_CRH_CNF14);   //PC14(DE/RE) - выход, режим - push-pull.
	GPIOC->CRH |=   GPIO_CRH_MODE14;   //PC14(DE/RE) - тактирование 50МГц.
	//Инициализация USART3.
	Uart1Init(USART1_BRR);
	Uart1Handler()->ReceiveByte         = &RS485_RxPacket;
	Uart1Handler()->ReceiveByteBroken   = &RS485_RxBufClrHead;
	Uart1Handler()->BufTransferComplete = &RS485_TxBufComplete;    
}
//*****************************************************************************
//Отсчет таймаута приема пакета.
//Если не сбрасывался счетчик в течении RS485ProtocolTimeOut_Const значит небыло запросов/ответов на запросы
//что говорит об отсутсвии связи.
void RS485_TimeOutInc(void){
  
	//Сброс счетчика прроисходит по приему запросов/ответов на запросы
  if(RS485TimeOutCounter <= RS485_PROTOCOL_TIMEOUT)
		{
			RS485TimeOutCounter++;
			RS485FlagsStr.TimeOutError = FLAG_CLEAR;
		}
  else RS485FlagsStr.TimeOutError = FLAG_SET; 
}
//*****************************************************************************
RS485Flags_t* RS485_Flags(void){
	
	return &RS485FlagsStr;
}
//*****************************************************************************
uint8_t* RS485_Cmd(void){

	static uint8_t CmdReg = 0;
	//--------------------
	return &CmdReg;
}
//*****************************************************************************
//Получение указателя приемный буфере.
RS485Buf_t* RS485_RxBuf(void){

  return &RS485RxBuf;
}
//*****************************************************************************
RS485Buf_t* RS485_TxBuf(void){

	return &RS485TxBuf;
}
//*****************************************************************************
//Ответ на запрос.
void RS485_StartTxResponse(void){	   
	
	RS485Buf_t* txBuf = &RS485TxBuf;
	//--------------------
	txBuf->Str.SinhroByte1 = RS485_SINCHRO_BYTE_1;
	txBuf->Str.SinhroByte2 = RS485_SINCHRO_BYTE_2;
	txBuf->Str.Type		     = FP_TYPE;
  txBuf->Str.Size		     = (uint8_t)sizeof(FPData_t) + RS485_BUF_HEAD_SIZE + RS485_BUF_CRC_SIZE;
	//-----------
	//CRC пакета.
	RS485_PackCrcCalc();
	//--------------------
  //Запуск передачи.
	RS485_StartBufTx(txBuf->Buf, txBuf->Str.Size);
}
//***************************************************************************************************
//***************************************************************************************************
//Определение команды для передачи в ЦП.
void RS485_TxDataToMB(void){
	
	FPData_t *txBuf = (FPData_t*)RS485_TxBuf()->Str.Data;
 	//---------------------	
	switch(*RS485_Cmd()){
		//--------------------
//		//Команда "Выдача статуса от FP".
//		case(FP_CMD_SEND_STATUS):
//			RS485_TxBuf()->Str.CmdCode = FP_CMD_SEND_STATUS;               
//			txBuf->MicState = MicGetState();
//			txBuf->KeyState = KeyGetState();
//			txBuf->Address  = Address;
//			txBuf->Group    = Group;  
//		break;
		//--------------------
		//Команда установки адрес блока
		case(FP_CMD_SET_ADDRES):
			RS485_TxBuf()->Str.CmdCode = FP_CMD_SET_ADDRES;
			txBuf->Address = FacePanel_WorkReg()->Address; //Address;
//			MenuBackgroundPage()->PageItems.StringVar[String1] = Address;
		break;
		//--------------------
		//Команда устновки группы блока
		case(FP_CMD_SET_GROUP):
			RS485_TxBuf()->Str.CmdCode = FP_CMD_SET_GROUP;
			txBuf->Group = FacePanel_WorkReg()->Group; //Group;
//			MenuBackgroundPage()->PageItems.StringVar[String2] = Group;
		break;
		//--------------------
		//Команда установики времени
		case(FP_CMD_SET_TIME):
			RS485_TxBuf()->Str.CmdCode = FP_CMD_SET_TIME;
		break;
		//-------------------- 
		//Команда установики даты
		case(FP_CMD_SET_DATA):
			RS485_TxBuf()->Str.CmdCode = FP_CMD_SET_DATA;
		break;
		//-------------------- 
		//Команда влючение тестирования зуммера ЦП
		case(FP_CMD_TEST_ZUMMER):
			RS485_TxBuf()->Str.CmdCode = FP_CMD_TEST_ZUMMER; 
		break;
		//-------------------- 
		//Команда сохранения опорных значений для Lc.
		//В KeyAndMicState будут активированные линии.
		//В Address будет номер канала для которого сохряняются опроные значения. 
		//В Group будет отклонения от установившегося значения для фиксации перех-го проц-са на линии. 
		case(FP_CMD_LC_SAVE_REF):
			RS485_TxBuf()->Str.CmdCode = FP_CMD_LC_SAVE_REF;
			txBuf->MicState = SpLine_Param()->Control;  //линии поставленные на контроль.           								
			txBuf->Address  = SpLine_Param()->LineRef;  //сохранения опорной величины для выбранной линииГр.	
			txBuf->TimeUTC  = SpLine_Param()->Deviation;//значение отклолнения линии Гр.
		  SpLine_Param()->ConfigFlag = FLAG_CLEAR;		
		break;
		//--------------------
		//Команда получения одного события из журнала.
		//в TimeUTC номер записи журнала.
		case(FP_CMD_GET_LOG):
			RS485_TxBuf()->Str.CmdCode = FP_CMD_GET_LOG;
			txBuf->TimeUTC = Log()->EventIndex;
		break;	
		//--------------------
		//Команда получени параметров одного входа пожарного шлейфа.
		case(FP_CMD_GET_INPUT_CONFIG):
			RS485_TxBuf()->Str.CmdCode = FP_CMD_GET_INPUT_CONFIG;
		break;			
		//--------------------
		//Команда усатноки параметров одного входа пожарного шлейфа.
		case(FP_CMD_SET_INPUT_CONFIG):
			RS485_TxBuf()->Str.CmdCode = FP_CMD_SET_INPUT_CONFIG;
		break;	
		//--------------------
		//Команда сохранения контроля питания блока.
		case(FP_CMD_SET_POWER_CHECK):
			RS485_TxBuf()->Str.CmdCode = FP_CMD_SET_POWER_CHECK;
			txBuf->MicState = PowerDevice()->Check.Byte; 
		break;	
		//--------------------
		//Команда получения информацио о EEPROM.
		case(FP_CMD_GET_EEPROM_INFO):
			RS485_TxBuf()->Str.CmdCode = FP_CMD_GET_EEPROM_INFO;
		break;	
		//--------------------
		//Команда "Выдача статуса от FP".
		default:
			RS485_TxBuf()->Str.CmdCode = FP_CMD_SEND_STATUS;               
			txBuf->MicState = MicState();
			txBuf->KeyState = KeyGetState();
			//txBuf->Address  = FacePanel_WorkReg()->Address;
			//txBuf->Group    = FacePanel_WorkReg()->Group;  
		break;
		//--------------------
	}
	//--------------------------------------------
	*RS485_Cmd() = 0;
	RS485_StartTxResponse();//Запуск передачи ответа на запрос.
}
//***************************************************************************************************
//***************************************************************************************************




















