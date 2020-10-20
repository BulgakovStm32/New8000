
#include "uart_ST.h"

//-----------------------------------------------------------------------------
//uint8_t (*USARTxRXInterupt)(uint8_t data) = USARTxRXInteruptHandler;

//static UsartWorkRegSTR_Type	U1WorkReg;
//************************************************************************************************** 
//**************************************************************************************************
void Uart1Init(void){
  
 	//Инициализация портов. PA9(U1TX), PA10(U1RX).
	GPIOA->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_CNF10);	
	
	GPIOA->CRH |= GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9;//PA9(U1TX) - выход, альтернативный режим push-pull.
																								 //PA9(U1TX) - тактирование 50МГц.
	GPIOA->CRH |= GPIO_CRH_CNF10_0;								 //PA10(U1RX) - Floating input.
  //--------------------
  //Инициализация USART1.
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;//Включение тактирование USART1.
	
	USART1->BRR  = USARTxBRR_Const;  //Set baudrate
  USART1->CR1 &= ~USART_CR1_M; 		 //8 бит данных.
  USART1->CR2 &= ~USART_CR2_STOP;  //1 стоп-бит.
  USART1->CR1 =  USART_CR1_RE    | //Включение RX USART1.	
                 USART_CR1_TE    | //Включение TX USART1.
                 USART_CR1_RXNEIE| //Включение прерывания от приемника USART1.		
                 USART_CR1_UE;     //Включение USART1.
  USART1->CR3 |= USART_CR3_DMAT;   //Подключение TX USART1 к DMA.          
	NVIC_SetPriority(USART1_IRQn, 14);//Приоритет прерывания USART1.
	NVIC_EnableIRQ(USART1_IRQn);      //Разрешаем прерывание от приемника USART1.
  //--------------------
  //Инициализация DMA. TX USART1 работает с DMA1 Ch4.
  RCC->AHBENR |= RCC_AHBENR_DMA1EN;//Включить тактирование DMA1
  DMA1Ch4InitForTx(USART1);  
}
//-----------------------------------------------------------------------------
//Передача буфера.
//void UsartStarTx(USART_TypeDef *usart, uint8_t *TxBuf, uint8_t size){

