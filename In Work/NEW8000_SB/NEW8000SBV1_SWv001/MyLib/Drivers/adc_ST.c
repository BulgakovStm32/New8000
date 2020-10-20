
#include "adc_ST.h"

//-----------------------------------------------------------------------------
//Настройка АЦП. 
void Adc_Init(void){

	//--------------------	
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; //Enable the peripheral clock of the ADC
	ADC1->CFGR1	 |= ADC_CFGR2_CKMODE_1; //PCLK/4 (Synchronous clock mode)
	ADC1->SMPR   |= ADC_SMPR_SMP;  			//239.5 ADC clock cycles. Select a sampling mode of 111 i.e. 239.5 ADC clk to be greater than 17.1us.
	
	//RCC->CR2 		 |= RCC_CR2_HSI14ON;				  /* (2) */
	//while((RCC->CR2 & RCC_CR2_HSI14RDY) == 0) /* (3) */
	//	{
	//		/*Для надежной реализации добавьте сюда управление тайм-аутом */
	//	}
	//ADC1->CFGR2 &= (~ADC_CFGR2_CKMODE); 		 /* (4) */
	//--------------------
	//Cамокалибровка АЦП.
	if((ADC1->CR & ADC_CR_ADEN) != 0) ADC1->CR |= ADC_CR_ADDIS;//Clear ADEN by setting ADDIS*/ 
	while(ADC1->CR & ADC_CR_ADDIS){};		 		        					 //
	ADC1->CFGR1 &= ~ADC_CFGR1_DMAEN;													 //
	ADC1->CR |= ADC_CR_ADCAL;																 	 //Запуск калибровки.
	while(ADC1->CR & ADC_CR_ADCAL){};									       	 //Ожидание окончения калибровки
	
  //Cамокалибровка АЦП.  
	/* (1) Ensure that ADEN = 0 */
	/* (2) Clear ADEN by setting ADDIS*/
	/* (3) Clear DMAEN */
	/* (4) Launch the calibration by setting ADCAL */
	/* (5) Wait until ADCAL=0 */
//	if((ADC1->CR & ADC_CR_ADEN) != 0) /* (1) */
//		{
//			ADC1->CR |= ADC_CR_ADDIS; 	  /* (2) */
//		}
//	while((ADC1->CR & ADC_CR_ADEN) != 0)
//		{
//			/*Для надежной реализации добавьте сюда управление тайм-аутом */
//		}
//	ADC1->CFGR1 &= ~ADC_CFGR1_DMAEN; 			 /* (3) */
//	ADC1->CR |= ADC_CR_ADCAL; 						 /* (4) */
//	while ((ADC1->CR & ADC_CR_ADCAL) != 0) /* (5) */
//		{
//			/*Для надежной реализации добавьте сюда управление тайм-аутом */
//		}
	//--------------------
	//ADC enable
	/* (1) Ensure that ADRDY = 0 */
	/* (2) Clear ADRDY */
	/* (3) Enable the ADC */
	/* (4) Wait until ADC ready */
	if((ADC1->ISR & ADC_ISR_ADRDY) != 0) ADC1->ISR &= ~ADC_ISR_ADRDY;// (1) (2)
	ADC1->CR |= ADC_CR_ADEN; 																				 // (3)
	while((ADC1->ISR & ADC_ISR_ADRDY) == 0)													 // (4) 
		{
			/*Для надежной реализации добавьте сюда управление тайм-аутом */
		}	
}
////-----------------------------------------------------------------------------
//uint32_t Adc_GetState(void){

//	return (ADC1->CR & ADC_CR_ADEN); 
//}
//-----------------------------------------------------------------------------
//Одно измерение АЦП.
uint16_t Adc_GetMeas(uint8_t ch){
  
	ADC1->CHSELR = (1 << ch);     			//загрузить номер канала. 
	//ADC1->SMPR  |= ADC_SMPR_SMP;  			//239.5 ADC clock cycles. Select a sampling mode of 111 i.e. 239.5 ADC clk to be greater than 17.1us.
	ADC1->CR    |= ADC_CR_ADSTART;			//запуск преобразования.
	while(!(ADC1->ISR & ADC_ISR_EOC)){};//дождаться окончания преобразования
	return ADC1->DR;	
}
//-----------------------------------------------------------------------------
void Adc_Start(void){

	ADC1->CR |= ADC_CR_ADEN; 							 
	while((ADC1->ISR & ADC_ISR_ADRDY) == 0){}; 
}
//-----------------------------------------------------------------------------
void Adc_Stop(void){
	
	ADC1->CR |= ADC_CR_ADSTP;
	while(ADC1->CR & ADC_CR_ADSTP){};	
}
//-----------------------------------------------------------------------------








