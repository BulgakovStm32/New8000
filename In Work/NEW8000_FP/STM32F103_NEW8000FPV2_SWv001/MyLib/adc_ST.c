//-----------------------------------------------------------------------------

#include "adc_ST.h"

//-----------------------------------------------------------------------------
struct{
      //--------------------
      uint8_t  MeasCount;
      uint32_t MeasTemp;
      uint8_t  MeasCountForLc;
      uint16_t mSCountForLc;
      uint8_t  Channel;
      uint16_t AdcFlagReg;
      uint16_t AdcAverageMeas;
      //--------------------
}AdcWorkRegSTR;
//----------------------------------------
//Структура для хранения измеренных и усредненных значений каналов АЦП.
//Значение хранится в мВ.
union{
	struct
    {
      //--------------------
      uint16_t Poj1Channel;     // 
      uint16_t Poj2Channel;     // 
      uint16_t Poj3Channel;     // 
      uint16_t ChSChannel;      //
      uint16_t LcChannel;       //
      //--------------------
		}STR;
    uint16_t BLK[5];
}AdcChannelValueSTR;
//----------------------------------------
union{
	struct
    {
      //--------------------     
      uint16_t Lc1LineChannel;  //
      uint16_t Lc1AttenChannel; //
      uint16_t Lc2LineChannel;  //
      uint16_t Lc2AttenChannel; //
      
