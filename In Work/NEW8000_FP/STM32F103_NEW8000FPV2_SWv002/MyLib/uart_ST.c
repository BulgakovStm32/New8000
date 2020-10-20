
#include "uart_ST.h"

//-----------------------------------------------------------------------------
//Буфер и переменные дла Приема.
MB_DataUNION_Type RS485_MB_RxBuf;
volatile uint16_t RS485RxCounterByte   = 0;
volatile uint16_t RS485ProtocolFlagReg = 0;
volatile uint16_t RS485ProtocolTimeOut = 0;
//-----------------------------------------------------------------------------
//Буфер и переменные для Передачи.
FPDataUNION_Type  TxBufUNION;
volatile uint16_t TxCounterByte = 0;//Счетчик переданных байт.
volatile uint8_t	TxBufSize	   = 0; //
volatile uint8_t *TxBufPointer;     //Указатель на передающий буфер.
//*********************************************************************************
//************************Аппаратный уровень.**************************************
void Uart1Init(void){
  
 	//Инициализация портов. PA9(U1TX), PA10(U1RX), PC14(DE/RE).
	GPIOA->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_CNF10);	
	
	GPIOA->CRH |= GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9;//PA9(U1TX) - выход, альтернативный режим push-pull.
																								 //PA9(U1TX) - тактирование 50МГц.
	GPIOA->CRH |= GPIO_CRH_CNF10_0;								 //PA10(U1RX) - Floating input.

  GPIOC->CRH &= ~(GPIO_CRH_CNF14);	             //PC14(DE/RE) - выход, режим - push-pull.
	GPIOC->CRH |=   GPIO_CRH_MODE14;							 //PC14(DE/RE) - тактирование 50МГц.		
  //--------------------
  //Инициализация USART1.
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;//Включение тактирование USART1.
  USART1->CR1  &= ~USART_CR1_M; 		   //8 бит данных.
  USART1->CR2  &= ~USART_CR2_STOP;     //1 стоп-бит.
  USART1->BRR   =  0x271; 					   //BaudRate 115200.
  
  USART1->CR1 = USART_CR1_RE    | //Включение RX USART1.	
                //USART_CR1_TE    | //Включение TX USART1.
                USART_CR1_RXNEIE| //Включение прерывания от приемника USART1.		
                USART_CR1_UE;     //Включение USART1.
                
	NVIC_SetPriority(USART1_IRQn, 14);//Приоритет прерывания USART1.
	NVIC_EnableIRQ(USART1_IRQn);      //Разрешаем прерывание от приемника USART1.
  //--------------------
  //Инициализация DMA1.
  //if (!(RCC->AHBENR & RCC_AHBENR_DMA1EN)) RCC->AHBENR |= RCC_AHBENR_DMA1EN;//Включить тактирование DMA1
  
