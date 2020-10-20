
#include "gpio_ST.h"

//-----------------------------------------------------------------------------
static volatile uint16_t KeyStateReg = 0; //
static volatile uint16_t MicStateReg = 0; //
//-----------------------------------------------------------------------------
//Инициализация переферии.
void GpioInit (void){
  
  //Включаем тактирование портов в/в и модуля альтернативных функций.
  RCC->APB2ENR |= (RCC_APB2ENR_IOPAEN | //порт A
                   RCC_APB2ENR_IOPBEN | //порт B
                   RCC_APB2ENR_IOPCEN | //порт C
                   RCC_APB2ENR_AFIOEN); //модуль альтернативных функций

	//Отключение JTAG-D от порта PA15, отладка через SWD.
  AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE; 
  //--------------------
  //Настройка вывода MCO(PA8) для вывода на нее системной частоты SYSCLK.
//   GPIOA->CRH &= ~GPIO_CRH_CNF8;

//   GPIOA->CRH |= GPIO_CRH_CNF8_1; 	 //PA8 -выход, альтернативный режим push-pull.
//   GPIOA->CRH |= GPIO_CRH_MODE8;	   //PA8 -выход, тактирование 50МГц.
//   RCC->CFGR |= RCC_CFGR_MCO_SYSCLK;//Подключение к выводу PA8 системную частоту.
  //RCC->CFGR |= RCC_CFGR_MCO_HSI;   //Подключение к выводу PA8 частоту HSI.
  //RCC->CFGR |= RCC_CFGR_MCO_HSE;      //Подключение к выводу PA8 частоту HSE.
  //RCC->CFGR |= RCC_CFGR_MCO_PLL;   //Подключение к выводу PA8 частоту PLL/2.  
  //--------------------
  //Замок. Подтянуты к +3,3В через 10кОм. Ножки контроллера на вход.
  //После сброса ножки мкроконтроллера находятся в режиме Input Floating mode (CNFx[1:0]=01b, MODEx[1:0]=00b).
  // Key1 - PC1.
  // Key2 - PC0.
  // Key3 - PC15.
     
  //--------------------
  //Кнопка "ПОЖАР"
  // POJ_SW  - PC2. - Вход
  // POJ_LED - PC3. - Выход
 	GPIOC->CRL &= ~GPIO_CRL_CNF3;  //- выход, режим - push-pull.
  GPIOC->CRL |=  GPIO_CRL_MODE3; //- тактирование 50МГц. 
  //--------------------
}
//-----------------------------------------------------------------------------
//Вызывается в прерывании таймера SysTick.
uint8_t GpioCheck(void){
	
 	static uint8_t  cycle       = 0;
 	static uint16_t KeyState[3] = {0, 0, 0};
  static uint16_t MicState[3] = {0, 0, 0};
	uint16_t micTemp;
  //-------------------------
	if(cycle < 3)
 		{
 			KeyState[cycle] = (KeyGPIO->IDR & (Key1Pin  | Key2Pin  | Key3Pin)); //Считывание выводов.
 			MicState[cycle] = (MicGPIO->IDR & (MicRxPin | MicTxPin | MicOkPin));//Считывание выводов.
      cycle++;
      return 0;
 		} 
  //-------------------------
	cycle = 0;
	//Мажоритарное определение состояния выводов Key.
	KeyStateReg = ~((KeyState[0] & KeyState[1]) |
									(KeyState[1] & KeyState[2]) |
									(KeyState[0] & KeyState[2]));
	//Мажоритарное определение состояния выводов Mic.
	micTemp = ((MicState[0] & MicState[1]) |
						 (MicState[1] & MicState[2]) |
						 (MicState[0] & MicState[2]));
		
	     if(micTemp == (MicTxPin | MicOkPin)) MicStateReg = MIC_CONNECTED;
  else if(micTemp == (MicRxPin))            MicStateReg = MIC_ACTIVE;
		
	return 1;
  //-------------------------
}
//-----------------------------------------------------------------------------
//Определение положения замка.
uint8_t KeyGetState(void){
	
	static uint16_t CountCycle = 0;
	uint8_t temp;
  //--------------------  		
  if(KeyStateReg & Key1Pin) 
		{
			CountCycle = 0;
			temp = KEY_CONTROL_STATE;
		}	
  if(KeyStateReg & Key2Pin)
		{
			CountCycle = 0;
			temp = KEY_CONFIG_STATE;
		}
  if(++CountCycle >= 100)
		{
			if(!(KeyStateReg & (Key1Pin | Key2Pin))) temp = KEY_LOCK_STATE;
		}	
	return temp;
}
//-----------------------------------------------------------------------------
//Получение состояния микрофона.
uint8_t MicGetStateIs(void){

	static uint16_t CountForMicState = 0;
  //--------------------  	
	if(MicStateReg == (MicTxPin | MicOkPin))
		{
			CountForMicState = 0;
      return MIC_CONNECTED;    
    }
  //--------------------
	if(MicStateReg == (MicRxPin))
    {
			CountForMicState = 0;
      return MIC_ACTIVE;    
    }
  //--------------------
  if(++CountForMicState >= 200) //500) //100)
    {
			if(MicStateReg == (MicRxPin | MicTxPin | MicOkPin))return MIC_NOT_CONNECT;
      if(MicStateReg == (MicRxPin | MicOkPin))           return MIC_FAULT;
    }
	//--------------------
	return 0;//MicNotConnectedConst;    
}
//-----------------------------------------------------------------------------
uint8_t MicState(void){

	return MicStateReg;
}
//-----------------------------------------------------------------------------













