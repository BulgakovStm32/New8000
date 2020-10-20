
#include "uart_ST.h"

//-----------------------------------------------------------------------------
//Буфер и переменные дла работы протакола.

#define RS485RxBufSizeMax  35

union {
	struct{
    //--------------------
    uint8_t  SinhroByte1;   // Синхробайт.
    uint8_t  SinhroByte2;   // Синхробайт.
    uint8_t  Size;          // Размер пакета.
    uint8_t  Type;          // Тип модуля.
    uint8_t  CommandCode;   // Код команды.
    //--------------------
    //Данные.
    uint8_t  Data[RS485RxBufSizeMax-5];         
    //--------------------
		  }STR;
	uint8_t BLK[RS485RxBufSizeMax];
}                 RS485RxBufUNION;
volatile uint16_t RS485RxCounterByte   = 0;
volatile uint16_t RS485ProtocolFlagReg = 0;
volatile uint16_t RS485ProtocolTimeOut = 0;
//-----------------------------------------------------------------------------
//Буфер и переменные для Передачи.
MBDataUNION_TypeDef    TxBufUNION;       //Буфер передачи.
volatile uint16_t      TxCounterByte = 0;//Счетчик переданных байт.
volatile uint8_t	     TxBufSize	   = 0;//
volatile uint8_t      *TxBufPointer;     //Указатель на передающий буфер.
//*********************************************************************************
//************************Аппаратный уровень.**************************************
//-----------------------------------------------------------------------------
////Буфер и переменные для Приема.
//RxBufferUNION_TypeDef RxBufferUNION;
//volatile uint16_t     RxCounterByte = 0;
////----------------------------------------
////Буфер и переменные для Передачи.
//MBDataUNION_TypeDef   MBDataUNION; 
//volatile uint16_t TxCounterByte = 0;	//Счетчик переданных байт.
//volatile uint8_t	TxBufSize	    = 0;
//volatile uint8_t *TxBufPointer;
////----------------------------------------

//volatile uint16_t RS485ProtocolFlagReg = 0;
//volatile uint16_t RS485ProtocolTimeOut = 0;
//-----------------------------------------------------------------------------
void Uart1Init(void){
  
 	//Инициализация USART1. PA9(U1TX), PA10(U1RX), PA12(U1RTS - DE/RE), PB10(RS485ActLed).
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN ; //Включение тактирование USART1.
  
	GPIOA->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_CNF10 | GPIO_CRH_CNF12);	
	
	GPIOA->CRH |= GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9;//PA9(U1TX) - выход, альтернативный режим push-pull.
																								 //PA9(U1TX) - тактирование 50МГц.
	GPIOA->CRH |= GPIO_CRH_CNF10_0;								 //PA10(U1RX) - Floating input.
	GPIOA->CRH |= GPIO_CRH_MODE12;								 //PA12(U1RTS - DE/RE) - выход, режим - push-pull.
																								 //PA12(U1RTS - DE/RE) - тактирование 50МГц.		
	GPIOB->CRH &= ~GPIO_CRH_CNF10;  							 //PB10(RS485ActLed) - выход, режим - push-pull.
  GPIOB->CRH |= GPIO_CRH_MODE10;  							 //PB10(RS485ActLed) - тактирование 50МГц. 

  USART1->CR1 &= ~USART_CR1_M; 		  //8 бит данных.
  USART1->CR2 &= ~USART_CR2_STOP;   //1 стоп-бит.
  USART1->BRR =   0x0D0;  				  //BaudRate 115200.
  
//  USART1->CR1 = USART_CR1_RE |      //Включение RX USART1.	
//                USART_CR1_TE |      //Включение TX USART1.	
//                USART_CR1_UE;       //Включение USART1.


  USART1->CR1 = USART_CR1_RE    | //Включение RX USART1.	
                //USART_CR1_TE    | //Включение TX USART1.
                USART_CR1_RXNEIE| //Включение прерывания от приемника USART1.		
                USART_CR1_UE;     //Включение USART1.
                
	NVIC_SetPriority(USART1_IRQn, 14);//Приоритет прерывания USART1.
	NVIC_EnableIRQ(USART1_IRQn);      //Разрешаем прерывание от приемника USART1.                
  //--------------------
  //Инициализация DMA1.