//  DMA1Ch4InitForTX();  //DMA1Ch4 - 
//  DMA1Ch5InitForRX();  //DMA1Ch5 - 
  
  //USART1->CR3 |= (USART_CR3_DMAT | USART_CR3_DMAR);//разрешить передачу и прием USART1 через DMA.
  //USART1->CR3 |= USART_CR3_DMAR;  //разрешить прием USART1 через DMA.
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
	if(USART1->SR & USART_SR_RXNE)
		{
      //проверяем нет ли ошибок при приеме байта.
      if((USART1->SR & (USART_SR_NE | USART_SR_FE | USART_SR_PE | USART_SR_ORE)) == 0)
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
  if(USART1->SR & USART_SR_ORE)
		{
      USART1->SR;//Такая последовательность сбрасывает флаги ошибки 
      USART1->DR;//
      RS485RxClrBuf(); 
    }
  //--------------------
  	/*!< Transmission Complete */
	if(USART1->SR & USART_SR_TC)
		{
      if(++TxCounterByte == TxBufSize)
        {
           //Запуск приема нового пакета.        
           USART1->CR1 &= ~USART_CR1_TE;//Отлючение прерывания TX.     
           USART1->CR1 |=  USART_CR1_RE;//Включение RX.
           RS485Direction(Rx);
           //RS485FP_TxEnd();
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
  if(USART1->SR & USART_SR_TXE)
 		{
      //USART1->SR &= ~USART_SR_TXE; //Сброс флага прерывания.
      //---------
 		}
	//--------------------
}
//*********************************************************************************
//************************Прикладной уровень.**************************************
//Управление микросхемой приемо-передатчика.
void RS485Direction(uint8_t dir){
  
  if(dir == Tx) RS485DEOn;
  if(dir == Rx) RS485REOn;
}
//-----------------------------------------------------------------------------
//Отсчет таймаута приема пакета.
//Если не сбрасывался счетчик в течении 1с значит небыло запросов/ответов на запросы
//что говорит об отсутсвии связи.
void RS485TimeOutInc(void){
  
	//Сброс счетчика прроисходит по приему запросов/ответов на запросы
  if(++RS485ProtocolTimeOut <= 1000)
		{
			 RS485ProtocolFlagReg &= ~RS485_TimeOutFlag;
			 //RS485ProtocolTimeOut  =  0;
		}
  else RS485ProtocolFlagReg |=  RS485_TimeOutFlag;
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
  RS485_MB_RxBuf.BLK[RS485_MB_RxBuf.STR.Size - 2] = 0;
  RS485_MB_RxBuf.BLK[RS485_MB_RxBuf.STR.Size - 1] = 0;

  RS485_MB_RxBuf.STR.SinhroByte1 = 0;
  RS485_MB_RxBuf.STR.SinhroByte2 = 0;
	RS485_MB_RxBuf.STR.Type		     = 0;
  RS485_MB_RxBuf.STR.Size		     = 0;
  RS485_MB_RxBuf.STR.CmdCode     = 0;
} 
//-----------------------------------------------------------------------------
//Прием пакета.
uint8_t RS485RxPacket(uint8_t data){
  
  //uint16_t CRCtemp = 0;	
  //--------------------
  //Если принят пакет и не обработан то ничего не принемаем, пока не обработаем принятый.
  //if(RS485ProtocolFlagReg & RS485_MBNewDataFlag) return 0;
  //--------------------
  RS485RxCounterByte++;
  if(RS485_MB_RxBuf.STR.Size > 0)
    {
      RS485_MB_RxBuf.BLK[RS485RxCounterByte-1] = data;//Прнятый байт в приемный буфер.
      //-------------------------
      if(RS485RxCounterByte >= RS485_MB_RxBuf.STR.Size)//Принято нужное количество байт.
        {	  
//          CRCtemp = Get_Crc16(RS485_MB_RxBuf.BLK, (RS485_MB_RxBuf.STR.Size - 2));//Проверка CRC.
//          if(((uint8_t)(CRCtemp >> 8)     == RS485_MB_RxBuf.BLK[RS485_MB_RxBuf.STR.Size - 1]) &&
//             ((uint8_t)(CRCtemp & 0x00FF) == RS485_MB_RxBuf.BLK[RS485_MB_RxBuf.STR.Size - 2]))
					if(RS485_MB_RxBuf.STR.CRC16 == Get_Crc16(RS485_MB_RxBuf.BLK, (RS485_MB_RxBuf.STR.Size - 2)))
            {  
              //Смотрим, от кого пришли данные.
              switch(RS485_MB_RxBuf.STR.Type){
                  //-----------
                  case(MB_TypeConst)://Данные ЦП.
                    //RS485ProtocolFlagReg |= RS485_MBNewDataFlag;//Выставление флага что есть данные от МВ.
                    //RS485FP_TxStarResponse();//Запуск передачи ответа на запрос.
                    //RTOS_SetTask(Task_RS485Protocol);
                    Task_RS485Protocol();
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
					 if((RS485_MB_RxBuf.STR.SinhroByte1 != SinhroByte1Const) &&
              (RS485_MB_RxBuf.STR.SinhroByte2 != SinhroByte2Const) &&
              (RS485_MB_RxBuf.STR.Type        != MB_TypeConst)     &&
							(data 													== SinhroByte1Const))
							{RS485_MB_RxBuf.STR.SinhroByte1  = data;}
      //Сохранение синхробайта 2.
		  else if((RS485_MB_RxBuf.STR.SinhroByte1 == SinhroByte1Const) &&
              (RS485_MB_RxBuf.STR.SinhroByte2 != SinhroByte2Const) &&
              (RS485_MB_RxBuf.STR.Type        != MB_TypeConst)     &&
						  (data 													== SinhroByte2Const))
						  {RS485_MB_RxBuf.STR.SinhroByte2  = data;}
      //Сохранение типа.
      else if((RS485_MB_RxBuf.STR.SinhroByte1 == SinhroByte1Const) &&
              (RS485_MB_RxBuf.STR.SinhroByte2 == SinhroByte2Const) &&
              (RS485_MB_RxBuf.STR.Type        != MB_TypeConst)     &&
							(data 													== MB_TypeConst))
              {RS485_MB_RxBuf.STR.Type = data;}
      //Сохранение размера.
      else if((RS485_MB_RxBuf.STR.SinhroByte1 == SinhroByte1Const) &&
              (RS485_MB_RxBuf.STR.SinhroByte2 == SinhroByte2Const) &&
              (RS485_MB_RxBuf.STR.Type        == MB_TypeConst)     &&
							(data 													  > 0))
              {RS485_MB_RxBuf.STR.Size = data;}  
      //Неправильная последовательность байт.  
      else RS485RxClrBuf();//Сброс приемного буфера
      //-------------------------
    }
  //---------------------------------------------------
  return 0;
}
//-----------------------------------------------------------------------------
//Получение указателя на байт в вприемном буфере.
MB_DataUNION_Type* RS485RxBuf(void){

  return &RS485_MB_RxBuf;
}
//-----------------------------------------------------------------------------
uint8_t RS485RxGetDataFromBuf(uint8_t p){
  
  return RS485_MB_RxBuf.BLK[p];
}
//-----------------------------------------------------------------------------
FPDataUNION_Type* RS485FP_TxBufSet(void){

  return &TxBufUNION;
}
//-----------------------------------------------------------------------------
void RS485FP_TxStartResponse(void){	   
	
  //--------------------
	//Ответ на запрос.
	TxBufUNION.STR.SinhroByte1 = SinhroByte1Const;
	TxBufUNION.STR.SinhroByte2 = SinhroByte2Const;
	TxBufUNION.STR.Type		     = FP_TypeConst;
  TxBufUNION.STR.Size		     = 16;
	//-----------
  //Остальные значения устанавливаются через RS485FP_TxBufSet()
//	TxBufUNION.STR.Comand_Code = CmdFromFPStatusFPConst;
//	TxBufferUNION.STR.Addres          = 0;
//	TxBufferUNION.STR.Group	          = 0;
//	TxBufferUNION.STR.Zone	          = 0;
//	TxBufferUNION.STR.Control_Buttons = 0; 
//	TxBufferUNION.STR.Preset_Buttons	= 0;
//	TxBufferUNION.STR.TimeUTC         = 0;
  //-----------
	TxBufUNION.STR.CRC16 = Get_Crc16(TxBufUNION.BLK, (TxBufUNION.STR.Size - 2));
	//--------------------
  //Запуск передачи.
  RS485Direction(Tx);  //Микросхему на передачу.
  Uart1StarTx(TxBufUNION.BLK, TxBufUNION.STR.Size);	
}
//-----------------------------------------------------------------------------
void RS485FP_TxEnd(void){
  
  RS485RxClrBuf();                  //Очистка "буфера" для приема нового запроса
  RS485FlagClr(RS485_MBNewDataFlag);//
}
//-----------------------------------------------------------------------------
//uint32_t RS485FP_GetTimeUTCFromMB(void){

////  uint32_t temp;
//  //--------------------
//  //USART1->CR1 &= ~USART_CR1_RXNEIE; //Отключение прерывания от приемника USART1.	
////  temp  =  RS485_MB_RxBuf.BLK[TimeUTCPoint];
////  temp |= (RS485_MB_RxBuf.BLK[TimeUTCPoint+1] << 8);
////  temp |= (RS485_MB_RxBuf.BLK[TimeUTCPoint+2] << 16);
////  temp |= (RS485_MB_RxBuf.BLK[TimeUTCPoint+3] << 24);
//  //USART1->CR1 |= USART_CR1_RXNEIE;  //Включение прерывания от приемника USART1.	
//  return RS485_MB_RxBuf.STR.TimeUTC;
//}
//-----------------------------------------------------------------------------
//Получение состояния линий Гр.
//uint8_t RS485FP_GetLcFromMB(uint8_t ch){

//  return (RS485RxBufUNION.BLK[ch] & LineMask);
//}
//-----------------------------------------------------------------------------
//Получения статуса зон.
uint8_t RS485FP_GetZoneFromMB(uint8_t ch){

  return (RS485_MB_RxBuf.BLK[ch] & ZoneMask);
}
//-----------------------------------------------------------------------------
////Получения отладочной информации
//uint16_t RS485FP_GetDebugFromMB(uint8_t ch){

//  if(ch == Debug1)return ((RS485RxGetDataFromBuf(DebugData1HPoint)<<8) | RS485RxGetDataFromBuf(DebugData1LPoint));
//  if(ch == Debug2)return ((RS485RxGetDataFromBuf(DebugData2HPoint)<<8) | RS485RxGetDataFromBuf(DebugData2LPoint));
//  return 0;
//}
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
