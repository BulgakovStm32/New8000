
#include "LM6063D.h"

//-----------------------------------------------------------------------------

unsigned char  Lm6063LcdCache [Lm6063_CACHE_SIZE];	//Cache buffer in SRAM 84*48 bits or 504 bytes
unsigned int   Lm6063LcdCacheIdx;              	//Cache index


unsigned char ComTable[]={0,1,2,3,4,5,6,7};

//-----------------------------------------------------------------------------
void Lm6063LcdPause(uint16_t pause){
  
  while(pause != 0) {--pause;}
}
//-----------------------------------------------------------------------------
void Lm6063LcdGpioInit(void){
  
 	//--------------------
	//Включаем тактирование порта 
	RCC->APB2ENR |= (RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN);
  //--------------------
  // A0   - PB5.
	// /RES - PB6.
	// /CS  - PB7.
  //выход, режим - push-pull.
	GPIOB->CRL &= ~( GPIO_CRL_CNF5 | 
                   GPIO_CRL_CNF6 |   
                   GPIO_CRL_CNF7 );
  //тактирование 50МГц.                 
  GPIOB->CRL |= ( GPIO_CRL_MODE5 |  
                  GPIO_CRL_MODE6 |                      
                  GPIO_CRL_MODE7 );	

  Lm6063LcdResHight;
  Lm6063LcdCsHight;
}	
//-----------------------------------------------------------------------------
//Передача байта в дисплей.
void Lm6063LcdSendByte(uint8_t byte, uint8_t cmd){
  
  Lm6063LcdCsLow;           //Enable display controller (active low)

  if(cmd) Lm6063LcdA1Hight;//данные.
  else    Lm6063LcdA1Low;  //команда.

  Spi2TxData(byte);      //
  
  Lm6063LcdCsHight;        //Disable display controller
}
//-----------------------------------------------------------------------------
//Copies the LCD cache into the device RAM
void Lm6063LcdUpdate(void){
  
  int i;
  //--------------------
  Lm6063LcdSendByte(0x80, Lm6063Cmd);		//команды установки указателя памяти дисплея на 0,0
  Lm6063LcdSendByte(0x40, Lm6063Cmd);

  for (i = 0; i < Lm6063_CACHE_SIZE; i++)		//грузим данные
    {
      Lm6063LcdSendByte(Lm6063LcdCache[i], Lm6063Data);
    }
}
//-----------------------------------------------------------------------------
//Заливка экрана.
void Lm6063LcdFillingScreen(uint8_t byte){
  
	unsigned char i,j;
  //--------------------
	Lm6063LcdSendByte(0x40, Lm6063Cmd);	//Set Display Start Line = com0
  
	for(i=0;i<8;i++)
    {
      Lm6063LcdSendByte(0xB0|i, Lm6063Cmd);	//Set Page Address as ComTable
      Lm6063LcdSendByte(0x10,   Lm6063Cmd); //Set Column Address = 0
      Lm6063LcdSendByte(0x00,   Lm6063Cmd); //Colum from 0 -> 131 auto add
      for(j=0;j<132;j++)
        {
          Lm6063LcdSendByte(byte, Lm6063Data);
        }
    }
}
//-----------------------------------------------------------------------------
//инициализация SPI и дисплея
void Lm6063LcdInit(void){
  
  Lm6063LcdGpioInit();
  Lm6063LcdPause(0x0FFF);//delay_ms(1);
 
  //дернули ресет
  Lm6063LcdResHight;
  Lm6063LcdResLow; 
  Lm6063LcdResHight;
  
	Lm6063LcdSendByte(0xaf, Lm6063Cmd);	//Display OFF
	Lm6063LcdSendByte(0x40, Lm6063Cmd);
  
  //--------------------
  //ADC select
  //Sets the display RAM address SEG output correspondence 
	//Lm6063LcdSendByte(0xa0, Lm6063Cmd);//normal
	Lm6063LcdSendByte(0xa1, Lm6063Cmd);//reverse 
  //--------------------
  //
	Lm6063LcdSendByte(0xa6, Lm6063Cmd);
	Lm6063LcdSendByte(0xa4, Lm6063Cmd);
	Lm6063LcdSendByte(0xa2, Lm6063Cmd);
	
  //--------------------
  //Common Output Mode Select
	Lm6063LcdSendByte(0xc0, Lm6063Cmd);//Normal
  //Lm6063LcdSendByte(0xc8, Lm6063Cmd);//Reverse 
	//--------------------
  
	Lm6063LcdSendByte(0x2f, Lm6063Cmd);
	Lm6063LcdSendByte(0x25, Lm6063Cmd);
	Lm6063LcdSendByte(0xf8, Lm6063Cmd);
	
	Lm6063LcdSendByte(0x00, Lm6063Cmd);
//	Lm6063LcdSendByte(0x01, Lm6063Cmd);


	//--------------------
	//Electronic volume mode set
	Lm6063LcdSendByte(0x81, Lm6063Cmd);
	Lm6063LcdSendByte(0x0f, Lm6063Cmd);//Electronic volume register set
	
	//--------------------	
	Lm6063LcdSendByte(0xaf, Lm6063Cmd);
}
//-----------------------------------------------------------------------------
void Lm6063LcdBmp(unsigned char *puts){
  
	unsigned char i,j;
	unsigned int  x = 0;
  //--------------------
	Lm6063LcdSendByte(0x40, Lm6063Cmd);	//Set Display Start Line = com0
  
	for(i=0;i<8;i++)
    {
      Lm6063LcdSendByte(0xB0|ComTable[i], Lm6063Cmd);	//Set Page Address
      Lm6063LcdSendByte(0x10, Lm6063Cmd);	//Set Column Address = 0
      Lm6063LcdSendByte(0x80, Lm6063Cmd);	//Colum from 0 -> 129 auto add
      for(j=0;j<128;j++)
        {
          Lm6063LcdSendByte(puts[x], Lm6063Data);
          x++;
        }
    }
}
//-----------------------------------------------------------------------------
//void LcdContrast (unsigned char contrast) 	//contrast -> Contrast value from 0x00 to 0x7F
//        {
//        if (contrast > 0x7F) return;
//        LcdSend( 0x21, LCD_CMD );               //LCD Extended Commands
//        LcdSend( 0x80 | contrast, LCD_CMD );    //Set LCD Vop (Contrast)
//        LcdSend( 0x20, LCD_CMD );               //LCD Standard Commands,Horizontal addressing mode
//        }
//-----------------------------------------------------------------------------        
//void LcdMode (unsigned char mode) 		//режим дисплея: 0 - blank, 1 - all on, 2 - normal, 3 - inverse
//        {
//        if (mode > 3) return;
//        LcdSend( 0b00001000|((mode<<1|mode)&0b00000101), LCD_CMD ); 	//LCD mode
//        }
//-----------------------------------------------------------------------------       
//void LcdPwrMode (void) 				//инвертирует состояние вкл/выкл дисплея
//        {
//        power_down = ~power_down;
//        LcdSend( 0x20|power_down<<3, LCD_CMD );
//        }
//-----------------------------------------------------------------------------
