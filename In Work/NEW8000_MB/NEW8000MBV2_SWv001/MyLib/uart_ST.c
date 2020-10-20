
#include "uart_ST.h"
#include "Rs485.h"
//-----------------------------------------------------------------------------
typedef struct{
	//--------------------
	volatile uint16_t CounterByte;//Счетчик переданных байт. 
	volatile uint8_t  BufSize;    //Количество передаваемых байт.
	volatile uint8_t *BufPtr;     //Указатель на передающий буфер.
	//--------------------
}UsartWorkRegSTR_TypeDef;
//----------------------------------------
UsartWorkRegSTR_TypeDef	U3WorkReg;
UsartWorkRegSTR_TypeDef	U1WorkReg;
//*********************************************************************************
//************************Аппаратный уровень.**************************************
void Uart1Init(void){

 	//Инициализация портов. 
	RCC->APB2ENR |=   RCC_APB2ENR_IOPAEN;             //Включение тактирование GPIOA.
	GPIOA->CRH   &= ~(GPIO_CRH_CNF9 | GPIO_CRH_CNF10);	
	GPIOA->CRH   |=   GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9;//PA9(U1TX) - выход, альтернативный режим push-pull.
                                                     //PA9(U1TX) - тактирование 50МГц.
	GPIOA->CRH |= GPIO_CRH_CNF10_0;								     //PA10(U1RX) - Floating input.
  //Инициализация USART1. APB2 тактируется 72МГц.
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;//Включение тактирование USART1.
  USART1->CR1 &= ~USART_CR1_M; 		     //8 бит данных.
  USART1->CR2 &= ~USART_CR2_STOP;      //1 стоп-бит.
  USART1->BRR =   0x1d44;  				     //BaudRate 9600.
  USART1->CR1 =   USART_CR1_RE    |    //Включение RX USART1.	
                  USART_CR1_TE    |    //Включение TX USART1.
                  //USART_CR1_RXNEIE|    //Включение прерывания от приемника USART1.		
                  USART_CR1_UE;        //Включение USART1.   
	//NVIC_SetPriority(USART1_IRQn, 15);   //Приоритет прерывания USART1.
	//NVIC_EnableIRQ(USART1_IRQn);         //Разрешаем прерывание от приемника USART1.                
  //--------------------
  //Инициализация DMA1.
//  if (!(RCC->AHBENR & RCC_AHBENR_DMA1EN)) RCC->AHBENR |= RCC_AHBENR_DMA1EN;//Включить тактирование DMA1
//  DMA1Ch4InitForTX();  //DMA1Ch4 - 
//  DMA1Ch5InitForRX();  //DMA1Ch5 - 
//  USART1->CR3 |= (USART_CR3_DMAT | USART_CR3_DMAR);//разрешить передачу и прием USART1 через DMA.
}
//-----------------------------------------------------------------------------
void Uart3Init(void){
  
 	//Инициализация портов. 
  RCC->APB2ENR |=   RCC_APB2ENR_IOPBEN;                //Включение тактирование GPIOB.
	GPIOB->CRH   &= ~(GPIO_CRH_CNF10 | GPIO_CRH_CNF11);
	GPIOB->CRH   |=   GPIO_CRH_CNF10_1 | GPIO_CRH_MODE10;//PB10(U3TX) - выход, альтернативный режим push-pull.
																								       //PB10(U3TX) - тактирование 50МГц.
	GPIOB->CRH   |= GPIO_CRH_CNF11_0;								     //PB11(U3RX) - Floating input.		
  //Инициализация USART3. APB1 тактируется 36МГц.
  RCC->APB1ENR |= RCC_APB1ENR_USART3EN ;//Включение тактирование USART3. 
  USART3->CR1 &= ~USART_CR1_M; 		      //8 бит данных.
  USART3->CR2 &= ~USART_CR2_STOP;       //1 стоп-бит.
  USART3->BRR =   0x138;  				      //BaudRate 115200.
  USART3->CR1 =   USART_CR1_RE    |     //Включение RX USART1.	
                  //USART_CR1_TE    |     //Включение TX USART1.
                  USART_CR1_RXNEIE|     //Включение прерывания от приемника USART1.		
                  USART_CR1_UE;         //Включение USART1.

	NVIC_SetPriority(USART3_IRQn, 14);    //Приоритет прерывания USART3.
	NVIC_EnableIRQ  (USART3_IRQn);        //Разрешаем прерывание от приемника USART3.                
  //--------------------
  //Инициализация DMA1.
  //if (!(RCC->AHBENR & RCC_AHBENR_DMA1EN)) RCC->AHBENR |= RCC_AHBENR_DMA1EN;//Включить тактирование DMA1
	
//	RCC->AHBENR |= RCC_AHBENR_DMA1EN;//Включить тактирование DMA1
//	DMA1Ch4InitForTX(USART3);  			 //DMA1Ch4 - 
//	USART3->CR3 |= USART_CR3_DMAT;
//	
//	NVIC_SetPriority(USART3_IRQn, 14);    //Приоритет прерывания USART3.
//	NVIC_EnableIRQ  (USART3_IRQn);        //Разрешаем прерывание от приемника USART3.   	
	
//  DMA1Ch5InitForRX();  //DMA1Ch5 - 
//  USART1->CR3 |= (USART_CR3_DMAT | USART_CR3_DMAR);//разрешить передачу и прием USART1 через DMA.
}
//-----------------------------------------------------------------------------
//Передача буфера.
void Uart1StarTx(uint8_t *TxBuf, uint8_t size){

	U1WorkReg.CounterByte = 0;		//Cброс счетчика байт.
	U1WorkReg.BufSize			= size; //Количество байт на передачу.	
	U1WorkReg.BufPtr		  = TxBuf;//Указатель на передающий буфер.
  //----------------
  USART1->CR1 &= ~USART_CR1_RE; //Отлючение RX USART1.
	USART1->DR   = *TxBuf;        //Передача первого байта.
	USART1->CR1 |= USART_CR1_TE  |//Включение TX USART1.
                 USART_CR1_TCIE;//Включение прерывания от TX USART1.
}
//-----------------------------------------------------------------------------
//Передача буфера.
void Uart3StarTx(uint8_t *TxBuf, uint8_t size){

	U3WorkReg.CounterByte = 0;		//Cброс счетчика байт.
	U3WorkReg.BufSize			= size; //Количество байт на передачу.	
	U3WorkReg.BufPtr		  = TxBuf;//Указатель на передающий буфер.
  //----------------
  USART3->CR1 &= ~USART_CR1_RE; //Отлючение RX USART1.
	USART3->DR   = *TxBuf;        //Передача первого байта.
	USART3->CR1 |= USART_CR1_TE | //Включение TX USART1.
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
         //Принимаем пакета.
         //if(RS485RxPacket((uint8_t)USART3->DR)) USART3->CR1 &= ~USART_CR1_RE;//Отлючение RX.        
        }
      //--------------------
      //Если байт битый, то пропускаем его
      else
        {
          USART1->SR;//Такая последовательность сбрасывает флаги ошибки 
          USART1->DR;//
        }
      //--------------------
		}
  //--------------------
  /*!< Overrun error */
  if (USART1->SR & USART_SR_ORE)
		{
      USART1->SR;//Такая последовательность сбрасывает флаги ошибки 
      USART1->DR;//
    }
  //--------------------
  	/*!< Transmission Complete */
	if (USART1->SR & USART_SR_TC)
		{
      //if(++U1TxCounterByte == U1TxBufSize)
			if(++U1WorkReg.CounterByte == U1WorkReg.BufSize)
        {
           //Запуск приема нового пакета.        
           USART1->CR1 &= ~USART_CR1_TE;  //USART_CR1_TCIE;//Отлючение прерывания по окончании передачи.  USART1.     
           USART1->CR1 |=  USART_CR1_RE;  //Включение RX USART1.
        }
      //Передача очередного байта.
      //else USART1->DR = *(U1TxBufPointer + U1TxCounterByte);
			else USART1->DR = *(U1WorkReg.BufPtr + U1WorkReg.CounterByte);
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
//-----------------------------------------------------------------------------
//Прерывание от USART3.
void USART3_IRQHandler(void){

  //--------------------
	/*!< Read Data Register Not Empty */
	if (USART3->SR & USART_SR_RXNE)
		{
      //проверяем нет ли ошибок при приеме байта.
      if ((USART3->SR & (USART_SR_NE | USART_SR_FE | USART_SR_PE | USART_SR_ORE)) == 0)
        {
          //Принимаем пакета.
          if(RS485RxPacket((uint8_t)USART3->DR)) USART3->CR1 &= ~USART_CR1_RE;//Отлючение RX.                       
        }
      //--------------------
      //Если байт битый, то пропускаем его и 
      //очищаем приемный буфер для запуска приема нового пакета.
      else
        {
          USART3->SR;//Такая последовательность сбрасывает флаги ошибки 
          USART3->DR;//
          RS485RxClrBuf(); 
        }
      //--------------------
		}
  //--------------------
  /*!< Overrun error */
  if(USART3->SR & USART_SR_ORE)
		{
      USART3->SR;//Такая последовательность сбрасывает флаги ошибки 
      USART3->DR;//
      RS485RxClrBuf(); 
    }
  //--------------------
  //Transmission Complete */
	if (USART3->SR & USART_SR_TC)
		{
			//RS485DE(RE);
			if(++U3WorkReg.CounterByte == U3WorkReg.BufSize)
        {
           //Запуск приема нового пакета. 
           RS485DE(RE);                 //Микросхему на прием.        
           USART3->CR1 &= ~USART_CR1_TE;//USART_CR1_TCIE;//Отлючение прерывания по окончании передачи.  USART1.            
           USART3->CR1 |=  USART_CR1_RE;//Включение RX USART1. 
        }
      //Передача очередного байта.
			else USART3->DR = *(U3WorkReg.BufPtr + U3WorkReg.CounterByte);
      //---------
      //Настройка ДМА на прием данных из USART1.
      //StartDMA1Ch5ForRX(ResponseUNION.BLK, 12);
      //---------      
		}
  //-------------------- 
 	//Transmit Data Register Empty */
  if (USART3->SR & USART_SR_TXE)
 		{
      //LC2SostGreenLedToggel;
 		}
	//--------------------
}
//************************************************************************************************** 
//**************************************************************************************************
//Инициализация канала 4 DMA1 передачи данных через USART1.   
void DMA1Ch4InitForTX(USART_TypeDef *ptr){
  
  //Задать адрес источника и приемника и количество данных для обмена
  DMA1_Channel4->CPAR  =  (uint32_t)&ptr->DR;   //адрес регистра перефирии
  
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
//Старт обмена в канале "память-DMA-USARTx"                                                                   
void DMA1Ch4StartTX(uint8_t *buf, uint16_t size){
  
  DMA1_Channel4->CCR  &= ~DMA_CCR4_EN; //запретить работу канала
  DMA1_Channel4->CMAR  = (uint32_t)buf;//адрес буфера в памяти
  DMA1_Channel4->CNDTR = size;       	 //загрузить количество данных для обмена  
  DMA1_Channel4->CCR  |= DMA_CCR4_EN;	 //разрешить работу канала
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
  
	//RelayOrLedToggle(LedRS485);//Моргаем светодиодом.
  //-------------------------
  //Если обмен завершен
  if(DMA1->ISR & DMA_ISR_TCIF4)
    {
      DMA1_Channel4->CCR  &= ~DMA_CCR4_EN;//запретить работу канала.
      DMA1->IFCR |= DMA_IFCR_CTCIF4;      //сбросить флаг окончания обмена.
      //---------
      //Это нужно что бы дождаться завершения передачи,
      //последнего байта и перейти на прием данных.
      USART3->CR1 |= USART_CR1_TCIE;		  //Включение прерывания по окончанию передачи USART1.	
      //NVIC_SetPriority(USART1_IRQn, 14);//риоритет рерывания USART1.
      NVIC_EnableIRQ(USART3_IRQn);       //Разрешаем прерывание от приемника USART1.
      //---------
      //RelayOrLedToggle(LedRS485);//Моргаем светодиодом.
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
