
#include "uart_ST.h"

//*********************************************************************************
static UxWorkReg_t	U1WorkReg;
static UxHandlers_t	U1Handlers;
//*********************************************************************************
//************************Аппаратный уровень.**************************************
void Usart1_Init(uint16_t usartBrr){

 	//Инициализация портов. 
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; //Включение тактирование GPIOA.
      
	GPIOA->MODER  &= ~(GPIO_MODER_MODER10   |GPIO_MODER_MODER9);
	GPIOA->MODER  |=  (GPIO_MODER_MODER10_1 |GPIO_MODER_MODER9_1); 
	GPIOA->PUPDR  |=   GPIO_PUPDR_PUPDR9;
	
	GPIOA->AFR[1] |=  (1<<8) |(1<<4);

  //Инициализация USART1. APB2 тактируется 48МГц.
  RCC->APB2ENR |=  RCC_APB2ENR_USART1EN;//Включение тактирование USART1.
  RCC->CFGR3   &= ~RCC_CFGR3_USART1SW;  //System clock (SYSCLK) selected as USART1 clock.
  RCC->CFGR3   |=  RCC_CFGR3_USART1SW_0;
	
	USART1->BRR =   usartBrr;	           
  USART1->CR1 &= ~USART_CR1_M; 		  //8 бит данных.
  USART1->CR2 &= ~USART_CR2_STOP;   //1 стоп-бит.
  USART1->CR1 =   USART_CR1_RE    | //Включение RX USART1.	
                  USART_CR1_TE    | //Включение TX USART1.
                  USART_CR1_RXNEIE| //Включение прерывания от приемника USART1.		
                  USART_CR1_UE;     //Включение USART1. 
	USART1->CR3 |= USART_CR3_DMAT;    //Подключение TX USART1 к DMA.	
	
	NVIC_SetPriority(USART1_IRQn, 15);//Приоритет прерывания USART1.
	NVIC_EnableIRQ(USART1_IRQn);      //Разрешаем прерывание от приемника USART1.                
}
//-----------------------------------------------------------------------------
//Передача буфера.
void Usart1_StartTx(uint8_t *TxBuf, uint16_t size){

	U1WorkReg.CounterByte = 0;		 //Cброс счетчика байт.
	U1WorkReg.BufSize			= size;  //Количество байт на передачу.	
	U1WorkReg.BufPtr		  = TxBuf; //Указатель на передающий буфер.
  //----------------
  USART1->CR1 &= ~USART_CR1_RE;  //Отлючение RX USART1.
	USART1->TDR  = *TxBuf;         //Передача первого байта.
	USART1->CR1 |=  USART_CR1_TCIE;//Включение прерывания от TX USART1.
}
//-----------------------------------------------------------------------------
//Указатели обработчиков событий
UxHandlers_t* Usart1_Handlers(void){
	
	return &U1Handlers;
}
//-----------------------------------------------------------------------------
static void Usart_ResetErr(USART_TypeDef *usart){

	//Такая последовательность сбрасывает флаги ошибки 
	usart->ICR |= (USART_ICR_ORECF | USART_ICR_FECF | USART_ICR_PECF | USART_ICR_NCF);
	usart->RDR;//
}
//************************************************************************************************** 
//**************************************************************************************************
//Прерывание от USART1.
void USART1_IRQHandler(void){

  //--------------------
	/*!< Read Data Register Not Empty */
	//This bit is set by hardware when the content of the RDR shift register has been transferred
	//to the USART_RDR register. It is cleared by a read to the USART_RDR register. 
	//The RXNE flag can also be cleared by writing 1 to the RXFRQ in the USART_RQR register.
	//An interrupt is generated if RXNEIE=1 in the USART_CR1 register.	
	if(USART1->ISR & USART_ISR_RXNE)
		{
      //проверяем нет ли ошибок при приеме байта.
      if ((USART1->ISR & (USART_ISR_NE | USART_ISR_FE | USART_ISR_PE | USART_ISR_ORE)) == 0)
        {
          //Принимаем пакета.
					U1Handlers.ReceiveByte((uint8_t)USART1->RDR);       					
				}
      //Если байт битый, то пропускаем его и 
      //очищаем приемный буфер для запуска приема нового пакета.
			else 
				{
					Usart_ResetErr(USART1);//сброс флагов ошибок 
					U1Handlers.ReceiveByteBroken();
				}
		}
  //--------------------
  /*!< Overrun error */
  if(USART1->ISR & (USART_ISR_NE | USART_ISR_FE | USART_ISR_PE | USART_ISR_ORE)) 
		{
			Usart_ResetErr(USART1);//сброс флагов ошибок 
			U1Handlers.ReceiveByteBroken();
		}
  //--------------------
  /*!< Transmission Complete */
	//This bit is set by hardware if the transmission of a frame containing data is complete and if
	//TXE is set. An interrupt is generated if TCIE=1 in the USART_CR1 register. It is cleared by
	//software, writing 1 to the TCCF in the USART_ICR register or by a write to the USART_TDR register.	
	if(USART1->ISR & USART_ISR_TC)
		{		
//			USART1->ICR |= USART_ICR_TCCF; //Сброс флага прерывания.
//			if(++U1WorkReg.CounterByte >= U1WorkReg.BufSize)
//				{
//					//Запуск приема нового пакета.
//					U1Handlers.BufTransferComplete();
//					USART1->CR1 &= ~USART_CR1_TCIE;//Отлючение прерывания по окончании передачи.  
//					USART1->CR1 |=  USART_CR1_RE;  //Включение RX USART1.
//				}
//      //Передача очередного байта.
//			else USART1->TDR = *(U1WorkReg.BufPtr + U1WorkReg.CounterByte);
      //---------
      //Работа с ДМА1 канал 2.
			U1Handlers.BufTransferComplete();//Обработка окнчания предачи буфера.
			USART1->CR1 &= ~USART_CR1_TCIE;  //Отлючение прерывания по окончании передачи.			
      //---------      
		}
  //-------------------- 
 	/*!< Transmit Data Register Empty */
	//This bit is set by hardware when the content of the USART_TDR register has been
	//transferred into the shift register. It is cleared by a write to the USART_TDR register.
	//An interrupt is generated if the TXEIE bit =1 in the USART_CR1 register.
  if (USART1->ISR & USART_ISR_TXE)
 		{
      USART1->ICR |= USART_ICR_TCCF; //Сброс флага прерывания.
      //---------
 		}
	//--------------------
}
//************************************************************************************************** 
//**************************************************************************************************





