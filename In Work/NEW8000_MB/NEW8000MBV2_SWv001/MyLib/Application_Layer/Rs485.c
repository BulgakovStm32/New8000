
#include "Rs485.h"

//-----------------------------------------------------------------------------
union {
	struct{
    //--------------------
    uint16_t Debug1_uint16;   // 
    uint16_t Debug2_uint16;   //
    uint16_t Debug3_uint16;   //
    uint16_t Debug4_uint16;   //
    uint16_t Debug5_uint16;   //
    //--------------------
		  }STR;
	uint8_t BLK[10];
}DebugUNION;
//----------------------------------------

volatile uint16_t RS485RxCounterByte   = 0;
volatile uint16_t RS485ProtocolFlagReg = 0;
volatile uint16_t RS485ProtocolTimeOut = 0;
//-----------------------------------------------------------------------------
RS485BufUNION_Type  RS485TxBufUNION;//Буфер передачи.
MB_DataUNION_Type		RS485_MB_TxBuf; //
//SB_DataUNION_Type		RS485_SB_TxBuf; //

RS485BufUNION_Type  RS485RxBufUNION; //Буфер приёма.
//*********************************************************************************
//*********************Прикладной уровень. Общие функции.**************************
void RS485Init(void){

  //DE/RE - PC6 - Управление микросхемой приемо-передатчика.
  RCC->APB2ENR |=   RCC_APB2ENR_IOPCEN;//Включение тактирование GPIOC.	
  GPIOC->CRL   &= ~(GPIO_CRL_CNF6);	   //PC6 - выход, режим - push-pull.
  GPIOC->CRL   |=   GPIO_CRL_MODE6;	   //PC6 - тактирование 50МГц.
												
  Uart3Init();  
}
//-----------------------------------------------------------------------------
//Управление микросхемой приемо-передатчика.
void RS485DE(uint8_t de){
  
  if(de == DE)RS485DEOn;
  else        RS485REOn;
}
//-----------------------------------------------------------------------------
//Отсчет таймаута приема пакета.
//Если не сбрасывался счетчик в течении 1с значит небыло запросов/ответов на запросы
//что говорит об отсутсвии связи.
void RS485TimeOutInc(void){
  
  if(RS485ProtocolTimeOut <= 1000)//Сброс счетчика прроисходит по приему запросов/ответов на запросы
    {
      ++RS485ProtocolTimeOut;
      RS485ProtocolFlagReg &= ~RS485_TimeOutFlag;
    }
  else RS485ProtocolFlagReg |= RS485_TimeOutFlag;
}
//-----------------------------------------------------------------------------
uint16_t RS485FlagGet(void){

  volatile uint16_t temp = 0;  
  //--------------------
  __disable_irq();
  temp = RS485ProtocolFlagReg;
  __enable_irq();
  return temp;
}
//-----------------------------------------------------------------------------
void RS485FlagSet(uint16_t flag){

  __disable_irq();
  RS485ProtocolFlagReg |= flag;
  __enable_irq();
}
//-----------------------------------------------------------------------------
void RS485FlagClr(uint16_t flag){

  __disable_irq();
  RS485ProtocolFlagReg &= ~flag;
  __enable_irq();
}
//-----------------------------------------------------------------------------
//Сброс приемного буфера.
void RS485RxClrBuf(void){

  __disable_irq();
	
	RS485RxCounterByte = 0;	
	//Сброс CRC.
	RS485RxBufUNION.BLK[RS485RxBufUNION.STR.Size - 2] = 0;
	RS485RxBufUNION.BLK[RS485RxBufUNION.STR.Size - 1] = 0;
	//Очистка "шапки" приемного буфера.
	RS485RxBufUNION.STR.SinhroByte1 = 0;
	RS485RxBufUNION.STR.SinhroByte2 = 0;
	RS485RxBufUNION.STR.Type  			= 0;
	RS485RxBufUNION.STR.Size				= 0;
	RS485RxBufUNION.STR.CmdCode		  = 0;
	
  __enable_irq();
} 
//-----------------------------------------------------------------------------
//Прием пакета.
uint8_t RS485RxPacket(uint8_t data){
  
  uint16_t CRCtemp = 0;	
  //--------------------
  //Если не обработан старый пакет то ничего не принемаем.
  //if(RS485ProtocolFlagReg & RS485_FPNewDataFlag) return 0;
	//if(RS485ProtocolFlagReg != 0) return 0;
  //--------------------
  RS485RxCounterByte++;
  if(RS485RxBufUNION.STR.Size > 0)
    {
      RS485RxBufUNION.BLK[RS485RxCounterByte-1] = data;//Прнятый байт в приемный буфер.
      //-------------------------
      if(RS485RxCounterByte >= RS485RxBufUNION.STR.Size)//Принято нужное количество байт.
        {	  
          CRCtemp = Get_Crc16(RS485RxBufUNION.BLK, (RS485RxBufUNION.STR.Size - 2));//Проверка CRC.
					if(CRCtemp == (uint16_t)( RS485RxBufUNION.BLK[RS485RxBufUNION.STR.Size - 2] |
																	 (RS485RxBufUNION.BLK[RS485RxBufUNION.STR.Size - 1] << 8)) )		
						{  
              //Смотрим, от кого пришли данные.
              switch(RS485RxBufUNION.STR.Type){
                  //-----------
                  case(FP_TypeConst)://Данные от FP.
                    RS485ProtocolFlagReg |= RS485_FPNewDataFlag;
                  break;
									//-----------
									case(SB_TypeConst)://Данные от SB.
                    RS485ProtocolFlagReg |= RS485_SBNewDataFlag;
                  break;
                  //-----------
                  case(IP_TypeConst)://Данные от IP.
                    RS485ProtocolFlagReg |=  RS485_IPNewDataFlag;
                  break;
                  //-----------
                  case(ZE_TypeConst)://Данные от ZE.
                    RS485ProtocolFlagReg |= RS485_ZENewDataFlag;
                  break;
                  //-----------
                  default: //Неизвестный тип устройства.
                    RS485RxClrBuf(); //Сброс приемного буфера.	
                    return 0;
                  //-----------
                }
              //------------------------- 
              RS485ProtocolTimeOut = 0;//Сброс счетчика таймаут
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
					 if((RS485RxBufUNION.STR.SinhroByte1 != SinhroByte1Const) &&
              (RS485RxBufUNION.STR.SinhroByte2 != SinhroByte2Const) &&
              (RS485RxBufUNION.STR.Type        == 0)                &&
							(data 													 == SinhroByte1Const))
							{RS485RxBufUNION.STR.SinhroByte1  = data;}
      //Сохранение синхробайта 2.
		  else if((RS485RxBufUNION.STR.SinhroByte1 == SinhroByte1Const) &&
              (RS485RxBufUNION.STR.SinhroByte2 != SinhroByte2Const) &&
              (RS485RxBufUNION.STR.Type        == 0)                &&
						  (data 													 == SinhroByte2Const))
						  {RS485RxBufUNION.STR.SinhroByte2  = data;}
      //Сохранение типа.
      else if((RS485RxBufUNION.STR.SinhroByte1 == SinhroByte1Const) &&
              (RS485RxBufUNION.STR.SinhroByte2 == SinhroByte2Const) &&
              (RS485RxBufUNION.STR.Type        == 0)					      &&
							(data > 0 ))
              {RS485RxBufUNION.STR.Type = data;}  
      //Сохранение размера.
      else if((RS485RxBufUNION.STR.SinhroByte1 == SinhroByte1Const) &&
              (RS485RxBufUNION.STR.SinhroByte2 == SinhroByte2Const) &&
              (RS485RxBufUNION.STR.Type > 0) 											  &&
							(data > 0))
              {RS485RxBufUNION.STR.Size = data;}  
      //Неправильная последовательность байт.  
      else RS485RxClrBuf();//Сброс приемного буфера
    }
  //---------------------------------------------------
  return 0;
}
//-----------------------------------------------------------------------------
uint8_t RS485RxGetDataFromBuf(uint8_t p){
  
  return RS485RxBufUNION.BLK[p];
}
//-----------------------------------------------------------------------------
RS485BufUNION_Type* RS485RxGetBufPtr(void){

	return &RS485RxBufUNION; //Буфер приёма.
}
//-----------------------------------------------------------------------------
uint32_t RS48RxGetTimeUTC(void){

  uint32_t temp;
  //--------------------
  //USART1->CR1 &= ~USART_CR1_RXNEIE; //Отключение прерывания от приемника USART1.	
  temp  =  RS485RxBufUNION.BLK[FP_TimeUTCPoint];
  temp |= (RS485RxBufUNION.BLK[FP_TimeUTCPoint+1] << 8);
  temp |= (RS485RxBufUNION.BLK[FP_TimeUTCPoint+2] << 16);
  temp |= (RS485RxBufUNION.BLK[FP_TimeUTCPoint+3] << 24);
  //USART1->CR1 |= USART_CR1_RXNEIE;  //Включение прерывания от приемника USART1.	
  return temp;
}
//-----------------------------------------------------------------------------
//*********************************************************************************
//*****************************Ф-и для работы ЦП.**********************************

volatile static uint8_t Address  = 1;
volatile static uint8_t Group    = 1;
volatile static uint8_t Priority = 0;

//-----------------------------------------------------------------------------
//Автомат состояний, определяющий порядок выдачи кодограмм.
void RS485MasterLoop(void){
	
  static uint8_t  step    = 0;
  static uint16_t mScount = 0;
	       uint8_t *TxBufPtr;
	       uint8_t  TxBufSize;
  //--------------------
	RS485TimeOutInc();     												//Отсчет таймаута приема пакета.
  if(++mScount < RS485ProtocolLoopPeriod)return;//Формирование паузы между запросами.
  mScount = 0;
  //--------------------
	//Автомат состояний, определяющий порядок выдачи кодограмм в цикле.
	switch(step++){
			//-------------------------
			//Выдача статуса от MB.
			case(0):
        RS485TxStatusMB(&RS485_MB_TxBuf);
				TxBufPtr  = RS485_MB_TxBuf.BLK;
				TxBufSize = RS485_MB_TxBuf.STR.Size;
			break;
			//-------------------------				
			//Запрос статуса к плете управления оповещателями.
			case(1):
				TxBufPtr  = SirenRS485TxBuf()->BLK;
				TxBufSize = SirenRS485TxBuf()->STR.Size;	
			
//        RS485TxRequestSB(&RS485_SB_TxBuf);
//				TxBufPtr  = RS485_SB_TxBuf.BLK;
//				TxBufSize = RS485_SB_TxBuf.STR.Size;
				step = 0;//Переход на следующий шаг.
			break;
			//-------------------------
//			//Запрос статуса к модулю IP.
//      case(2):
//        RS485TxRequestIP(&RS485TxBufUNION);
//        //step = 3;//Переход на следующий шаг.
//			break;
//			//-------------------------
//			//Запрос статуса к модулю рсширения зон.
//			case(3):
//        RS485TxRequestZE(&RS485TxBufUNION);
//				//step = 4;//Переход на следующий шаг.
//			break;
//			//-------------------------				
//			//Запрос статуса к плете интерфейсов.
//			case(4):
//        RS485TxRequestZE(&RS485TxBufUNION);
//				step = 0;//Переход на следующий шаг.
//			break;
//			//-------------------------						
		}
	//------------------------------------------------------------------
  RS485DE(DE);  //Микросхему на передачу.
  Uart3StarTx(TxBufPtr, TxBufSize);
	//DMA1Ch4StartTX(RS485TxBufUNION.BLK, RS485TxBufUNION.STR.Size);
	//------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
//Выдача статуса от MB.
void RS485TxStatusMB(MB_DataUNION_Type *TxBuf){
	
	TxBuf->STR.SinhroByte1 = SinhroByte1Const;
  TxBuf->STR.SinhroByte2 = SinhroByte2Const;
	TxBuf->STR.Type        = MB_TypeConst;
  TxBuf->STR.Size        = 40;                 //Размер всего пакета с CRC.
  TxBuf->STR.CmdCode 		 = CmdMBStatusConst;
  //-----------
  TxBuf->STR.Addres      = Address;
  TxBuf->STR.Group       = Group;
  TxBuf->STR.Priority    = Priority;//Данный байт устанавливается через RS485MB_SetPriority(uint8_t stat)

  TxBuf->STR.StatusZone1 = (LcGetStatus(Lc1Line)  | ZoneGetStatus(Zone1));
  TxBuf->STR.StatusZone2 = (LcGetStatus(Lc1Atten) | ZoneGetStatus(Zone2));
  TxBuf->STR.StatusZone3 = (LcGetStatus(Lc2Line)  | ZoneGetStatus(Zone3));
  TxBuf->STR.StatusZone4 = (LcGetStatus(Lc2Atten) | ZoneGetStatus(Zone4));
  TxBuf->STR.StatusZone5 = ZoneGetStatus(Zone5);
  TxBuf->STR.StatusZone6 = ZoneGetStatus(Zone6);

  TxBuf->STR.StatusPoj1  = PojLineGetStatus(Poj1);
  TxBuf->STR.StatusPoj2  = PojLineGetStatus(Poj2);
  TxBuf->STR.StatusPoj3  = PojLineGetStatus(Poj3);
  TxBuf->STR.StatusGOChS = PojLineGetStatus(ChS);

  TxBuf->STR.StatusPOWER = (PowerBatGetStatus() | PowerGetStatus()); 
  TxBuf->STR.StatusPA    = AmpGetStat();

  TxBuf->STR.TimeUTC     = GetCounterRTC();

  TxBuf->STR.DebugData1       = BatLevel();
  //TxBufUNION.STR.DebugData1   = (uint16_t)(LcGetMeas(4)*5.62)/10;
  TxBuf->STR.DebugData2       = (uint16_t)(LcGetMeas(5)*5.62)/10;
  TxBuf->STR.LcDeviationVolue = LcGetDeviation();
	//-----------
	//Состояние оповещателей.
	TxBuf->STR.SirenPower   = SirenGetStatus()->SirenPower;
	TxBuf->STR.Siren1Status = SirenGetStatus()->Siren1Status;
	TxBuf->STR.Siren2Status = SirenGetStatus()->Siren2Status;
	TxBuf->STR.Siren3Status = SirenGetStatus()->Siren3Status;
  //-----------
	TxBuf->STR.CRC16	      = Get_Crc16(TxBuf->BLK, (TxBuf->STR.Size - 2));
  //-----------
}
//-----------------------------------------------------------------------------
//Запрос статуса к IP-плате.
void RS485TxRequestIP(RS485BufUNION_Type *TxBuf){

  //-----------
  TxBuf->STR.SinhroByte1 = SinhroByte1Const;
  TxBuf->STR.SinhroByte2 = SinhroByte2Const;
  TxBuf->STR.Size        = 7;                 //Размер всего пакета с CRC.
  TxBuf->STR.Type        = IP_TypeConst;
  TxBuf->STR.CmdCode 		 = CmdIPStatusConst;
  //-----------
//  TxBuf->STR.CRC16_H     = (uint8_t)  Get_Crc16(TxBufUNION.BLK, (TxBufUNION.STR.Size - 2));
//  TxBuf->STR.CRC16_L     = (uint8_t)((Get_Crc16(TxBufUNION.BLK, (TxBufUNION.STR.Size - 2)) & 0xFF00) >> 8);
  //-----------
}
//-----------------------------------------------------------------------------
//Запрос статуса к модулю рсширения зон ZE.
void RS485TxRequestZE(RS485BufUNION_Type *TxBuf){

  //-----------
  TxBuf->STR.SinhroByte1 = SinhroByte1Const;
  TxBuf->STR.SinhroByte2 = SinhroByte2Const;
  TxBuf->STR.Size        = 7;                 //Размер всего пакета с CRC.
  TxBuf->STR.Type        = ZE_TypeConst;
  TxBuf->STR.CmdCode 		 = CmdZEStatusConst;
  //-----------
//  TxBuf->STR.CRC16_H     = (uint8_t)  Get_Crc16(TxBufUNION.BLK, (TxBufUNION.STR.Size - 2));
//  TxBuf->STR.CRC16_L     = (uint8_t)((Get_Crc16(TxBufUNION.BLK, (TxBufUNION.STR.Size - 2)) & 0xFF00) >> 8);
  //-----------
}
////-----------------------------------------------------------------------------
////Запрос статуса к плате управления оповещателями.
//void RS485TxRequestSB(SB_DataUNION_Type *TxBuf){

//  //-----------
//  TxBuf->STR.SinhroByte1 = SinhroByte1Const;
//  TxBuf->STR.SinhroByte2 = SinhroByte2Const;
//	TxBuf->STR.Type        = SB_TypeConst;
//  TxBuf->STR.Size        = 19;                 //Размер всего пакета с CRC.
//  TxBuf->STR.CmdCode 		 = SB_CmdControlConst;//SB_CmdStatusConst;
//	
//	//Состояние оповещателей.
////	TxBuf->STR.SirenPower   = 1;
////	TxBuf->STR.Siren1Status = 1;
////	TxBuf->STR.Siren2Status = 3;
////	TxBuf->STR.Siren3Status = 4;
////	
////	TxBuf->STR.DebugData1   = 5;
////	TxBuf->STR.DebugData2   = 6;
//  //-----------
//	TxBuf->STR.CRC16	      = Get_Crc16(TxBuf->BLK, (TxBuf->STR.Size - 2));
//  //-----------
//}
//-----------------------------------------------------------------------------
void SetAddress(uint8_t adr){

  Address = adr;
}
uint8_t GetAddress(void){

  return Address;
}
//-----------------------------------------------------------------------------
void SetGroup(uint8_t grp){

  Group = grp;
}
uint8_t GetGroup(void){

  return Group;
}
//-----------------------------------------------------------------------------
void SetPriority(uint8_t prt){

  Priority = prt;
}
uint8_t GetPriority(void){

  return Priority;
}
//-----------------------------------------------------------------------------
void RS485MB_SetPriority(uint8_t stat){

  RS485TxBufUNION.BLK[PriorityPoint] = stat;
}
//-----------------------------------------------------------------------------
RS485BufUNION_Type* RS485MB_SetTxBuf(void){

  return &RS485TxBufUNION;
}

//*********************************************************************************
//*****************************Ф-и для работы FP.**********************************
//Положение ключа.
//uint8_t KeyState(void){

//  return RS485RxGetDataFromBuf(FP_KeyAndMicStatePoint) & KeyMaskConst;
//}
//-----------------------------------------------------------------------------
//Состояние микрофона.
//uint8_t MicStatus(void){

//  return RS485RxGetDataFromBuf(FP_KeyAndMicStatePoint) & MicMaskConst;
//}
//-----------------------------------------------------------------------------
//Состояние кнопок персетов.
//uint8_t ButPresetState(void){

//  return RS485RxGetDataFromBuf(FP_PresetButtonsPoint);
//}
//-----------------------------------------------------------------------------
//Состояние кнопок управления.
//uint8_t ButControlState(void){

//  return RS485RxGetDataFromBuf(FP_ControlButtonsPoint);
//}
//*********************************************************************************
//*****************************Ф-и для работы c ZE.********************************
//Состояние зоны 1.
uint8_t StatusZone1(void){

  return RS485RxGetDataFromBuf(ZE_StatusZone1Point);
}
//-----------------------------------------------------------------------------
//Состояние зоны 2.
uint8_t StatusZone2(void){

  return RS485RxGetDataFromBuf(ZE_StatusZone2Point);
}
//*********************************************************************************
//*********************************************************************************





