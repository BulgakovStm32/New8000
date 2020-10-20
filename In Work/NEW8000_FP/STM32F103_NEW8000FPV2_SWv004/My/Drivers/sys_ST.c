
#include "sys_ST.h"

//-----------------------------------------------------------------------------
//Настройка тактирования 72МГц. Внешний кварц 8 МГц.
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
        /* Enable Prefetch Buffer */
        FLASH->ACR |= FLASH_ACR_PRFTBE;
        /* Flash 2 wait state */
        FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
        FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;   
        //--------------------
        //Настройка тактирования шин AHB, APB1 и APB2.
        RCC->CFGR |= (uint32_t)(RCC_CFGR_HPRE_DIV1  | // AHB prescaler. SYSCLK not divided
                                RCC_CFGR_PPRE2_DIV1 | // APB2 = AHB. APB2 Fmax = 72MHz.
                                RCC_CFGR_PPRE1_DIV2); // APB1 = (AHB / 2). APB1 Fmax = 36MHz.

        //--------------------
        //PLL configuration:  = HSE * 9 = 72 MHz */
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
        RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE        |  // HSE oscillator clock selected as PLL input clock
                                //RCC_CFGR_PLLXTPRE_HSE_Div2 |  // HSE clock divided by 2                                
                                RCC_CFGR_PLLMULL9);
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
  //RCC->CFGR2 = 0x00000000;      			/* Reset CFGR2 register */  
	
  InitRCC();//Настройка тактирования микроконтроллера. Внешний кварц 12 МГц.
  
  //Разблокировка работы с flash
  FLASH->KEYR = 0x45670123;
  FLASH->KEYR = 0xCDEF89AB;
}
//*****************************************************************************
//*****************************************************************************
//Прерываени блока CSS. Возникает при сбое тактирования HSE.
void NMI_Handler( void){

  //RCC->CIR |= RCC_CIR_CSSC;//Очищаем флаг прерывания CSS иначе прерывание будет генерироваться бесконечно.
	//NVIC_SystemReset();

  LcdClear();
  LcdOutStr("NMI");
  LcdSetCursor(1, 1);
  LcdUpdate();
}
//*****************************************************************************
//*****************************************************************************
extern volatile uint32_t TaskAdr;

void HardFault_Handler_2(unsigned int *hardfault_args){
	
	unsigned int stacked_lr  = ((unsigned long) hardfault_args[5]);
	unsigned int stacked_pc  = ((unsigned long) hardfault_args[6]);
	unsigned int stacked_psr = ((unsigned long) hardfault_args[7]);
	//--------------------
	LcdClear();
	LcdSetCursor(1, 1);
  LcdOutStr("HardFault");
	
	LcdSetCursor(1, 2);
	LcdOutStr("PC     =");
	LcdPrintHEX(stacked_pc);
	
	LcdSetCursor(1, 3);
	LcdOutStr("PSR    =");
	LcdPrintHEX(stacked_psr);
	
	LcdSetCursor(1, 4);
	LcdOutStr("LR     =");
	LcdPrintHEX(stacked_lr);
	
	LcdSetCursor(1, 5);
	LcdOutStr("TaskAdr=");
	LcdPrintHEX(TaskAdr);
	
	LcdUpdateAll();
}	
//-----------------------------------------------------------
//Ассемблерная "надстройка" для обработчика "тяжёлого" отказа
//Определяет адрес стекового фрейма и передает его как
//указатель в обработчик исключенияю
__ASM void HardFault_Handler(void){

	tst	  lr, #4
	ite	  eq
	mrseq	r0, msp
	mrsne	r0, psp
	b			__cpp(HardFault_Handler_2)
}

//void HardFault_Handler(void){
//	
//	uint32_t *msp       = (uint32_t*)__get_MSP;
//	uint32_t *psp       = (uint32_t*)__get_PSP;	
//	uint32_t faultMask = (uint32_t)__get_FAULTMASK;
//	//--------------------
//  LcdClear();
//	LcdString(1, 1);
//  LcdOutStr("HardFault");
//	//Значение счетчика программ.
////	LcdPrint(Char1, String2,
////					 "PC = ");
//	
//	
//	LcdString(1, 2);
//	LcdOutStr("CFSR=");
//	LcdPrintHEX(SCB->CFSR);
//	
//	LcdString(1, 3);
//	LcdOutStr("HFSR=");
//	LcdPrintHEX(SCB->HFSR);	
//	
//	LcdString(1, 4);
//	LcdOutStr("DFSR=");
//	LcdPrintHEX(SCB->DFSR);	
//	
//	LcdString(1, 5);
//	LcdOutStr("AFSR=");
//	LcdPrintHEX(SCB->AFSR);

//	LcdString(1, 7);
//	LcdOutStr("MMFAR=");
//	LcdPrintHEX(SCB->MMFAR);
//	
//	LcdString(1, 8);
//	LcdOutStr("BFAR =");
//	LcdPrintHEX(SCB->BFAR);
//	
//  LcdUpdate();
//}
//*****************************************************************************
//*****************************************************************************
void MemManage_Handler( void){

  LcdClear();
	LcdSetCursor(1, 1);
  LcdOutStr("MemManage");
  LcdUpdate();
}
//*****************************************************************************
//*****************************************************************************
void BusFault_Handler( void){

  LcdClear();
	LcdSetCursor(1, 1);
  LcdOutStr("BusFault");
  LcdUpdate();
}
//*****************************************************************************
//*****************************************************************************
void UsageFault_Handler( void){

  LcdClear();
	LcdSetCursor(1, 1);
  LcdOutStr("UsageFault");
  LcdUpdate();
}
//*****************************************************************************
//*****************************************************************************
void SVC_Handler( void){

  LcdClear();
	LcdSetCursor(1, 1);
  LcdOutStr("SVC");
  LcdUpdate();
}
//*****************************************************************************
//*****************************************************************************








