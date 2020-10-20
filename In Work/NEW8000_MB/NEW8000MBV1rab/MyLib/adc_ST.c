//-----------------------------------------------------------------------------

#include "adc_ST.h"

//-----------------------------------------------------------------------------
//struct{
//      //--------------------
//      uint8_t  MeasCount;
//      uint32_t MeasTemp;
//      uint8_t  MeasCountForLc;
//      uint16_t mSCountForLc;
//      uint8_t  Channel;
//      uint16_t AdcFlagReg;
//      uint16_t AdcAverageMeas;
//      //--------------------
//}AdcWorkRegSTR;
//----------------------------------------
//Структура для хранения измеренных и усредненных значений каналов АЦП.
//Значение хранится в мВ.
union{
	struct
    {
      //--------------------
      uint16_t Poj1Ch;     // 
      uint16_t Poj2Ch;     // 
      uint16_t Poj3Ch;     // 
      uint16_t ChSCh;      //
      uint16_t LcCh;       //
      //--------------------
		}STR;
    uint16_t BLK[5];
}AdcChMeasSTR;
//----------------------------------------
union{
	struct
    {
      //--------------------     
      uint16_t Lc1Line;  //
      uint16_t Lc1Atten; //
      uint16_t Lc2Line;  //
      uint16_t Lc2Atten; //
      
