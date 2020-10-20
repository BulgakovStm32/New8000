
#include "Eep.h"

//-----------------------------------------------------------------------------
EepHeaderUNION_Type EepHeaderUNION;

//EepFrameUNION_Type  EepErrorFrameUNION;


static EepInfoStr_t EepInfoStr;
//-----------------------------------------------------------------------------
//Управление EEPROM. PA8(EEP_HOLD), PB11(EEP_WP), PB12(EEP_CS).
//Включение тактирования порта А и В в GpioInit().
void EepInit(void){
  
  uint8_t count = 0;
  //--------------------
  //Настройка пина PB12(EEP_CS). EEP_CS подтянут через 100кОм к +3,3В.
  while ( (GPIOA->IDR & GPIO_IDR_IDR11) == 0)//Ждем когда на выводе установится "1".
    {
      count++;
      if(count == 0xFF)//Если "1" так и не установилась, то EEP_CS
        {
          
        }     
    }
  //--------------------
	//GPIOA->CRH &= ~(GPIO_CRH_CNF8);	
	GPIOB->CRH &= ~GPIO_CRH_CNF11;
  
	//После сброса CNF[1:0]=01b 
	GPIOA->CRH |=  GPIO_CRH_MODE8;	 //PA8 (EEP_HOLD)        - выход, Open-drain, 50МГц.
	GPIOB->CRH |=  GPIO_CRH_MODE11;  //PB11(EEP_WP)          - выход, Open-drain, 50МГц.
							   //GPIO_CRH_MODE12); //PB12(SPI2_NSS/EPP_CS) - выход, Push-pull,  50МГц.	
	
  //--------------------
  EepChipUnsel;	
  EepHoldHight;
  EepWPHight;  
}
//-----------------------------------------------------------------------------
static uint8_t EepCheckWriteProgress(void){ 
  
  uint8_t temp = 0xFF; 
  //--------------------
  while(temp--);
  //Чтение регистра статуса и проверка разрешена ли запись.
  EepChipSel;                   //Выбор чипа памяти.
  
  Spi2SendData(EepCmdRDSRConst);//передача комады "Read Status Register"
  temp = Spi2SendData(0x00);    //прием ответа от чипа.
  
  EepChipUnsel;                 //
  
  
  //--------------------
  if(temp & EepStatRegBitWIP) EepCheckWriteProgress();
  
  return 0;  
}
//-----------------------------------------------------------------------------
//Проверка ячеек памяти микросхемы EEPROM.
void EepMemArrayCheck(void){
  
  volatile uint16_t  count = 0;
  volatile uint8_t   temp  = 0;
  //--------------------
  while( EepCheckWriteProgress() == 0){} 
  
  //Разрешение записи.
  EepChipSel;
  Spi2SendData(EepCmdWRENConst);
  EepChipUnsel;
  
  //Пауза.
  //while(count < 0xFF00){count++;}
  //--------------------
  //Чтение регистра статуса и проверка разрешена ли запись.
  EepChipSel;
  
  Spi2SendData(EepCmdWRENConst);
  temp = Spi2SendData(0x00);
  EepChipUnsel;
  
  if(temp & EepStatRegBitWEL)
    {
      EepHeaderUNION.STR.NumBadSectors |= (1 << 0);
    }
    
  //Пауза.
  //count = 0;
  //while(count < 0xFF00){count++;}  
  //--------------------
  //Запись "0" во все ячейки.
  EepChipSel;    
  Spi2SendData(EepCmdWRITEConst);                  //Команда на чтение из памяти.
  Spi2SendData(EepMemArrayAddrStartConst & 0x00FF);//Младший байт адреса откуда начнем чтение.
  Spi2SendData(EepMemArrayAddrStartConst >> 8);    //Старший байт адреса
   
  for(count = 0; count < EepMemArrayAddrEndConst; count++)
    {
      Spi2SendData(0x00);
      //Пауза.
      temp = 0;
      while(temp <= 0xFF){temp++;}
    } 
    
   EepChipUnsel;
  
  EepHeaderUNION.STR.NumBadSectors |= (1 << 1);    
  //Пауза.
  count = 0;
  while(count < 0xFF00){count++;}     
  //--------------------
  //Чтение. Должны быть везде нули.
  EepChipSel;
  Spi2SendData(EepCmdREADConst);                   //Команда на чтение из памяти.
  Spi2SendData(EepMemArrayAddrStartConst & 0x00FF);//Младший байт адреса откуда начнем чтение.
  Spi2SendData(EepMemArrayAddrStartConst >> 8);    //Старший байт адреса

  for(count = 0; count < EepMemArrayAddrEndConst; count++)
    {
      temp = Spi2SendData(0x00);
      if(temp == 0) 
        {
          EepHeaderUNION.STR.NumBadSectors |= (1 << 2);
        }
    }
    
  EepChipUnsel;
  //--------------------
}
//-----------------------------------------------------------------------------
uint16_t EepGetNumberBadSectors(void){
  
  return EepHeaderUNION.STR.NumBadSectors;
}
//-----------------------------------------------------------------------------
void EepReadSR(void){
  
  EepChipSel;
  
  Spi2SendData(EepCmdWRENConst);
  Spi2SendData(0x00);
 
  while(Spi2GetStat());
  
  EepChipUnsel;
  
  
  
  EepChipSel;
  
  Spi2SendData(EepCmdWRSRConst);
  Spi2SendData(0x0c);
 
  while(Spi2GetStat());
  
  EepChipUnsel;
}
//-----------------------------------------------------------------------------
void EepWrite(uint8_t* data){
  
  
  //--------------------

  //--------------------
  
  //--------------------
}
//-----------------------------------------------------------------------------
EepInfoStr_t* EepInfo(void){

	return &EepInfoStr;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------












