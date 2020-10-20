
#include "spi_ST.h"
#include "gpio_ST.h"

//-----------------------------------------------------------------------------
//Инициализация SPI2.PB12(SPI2_NS), PB13(SPI2_SCK), PB14(SPI2_MISO), PB15(SPI2_MOSI).
void Spi2Init(void){
  
	GPIOB->CRH &= ~(GPIO_CRH_CNF12 | GPIO_CRH_CNF13 | GPIO_CRH_CNF14 | GPIO_CRH_CNF15);
	
  GPIOB->CRH |= GPIO_CRH_MODE12; //выход, режим - push-pull, тактирование 50МГц.
  
  GPIOB->CRH |= GPIO_CRH_CNF13_1;//PB13(SPI2_SCK) - выход, альтернативный режим push-pull.
  GPIOB->CRH |= GPIO_CRH_MODE13; //PB13(SPI2_SCK) - тактирование 50МГц.
	
	GPIOB->CRH |= GPIO_CRH_CNF14_0;//PB14(SPI2_MISO) - вход, подтяжка к плюсу.
  //GPIOB->ODR  = GPIOB->ODR | GPIO_ODR_ODR14; //подтяжка к плюсу.
	
	GPIOB->CRH |= GPIO_CRH_CNF15_1;//PB15(SPI2_MOSI) - выход, альтернативный режим push-pull.
  GPIOB->CRH |= GPIO_CRH_MODE15; //PB15(SPI2_MOSI) - тактирование 50МГц.
																	
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;//Включение тактирования SPI2.                                      /
  SPI2->CR1     = 0x0000;            //очистить первый управляющий регистр
	SPI2->CR2     = 0x0000;            //очистить второй управляющий регистр
  
	SPI2->CR1 |=(SPI_CR1_MSTR|      //режим "мастер".
               //задаем скорость.  BR[2:0]: Baud rate control [100]: fPCLK/32
               //SPI_CR1_BR_0 |     //
               //SPI_CR1_BR_1 |     //
               SPI_CR1_BR_2 |     //
               //Настройка для SM1628
               SPI_CR1_BIDIMODE | // 1-line bidirectional data mode selected
               SPI_CR1_CPHA |     // 
               SPI_CR1_CPOL |     //
               SPI_CR1_LSBFIRST | //LSB transmitted firstНастройка для SM1628
               
               //SPI_CR1_DFF  |	  //16 бит данных.
               SPI_CR1_SSI  |     //обеспечить высокий уровень программного NSS
               SPI_CR1_SSM  |     //разрешить программное формирование NSS
               SPI_CR1_SPE   );   //разрешить работу модуля SPI
                  
//	SPI2->CR1    |= SPI_CR1_LSBFIRST;
//	SPI2->CR1    |= SPI_CR1_DFF;				// 16 бит данных.
//	SPI2->CR1    |= SPI_CR1_SSI;        //обеспечить высокий уровень программного NSS
//	SPI2->CR1    |= SPI_CR1_SSM;        //разрешить программное формирование NSS
//	SPI2->CR1    |= SPI_CR1_SPE;        //разрешить работу модуля SPI
	
// 	SPI2->CR2 |= SPI_CR2_TXEIE;        //разрешить прерывание по окончанию передачи               /
// 	SPI2->CR2 |= SPI_CR2_RXNEIE;       //разрешить прерывание, если принят байт данных
// 	SPI2->CR2 |= SPI_CR2_ERRIE;        //разрешить прерывание при возникновении ошибки
//	NVIC_EnableIRQ (SPI2_IRQn);  
}
//-----------------------------------------------------------------------------
void Spi2BiDirMode(uint8_t mode){
  
  SPI2->CR1 &= ~(SPI_CR1_SPE);//Останов модуля SPI2.
  if(mode == SPI_TX_ONLY_MODE) SPI2->CR1 |=   SPI_CR1_BIDIOE; //Output enabled (transmit-only mode). 
  else                         SPI2->CR1 &= ~(SPI_CR1_BIDIOE);//Output disabled(receive-only mode).  
  SPI2->CR1 |=   SPI_CR1_SPE; //Запуск модуля SPI2.
}
//-----------------------------------------------------------------------------
//Передача данных(8 бит) в SPI2.
void Spi2TxData(uint8_t data){

  volatile uint32_t SpiWaitCount = 0;
  //--------------------
  while(!(SPI2->SR & SPI_SR_TXE))//Ожидание освобождения передающего буфера.
  {if(++SpiWaitCount > SPI_WAIT)return;}
  SpiWaitCount = 0;

  SPI2->DR = data;

  while(SPI2->SR & SPI_SR_BSY)
  {if(++SpiWaitCount > SPI_WAIT)return;}
} 
//-----------------------------------------------------------------------------
uint8_t Spi2RxData(void){
    
  volatile uint32_t SpiWaitCount = 0;
  //--------------------
  while(SPI2->SR & SPI_SR_BSY)
  {if(++SpiWaitCount > SPI_WAIT)return 0;}
  SpiWaitCount = 0;

  SPI2->DR = 0xFF;

  while(!(SPI2->SR & SPI_SR_RXNE))
  {if(++SpiWaitCount > SPI_WAIT)return 0;}
  return SPI2->DR;
} 
//************************************************************************************************** 
//************************************************************************************************** 
//Прерываени от SPI2.
void SPI2_IRQHandler(void){
  
  //uint16_t tmp;
  //--------------------
  //причина прерывания - окончание передачи байта
  if(SPI2->SR & SPI_SR_TXE)
    {
      //SPI2->CR1 &= ~SPI_CR1_SPE;  //Запрет работы модуля SPI
      //SPI2->SR  &= ~SPI_SR_TXE;   //сбрасываем флаг события, которое вызвало прерывание
      //SPI2->CR2 |=  SPI_CR2_TXEIE;//Запрет прерывания по окончанию передачи 
      
      //StpicCSHight;
      //StpicCSLow; 

      //LC2SostGreenLedToggel;//Мигаем для отладки          
    }
  //--------------------
  //причина прерывания - принят байт данных
  if(SPI2->SR & SPI_SR_RXNE)
    {
      //tmp = SPI1->DR;                //прочитать принятые данные
      //выполняем какие-то действия      
    }
  //--------------------
  //причина прерывания - ошибка режима ведущего
  if(SPI2->SR & SPI_SR_MODF)
    {
      //сбрасываем флаг события, которое вызвало прерывание и делаем что-то полезное
    }
  //--------------------
  //причина прерывания - переполнение приемника данных
  if(SPI2->SR & SPI_SR_OVR)
    {
      //сбрасываем флаг события, которое вызвало прерывание и делаем что-то полезное
    }
  //--------------------
  //причина прерывания - ошибка данных
//  if(SPI2->SR &= SSPI_SR_CRCERR)
//    {
//      //сбрасываем флаг события, которое вызвало прерывание и делаем что-то полезное
//    }
  //--------------------
}
//************************************************************************************************** 
//************************************************************************************************** 