      uint16_t Lc1LineOld;
      uint16_t Lc1AttenOld;
      uint16_t Lc2LineOld;
      uint16_t Lc2AttenOld;
      //--------------------
		}STR;
    uint16_t BLK[8];
}AdcLcChMeasSTR;
//----------------------------------------
union{
	struct
    {
      //--------------------
      uint8_t Poj1Line;     // 
      uint8_t Poj2Line;     // 
      uint8_t Poj3Line;     // 
      uint8_t ChSLine;      // 
      uint8_t PojLineAlarms;//
      //--------------------
		}STR;
    uint8_t BLK[5];
}AdcPojLineStateSTR;
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
  ADC1->CR2 |= ADC_CR2_RSTCAL;         //Сброс калибровки
  while (ADC1->CR2 & ADC_CR2_RSTCAL){};//ждем окончания сброса
  ADC1->CR2 |= ADC_CR2_CAL;            //запуск калибровки 
  while (ADC1->CR2 & ADC_CR2_CAL){};   //ждем окончания калибровки 
    
  OptSPAllOff;
}
//-----------------------------------------------------------------------------
static uint16_t AdcGetMeas(uint8_t channel){
  
  ADC1->SQR3 = (channel + 3);       //загрузить номер канала. 
                                    //+3 потому, что Poj1_AdcCh подключен к каналу 3 АЦП.
  ADC1->CR2 |= ADC_CR2_SWSTART;     //запуск преобразования в регулярном канале.
  while(!(ADC1->SR & ADC_SR_EOC)){};//дождаться окончания преобразования
  //Вычитать значение самокалибровки ненужно, АЦП это делает сам.
  return (uint16_t)((ADC1->DR * 804) / 1000); 
}
//-----------------------------------------------------------------------------
//Определение состояния линии пожарных шлейфов.
static uint8_t PojLineState(uint16_t channel){
  
  //--------------------
  if((channel >= 200) && (channel < 1000))
    {
      return NormOpenSost;
    }
  //--------------------  
  else if((channel >= 1000) && (channel <= 2500))
    {
      return NormCloseSost;
    } 
  //--------------------  
  else if(channel < 200)
    {
      return BreakLineSost;
    }
   //-------------------- 
  else if(channel > 2500)
    {
      return ShortLineSost;
    }
  //--------------------
  return 0;  
}
//-----------------------------------------------------------------------------
//Измерение и усреднение по 16 выборкам для 5-ти каналов АЦП.
void AdcAverageMeasForPojLineLoop(void){
  
  static uint32_t MeasTemp  = 0;
  static uint8_t  MeasCount = 0;
  static uint8_t  Channel   = 0; 
  //--------------------
  MeasTemp += (uint32_t)AdcGetMeas(Channel);
  if(++MeasCount == AdcMeasQuantity)
    {      
      AdcChMeasSTR.BLK[Channel] = (uint16_t)(MeasTemp >> AdcMeasShiftDev);
      MeasTemp  = 0;
      MeasCount = 0;
      //--------------------
      //Если получены измерения для всех 5-ти каналов АЦП, 
      //то обновление состояний пожарных шлейфов.
      if(++Channel > 3)//4)
        {  
          Channel = 0;
          //--------------------
          //Обновление состояний шлейфов.
          AdcPojLineStateSTR.STR.Poj1Line = PojLineGetState(AdcChMeasSTR.STR.Poj1Ch);
          AdcPojLineStateSTR.STR.Poj2Line = PojLineGetState(AdcChMeasSTR.STR.Poj2Ch);
          AdcPojLineStateSTR.STR.Poj3Line = PojLineGetState(AdcChMeasSTR.STR.Poj3Ch);
          AdcPojLineStateSTR.STR.ChSLine  = PojLineGetState(AdcChMeasSTR.STR.ChSCh);
          //Обновление состояний КЗ и обрыва шлейфов
          AdcPojLineStateSTR.STR.PojLineAlarms = (AdcPojLineStateSTR.STR.Poj1Line |
                                                  AdcPojLineStateSTR.STR.Poj2Line |
                                                  AdcPojLineStateSTR.STR.Poj3Line |
                                                  AdcPojLineStateSTR.STR.ChSLine);
          //--------------------
        } 
    }
  //--------------------
//  AdcWorkRegSTR.MeasTemp += (uint32_t)GetAdcMeas(AdcWorkRegSTR.Channel);
//  if(++AdcWorkRegSTR.MeasCount == AdcMeasQuantity)
//    {      
//      AdcChannelValueSTR.BLK[AdcWorkRegSTR.Channel] = (uint16_t)(AdcWorkRegSTR.MeasTemp >> AdcMeasShiftDev);
//      AdcWorkRegSTR.MeasTemp  = 0;
//      AdcWorkRegSTR.MeasCount = 0;
//      //--------------------
//      //Если получены измерения для всех 5-ти каналов АЦП, 
//      //то обновление состояний пожарных шлейфов.
//      if(++AdcWorkRegSTR.Channel > 3)//4)
//        {  
//          AdcWorkRegSTR.Channel = 0;
//          //--------------------
//          //Обновление состояний шлейфов.
//          AdcPojLineStateSTR.STR.Poj1 = PojLineGetState(AdcChannelValueSTR.STR.Poj1Ch);
//          AdcPojLineStateSTR.STR.Poj2 = PojLineGetState(AdcChannelValueSTR.STR.Poj2Ch);
//          AdcPojLineStateSTR.STR.Poj3 = PojLineGetState(AdcChannelValueSTR.STR.Poj3Ch);
//          AdcPojLineStateSTR.STR.ChS  = PojLineGetState(AdcChannelValueSTR.STR.ChSCh);
//          //Обновление состояний КЗ и обрыва шлейфов
//         AdcPojLineStateSTR.STR.PojLineAlarms = (AdcPojLineStateSTR.STR.Poj1 |
//                                                 AdcPojLineStateSTR.STR.Poj2 |
//                                                 AdcPojLineStateSTR.STR.Poj3 |
//                                                 AdcPojLineStateSTR.STR.ChS  );
//          //--------------------
//          AdcWorkRegSTR.AdcFlagReg |= AdcAllChConvertionEndFlag;
//          //--------------------
//        } 
//    }

}
//-----------------------------------------------------------------------------
uint8_t AdcAverageMeasForLc(void){
  
  static uint32_t LcMeasTemp  = 0;
  static uint16_t LcMeasCount = 0;
  //--------------------
  LcMeasTemp += (uint32_t)AdcGetMeas(LC_AdcCh);
  if(++LcMeasCount == AdcMeasQuantity)
    {
      AdcChMeasSTR.BLK[LC_AdcCh] = (uint16_t)(LcMeasTemp >> AdcMeasShiftDev);
      LcMeasTemp  = 0;
      LcMeasCount = 0;
      return 1;
    }
  return 0;
}
//-----------------------------------------------------------------------------
void AdcLcMeasLoop(void){
  
  static uint16_t mSCount   = 0;
  static uint16_t MeasIndex = 0;
  //--------------------
  mSCount++;
  switch(MeasIndex)
    {
      //--------------------
      case(0):
        if(mSCount == LcOptoReleyOnTimeOut) OptSP1LineOn;
        if(mSCount >= LcMeasPeriod)
          {
            if(AdcAverageMeasForLc() == 0) break;//Измерение напряжения.
            AdcLcChMeasSTR.STR.Lc1LineOld = AdcLcChMeasSTR.STR.Lc1Line;//Сохранение старого значения.
            AdcLcChMeasSTR.STR.Lc1Line    = AdcChMeasSTR.STR.LcCh;     //Сохранение нового значения.
            
            OptSP1LineOff; 
            mSCount   = 0;
            MeasIndex = 1; 
          }  
      break;
      //--------------------
      case(1):             
        if(mSCount == LcOptoReleyOnTimeOut) OptSP1AttOn;
        if(mSCount >= LcMeasPeriod)
          {
            if(AdcAverageMeasForLc() == 0) break; //Измерение напряжения.
            AdcLcChMeasSTR.STR.Lc1AttenOld = AdcLcChMeasSTR.STR.Lc1Atten;//Сохранение старого значения.            
            AdcLcChMeasSTR.STR.Lc1Atten    = AdcChMeasSTR.STR.LcCh;      //Сохранение нового значения.

            OptSP1AttOff; 
            mSCount   = 0;
            MeasIndex = 0;
          }
      break;
//      //--------------------
//      case(Lc2LineValue):
//        if(AdcWorkRegSTR.mSCountForLc == LcOptoReleyOnTimeOut) OptSP2LineOn;
//        if(AdcWorkRegSTR.mSCountForLc >= LcMeasPeriod)
//          {
//            //Измерение напряжения.
//            if(AdcAverageMeasForLc() == 0) break;
//            //Сохранение старого значения.
//            AdcLcChannelValueSTR.STR.Lc2LineOldValueChannel = AdcLcChannelValueSTR.STR.Lc2LineChannel; 
//            //Сохранение нового значения.            
//            AdcLcChannelValueSTR.STR.Lc2LineChannel = AdcChannelValueSTR.STR.LcChannel;
//            
//            AdcWorkRegSTR.mSCountForLc = 0;  
//            OptSP2LineOff;          
//            
//            AdcWorkRegSTR.MeasCountForLc = Lc2AttenValue;  
//          }     
//      break;
//      //--------------------
//      case(Lc2AttenValue):
//        if(AdcWorkRegSTR.mSCountForLc == LcOptoReleyOnTimeOut) OptSP2AttOn;
//        if(AdcWorkRegSTR.mSCountForLc >= LcMeasPeriod)
//          {
//            //Измерение напряжения.
//            if(AdcAverageMeasForLc() == 0) break;
//            //Сохранение старого значения.
//            AdcLcChannelValueSTR.STR.Lc2AttenOldValueChannel = AdcLcChannelValueSTR.STR.Lc2AttenChannel;
//            //Сохранение нового значения.
//            AdcLcChannelValueSTR.STR.Lc2AttenChannel = AdcChannelValueSTR.STR.LcChannel; 
//            
//            AdcWorkRegSTR.mSCountForLc = 0;
//            OptSP2AttOff;       
//            
//            AdcWorkRegSTR.MeasCountForLc = Lc1LineValue;     
//   
//            AdcWorkRegSTR.AdcFlagReg |= AdcAllChLCConvertionEndFlag; 
//          }      
//      break;
//      //--------------------      
    }
  //-------------------- 
}
//-----------------------------------------------------------------------------
uint16_t AdcGetChMeas(uint8_t ch){
  
  return AdcChMeasSTR.BLK[ch];
}
//-----------------------------------------------------------------------------
uint16_t AdcGetLcMeas(uint8_t ch){
  
  return AdcLcChMeasSTR.BLK[ch];	
}
//-----------------------------------------------------------------------------
//uint16_t AdcGetFlagReg(void){
//  
//  return AdcWorkRegSTR.AdcFlagReg;
//}
//-----------------------------------------------------------------------------
//void AdcClereFlag(uint16_t flag){
//  
//  AdcWorkRegSTR.AdcFlagReg &= ~(flag) ;
//}
//-----------------------------------------------------------------------------
uint8_t PojLineGetState(uint8_t ch){
  
  return AdcPojLineStateSTR.BLK[ch]; 
}  
//-----------------------------------------------------------------------------
