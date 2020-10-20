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
//Структура для хранения усредненных значений каналов АЦП.
//Значение хранится в мВ.
typedef union{
	struct
    {
      //--------------------
      uint16_t Poj1AvrMeas; // 
      uint16_t Poj2AvrMeas; // 
      uint16_t Poj3AvrMeas; // 
      uint16_t ChSAvrMeas;  //
      uint16_t ZUAvrMeas;   //
      //--------------------
		}STR;
    uint16_t BLK[5];
}PojLineAvrMeasUNION_Type;
//----------------------------------------
//Структура для хранения состояний шлейфов.
volatile typedef union{
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
}PojLineStatusUNION_Type;
//-------------------------------------------------------
//-------------------------------------------------------
//Структура для хранения усредненных значений линий Гр..
typedef union{
	struct
    {
      //--------------------     
      uint16_t Lc1LineMeas;//
      uint16_t Lc1AttMeas; //
      uint16_t Lc2LineMeas;//
      uint16_t Lc2AttMeas; //
      
      uint16_t Lc1LineOldMeas;
      uint16_t Lc1AttOldMeas;
      uint16_t Lc2LineOldMeas;
      uint16_t Lc2AttOldMeas;
      //--------------------
		}STR;
    uint16_t BLK[8];
}LcMeasUNION_Type;
//----------------------------------------
//Структура для хранения статуса линий Гр.
typedef union{
	struct
    {
      //--------------------
      uint8_t Lc1LineStatus;//
      uint8_t Lc1AttStatus; //
      uint8_t Lc2LineStatus;//
      uint8_t Lc2AttStatus; //
      //--------------------
		}STR;
    uint8_t BLK[4];
}LcStatusUNION_Type;
//-------------------------------------------------------
//-------------------------------------------------------
PojLineAvrMeasUNION_Type PojLineAvrMeasSTR;//хранение усредненных значений каналов АЦП. В мВ.
PojLineStatusUNION_Type  PojLineStatusSTR; //хранение состояний пож. шлейфов.
LcMeasUNION_Type         LcMeasSTR;        //хранение усредненных значений каналов АЦП. В мВ.
LcStatusUNION_Type       LcStatusSTR;      //хранение статуса линий Гр.
static volatile uint16_t LcAvrMeas   = 0;  //усредненное измерение канала LC.
static volatile uint16_t MeasFlagReg = 0;  //регистр флагов
//*********************************************************************************
//************************Аппаратный уровень.**************************************
//Настройка АЦП. 
void AdcInit(void){

	RCC->APB2ENR |= (RCC_APB2ENR_ADC1EN |  //Разрешить тактирование АЦП1.
                   RCC_APB2ENR_IOPCEN ); //Разрешить тактирование порта PORTC
  //--------------------	  
	GPIOC->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0 |	//PC0 - аналоговый вход.
									GPIO_CRL_CNF1 | GPIO_CRL_MODE1 |	//PC1 - аналоговый вход.
									GPIO_CRL_CNF2 | GPIO_CRL_MODE2 |	//PC2 - аналоговый вход.
									GPIO_CRL_CNF3 | GPIO_CRL_MODE3 |	//PC3 - аналоговый вход.
									GPIO_CRL_CNF4 | GPIO_CRL_MODE4 |	//PC4 - аналоговый вход.
									GPIO_CRL_CNF5 | GPIO_CRL_MODE5 );	//PC5 - аналоговый вход.
	//--------------------	
  RCC->CFGR &= ~RCC_CFGR_ADCPRE;		 //				
	RCC->CFGR |=  RCC_CFGR_ADCPRE_DIV8;//предделитель 8 (72МГц/8 = 9МГц).
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
    
  OptSP1AttOff;
  OptSP1LineOff;
  OptSP2LineOff;
  OptSP2AttOff;
}
//-----------------------------------------------------------------------------
//Одно измерение АЦП.
uint16_t AdcGetMeas(uint8_t channel){
  
  ADC1->SQR3 = (channel + 10);      //загрузить номер канала. 
                                    //+10 потому, что Poj1_AdcCh подключен к каналу 10 АЦП.
  ADC1->CR2 |= ADC_CR2_SWSTART;     //запуск преобразования в регулярном канале.
  while(!(ADC1->SR & ADC_SR_EOC)){};//дождаться окончания преобразования
  //Вычитать значение самокалибровки ненужно, АЦП это делает сам.
  return (uint16_t)((ADC1->DR * AdcQuant) / 10000);//(uint16_t)((ADC1->DR * 8068) / 10000); 
}
//*********************************************************************************
//***************************Прикладной уровень.***********************************
//Измерение и усреднение по 16 выборкам для 5-ти каналов АЦП.
void PojLineMeasLoop(void){
  
  static uint32_t MeasTemp  = 0;
  static uint8_t  MeasCount = 0;
  static uint8_t  Channel   = 0; 
  //--------------------
  //Если измерения не прочитаны то выходим.
  if(MeasFlagReg & MeasPojLineEndFlag) return;
  //--------------------
  MeasTemp += (uint32_t)AdcGetMeas(Channel);
  if(++MeasCount == AdcMeasQuantity)
    {      
      PojLineAvrMeasSTR.BLK[Channel] = (uint16_t)(MeasTemp >> AdcMeasShiftDev);
      MeasTemp  = 0;
      MeasCount = 0;
      //--------------------
      //Если получены измерения для всех 5-ти каналов АЦП, 
      //то обновление состояний пожарных шлейфов.
      if(++Channel > 4)
        {  
          Channel = 0;
          //--------------------
          //Обновление состояний шлейфов.
          PojLineStatusSTR.STR.Poj1Line = PojLineStatus(PojLineAvrMeasSTR.STR.Poj1AvrMeas);
          PojLineStatusSTR.STR.Poj2Line = PojLineStatus(PojLineAvrMeasSTR.STR.Poj2AvrMeas);
          PojLineStatusSTR.STR.Poj3Line = PojLineStatus(PojLineAvrMeasSTR.STR.Poj3AvrMeas);
          PojLineStatusSTR.STR.ChSLine  = PojLineStatus(PojLineAvrMeasSTR.STR.ChSAvrMeas);
          //--------------------
          MeasFlagReg |= MeasPojLineEndFlag;
        } 
    }
  //--------------------
}
//-----------------------------------------------------------------------------
//Определение состояния линии пожарных шлейфов.
uint8_t PojLineStatus(uint16_t meas){
  
  //--------------------
  if((meas >= 200) && (meas < 800))
    {
      return NormOpenSost;
    }
  //--------------------  
  else if((meas >= 800) && (meas <= 2000))
    {
      return NormCloseSost;
    } 
  //--------------------  
  else if(meas < 200)
    {
      return BreakLineSost;
    }
   //-------------------- 
  else if(meas > 2000)
    {
      return ShortLineSost;
    }
  //--------------------
  return 0;  
}
//-----------------------------------------------------------------------------
uint16_t MeasFlagGet(void){

  volatile uint16_t temp = 0;  
  //--------------------
  __disable_irq();
  temp = MeasFlagReg;
  __enable_irq();
  return temp;
}
//-----------------------------------------------------------------------------
void MeasFlagSet(uint16_t flag){

  __disable_irq();
  MeasFlagReg |= flag;
  __enable_irq();
}
//-----------------------------------------------------------------------------
void MeasFlagClr(uint16_t flag){

  __disable_irq();
  MeasFlagReg &= ~flag;
  __enable_irq();
}
//-----------------------------------------------------------------------------
//Получение статуса пожарных линий.
uint8_t PojLineGetStatus(uint8_t line){

  uint8_t temp;
  //-------------------- 
  __disable_irq();	//Глобальный запрет прерываний.
  temp = PojLineStatusSTR.BLK[line];
  __enable_irq();	  //Глобальное разрешение прерываний. 
  return temp; 
}
//-----------------------------------------------------------------------------
//Получение измеренных значений пожарных линий в мВ.
uint16_t PojLineGetMeas(uint8_t ch){
  
  return PojLineAvrMeasSTR.BLK[ch];
}
//-----------------------------------------------------------------------------
//Получение напряжения на АКБ в мВ.
uint16_t BatLevel(void){
  
  return(uint16_t)((PojLineAvrMeasSTR.STR.ZUAvrMeas * 11)/10);
}
//*********************************************************************************
//******************************Линии Гр.******************************************
uint8_t LcGetAvrMeas(void){
  
  static uint32_t LcMeasTemp  = 0;
  static uint16_t LcMeasCount = 0;
  //--------------------
  LcMeasTemp += (uint32_t)AdcGetMeas(LC_AdcCh);
  if(++LcMeasCount == AdcMeasQuantity)
    {
      LcAvrMeas   = (uint16_t)(LcMeasTemp >> AdcMeasShiftDev);
      LcMeasTemp  = 0;
      LcMeasCount = 0;
      return 1;
    }
  return 0;
}
//-----------------------------------------------------------------------------
void LcMeasLoop(void){
  
  static uint16_t mSCount   = 0;
  static uint8_t  MeasIndex = 0;
  //--------------------
  mSCount++;
  switch(MeasIndex)
    {
      //--------------------
      case(0):
        if(mSCount == LcOptoReleyOnTimeOut) OptSP1LineOn;
        if(mSCount >= LcMeasPeriod)
          {
            if(LcGetAvrMeas() == 0) break;                           //Измерение напряжения.
            LcMeasSTR.STR.Lc1LineOldMeas = LcMeasSTR.STR.Lc1LineMeas;//Сохранение старого значения.
            LcMeasSTR.STR.Lc1LineMeas    = LcAvrMeas;                //Сохранение нового значения.
            
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
            if(LcGetAvrMeas() == 0) break;                         //Измерение напряжения.
            LcMeasSTR.STR.Lc1AttOldMeas = LcMeasSTR.STR.Lc1AttMeas;//Сохранение старого значения.            
            LcMeasSTR.STR.Lc1AttMeas    = LcAvrMeas;               //Сохранение нового значения.

            OptSP1AttOff; 
            mSCount   = 0;
            MeasIndex = 2;
          }
      break;
      //--------------------
      case(2):
        if(mSCount == LcOptoReleyOnTimeOut) OptSP2LineOn;
        if(mSCount >= LcMeasPeriod)
          {
            if(LcGetAvrMeas() == 0) break;                           //Измерение напряжения.
            LcMeasSTR.STR.Lc2LineOldMeas = LcMeasSTR.STR.Lc2LineMeas;//Сохранение старого значения.            
            LcMeasSTR.STR.Lc2LineMeas    = LcAvrMeas;                //Сохранение нового значения.

            OptSP2LineOff;
            mSCount   = 0;
            MeasIndex = 3;
          }
      break;
      //--------------------
      case(3):             
        if(mSCount == LcOptoReleyOnTimeOut) OptSP2AttOn;
        if(mSCount >= LcMeasPeriod)
          {
            if(LcGetAvrMeas() == 0) break;                         //Измерение напряжения.
            LcMeasSTR.STR.Lc2AttOldMeas = LcMeasSTR.STR.Lc2AttMeas;//Сохранение старого значения.            
            LcMeasSTR.STR.Lc2AttMeas    = LcAvrMeas;               //Сохранение нового значения.

            OptSP2AttOff; 
            mSCount   = 0;
            MeasIndex = 0;
          }
      break;
      //--------------------  
    }
  //-------------------- 
}
//-----------------------------------------------------------------------------
//Получение измеренного значения 
uint16_t LcGetValue(uint8_t line){

//  uint16_t temp;
//  __disable_irq();	//Глобальный запрет прерываний.
//  temp = LcMeasSTR.BLK[line];
//  __enable_irq();	  //Глобальное разрешение прерываний. 
//  return temp; 
  return LcMeasSTR.BLK[line];
}
//-----------------------------------------------------------------------------








