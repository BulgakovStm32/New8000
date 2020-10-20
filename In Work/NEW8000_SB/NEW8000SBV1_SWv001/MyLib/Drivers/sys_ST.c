
#include "sys_ST.h"

//***************************************************************************************************
//***************************************************************************************************
//Настройка тактирования 48МГц. Внешний кварц 8 МГц.
static uint32_t Init_RCC(void){
	
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
	
  /* SYSCLK, HCLK, PCLK configuration ----------------------------------------*/
  /* Enable HSE */    
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
  /* Wait till HSE is ready and if Time out is reached exit */
  do
		{
			HSEStatus = RCC->CR & RCC_CR_HSERDY;
			StartUpCounter++;  
		} while((HSEStatus == 0) && (StartUpCounter != 0x5000));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
		{
			HSEStatus = (uint32_t)0x01;
		}
  else
		{
			HSEStatus = (uint32_t)0x00;
		}  

  if (HSEStatus == (uint32_t)0x01)
		{
			/* Enable Prefetch Buffer and set Flash Latency */
			FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;
			
			
			
			/* (1)  Test if PLL is used as System clock */
			/* (2)  Select HSI as system clock */
			/* (3)  Wait for HSI switched */
			if ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_PLL)			 /* (1) */
			{          
				RCC->CFGR &= (uint32_t) (~RCC_CFGR_SW); 							 /* (2) */
				while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) /* (3) */
				{
					/* For robust implementation, add here time-out management */      
				}
			}
			//(4)  Disable the PLL */
			//(5)  Wait until PLLRDY is cleared */
			RCC->CR &= (uint32_t)(~RCC_CR_PLLON); /* (4) */        
			while((RCC->CR & RCC_CR_PLLRDY) != 0) /* (5) */
				{
					/* For robust implementation, add here time-out management */    
				}
			//HSE/PREDIV selected as PLL input clock 	
			RCC->CFGR |= RCC_CFGR_PLLSRC_HSE_PREDIV;
			//(6)  Set the PLL multiplier to 6 */
			RCC->CFGR &= ~RCC_CFGR_PLLMUL;
			RCC->CFGR |=  RCC_CFGR_PLLMUL6;	
			//(7)  Enable the PLL */ 
			//(8)  Wait until PLLRDY is set */				
			RCC->CR |= RCC_CR_PLLON; 						 /* (7) */
			while((RCC->CR & RCC_CR_PLLRDY) == 0)/* (8) */
				{
					/* For robust implementation, add here time-out management */
				}
			//(9)  Select PLL as system clock */
			//(10) Wait until the PLL is switched on */			
			RCC->CFGR |= (uint32_t) (RCC_CFGR_SW_PLL); 						/* (9) */
			while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)/* (10) */
				{
					/* For robust implementation, add here time-out management */
				}
		}
  else
		{ /* If HSE fails to start-up, the application will have wrong clock 
					 configuration. User can add here some code to deal with this error */
		}  
	return HSEStatus;
}
//*****************************************************************************
void Sys_Init (void){
	
  /* Set HSION bit */
  RCC->CR |= (uint32_t)0x00000001;

#if defined(STM32F051)  
  /* Reset SW[1:0], HPRE[3:0], PPRE[2:0], ADCPRE and MCOSEL[2:0] bits */
  RCC->CFGR &= (uint32_t)0xF8FFB80C;
#else
  /* Reset SW[1:0], HPRE[3:0], PPRE[2:0], ADCPRE, MCOSEL[2:0], MCOPRE[2:0] and PLLNODIV bits */
  RCC->CFGR &= (uint32_t)0x08FFB80C;
#endif /* STM32F051 */
	 
  /* Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xFEF6FFFF;
  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;
  /* Reset PLLSRC, PLLXTPRE and PLLMUL[3:0] bits */
  RCC->CFGR &= (uint32_t)0xFFC0FFFF;
  /* Reset PREDIV1[3:0] bits */
  RCC->CFGR2 &= (uint32_t)0xFFFFFFF0;
  /* Reset USART1SW[1:0], I2C1SW, USBSW and ADCSW bits */
  RCC->CFGR3 &= (uint32_t)0xFFFFFE6CU;
  /* Reset HSI14 bit */
  RCC->CR2 &= (uint32_t)0xFFFFFFFE;
  /* Disable all interrupts */
  RCC->CIR = 0x00000000;  
	
  Init_RCC();//Настройка тактирования микроконтроллера. Внешний кварц 8 МГц.
}
//***************************************************************************************************
//***************************************************************************************************
//Прерываени блока CSS. Возникает при сбое тактирования HSE.
void NMI_Handler( void){

  //RCC->CIR |= RCC_CIR_CSSC;//Очищаем флаг прерывания CSS иначе прерывание будет генерироваться бесконечно.
	//NVIC_SystemReset();
}
//*****************************************************************************
//*****************************************************************************
void HardFault_Handler( void){

//	NVIC_SystemReset();
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
//***************************************************************************************************
//***************************************************************************************************