//  if (!(RCC->AHBENR & RCC_AHBENR_DMA1EN)) RCC->AHBENR |= RCC_AHBENR_DMA1EN;//Включить тактирование DMA1
//  
//  DMA1Ch4InitForTX();  //DMA1Ch4 - 
//  DMA1Ch5InitForRX();  //DMA1Ch5 - 
//  
//  USART1->CR3 |= (USART_CR3_DMAT | USART_CR3_DMAR);//разрешить передачу и прием USART1 через DMA.
}
//-----------------------------------------------------------------------------
//Передача буфера.
void Uart1StarTx(uint8_t *TxBuf, uint8_t size){

  //RS485DEOn;                    //Микросхему на передачу.
  //----------------
	TxCounterByte = 0;						//Cброс счетчика байт.
	TxBufSize	    = size;         //Количество байт на передачу.	
  TxBufPointer  = TxBuf;        //Указатель на передающий буфер.
  //----------------
  USART1->CR1  &= ~USART_CR1_RE; //Отлючение RX USART1.
	USART1->DR    = *TxBuf;        //Передача первого байта.
	USART1->CR1  |= USART_CR1_TE  |//Включение TX USART1.
                  USART_CR1_TCIE;//Включение прерывания от TX USART1.
}
//-----------------------------------------------------------------------------
//Прерывание от USART1.
void USART1_IRQHandler(void){

  //--------------------
	/*!< Read Data Register Not Empty */
	if (USART1->SR & USART_SR_RXNE)
		{
      //проверяем нет ли ошибок при приеме байта.
      if ((USART1->SR & (USART_SR_NE | USART_SR_FE | USART_SR_PE | USART_SR_ORE)) == 0)
        {
          //Принимаем запрос от МВ.
          if(RS485RxPacket((uint8_t)USART1->DR)) USART1->CR1 &= ~USART_CR1_RE;//Отлючение RX USART1.                       
          //else  return;
        }
      //--------------------
      //Если байт битый, то пропускаем его и 
      //очищаем приемный буфер для запуска приема нового пакета.
      else
        {
          USART1->SR;//Такая последовательность сбрасывает флаги ошибки 
          USART1->DR;//
          RS485RxClrBuf(); 
        }
      //--------------------
		}
  //--------------------
  /*!< Overrun error */
  if (USART1->SR & USART_SR_ORE)
		{
      USART1->SR;//Такая последовательность сбрасывает флаги ошибки 
      USART1->DR;//
      RS485RxClrBuf(); 
    }
  //--------------------
  	/*!< Transmission Complete */
	if (USART1->SR & USART_SR_TC)
		{
      if(++TxCounterByte == TxBufSize)
        {
           //Запуск приема нового пакета.        
           USART1->CR1 &= ~USART_CR1_TE;  //USART_CR1_TCIE;//Отлючение прерывания по окончании передачи.  USART1.     
           USART1->CR1 |=  USART_CR1_RE;  //Включение RX USART1.
           RS485DE(RE);
           //RS485REOn;       //Микросхему на прием.      
        }
      //Передача очередного байта.
      else USART1->DR = *(TxBufPointer + TxCounterByte);
      //---------
      //Настройка ДМА на прием данных из USART1.
      //StartDMA1Ch5ForRX(ResponseUNION.BLK, 12);
      //---------      
		}
  //-------------------- 
 	/*!< Transmit Data Register Empty */
  if (USART1->SR & USART_SR_TXE)
 		{
      //USART1->SR &= ~USART_SR_TXE; //Сброс флага прерывания.
      //---------
 			
 		}
	//--------------------
}
//*********************************************************************************
//*********************Прикладной уровень. Общие функции.**************************
//Управление микросхемой приемо-передатчика.
void RS485DE(uint8_t de){
  
  if(de == DE)RS485DEOn;
  else        RS485REOn;
}
//-----------------------------------------------------------------------------
//Отсчет таймаута приема пакета.
void RS485TimeOutInc(void){

//  if( (RS485ProtocolTimeOut <= 1000) &&
//     !(RS485ProtocolFlagReg & RS485_TimeOutFlag))
  if(RS485ProtocolTimeOut <= 1000)
    {
       ++RS485ProtocolTimeOut;
       RS485ProtocolFlagReg &= ~RS485_TimeOutFlag;
    }
  //Если не сбрасывался счетчик в течении 1с значит небыло запросов
  //что говорит об отсутсвии связи.
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

  RS485RxCounterByte = 0;
  //Сброс CRC.
  RS485RxBufUNION.BLK[RS485RxBufUNION.STR.Size - 2] = 0;
  RS485RxBufUNION.BLK[RS485RxBufUNION.STR.Size - 1] = 0;

  RS485RxBufUNION.STR.SinhroByte1 = 0;
  RS485RxBufUNION.STR.SinhroByte2 = 0;
  RS485RxBufUNION.STR.Size		    = 0;
  RS485RxBufUNION.STR.Type		    = 0;
  RS485RxBufUNION.STR.CommandCode = 0;
} 
//-----------------------------------------------------------------------------
//Прием пакета.
uint8_t RS485RxPacket(uint8_t data){
  
  uint16_t CRCtemp = 0;	
  //--------------------
  //Если не обработан старый пакет то ничего не принемаем.
  if(RS485ProtocolFlagReg & RS485_MBNewDataFlag) return 0;
  //--------------------
  RS485RxCounterByte++;
  if(RS485RxBufUNION.STR.Size != 0)
    {
      RS485RxBufUNION.BLK[RS485RxCounterByte-1] = data;//Прнятый байт в приемный буфер.
      //-------------------------
      if(RS485RxCounterByte >= RS485RxBufUNION.STR.Size)//Принято нужное количество байт.
        {	  
          CRCtemp = Get_Crc16(RS485RxBufUNION.BLK, (RS485RxBufUNION.STR.Size - 2));//Проверка CRC.
          if(((uint8_t)(CRCtemp >> 8)     == RS485RxBufUNION.BLK[RS485RxBufUNION.STR.Size - 1]) &&
             ((uint8_t)(CRCtemp & 0x00FF) == RS485RxBufUNION.BLK[RS485RxBufUNION.STR.Size - 2]))
            {  
              //Смотрим, от кого пришли данные.
              switch(RS485RxBufUNION.STR.Type){
                  //-----------
                  case(MB_TypeConst)://Данные ЦП.
                   RS485ProtocolTimeOut  = 0;//Сброс счетчика таймаут
                   RS485ProtocolFlagReg |= RS485_MBNewDataFlag;//Выставление флага что есть данные от МВ.
                   //RS485FP_TxStarResponse();//Запуск передачи ответа на запрос. 
                  break;
                  //-----------
                  case(FP_TypeConst)://Данные FP.
                   RS485ProtocolTimeOut  = 0;//Сброс счетчика таймаут
                   RS485ProtocolFlagReg |= RS485_FPNewDataFlag;
                  break;
                  //-----------
                  //case():
                  //break;
                  //-----------
                  default: //Неизвестные данные.
                   RS485RxClrBuf(); //Сброс приемного буфера.	
                  return 0;
                  //-----------
                }
              //------------------------- 
              RS485ActLedToggel;
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
           if((data == SinhroByte1Const) &&
              (RS485RxBufUNION.STR.SinhroByte1 != SinhroByte1Const))
              {RS485RxBufUNION.STR.SinhroByte1  = data;}
      //Сохранение синхробайта 2.
      else if((data == SinhroByte2Const) &&
              (RS485RxBufUNION.STR.SinhroByte1 == SinhroByte1Const))
              {RS485RxBufUNION.STR.SinhroByte2  = data;}
      //Сохранение размера.
      else if((data != 0) &&
              (RS485RxBufUNION.STR.SinhroByte1 == SinhroByte1Const) &&
              (RS485RxBufUNION.STR.SinhroByte2 == SinhroByte2Const))
              {RS485RxBufUNION.STR.Size         = data;} 
      //Сохранение типа.
//      else if((data == MB_TypeConst) &&
//              (RS485RxBufUNION.STR.SinhroByte1 == SinhroByte1Const) &&
//              (RS485RxBufUNION.STR.SinhroByte2 == SinhroByte2Const) &&
//              (RS485RxBufUNION.STR.Size        != 0))
//              {RS485RxBufUNION.STR.Type         = MB_TypeConst;}  
      //Неправильная последовательность байт.  
      else RS485RxClrBuf();//Сброс приемного буфера
      //-------------------------
    }
  //---------------------------------------------------
  return 0;
}
//-----------------------------------------------------------------------------
uint8_t RS485RxGetDataFromBuf(uint8_t p){
  
  return RS485RxBufUNION.BLK[p];
}
//*********************************************************************************
//*****************************Ф-и для работы ЦП.**********************************
//Автомат состояний, определяющий порядок выдачи кодограмм.
void RS485MB_ProtocolLoop(void){
	
  static uint8_t step = 0;
  //------------------------------------------------------------------
	//Автомат состояний, определяющий порядок выдачи кодограмм в цикле.	
	switch(step)
		{
			//---------------------------------------------------
			//Выдача статуса от MB.
			case (0):
				//-----------
        TxBufUNION.STR.SinhroByte1  = SinhroByte1Const;
        TxBufUNION.STR.SinhroByte2  = SinhroByte2Const;
        TxBufUNION.STR.Size         = 26;                 //Размер всего пакета с CRC.
        TxBufUNION.STR.Type         = MB_TypeConst;
        TxBufUNION.STR.CommandCode  = CmdFromMBStatusConst;
        //-----------
        TxBufUNION.STR.Addres       = 10;//(uint8_t)EepGetNumberBadSectors();
        TxBufUNION.STR.Group        = 11;//(uint8_t)((EepGetNumberBadSectors() & 0xFF00) >> 8);
        TxBufUNION.STR.Priority     = 12;
      
        TxBufUNION.STR.StatusZone1  = LcGetState(Lc1Line);
        TxBufUNION.STR.StatusZone2  = LcGetState(Lc1Atten);
        TxBufUNION.STR.StatusZone3  = 88;//LcGetStateValue(Lc1Line +4);
        TxBufUNION.STR.StatusZone4  = 88;//LcGetStateValue(Lc1Atten+4);
        TxBufUNION.STR.StatusZone5  = 88;
        TxBufUNION.STR.StatusZone6  = 88;
      
        TxBufUNION.STR.StatusPoj1   = PojLineGetState(Poj1);
        TxBufUNION.STR.StatusPoj2   = PojLineGetState(Poj2);
        TxBufUNION.STR.StatusPoj3   = PojLineGetState(Poj3);
        TxBufUNION.STR.StatusGOChS  = PojLineGetState(ChS);
    
        TxBufUNION.STR.StatusPOWER  = 0x11;
        TxBufUNION.STR.StatusPA     = 2;
        
        TxBufUNION.STR.TimeUTC      = GetCounterRTC();
        //-----------
        //MB_StatusUNION.STR.Debug        = EepGetNumberBadSectors();
        //-----------
        TxBufUNION.STR.CRC16_H      = (uint8_t)  Get_Crc16(TxBufUNION.BLK, (TxBufUNION.STR.Size - 2));
        TxBufUNION.STR.CRC16_L      = (uint8_t)((Get_Crc16(TxBufUNION.BLK, (TxBufUNION.STR.Size - 2)) & 0xFF00) >> 8);
        //-------------------------
				//Переход на следующий шаг.
				step = 0;//step = 1;
				//-------------------------
			break;
			//---------------------------------------------------
			//Передача запросов GetStat. Всего 73 запроса.
			case (1):
	
        //--------------------------------------------------
				//Переход на следующий шаг.
				step = 0;
        //-------------------------
			break;
			//---------------------------------------------------				
		}
	//------------------------------------------------------------------
  //StartDMA1Ch4ForTX(MB_StatusUNION.BLK, MB_StatusUNION.STR.Size);
  RS485DE(DE);  //Микросхему на передачу.
  Uart1StarTx(TxBufUNION.BLK, TxBufUNION.STR.Size);
	//------------------------------------------------------------------
}
//*********************************************************************************
//*****************************Ф-и для работы FP.**********************************
//volatile  FPDataUNION_TypeDef* RS485FP_SetParam(void){

//  return &TxBufUNION;
//}
////-----------------------------------------------------------------------------
//void RS485FP_TxStarResponse(void){	   
//	
//  //--------------------
//	//Ответ на запрос.
//	TxBufUNION.STR.SinhroByte1 = SinhroByte1Const;
//	TxBufUNION.STR.SinhroByte2 = SinhroByte2Const;
//  TxBufUNION.STR.Size		     = 16;
//	TxBufUNION.STR.Type		     = FP_TypeConst;
//	TxBufUNION.STR.Command_Code = CmdFromFPStatusFPConst;
//	//-----------
//  //Остальные значения устанавливаются через RS485SetStateFP()

////	TxBufferUNION.STR.Addres          = 0;
////	TxBufferUNION.STR.Group	          = 0;
////	TxBufferUNION.STR.Zone	          = 0;
////	TxBufferUNION.STR.Control_Buttons = 0; //GetButton();
////	TxBufferUNION.STR.Preset_Buttons	= Sm1628GetToggleButton(PRESET_BUTTONS);//0;
////	TxBufferUNION.STR.TimeUTC         = 0;
//  //-----------
//	TxBufUNION.STR.CRC16 = Get_Crc16(TxBufUNION.BLK, (TxBufUNION.STR.Size - 2));
//	//--------------------
//  //Запуск передачи.
//  RS485DE(DE);  //Микросхему на передачу.
//  //RS485DEOn;
//  Uart1StarTx(&TxBufUNION.BLK[0], TxBufUNION.STR.Size);	
//}
////-----------------------------------------------------------------------------
//uint32_t RS485FP_GetTimeUTCFromMB(void){

//  uint32_t temp;
//  //--------------------
//  //USART1->CR1 &= ~USART_CR1_RXNEIE; //Отключение прерывания от приемника USART1.	
//  temp  =  RS485RxBufUNION.BLK[TimeUTCPoint];
//  temp |= (RS485RxBufUNION.BLK[TimeUTCPoint+1] << 8);
//  temp |= (RS485RxBufUNION.BLK[TimeUTCPoint+2] << 16);
//  temp |= (RS485RxBufUNION.BLK[TimeUTCPoint+3] << 24);
//  //USART1->CR1 |= USART_CR1_RXNEIE;  //Включение прерывания от приемника USART1.	
//  return temp;
//}
//-----------------------------------------------------------------------------


//************************************************************************************************** 
//**************************************************************************************************
//************************************************************************************************** 
//**************************************************************************************************
//Инициализация канала 4 DMA1 передачи данных через USART1.   
void DMA1Ch4InitForTX(void){
  
  //Задать адрес источника и приемника и количество данных для обмена
  DMA1_Channel4->CPAR  =  (uint32_t)&USART1->DR;   //адрес регистра перефирии
  
  //DMA1_Channel4->CMAR  =  (uint32_t)&MB_StatusSTR.BLK;   //адрес буфера в памяти
  //DMA1_Channel4->CNDTR =  31;                      //количество данных для обмена
  //----------------- Манипуляции с регистром конфигурации  ----------------
  //Следующие действия можно обьединить в одну команду (разбито для наглядности)
  DMA1_Channel4->CCR  =  0;                       //предочистка регистра конфигурации
  DMA1_Channel4->CCR &= ~DMA_CCR4_CIRC;           //выключить циклический режим
  DMA1_Channel4->CCR |=  DMA_CCR4_DIR;            //направление: чтение из памяти
  //Настроить работу с переферийным устройством
  DMA1_Channel4->CCR &= ~DMA_CCR4_PSIZE;          //размерность данных 8 бит
  DMA1_Channel4->CCR &= ~DMA_CCR4_PINC;           //неиспользовать инкремент указателя
  //Настроить работу с памятью
  DMA1_Channel4->CCR &= ~DMA_CCR4_MSIZE;          //размерность данных 8 бит
  DMA1_Channel4->CCR |=  DMA_CCR4_MINC;           //использовать инкремент указателя

  DMA1_Channel4->CCR |= DMA_CCR4_TCIE; //Разрешить прерывание по завершении обмена
  NVIC_EnableIRQ (DMA1_Channel4_IRQn); //Разрешить прерывания от DMA канал №4  
}
//-----------------------------------------------------------------------------
//Инициализация канала 5 DMA1 для приема данных из USART1.   
void DMA1Ch5InitForRX(void){
  
  //DMA1_Channel5->CMAR = (uint32_t) &RxBuff[0];//адрес буфера приемника.
  DMA1_Channel5->CPAR = (uint32_t)&USART1->DR;  //адрес регистра данных приемника.
  //DMA1_Channel5->CNDTR = 64;                  //количество данных для приема.
  //--------------------
  //Настроить работу с переферийным устройством
  DMA1_Channel5->CCR  =  0;                 //предочистка регистра конфигурации
  DMA1_Channel5->CCR &= ~DMA_CCR5_CIRC;     //выключить циклический режим
  DMA1_Channel5->CCR &= ~DMA_CCR5_DIR;      //направление: чтение из перифферии.
  DMA1_Channel5->CCR &= ~DMA_CCR5_PSIZE;    //размерность данных 8 бит
  DMA1_Channel5->CCR &= ~DMA_CCR5_PINC;     //неиспользовать инкремент указателя
  //--------------------
  //Настроить работу с памятью
  DMA1_Channel5->CCR &= ~DMA_CCR5_MSIZE;    //размерность данных 8 бит
  DMA1_Channel5->CCR |=  DMA_CCR5_MINC;     //использовать инкремент указателя
  //--------------------
  DMA1_Channel5->CCR |= DMA_CCR5_TCIE;      //Разрешить прерывание по завершении обмена
  NVIC_EnableIRQ(DMA1_Channel5_IRQn);       //Разрешить прерывания от DMA канал №5
}
//-----------------------------------------------------------------------------
//Проверка флага окончания обмена в канале "память-DMA-USART1"      
//Result  : 0 - обмен не закончен; 1 - обмен закончен;  
uint8_t GetStateDMAChannel4(void){
  
  if(DMA1->ISR & DMA_ISR_TCIF4) return 1;   //обмен окончен
  return 0;                                 //обмен продолжается
}
//-----------------------------------------------------------------------------
//Старт обмена в канале "память-DMA-USART1"                                                                   
void StartDMA1Ch4ForTX(uint8_t *BuffAdr, uint16_t LengthBufer){
  
  RS485DEOn;           //Микросхему на передачу.
  //--------------------
  DMA1_Channel4->CCR  &= ~DMA_CCR4_EN;      //запретить работу канала
  DMA1_Channel4->CMAR  = (uint32_t)BuffAdr; //адрес буфера в памяти
  DMA1_Channel4->CNDTR = LengthBufer;       //загрузить количество данных для обмена
  
  //DMA1_Channel4->CCR |= DMA_CCR4_TCIE;//канал 4
  //DMA1->IFCR          |= DMA_IFCR_CTCIF4;//сбросить флаг окончания обмена
  
  DMA1_Channel4->CCR  |= DMA_CCR4_EN;//разрешить работу канала
}
//-----------------------------------------------------------------------------
//Старт канала 5 DMA1 для приема данных из USART1.                                                            
void StartDMA1Ch5ForRX(uint8_t *BuffAdr, uint16_t LengthBufer){
  
  //RS485REOn;           //Микросхему на прием.
  //--------------------
  DMA1_Channel5->CCR  &= ~DMA_CCR5_EN;     //запретить работу канала
  DMA1_Channel5->CMAR  = (uint32_t)BuffAdr;//адрес буфера приемника.
  DMA1_Channel5->CNDTR = LengthBufer;      //загрузить количество данных для обмена
  
  //DMA1_Channel4->CCR |= DMA_CCR4_TCIE;//канал 4
  //DMA1->IFCR         |= DMA_IFCR_CTCIF4;//сбросить флаг окончания обмена
  
  DMA1_Channel5->CCR  |= DMA_CCR5_EN;//разрешить работу канала
}
//-----------------------------------------------------------------------------
//Прерываение от DMA1_Ch4.
void DMA1_Channel4_IRQHandler(void){
  
  //-------------------------
  //Если обмен завершен
  if(DMA1->ISR & DMA_ISR_TCIF4)
    {
      DMA1_Channel4->CCR  &= ~DMA_CCR4_EN;//запретить работу канала.
      DMA1->IFCR |= DMA_IFCR_CTCIF4;      //сбросить флаг окончания обмена.
      //---------
      //Это нужно что бы дождаться завершения передачи,
      //последнего байта и перейти на прием данных.
      USART1->CR1 |= USART_CR1_TCIE;		  //Включение прерывания по окончанию передачи USART1.	
      //NVIC_SetPriority(USART1_IRQn, 14);//риоритет рерывания USART1.
      NVIC_EnableIRQ(USART1_IRQn);       //Разрешаем прерывание от приемника USART1.
      //---------
      //RS485ActLedToggel;//Моргаем светодиодом.
    }      
  //-------------------------
  //Если передана половина буфера
  if(DMA1->ISR & DMA_ISR_HTIF4)
    {

    }  
  //-------------------------
  //Если произошла ошибка при обмене
  if(DMA1->ISR & DMA_ISR_TEIF4)
    {

    }   
  //-------------------------
}
//-----------------------------------------------------------------------------
//Прерываени от DMA1_Ch5.
void DMA1_Channel5_IRQHandler(void){
   
  //static uint8_t LedState = 0;
  //uint16_t CRCtemp = 0;
  //-------------------------
  //Если обмен завершен
  if(DMA1->ISR & DMA_ISR_TCIF5)
    {
      DMA1_Channel5->CCR  &= ~DMA_CCR5_EN;//запретить работу канала.
      DMA1->IFCR |= DMA_IFCR_CTCIF5;      //сбросить флаг окончания обмена.
      //-------------------------

      



      //Анализ ответа от MB.
//      CRCtemp = Get_Crc16(ReceiverBufferSTR.BLK, (ReceiverBufferSTR.STR.Size - 2));
//      if (((uint8_t)(CRCtemp >> 8)     == ReceiverBufferSTR.BLK[ReceiverBufferSTR.STR.Size ]) &&
//          ((uint8_t)(CRCtemp & 0x00FF) == ReceiverBufferSTR.BLK[ReceiverBufferSTR.STR.Size - 1]))
//        {
////          LedState ^= LedGreen;            
////          CommunicationLedSet(LedState);     //Моргаем светодиодом.

////          RS485ClearReceiverBuffer();
////          NVIC_EnableIRQ(USART1_IRQn);//Разрешаем прерывание от приемника USART1.

//          RS485SetFlagReg(RS485_MB_Flag);
//        }
      //-------------------------
    }      
  //-------------------------
  //Если передана половина буфера
  if(DMA1->ISR & DMA_ISR_HTIF5)
    {

    }  
  //-------------------------
  //Если произошла ошибка при обмене
  if(DMA1->ISR & DMA_ISR_TEIF5)
    {

    }   
  //-------------------------
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//************************************************************************************************** 
//**************************************************************************************************
//************************************************************************************************** 
//**************************************************************************************************

////Автомат состояний, определяющий порядок выдачи кодограмм.
//void RS485ProtocolLoop(void){
//	
//  static uint8_t step = 0;
//  //-------------------------
//  //Сброс флага присутствия лиц.панели по таймауту.
//  if(++RS485ProtocolTimeOut >= 1000)
//    {
//      RS485ProtocolTimeOut = 0;
//      RS485ProtocolFlagReg |= RS485_FPErrorFlag;
//      step = 0;
//      return;
//    }
//  //------------------------------------------------------------------
//	//Автомат состояний, определяющий порядок выдачи кодограмм в цикле.	
//	switch(step)
//		{
//			//---------------------------------------------------
//			//Выдача статуса от MB.
//			case (0):
//				//-----------
//        MBDataUNION.STR.SinhroByte1  = SinhroByte1Const;
//        MBDataUNION.STR.SinhroByte2  = SinhroByte2Const;
//        MBDataUNION.STR.Size         = 26;                 //Размер всего пакета с CRC.
//        MBDataUNION.STR.Type         = MB_ModuleTypeConst;
//        MBDataUNION.STR.ComandCode   = MB_ModuleCmdRetStatConst;
//        //-----------
//        MBDataUNION.STR.Addres       = 10;//(uint8_t)EepGetNumberBadSectors();
//        MBDataUNION.STR.Group        = 11;//(uint8_t)((EepGetNumberBadSectors() & 0xFF00) >> 8);
//        MBDataUNION.STR.SubPriority  = 12;
//      
//        MBDataUNION.STR.StatusZone1  = 10;//LcGetState(Lc1Line);
//        MBDataUNION.STR.StatusZone2  = 11;//LcGetState(Lc1Atten);
//        MBDataUNION.STR.StatusZone3  = 12;//LcGetStateValue(Lc1Line +4);
//        MBDataUNION.STR.StatusZone4  = 14;//LcGetStateValue(Lc1Atten+4);
//        MBDataUNION.STR.StatusZone5  = 88;
//        MBDataUNION.STR.StatusZone6  = 88;
//      
//        MBDataUNION.STR.StatusPoj1   = NormCloseSost;//GetPojLineState(Poj1);
//        MBDataUNION.STR.StatusPoj2   = NormCloseSost;//GetPojLineState(Poj2);
//        MBDataUNION.STR.StatusPoj3   = NormCloseSost;//GetPojLineState(Poj3);
//        MBDataUNION.STR.StatusGOChS  = NormCloseSost;//GetPojLineState(ChS);
//    
//        MBDataUNION.STR.StatusPOWER  = 0;
//        MBDataUNION.STR.StatusPA     = 0;
//        
//        MBDataUNION.STR.TimeUTC      = GetCounterRTC();
//        //-----------
//        //MB_StatusUNION.STR.Debug        = EepGetNumberBadSectors();
//        //-----------
//        MBDataUNION.STR.CRC16_H      = (uint8_t)  Get_Crc16(MBDataUNION.BLK, (MBDataUNION.STR.Size - 2));
//        MBDataUNION.STR.CRC16_L      = (uint8_t)((Get_Crc16(MBDataUNION.BLK, (MBDataUNION.STR.Size - 2)) & 0xFF00) >> 8);
//        //-------------------------
//				//Переход на следующий шаг.
//				step = 0;//step = 1;
//				//-------------------------
//			break;
//			//---------------------------------------------------
//			//Передача запросов GetStat. Всего 73 запроса.
//			case (1):
//	
//        //--------------------------------------------------
//				//Переход на следующий шаг.
//				step = 0;
//        //-------------------------
//			break;
//			//---------------------------------------------------				
//		}
//	//------------------------------------------------------------------
//  //StartDMA1Ch4ForTX(MB_StatusUNION.BLK, MB_StatusUNION.STR.Size);
//  RS485StarTx(MBDataUNION.BLK, MBDataUNION.STR.Size);
//	//------------------------------------------------------------------
//}
//-----------------------------------------------------------------------------
//void RS485StarTx(uint8_t *TxBuf, uint8_t size){

//  RS485DEOn;                       //Микросхему на передачу.
//  //----------------
//	TxCounterByte = 0;						   //Cброс счетчика байт.
//	TxBufSize	    = size;            //Количество байт на передачу.	
//  TxBufPointer  = TxBuf;           //Указатель на передающий буфер.
//  //----------------
//  USART1->CR1  &= ~(USART_CR1_RE);//Отлючение RX USART1.
//	USART1->DR    = *TxBuf;         //Передача первого байта.
//	USART1->CR1  |= USART_CR1_TE  | //Включение TX USART1.
//                  USART_CR1_TCIE; //Включение прерывания от TX USART1.
//}
////-----------------------------------------------------------------------------
//void RS485ClearRxBuf(void){

//  RxCounterByte = 0;
//  //Сброс CRC.
//  RxBufferUNION.BLK[RxBufferUNION.STR.Size - 2] = 0;
//  RxBufferUNION.BLK[RxBufferUNION.STR.Size - 1] = 0;

//  RxBufferUNION.STR.SinhroByte1 = 0;
//  RxBufferUNION.STR.SinhroByte2 = 0;
//  RxBufferUNION.STR.Size		    = 0;
//  RxBufferUNION.STR.Type		    = 0;
//  //ReceiverBufferSTR.STR.CommandCode = 0;
//} 
////-----------------------------------------------------------------------------
////Прием пакета.
//uint8_t RS485RxPacket(uint8_t data){
//  
//  uint16_t CRCtemp = 0;	
//  //--------------------
//  RxCounterByte++;
//  if ((RxBufferUNION.STR.SinhroByte1 == SinhroByte1Const) && 
//      (RxBufferUNION.STR.SinhroByte2 == SinhroByte2Const) && 
//      (RxBufferUNION.STR.Size		     != 0))
//    {
//      RxBufferUNION.BLK[RxCounterByte-1] = data;//Прнятый байт в приемный буфер.
//      //-------------------------
//      //Если приняты нужное количество байт.
//      if (RxCounterByte >= RxBufferUNION.STR.Size)
//        {	
//          //Проверка CRC.
//          CRCtemp = Get_Crc16(RxBufferUNION.BLK, (RxBufferUNION.STR.Size - 2));
//          if (((uint8_t)(CRCtemp >> 8)     == RxBufferUNION.BLK[RxBufferUNION.STR.Size - 1]) &&
//              ((uint8_t)(CRCtemp & 0x00FF) == RxBufferUNION.BLK[RxBufferUNION.STR.Size - 2]))
//            {              
//              return RxBufferUNION.STR.Type;
//            }
//          //-------------------------
//          //CRC не совпало. 			
//          else
//            {	
//              RS485ClearRxBuf(); //Сброс значений приемного буфера.
//            }							
//        }
//      //-------------------------
//    }
//  //---------------------------------------------------
//  else
//    {	
//      //-------------------------
//      if ((data == SinhroByte1Const) &&
//          (RxBufferUNION.STR.SinhroByte1 != SinhroByte1Const))
//        {
//          RxBufferUNION.STR.SinhroByte1 = SinhroByte1Const;
//        }
//      else if ((data == SinhroByte2Const) &&
//               (RxBufferUNION.STR.SinhroByte1 == SinhroByte1Const))
//        {
//          RxBufferUNION.STR.SinhroByte2 = SinhroByte2Const;					
//        }
//      else if ((data != 0) &&
//               (RxBufferUNION.STR.SinhroByte1 == SinhroByte1Const) &&
//               (RxBufferUNION.STR.SinhroByte2 == SinhroByte2Const))
//        {
//          RxBufferUNION.STR.Size = data;
//        }  
//      //Сброс приемного буфера если неправильная последовательность байт.  
//      else 
//        {
//          RS485ClearRxBuf();
//        }
//      //-------------------------
//    }
//  //---------------------------------------------------
//  return 0;
//}
////-----------------------------------------------------------------------------
//volatile uint16_t* RS485FlagReg(void){

//  volatile uint16_t* temp;
//  //--------------------
//  __disable_irq();
//  temp = &RS485ProtocolFlagReg;
//  __enable_irq();
//  return  temp;
//}
////-----------------------------------------------------------------------------
//void RS485ProtocolClearTimeOut(void){
//  
//  RS485ProtocolTimeOut = 0;
//}
////-----------------------------------------------------------------------------
//uint8_t RS485GetDataFromFP(uint8_t pointer){
//  
//  return RxBufferUNION.BLK[pointer];
//}
////-----------------------------------------------------------------------------
////Прерывание от USART1.
//void USART1_IRQHandler(void){
//  
//  //--------------------
//	/*!< Read Data Register Not Empty */
//	if (USART1->SR & USART_SR_RXNE)
//		{
//      //проверяем нет ли ошибок при приеме байта.
//      if ((USART1->SR & (USART_SR_NE | USART_SR_FE | USART_SR_PE | USART_SR_ORE)) == 0)
//        {
//          //Принимаем запрос от МВ.
//          if(RS485RxPacket((uint8_t)USART1->DR) == FP_ModuleTypeConst)//MB_TypeConst)
//            {
//              USART1->CR1 &= ~USART_CR1_RE;     //Отлючение RX USART1.
//              //RS485DEOn;                      //Микросхему на передачу.
//              RS485ProtocolTimeOut = 0;         //Сброс счетчика таймаут.
//              RS485ActLedToggel;                //Моргаем светодиодом.
//              *RS485FlagReg() |= RS485_FPOkFlag;//Выставление флага что есть ответ от FP.

//              //Настройка ДМА на прием данных из USART1.
//              //USART1->CR3 |= USART_CR3_DMAR;  //разрешить прием USART1 через DMA.
//              //StartDMA1Ch5ForRX((ReceiverBufferSTR.BLK + 4U), (uint16_t)(ReceiverBufferSTR.STR.Size - 4U));
//            }
//        }
//      //--------------------
//      //Если байт битый, то пропускаем его и 
//      //очищаем приемный буфер для запуска приема нового пакета.
//      else
//        {
//          USART1->SR;//Такая последовательность сбрасывает флаги ошибки 
//          USART1->DR;//
//          RS485ClearRxBuf(); 
//        }
//      //--------------------
//		}
//  //--------------------
//  /*!< Overrun error */
//  if (USART1->SR & USART_SR_ORE)
//		{
//      USART1->SR;//Такая последовательность сбрасывает флаги ошибки 
//      USART1->DR;//
//      RS485ClearRxBuf(); 
//    }
//  //--------------------
// 	/*!< Transmission Complete */
//	if (USART1->SR & USART_SR_TC)
//		{
//      if(++TxCounterByte == TxBufSize)
//        {
//           //Запуск приема нового пакета.        
//           USART1->CR1 &= ~USART_CR1_TE;  //USART_CR1_TCIE;//Отлючение прерывания по окончании передачи.  USART1.
//           RS485REOn;                     //Микросхему на прием.           
//           USART1->CR1 |=  USART_CR1_RE;  //Включение RX USART1.
//        }
//      //Передача очередного байта.
//      else USART1->DR = *(TxBufPointer + TxCounterByte);
//      //---------
//      //Настройка ДМА на прием данных из USART1.
//      //StartDMA1Ch5ForRX(ResponseUNION.BLK, 12);
//      //---------      
//		}
//  //-------------------- 
// 	/*!< Transmit Data Register Empty */
// if (USART1->SR & USART_SR_TXE)
// 		{
//      //USART1->SR &= ~USART_SR_TXE; //Сброс флага прерывания.
//      //---------
// 		}
//	//--------------------
//}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

