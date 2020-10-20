
#include "Rs485.h"

//-----------------------------------------------------------------------------
static RS485Buf_t		RS485Buf;     //Буфер для работы протокола.
static RS485Flags_t RS485FlagsSTR;//

static volatile uint16_t  RS485ProtocolFlagReg = 0;
static volatile uint16_t  RS485ProtocolTimeOut = 0;
//*********************************************************************************
//*********************************************************************************
void RS485Init(void){

	//DE_RE - PC14 - Управление микросхемой приемо-передатчика.
	GPIOC->CRH &= ~(GPIO_CRH_CNF14);//PC14(DE/RE) - выход, режим - push-pull.
	GPIOC->CRH |=   GPIO_CRH_MODE14;//PC14(DE/RE) - тактирование 50МГц.

	//CRC_Init();    
	Uart1Init();
}
//-----------------------------------------------------------------------------
//Отсчет таймаута приема пакета.
//Если не сбрасывался счетчик в течении RS485ProtocolTimeOut_Const значит небыло запросов/ответов на запросы
//что говорит об отсутсвии связи.
void RS485TimeOutInc(void){
  
	//Сброс счетчика прроисходит по приему запросов/ответов на запросы
  if(RS485ProtocolTimeOut <= RS485ProtocolTimeOut_Const)
		{
			++RS485ProtocolTimeOut;
			RS485FlagsSTR.TimeOutFlag = FlagClr;
		}
  else RS485FlagsSTR.TimeOutFlag = FlagSet; 
}
//-----------------------------------------------------------------------------
RS485Flags_t* RS485Flags(void){
	
	return &RS485FlagsSTR;
}
//-----------------------------------------------------------------------------
uint8_t* RS485Cmd(void){

	static uint8_t CmdReg = 0;
	//--------------------
	return &CmdReg;
}
//-----------------------------------------------------------------------------
//Сброс приемного буфера.
void RS485RxBufClr(void){

  RS485Buf.SinhroByte1 = 0;
  RS485Buf.SinhroByte2 = 0;
	RS485Buf.Type		     = 0;
  RS485Buf.Size		     = 0;
  //RS485Buf.CmdCode     = 0;
} 
//-----------------------------------------------------------------------------
//Прием пакета.
uint8_t RS485RxPacket(uint8_t data){
  
	static uint8_t RS485RxCounterByte = 0;
  //--------------------
  RS485RxCounterByte++;
	if(RS485Buf.Size != 0)
    {
      RS485Buf.Data.Buf[RS485RxCounterByte - RS485_BUF_HEADER_SIZE - 1] = data;//Прнятый байт в приемный буфер.
      //-------------------------
      if(RS485RxCounterByte >= RS485Buf.Size)//Принято нужное количество байт.
        {	  
					RS485RxCounterByte = 0;
					//Проверка CRC.
					if(*(uint16_t*)((uint8_t*)&RS485Buf + RS485Buf.Size - 2) ==
						    Get_Crc16((uint8_t*)&RS485Buf, (RS485Buf.Size - 2)))
            {  
              //Смотрим, от кого пришли данные.
              switch(RS485Buf.Type){
                  //-----------
                  case(MB_TypeConst)://Данные ЦП.
										RS485FlagsSTR.MBNewDataFlag = FlagSet;
									break;
									//-----------
                  case(FP_TYPE)://Данные ЦП.
										RS485FlagsSTR.MBNewDataFlag = FlagSet;
									break;
                  //-----------								
                  default: //Неизвестный тип устройства.
                    RS485RxBufClr(); //Сброс приемного буфера.	
                  return 0;
                  //-----------
                }
              //------------------------- 
              RS485ProtocolTimeOut = 0;//Сброс счетчика таймаута.
              return 1;
            }
          //-------------------------
          //CRC не совпало. 			
					RS485RxBufClr(); //Сброс приемного буфера.
        }
      //-------------------------
    }
  //---------------------------------------------------
  else
    {	
      //Сохранение синхробайта 1.
					 if((RS485Buf.SinhroByte1 != SinhroByte1Const) &&
              (RS485Buf.SinhroByte2 != SinhroByte2Const) &&
              (RS485Buf.Type        == 0)                && //!=MB_TypeConst)     &&
							(data 								== SinhroByte1Const))
							{RS485Buf.SinhroByte1  = data;}
      //Сохранение синхробайта 2.
		  else if((RS485Buf.SinhroByte1 == SinhroByte1Const) &&
              (RS485Buf.SinhroByte2 != SinhroByte2Const) &&
              (RS485Buf.Type        == 0)                && //!=MB_TypeConst)     &&
						  (data 								== SinhroByte2Const))
						  {RS485Buf.SinhroByte2  = data;}
      //Сохранение типа.
      else if((RS485Buf.SinhroByte1 == SinhroByte1Const) &&
              (RS485Buf.SinhroByte2 == SinhroByte2Const) &&
              (RS485Buf.Type        == 0)                && //!=MB_TypeConst)     &&
							(data != 0))
              {RS485Buf.Type = data;}
      //Сохранение размера.
      else if((RS485Buf.SinhroByte1 == SinhroByte1Const) &&
              (RS485Buf.SinhroByte2 == SinhroByte2Const) &&
              (RS485Buf.Type != 0)                       && //== MB_TypeConst)     &&
							(data != 0))
              {RS485Buf.Size = data;} 
		  //Сохранение команды. Не отлажено!!!!!
//      else if((RS485Buf.SinhroByte1 == SinhroByte1Const) &&
//              (RS485Buf.SinhroByte2 == SinhroByte2Const) &&
//              (RS485Buf.Type != 0)                       && //== MB_TypeConst)     &&
//							(RS485Buf.Size != 0)											 &&
//							(data != 0))
//              {RS485Buf.CmdCode = data;} 
      //Неправильная последовательность байт.  
      else 
				{
					RS485RxBufClr();//Сброс приемного буфера
					RS485RxCounterByte = 0;
				}
      //-------------------------
    }
  //---------------------------------------------------
  return 0;
}
//-----------------------------------------------------------------------------
//Получение указателя приемный буфере.
RS485Buf_t* RS485RxBufPtr(void){

  return &RS485Buf;
}
//-----------------------------------------------------------------------------
RS485Buf_t* RS485TxBufPtr(void){

  return &RS485Buf;
}
//-----------------------------------------------------------------------------
static void RS485StartTx(uint8_t* buf, uint16_t size){
	
	RS485DirectionTX();//Микросхему на передачу.
	//UsartStarTx(USART1, buf, size);
	DMA1Ch4StartTx(buf, size);	
}
//-----------------------------------------------------------------------------
//Ответ на запрос.
void RS485StartTxResponse(void){	   
	
	RS485Buf.SinhroByte1 = SinhroByte1Const;
	RS485Buf.SinhroByte2 = SinhroByte2Const;
	RS485Buf.Type		     = FP_TYPE;
  RS485Buf.Size		     = (uint8_t)(sizeof(RS485Buf.Data.FP) + RS485_BUF_HEADER_SIZE);
	//RS485RxBuf.CmdCode     = CmdFPStatusConst;
	//-----------
	RS485Buf.Data.FP.CRC16 = Get_Crc16((uint8_t*)&RS485Buf, (RS485Buf.Size - 2));
	//--------------------
  //Запуск передачи.
	RS485StartTx((uint8_t*)&RS485Buf, RS485Buf.Size);
}
//-----------------------------------------------------------------------------
























