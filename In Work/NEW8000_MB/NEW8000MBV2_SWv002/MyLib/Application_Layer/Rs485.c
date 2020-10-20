
#include "Rs485.h"

//-----------------------------------------------------------------------------
static RS485RxBuf_t       RS485RxBufSTR;       //Буфер приёма.
static volatile uint16_t  RS485RxCountByte = 0;//Счетчик принятых байт
static RS485TimeOut_t  		RS485TimeOutSTR;
static RS485Flags_t    		RS485FlagsSTR;
//*********************************************************************************
//*********************Прикладной уровень. Общие функции.**************************
void RS485Init(void){

  //DE/RE - PC6 - Управление микросхемой приемо-передатчика.
  RCC->APB2ENR |=   RCC_APB2ENR_IOPCEN;//Включение тактирование GPIOC.	
  GPIOC->CRL   &= ~(GPIO_CRL_CNF6);	   //PC6 - выход, режим - push-pull.
  GPIOC->CRL   |=   GPIO_CRL_MODE6;	   //PC6 - тактирование 50МГц.
	//Инициализация USART3.
  Uart3Init();
	Uart3Handler()->ReceiveByte          		 = &RS485RxPacket;
	Uart3Handler()->ReceiveByteBroken    		 = &RS485RxClrBuf;
	Uart3Handler()->ByteTransmissionComplete = &RS485DirectionRX;
	//Инициализация DMA. TX USART3 работает с DMA1 Ch2.
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;//Включить тактирование DMA1
	DMA1Ch2InitForTX(USART3);  			 //DMA1Ch2 - 
}
//-----------------------------------------------------------------------------
//Отсчет таймаута приема пакета.
//Если не сбрасывался счетчик в течении 1с значит небыло запросов/ответов на запросы
//что говорит об отсутсвии связи.
void RS485TimeOutInc(void){
  
	//--------------------
	//Сброс счетчика происходит по приему запросов/ответов на запросы
  if(RS485TimeOutSTR.CountTimeOutProtocol <= 1000) 
		{
			 RS485TimeOutSTR.CountTimeOutProtocol++;
			 RS485FlagsSTR.TimeOutFlag = FlagClr;
		}
  else RS485FlagsSTR.TimeOutFlag = FlagSet;
	//--------------------
	//Таймаут приема пакета от лицевой панели.
	if(RS485TimeOutSTR.CountTimeOutAckFP <= RS485NAckTimeOut) 
		{
			RS485TimeOutSTR.CountTimeOutAckFP++;
			RS485FlagsSTR.FPNAckFlag = FlagClr;	
		}
  else RS485FlagsSTR.FPNAckFlag = FlagSet;
	//--------------------
	//Таймаут приема пакета от SB.
	if(RS485TimeOutSTR.CountTimeOutAckSB <= RS485NAckTimeOut) 
		{
			RS485TimeOutSTR.CountTimeOutAckSB++;
			RS485FlagsSTR.SBNAckFlag = FlagClr;	
		}
  else RS485FlagsSTR.SBNAckFlag = FlagSet;
	//--------------------
	//Таймаут приема пакета от IP.
		
	//--------------------
	//Таймаут приема пакета от ZE.
		
	//--------------------
}
//-----------------------------------------------------------------------------
RS485Flags_t* RS485Flags(void){

	return &RS485FlagsSTR;
}
//-----------------------------------------------------------------------------
void RS485DirectionTX(void){

	RS485DirectTX();
}
//-----------------------------------------------------------------------------
void RS485DirectionRX(void){

	RS485DirectRX();
}
//-----------------------------------------------------------------------------
//Сброс приемного буфера.
void RS485RxClrBuf(void){

  //__disable_irq();
	
	RS485RxCountByte = 0;	
	//Очистка "шапки" приемного буфера.
	RS485RxBufSTR.SinhroByte1 = 0;
	RS485RxBufSTR.SinhroByte2 = 0;
	RS485RxBufSTR.Type  			= 0;
	RS485RxBufSTR.Size				= 0;
	//Сброс CRC.
	//RS485RxBufSTR.Data.FP.CRC16 = 0;
	//RS485RxBufSTR.Data.SB.CRC16 = 0;
	
  //__enable_irq();
} 
//-----------------------------------------------------------------------------
//Прием пакета.
uint8_t RS485RxPacket(uint8_t data){
  
  //--------------------
  RS485RxCountByte++;
  if(RS485RxBufSTR.Size > 0)
    {
      RS485RxBufSTR.Data.Buf[RS485RxCountByte - RS485_BUF_HEADER_SIZE - 1] = data;//Прнятый байт в приемный буфер.
      //-------------------------
      if(RS485RxCountByte >= RS485RxBufSTR.Size)//Принято нужное количество байт.
        {	  
					//Проверка CRC.
					if(*(uint16_t*)((uint8_t*)&RS485RxBufSTR + RS485RxBufSTR.Size - 2) ==
						    Get_Crc16((uint8_t*)&RS485RxBufSTR, (RS485RxBufSTR.Size - 2)))
						{  
              //Смотрим, от кого пришли данные.
              switch(RS485RxBufSTR.Type){
								//-----------
								case(FP_TypeConst)://Данные от FP.
									RS485FlagsSTR.FPNewDataFlag	= FlagSet;
								  RS485TimeOutSTR.CountTimeOutAckFP = 0;
								break;
								//-----------
								case(SB_TypeConst)://Данные от SB.
									RS485FlagsSTR.SBNewDataFlag	= FlagSet;
									RS485TimeOutSTR.CountTimeOutAckSB = 0;
								break;
								//-----------
								case(IP_TypeConst)://Данные от IP.
									RS485FlagsSTR.IPNewDataFlag	= FlagSet;
									RS485TimeOutSTR.CountTimeOutAckIP = 0;
								break;
								//-----------
								case(ZE_TypeConst)://Данные от ZE.
									RS485FlagsSTR.ZENewDataFlag	= FlagSet;
									RS485TimeOutSTR.CountTimeOutAckZE = 0;
								break;
								//-----------
								default: //Неизвестный тип устройства.
									RS485RxClrBuf(); //Сброс приемного буфера.	
								return 0;
								//-----------
                }
              //-------------------------
              RS485TimeOutSTR.CountTimeOutProtocol = 0;  //Сброс счетчика таймаут
              return 1;
            }
          //-------------------------
          //CRC не совпало. 			
          else RS485RxClrBuf(); //Сброс приемного буфера.					
        }
      //-------------------------
    }
  //---------------------------------------------------
  else
    {	
      //Сохранение синхробайта 1.
					 if((RS485RxBufSTR.SinhroByte1 != SinhroByte1Const) &&
              (RS485RxBufSTR.SinhroByte2 != SinhroByte2Const) &&
              (RS485RxBufSTR.Type        == 0)                &&
							(data 									   == SinhroByte1Const))
							{RS485RxBufSTR.SinhroByte1  = data;}
      //Сохранение синхробайта 2.
		  else if((RS485RxBufSTR.SinhroByte1 == SinhroByte1Const) &&
              (RS485RxBufSTR.SinhroByte2 != SinhroByte2Const) &&
              (RS485RxBufSTR.Type        == 0)                &&
						  (data 									   == SinhroByte2Const))
						  {RS485RxBufSTR.SinhroByte2  = data;}
      //Сохранение типа.
      else if((RS485RxBufSTR.SinhroByte1 == SinhroByte1Const) &&
              (RS485RxBufSTR.SinhroByte2 == SinhroByte2Const) &&
              (RS485RxBufSTR.Type        == 0)					      &&
							(data > 0 ))
              {RS485RxBufSTR.Type = data;}  
      //Сохранение размера.
      else if((RS485RxBufSTR.SinhroByte1 == SinhroByte1Const) &&
              (RS485RxBufSTR.SinhroByte2 == SinhroByte2Const) &&
              (RS485RxBufSTR.Type > 0) 		  								  &&
							(data > 0))
              {RS485RxBufSTR.Size = data;}  
      //Неправильная последовательность байт.  
      else RS485RxClrBuf();//Сброс приемного буфера
    }
  //---------------------------------------------------
  return 0;
}
//-----------------------------------------------------------------------------
RS485RxBuf_t* RS485RxBuf(void){

	return &RS485RxBufSTR; 
}
//-----------------------------------------------------------------------------
void RS485StartTx(uint8_t* buf, uint16_t size){

	RS485DirectionTX();  //Микросхему на передачу.
	DMA1Ch2StartTX(buf, size);
}
//***************************************************************************************************
//*****************************Ф-и для работы ЦП.****************************************************
//Автомат состояний, определяющий порядок выдачи кодограмм.
void RS485MasterLoop(void){
	
  static uint8_t  step    = SEND_MB_STATUS_STEP;
  static uint16_t mScount = 0;
  //--------------------
	RS485TimeOutInc();     									 //Отсчет таймаута приема пакета.
  if(++mScount < RS485RequestPeriod)return;//Формирование паузы между запросами.
  mScount = 0;
  //--------------------
	//Автомат состояний, определяющий порядок выдачи кодограмм в цикле.
	switch(step++){
			//-------------------------
			//Выдача статуса от MB.
			case(SEND_MB_STATUS_STEP):
				RS485StartTx(MotherBoardRS485TxBuf()->BLK, MotherBoardRS485TxBuf()->STR.Size);
			break;
			//-------------------------				
			//Запрос статуса к плате управления оповещателями.
			case(REQUEST_SB_STATUS_STEP):
			  RS485StartTx(SirenRS485TxBuf()->BLK, SirenRS485TxBuf()->STR.Size);
			
				step = SEND_MB_STATUS_STEP;
			break;
//			//-------------------------
//			//Запрос статуса к модулю IP.
//			case(2):
//				//RS485TxRequestIP(&RS485TxBufUNION);
//				//step = 3;//Переход на следующий шаг.
//			break;
//			//-------------------------
//			//Запрос статуса к модулю рсширения зон.
//			case(3):
//				//RS485TxRequestZE(&RS485TxBufUNION);
//				//step = 4;//Переход на следующий шаг.
//			break;
//			//-------------------------				
//			//Запрос статуса к плате интерфейсов.
//			case(4):
//				//RS485TxRequestZE(&RS485TxBufUNION);
//				step = 0;//Переход на следующий шаг.
//			break;
//			//-------------------------						
		}
	//------------------------------------------------------------------
}
//***************************************************************************************************
//*****************************Ф-и для работы FP.****************************************************


//***************************************************************************************************
//*****************************Ф-и для работы SB.****************************************************


//***************************************************************************************************
//*****************************Ф-и для работы c ZE.**************************************************


//***************************************************************************************************
//*****************************Ф-и для работы IP.****************************************************


//***************************************************************************************************
//***************************************************************************************************