      uint16_t Lc1LineOldValueChannel;
      uint16_t Lc1AttenOldValueChannel;
      uint16_t Lc2LineOldValueChannel;
      uint16_t Lc2AttenOldValueChannel;
      //--------------------
		}STR;
    uint16_t BLK[8];
}AdcLcChannelValueSTR;
//----------------------------------------
union{
	struct
    {
      //--------------------
      uint8_t Poj1Sost;         // 
      uint8_t Poj2Sost;         // 
      uint8_t Poj3Sost;         // 
      uint8_t ChSSost;          // 
//      uint8_t Lc1LineSost;      // 
//      uint8_t Lc1AttenSost;     // 
//      uint8_t Lc2LineSost;      // 
//      uint8_t Lc2AttenSost;     //  
      uint8_t AllLineAlarmsSost;//
      //--------------------
		}STR;
    uint8_t BLK[5];
}AdcChannelSostSTR;
//-----------------------------------------------------------------------------
//Настройка АЦП. 
void AdcInit(void){

	RCC->APB2ENR |= ( RCC_APB2ENR_ADC1EN |  //Разрешить тактирование АЦП1.
                    RCC_APB2ENR_IOPAEN ); //Разрешить тактирование порта PORTA
  //--------------------	  
	GPIOA->CRL &= ~(GPIO_CRL_CNF3 | GPIO_CRL_MODE3 |	//PA3 - аналоговый вход.
									GPIO_CRL_CNF4 | GPIO_CRL_MODE4 |	//PA4 - аналоговый вход.
									GPIO_CRL_CNF5 | GPIO_CRL_MODE5 |	//PA5 - аналоговый вход.
									GPIO_CRL_CNF6 | GPIO_CRL_MODE6 |	//PA6 - аналоговый вход.
									GPIO_CRL_CNF7 | GPIO_CRL_MODE7 );	//PA7 - аналоговый вход.
	//--------------------	
  RCC->CFGR &= ~RCC_CFGR_ADCPRE;		//				
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV8;//предделитель 8 (24МГц/8 = 3МГц).
  //предочистка регистра.
  ADC1->CR1  = 0;       
  ADC1->SQR1 = 0;
  ADC1->SQR3 = 0; 
  //Настройка работы АЦП.
  ADC1->CR2  =  ADC_CR2_EXTSEL;   //выбрать источником запуска разряд SWSTART
  ADC1->CR2 |= (ADC_CR2_EXTTRIG | //разр. внешний запуск регулярного канала
                ADC_CR2_ADON);    //включить АЦП
  //Рабочий вариант самокалибровки АЦП.  
  //Вычитать значение самокалибровки ненужно, АЦП это делает сам.
  ADC1->CR2 |= ADC_CR2_RSTCAL;          //Сброс калибровки
  while (ADC1->CR2 & ADC_CR2_RSTCAL){}; //ждем окончания сброса
  ADC1->CR2 |= ADC_CR2_CAL;             //запуск калибровки 
  while (ADC1->CR2 & ADC_CR2_CAL){};    //ждем окончания калибровки 
    
  OptSPAllOff;
}
//-----------------------------------------------------------------------------
static uint16_t GetAdcMeas(uint8_t channel){
  
  //-------------------- 
  ADC1->SQR3 = (channel + 3);       //загрузить номер канала. 
                                    //+3 потому, что Poj1_AdcCh подключен к каналу 3 АЦП.
  ADC1->CR2 |= ADC_CR2_SWSTART;     //запуск преобразования в регулярном канале.
  while(!(ADC1->SR & ADC_SR_EOC)){};//дождаться окончания  преобразования
  //Вычитать значение самокалибровки ненужно, АЦП это делает сам.
  return (uint16_t)((ADC1->DR * 804) / 1000); 
}
//-----------------------------------------------------------------------------
//Определение состояния линии пожарных шлейфов.
static uint8_t LineSost(uint16_t channel){
  
//  //--------------------
//  if((channel >= 200) && (channel < 1000))
//    {
//      return NormOpenSost;
//    }
//  //--------------------  
//  else if((channel >= 1000) && (channel <= 2500))
//    {
//      return NormCloseSost;
//    } 
//  //--------------------  
//  else if(channel < 200)
//    {
//      return BreakLineSost;
//    }
//   //-------------------- 
//  else if(channel > 2500)
//    {
//      return ShortLineSost;
//    }
//  //--------------------
  return 0;  
}
//-----------------------------------------------------------------------------
//Измерение и усреднение по 16 выборкам для 5-ти каналов АЦП.
void AdcAverageMeasForLineLoop(void){
  
  //--------------------
  AdcWorkRegSTR.MeasTemp += (uint32_t)GetAdcMeas(AdcWorkRegSTR.Channel);
  if(++AdcWorkRegSTR.MeasCount == AdcMeasQuantity)
    {      
      AdcChannelValueSTR.BLK[AdcWorkRegSTR.Channel] = (uint16_t)(AdcWorkRegSTR.MeasTemp >> AdcMeasShiftDev);
      AdcWorkRegSTR.MeasTemp  = 0;
      AdcWorkRegSTR.MeasCount = 0;
      //--------------------
      //Если получены измерения для всех 5-ти каналов АЦП, 
      //то обновление состояний пожарных шлейфов.
      if(++AdcWorkRegSTR.Channel > 3) //4)
        {  
          AdcWorkRegSTR.Channel = 0;
          //--------------------
          //Обновление состояний шлейфов.
          AdcChannelSostSTR.STR.Poj1Sost = LineSost(AdcChannelValueSTR.STR.Poj1Channel);
          AdcChannelSostSTR.STR.Poj2Sost = LineSost(AdcChannelValueSTR.STR.Poj2Channel);
          AdcChannelSostSTR.STR.Poj3Sost = LineSost(AdcChannelValueSTR.STR.Poj3Channel);
          AdcChannelSostSTR.STR.ChSSost  = LineSost(AdcChannelValueSTR.STR.ChSChannel);
          //Обновление состояний КЗ и обрыва шлейфов
          AdcChannelSostSTR.STR.AllLineAlarmsSost = (AdcChannelSostSTR.STR.Poj1Sost |
                                                     AdcChannelSostSTR.STR.Poj2Sost |
                                                     AdcChannelSostSTR.STR.Poj3Sost |
                                                     AdcChannelSostSTR.STR.ChSSost  );
          //--------------------
          AdcWorkRegSTR.AdcFlagReg |= AdcAllChConvertionEndFlag;
          //--------------------
        } 
    }
}
//-----------------------------------------------------------------------------
uint8_t AdcAverageMeasForLc(void){
  
  static uint32_t LcMeasTemp  = 0;
  static uint16_t LcMeasCount = 0;
  //--------------------
  LcMeasTemp += (uint32_t)GetAdcMeas(LC_AdcCh);
  if(++LcMeasCount == AdcMeasQuantity)
    {
      AdcChannelValueSTR.BLK[LC_AdcCh] = (uint16_t)(LcMeasTemp >> AdcMeasShiftDev);
      LcMeasTemp  = 0;
      LcMeasCount = 0;
      return 1;
    }
  return 0;
}
//-----------------------------------------------------------------------------
void AdcLcMeasLoop(void){
  
  AdcWorkRegSTR.mSCountForLc++;
  //--------------------
  switch(AdcWorkRegSTR.MeasCountForLc)
    {
      //--------------------
      case(Lc1LineValue):
        if(AdcWorkRegSTR.mSCountForLc == LcOptoReleyOnTimeOut) OptSP1LineOn;
        if(AdcWorkRegSTR.mSCountForLc >= LcMeasPeriod)
          {
            //Измерение напряжения.
            if(AdcAverageMeasForLc() == 0 ) break;
            //Сохранение старого значения.
            AdcLcChannelValueSTR.STR.Lc1LineOldValueChannel = AdcLcChannelValueSTR.STR.Lc1LineChannel;
            //Сохранение нового значения.
            AdcLcChannelValueSTR.STR.Lc1LineChannel = AdcChannelValueSTR.STR.LcChannel; 
            
            AdcWorkRegSTR.mSCountForLc = 0;
            OptSP1LineOff; 
            
            AdcWorkRegSTR.MeasCountForLc = Lc1AttenValue; 
          }  
      break;
      //--------------------
      case(Lc1AttenValue):             
        if(AdcWorkRegSTR.mSCountForLc == LcOptoReleyOnTimeOut) OptSP1AttOn;
        if(AdcWorkRegSTR.mSCountForLc >= LcMeasPeriod)
          {
            //Измерение напряжения.
            if(AdcAverageMeasForLc() == 0 ) break;
            //Сохранение старого значения.
            AdcLcChannelValueSTR.STR.Lc1AttenOldValueChannel = AdcLcChannelValueSTR.STR.Lc1AttenChannel;
            //Сохранение нового значения.            
            AdcLcChannelValueSTR.STR.Lc1AttenChannel = AdcChannelValueSTR.STR.LcChannel; 
            
            AdcWorkRegSTR.mSCountForLc = 0;   
            OptSP1AttOff; 
            
            AdcWorkRegSTR.MeasCountForLc = Lc2LineValue;
          }
      break;
      //--------------------
      case(Lc2LineValue):
        if(AdcWorkRegSTR.mSCountForLc == LcOptoReleyOnTimeOut) OptSP2LineOn;
        if(AdcWorkRegSTR.mSCountForLc >= LcMeasPeriod)
          {
            //Измерение напряжения.
            if(AdcAverageMeasForLc() == 0 ) break;
            //Сохранение старого значения.
            AdcLcChannelValueSTR.STR.Lc2LineOldValueChannel = AdcLcChannelValueSTR.STR.Lc2LineChannel; 
            //Сохранение нового значения.            
            AdcLcChannelValueSTR.STR.Lc2LineChannel = AdcChannelValueSTR.STR.LcChannel;
            
            AdcWorkRegSTR.mSCountForLc = 0;  
            OptSP2LineOff;          
            
            AdcWorkRegSTR.MeasCountForLc = Lc2AttenValue;  
          }     
      break;
      //--------------------
      case(Lc2AttenValue):
        if(AdcWorkRegSTR.mSCountForLc == LcOptoReleyOnTimeOut) OptSP2AttOn;
        if(AdcWorkRegSTR.mSCountForLc >= LcMeasPeriod)
          {
            //Измерение напряжения.
            if(AdcAverageMeasForLc() == 0 ) break;
            //Сохранение старого значения.
            AdcLcChannelValueSTR.STR.Lc2AttenOldValueChannel = AdcLcChannelValueSTR.STR.Lc2AttenChannel;
            //Сохранение нового значения.
            AdcLcChannelValueSTR.STR.Lc2AttenChannel = AdcChannelValueSTR.STR.LcChannel; 
            
            AdcWorkRegSTR.mSCountForLc = 0;
            OptSP2AttOff;       
            
            AdcWorkRegSTR.MeasCountForLc = Lc1LineValue;        
            AdcWorkRegSTR.AdcFlagReg |= AdcAllChLCConvertionEndFlag; 
          }      
      break;
      //--------------------      
    }
  //-------------------- 
}
//-----------------------------------------------------------------------------
uint16_t GetAdcChannelVolue(uint8_t channel){
  
  return AdcChannelValueSTR.BLK[channel];
}
//-----------------------------------------------------------------------------
uint16_t GetLcChannelVolue(uint8_t channel){
  
  return AdcLcChannelValueSTR.BLK[channel];	
}
//-----------------------------------------------------------------------------
uint16_t GetAdcFlagReg(void){
  
  return AdcWorkRegSTR.AdcFlagReg;
}
//-----------------------------------------------------------------------------
void ClereAdcFlag(uint16_t flag){
  
  AdcWorkRegSTR.AdcFlagReg &= ~(flag) ;
}
//-----------------------------------------------------------------------------
uint8_t GetLineSost(uint8_t channel){
  
  return AdcChannelSostSTR.BLK[channel]; 
}  
//-----------------------------------------------------------------------------
