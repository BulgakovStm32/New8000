
#include "sys_ST.h"

//-----------------------------------------------------------------------------
//Настройка тактирования микроконтроллера. Внешний кварц 8 МГц.
uint32_t InitRCC(void){
	
    uint32_t StartUpCounter = 0, HSEStatus = 0;
    //--------------------
    //SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/        
    RCC->CR |= ((uint32_t)RCC_CR_HSEON);//Enable HSE.
    /* Wait till HSE is ready and if Time out is reached exit */
    do
      {
				HSEStatus = RCC->CR & RCC_CR_HSERDY;
				StartUpCounter++;  
      } 
    while((HSEStatus == 0) && (StartUpCounter != HSEStartUp_TimeOut));
    //--------------------
    if ((RCC->CR & RCC_CR_HSERDY) != RESET)
      {
        HSEStatus = (uint32_t)0x01;
      }
    else
      {
        HSEStatus = (uint32_t)0x00;
      }  
    //--------------------
    if (HSEStatus == (uint32_t)0x01)
      {
        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1; /* HCLK = SYSCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;/* PCLK2 = HCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;/* PCLK1 = HCLK */
        /*  PLL configuration:  = (HSE / 2) * 6 = 24 MHz */
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
        RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_PREDIV1 | RCC_CFGR_PLLXTPRE_PREDIV1_Div2 | RCC_CFGR_PLLMULL6);
        /* Enable PLL */
        RCC->CR |= RCC_CR_PLLON;
        /* Wait till PLL is ready */
        while((RCC->CR & RCC_CR_PLLRDY) == 0) {}
        /* Select PLL as system clock source */
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
        RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    
        /* Wait till PLL is used as system clock source */
        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)  {}        
        RCC->CR |= RCC_CR_CSSON; //Включаем защиту HSE от сбоев - CSS.
      }
    else
      {
          /* If HSE fails to start-up, the application will have wrong clock 
           configuration. User can add here some code to deal with this error */
      } 
    return HSEStatus;
}
//-----------------------------------------------------------------------------
void SysInit (void){
	
  //Reset the RCC clock configuration to the default reset state(for debug purpose).
  RCC->CR 	|= (uint32_t)0x00000001;	/* Set HSION bit */
  RCC->CFGR &= (uint32_t)0xF0FF0000;	/* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
  RCC->CR 	&= (uint32_t)0xFEF6FFFF;  /* Reset HSEON, CSSON and PLLON bits */
  RCC->CR 	&= (uint32_t)0xFFFBFFFF;  /* Reset HSEBYP bit */
  RCC->CFGR &= (uint32_t)0xFF80FFFF;	/* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
  RCC->CIR 	 = 0x009F0000;  				  /* Disable all interrupts and clear pending bits  */
  RCC->CFGR2 = 0x00000000;      			/* Reset CFGR2 register */  
	
  InitRCC();//Настройка тактирования микроконтроллера. Внешний кварц 12 МГц.

  //Разблокировка работы с flash
  FLASH->KEYR = 0x45670123;
  FLASH->KEYR = 0xCDEF89AB;
}
//-----------------------------------------------------------------------------
//Стирание страницы flash.
//Перед записью необходимо стереть данные по нужным адресам, 
//это особенность флеша.
//pageAddress - любой адрес, принадлежащий стираемой странице
void Internal_Flash_Erase(uint32_t pageAddress){

	while (FLASH->SR & FLASH_SR_BSY);
	if (FLASH->SR & FLASH_SR_EOP)
    {
      FLASH->SR = FLASH_SR_EOP;
    }

	FLASH->CR |= FLASH_CR_PER;
	FLASH->AR = pageAddress;
	FLASH->CR |= FLASH_CR_STRT;
	while (!(FLASH->SR & FLASH_SR_EOP));
	FLASH->SR = FLASH_SR_EOP;
	FLASH->CR &= ~FLASH_CR_PER;
}
//-----------------------------------------------------------------------------
//Запись страницы flash.
//data - указатель на записываемые данные
//address - адрес во flash
//count - количество записываемых байт, должно быть кратно 2
void Internal_Flash_Write(unsigned char* data, uint32_t address, unsigned int count){

	unsigned int i;
  //--------------------
	while (FLASH->SR & FLASH_SR_BSY);
	if (FLASH->SR & FLASH_SR_EOP)
    {
      FLASH->SR = FLASH_SR_EOP;
    }

	FLASH->CR |= FLASH_CR_PG;

	for (i = 0; i < count; i += 2)
    {
      *(volatile unsigned short*)(address + i) = (((unsigned short)data[i + 1]) << 8) + data[i];
      while (!(FLASH->SR & FLASH_SR_EOP));
      FLASH->SR = FLASH_SR_EOP;
    }
	FLASH->CR &= ~(FLASH_CR_PG);
}
//-----------------------------------------------------------------------------
uint8_t Internal_Flash_Read(uint32_t address){

	return *((uint8_t *)address);
}
//*****************************************************************************
//*****************************************************************************
//Прерываени блока CSS. Возникает при сбое тактирования HSE.
void NMI_Handler( void){

  RCC->CIR |= RCC_CIR_CSSC;//Очищаем флаг прерывания CSS иначе прерывание будет генерироваться бесконечно.
	NVIC_SystemReset();
}
//*****************************************************************************
//*****************************************************************************
void HardFault_Handler( void){

 
}
//*****************************************************************************
//*****************************************************************************
void MemManage_Handler( void){

 
}
//*****************************************************************************
//*****************************************************************************
void BusFault_Handler( void){


}
//*****************************************************************************
//*****************************************************************************
void UsageFault_Handler( void){

 
}
//*****************************************************************************
//*****************************************************************************
void SVC_Handler( void){


}
//*****************************************************************************
//*****************************************************************************