//	U1WorkReg.CounterByte = 0;		 //Cброс счетчика байт.
//	U1WorkReg.BufSize			= size;  //Количество байт на передачу.	
//	U1WorkReg.BufPtr		  = TxBuf; //Указатель на передающий буфер.
//  //----------------
//  usart->CR1 &= ~USART_CR1_RE;  //Отлючение RX USART1.
//	usart->DR   = *TxBuf;         //Передача первого байта.
//	usart->CR1 |=  USART_CR1_TE | //Включение TX USART1.
//								 USART_CR1_TCIE;//Включение прерывания от TX USART1.
//}
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
          //if(RS485RxPacket((uint8_t)USART1->DR)) USART1->CR1 &= ~USART_CR1_RE;//Отлючение RX USART1. 
					RS485RxPacket((uint8_t)USART1->DR);
        }
      //--------------------
      //Если байт битый, то пропускаем его и 
      //очищаем приемный буфер для запуска приема нового пакета.
      else
        {
          USART1->SR;//Такая последовательность сбрасывает флаги ошибки 
          USART1->DR;//
          RS485RxBufClr(); 
        }
      //--------------------
		}
  //--------------------
  /*!< Overrun error */
  if(USART1->SR & USART_SR_ORE)
		{
      USART1->SR;//Такая последовательность сбрасывает флаги ошибки 
      USART1->DR;//
      RS485RxBufClr(); 
    }
  //--------------------
  	/*!< Transmission Complete */
	if(USART1->SR & USART_SR_TC)
		{
//      if(++U1WorkReg.CounterByte == U1WorkReg.BufSize)
//        {
//           //Запуск приема нового пакета.
//					 RS485Direction(Rx);					
//           USART1->CR1 &= ~USART_CR1_TE;//Отлючение прерывания TX. 
//					 //USART1->CR1 &= ~USART_CR1_TCIE;//Отлючение прерывания по окончании передачи.  					
//           USART1->CR1 |=  USART_CR1_RE;//Включение RX.
//        }
//      //Передача очередного байта.
//      else USART1->DR = *(U1WorkReg.BufPtr + U1WorkReg.CounterByte);
      //---------
      //Работа с ДМА1 канал 4.
      RS485DirectionRX();            //Микросхему на прием.
			USART1->CR1 &= ~USART_CR1_TCIE;//Отлючение прерывания по окончанию передачи		
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
//************************************************************************************************** 
//**************************************************************************************************
//************************************************************************************************** 
//**************************************************************************************************
//Инициализация канала 4 DMA1 передачи данных через USART1.   
void DMA1Ch4InitForTx(USART_TypeDef *usart){
  
  //Задать адрес источника и приемника и количество данных для обмена
  DMA1_Channel4->CPAR = (uint32_t)&usart->DR;//адрес регистра перефирии
  //----------------- Манипуляции с регистром конфигурации  ----------------
  //Следующие действия можно обьединить в одну команду (разбито для наглядности)
	//После сброса значение CCR = 0.
  //DMA1_Channel4->CCR  =  0;            //предочистка регистра конфигурации
  //DMA1_Channel4->CCR &= ~DMA_CCR4_CIRC;//выключить циклический режим
  DMA1_Channel4->CCR |= DMA_CCR4_DIR;    //направление: чтение из памяти
  //Настроить работу с переферийным устройством
  //DMA1_Channel4->CCR &= ~DMA_CCR4_PSIZE;//размерность данных 8 бит
  //DMA1_Channel4->CCR &= ~DMA_CCR4_PINC; //неиспользовать инкремент указателя
  //Настроить работу с памятью
  //DMA1_Channel4->CCR &= ~DMA_CCR4_MSIZE;//размерность данных 8 бит
  DMA1_Channel4->CCR |= DMA_CCR4_MINC;    //использовать инкремент указателя

  DMA1_Channel4->CCR |= DMA_CCR4_TCIE; //Разрешить прерывание по завершении обмена
  NVIC_EnableIRQ (DMA1_Channel4_IRQn); //Разрешить прерывания от DMA канал №4  
}
//-----------------------------------------------------------------------------
//Старт обмена в канале "память-DMA-USART1"                                                                   
void DMA1Ch4StartTx(uint8_t *buf, uint16_t size){
  
  DMA1_Channel4->CCR  &= ~DMA_CCR4_EN; //запретить работу канала
  DMA1_Channel4->CMAR  = (uint32_t)buf;//адрес буфера в памяти
  DMA1_Channel4->CNDTR = size;         //загрузить количество данных для обмена
  DMA1_Channel4->CCR  |= DMA_CCR4_EN;  //разрешить работу канала
}
//-----------------------------------------------------------------------------
//Прерываение от DMA1_Ch4.
void DMA1_Channel4_IRQHandler(void){
  
  //-------------------------
  //Если обмен завершен
  if(DMA1->ISR & DMA_ISR_TCIF4)
    {
			DMA1->IFCR |= DMA_IFCR_CTCIF4;      //сбросить флаг окончания обмена.
      DMA1_Channel4->CCR  &= ~DMA_CCR4_EN;//запретить работу канала.
      //Это нужно что бы дождаться завершения передачи последнего байта.
      USART1->CR1 |= USART_CR1_TCIE;		  //Включение прерывания по окончанию передачи USART1.	
    }      
//  //-------------------------
//  //Если передана половина буфера
//  if(DMA1->ISR & DMA_ISR_HTIF4)
//    {

//    }  
//  //-------------------------
//  //Если произошла ошибка при обмене
//  if(DMA1->ISR & DMA_ISR_TEIF4)
//    {

//    }   
//  //-------------------------
}
//-----------------------------------------------------------------------------
//Инициализация канала 5 DMA1 для приема данных из USART1.   
//void DMA1Ch5InitForRX(void){
//  
//  DMA1_Channel5->CPAR = (uint32_t)&USART1->DR;  //адрес регистра данных приемника.
//  //Настроить работу с переферийным устройством
//  DMA1_Channel5->CCR  =  0;                 //предочистка регистра конфигурации
//  DMA1_Channel5->CCR &= ~DMA_CCR5_CIRC;     //выключить циклический режим
//  DMA1_Channel5->CCR &= ~DMA_CCR5_DIR;      //направление: чтение из перифферии.
//  DMA1_Channel5->CCR &= ~DMA_CCR5_PSIZE;    //размерность данных 8 бит
//  DMA1_Channel5->CCR &= ~DMA_CCR5_PINC;     //неиспользовать инкремент указателя
//  //--------------------
//  //Настроить работу с памятью
//  DMA1_Channel5->CCR &= ~DMA_CCR5_MSIZE;    //размерность данных 8 бит
//  DMA1_Channel5->CCR |=  DMA_CCR5_MINC;     //использовать инкремент указателя
//  //--------------------
//  DMA1_Channel5->CCR |= DMA_CCR5_TCIE;      //Разрешить прерывание по завершении обмена
//  NVIC_EnableIRQ(DMA1_Channel5_IRQn);       //Разрешить прерывания от DMA канал №5
//}
//-----------------------------------------------------------------------------
////Старт канала 5 DMA1 для приема данных из USART1.                                                            
//void StartDMA1Ch5ForRX(uint8_t *BuffAdr, uint16_t LengthBufer){
//  
//  DMA1_Channel5->CCR  &= ~DMA_CCR5_EN;     //запретить работу канала
//  DMA1_Channel5->CMAR  = (uint32_t)BuffAdr;//адрес буфера приемника.
//  DMA1_Channel5->CNDTR = LengthBufer;      //загрузить количество данных для обмена
//  DMA1_Channel5->CCR  |= DMA_CCR5_EN;//разрешить работу канала
//}
//-----------------------------------------------------------------------------
//Прерываени от DMA1_Ch5.
//void DMA1_Channel5_IRQHandler(void){
//   
//  //static uint8_t LedState = 0;
//  //uint16_t CRCtemp = 0;
//  //-------------------------
//  //Если обмен завершен
//  if(DMA1->ISR & DMA_ISR_TCIF5)
//    {
//      DMA1_Channel5->CCR  &= ~DMA_CCR5_EN;//запретить работу канала.
//      DMA1->IFCR |= DMA_IFCR_CTCIF5;      //сбросить флаг окончания обмена.
//      //-------------------------

//      



//      //Анализ ответа от MB.
////      CRCtemp = Get_Crc16(ReceiverBufferSTR.BLK, (ReceiverBufferSTR.STR.Size - 2));
////      if (((uint8_t)(CRCtemp >> 8)     == ReceiverBufferSTR.BLK[ReceiverBufferSTR.STR.Size ]) &&
////          ((uint8_t)(CRCtemp & 0x00FF) == ReceiverBufferSTR.BLK[ReceiverBufferSTR.STR.Size - 1]))
////        {
//////          LedState ^= LedGreen;            
//////          CommunicationLedSet(LedState);     //Моргаем светодиодом.

//////          RS485ClearReceiverBuffer();
//////          NVIC_EnableIRQ(USART1_IRQn);//Разрешаем прерывание от приемника USART1.

////          RS485SetFlagReg(RS485_MB_Flag);
////        }
//      //-------------------------
//    }      
//  //-------------------------
//  //Если передана половина буфера
//  if(DMA1->ISR & DMA_ISR_HTIF5)
//    {

//    }  
//  //-------------------------
//  //Если произошла ошибка при обмене
//  if(DMA1->ISR & DMA_ISR_TEIF5)
//    {

//    }   
//  //-------------------------
//}
//-----------------------------------------------------------------------------
