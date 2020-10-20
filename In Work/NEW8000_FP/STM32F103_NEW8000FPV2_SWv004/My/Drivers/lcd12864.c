
#include "lcd12864.h"

//-----------------------------------------------------------------------------
static uint8_t LcdCache[LCD12864_CACHE_SIZE];//Cache buffer in SRAM 128*64 bits or 1024 bytes
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
static void Pause(volatile uint16_t pause){
  
  while(--pause); 
}
//-----------------------------------------------------------------------------
//Настройка порта данных на ВЫХОД.
static void Lcd12864DataPortOUTPUT(void){
	
	//DB0 - PA0. DB1 - PA1.
	//DB2 - PA2. DB3 - PA3.	
	//DB4 - PA4. DB5 - PA5.
	//DB6 - PA6. DB7 - PA7.
	//выход, режим - push-pull.
	GPIOA->CRL &= ~( GPIO_CRL_CNF0 | GPIO_CRL_CNF1 |
									 GPIO_CRL_CNF2 | GPIO_CRL_CNF3 |
									 GPIO_CRL_CNF4 | GPIO_CRL_CNF5 |
									 GPIO_CRL_CNF6 | GPIO_CRL_CNF7 );
  //тактирование 50МГц.                 
  GPIOA->CRL |= ( GPIO_CRL_MODE0 | GPIO_CRL_MODE1 |
									GPIO_CRL_MODE2 | GPIO_CRL_MODE3 |
									GPIO_CRL_MODE4 | GPIO_CRL_MODE5 |
									GPIO_CRL_MODE6 | GPIO_CRL_MODE7 );
}
//-----------------------------------------------------------------------------
//Настройка порта данных на ВХОД.
static void Lcd12864DataPortINPUT(void){
	
	//DB0 - PA0. DB1 - PA1.
	//DB2 - PA2. DB3 - PA3.	
	//DB4 - PA4. DB5 - PA5.
	//DB6 - PA6. DB7 - PA7.
	//вход
	GPIOA->CRL &= ~( GPIO_CRL_MODE0 | GPIO_CRL_MODE1 |
									 GPIO_CRL_MODE2 | GPIO_CRL_MODE3 |
								   GPIO_CRL_MODE4 | GPIO_CRL_MODE5 |
									 GPIO_CRL_MODE6 | GPIO_CRL_MODE7 );
	//Сброс нужных битов.
	GPIOA->CRL &= ~( GPIO_CRL_CNF0 | GPIO_CRL_CNF1 |
									 GPIO_CRL_CNF2 | GPIO_CRL_CNF3 |
									 GPIO_CRL_CNF4 | GPIO_CRL_CNF5 |
									 GPIO_CRL_CNF6 | GPIO_CRL_CNF7 ); 
	//режим - Input floating.
	GPIOA->CRL |= ( GPIO_CRL_CNF0_0 | GPIO_CRL_CNF1_0 |
									GPIO_CRL_CNF2_0 | GPIO_CRL_CNF3_0 |
									GPIO_CRL_CNF4_0 | GPIO_CRL_CNF5_0 |
									GPIO_CRL_CNF6_0 | GPIO_CRL_CNF7_0 );
 }
//-----------------------------------------------------------------------------
static void Lcd12864GpioInit(void){
	
	//--------------------
	//Включаем тактирование порта 
	RCC->APB2ENR |= (RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN);
  //--------------------
  //E1  - PC4.
	//E2  - PC5.
	//RES - PB0.
  //R/W - PB1.
  //A0  - PB2.
  //E   - PB10.
	//LED - PB11.

  //выход, режим - push-pull.
  GPIOC->CRL &= ~( GPIO_CRL_CNF4 | 
                   GPIO_CRL_CNF5 );

	GPIOB->CRL &= ~( GPIO_CRL_CNF0 | 
                   GPIO_CRL_CNF1 |   
                   GPIO_CRL_CNF2 );
	
	GPIOB->CRH &= ~( GPIO_CRH_CNF10 | 
                   GPIO_CRH_CNF11 );
  //тактирование 50МГц.  
  GPIOC->CRL |= ( GPIO_CRL_MODE4_1 |               
                  GPIO_CRL_MODE5_1 );

  GPIOB->CRL |= ( GPIO_CRL_MODE0_1 |
                  GPIO_CRL_MODE1_1 |                 
                  GPIO_CRL_MODE2_1 );
									
  GPIOB->CRH |= ( GPIO_CRH_MODE10_1 |
                  GPIO_CRH_MODE11_1 );
}
//-----------------------------------------------------------------------------
//Строб.
static void Lcd12864Strob(void){

	Lcd12864EHight;
  Pause(LCD12864PauseConst);
	Lcd12864ELow;
  Pause(LCD12864PauseConst); 
}
//-----------------------------------------------------------------------------
//Чтение статуса из дисплея.
static uint8_t Lcd12864ReadStatusReg(void){
	
	uint16_t	temp;
	//-------------------- 
	Lcd12864CS1On;
	Lcd12864SetCmd;
	Lcd12864Read; 
	Lcd12864DataPortINPUT();//Порт данных на вход.
	//--------------------
	//Строб.
  Lcd12864Strob();
  //--------------------
	temp = (Lcd12864DataPort->IDR & 0x00FF);
	//--------------------
	Lcd12864CS1Off;
	Lcd12864SetCmd;
	Lcd12864Write; 
	Lcd12864DataPortOUTPUT();//Порт данных на выход.
	
	return (uint8_t)temp;
} 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Передача данных в дисплей.
void Lcd12864_Send(uint8_t data, uint8_t type){
	
  if(type) Lcd12864SetData;//Передаются данные.
  else     Lcd12864SetCmd; //Передаются команды.
  //--------------------
  Lcd12864DataSet(data);  //Вывод данных в порт.
	Pause(LCD12864PauseConst);
  Lcd12864Strob();        //Строб.
}
//-----------------------------------------------------------------------------
void Lcd12864_BackLight(uint8_t state){
	
  if(state) Lcd12864LedOn;
  else      Lcd12864LedOff; 
}
//-----------------------------------------------------------------------------
void Lcd12864_Init(void){
	
  Lcd12864GpioInit();               //Инициализация портов  
	Lcd12864ResHiht;                  //Перевод в рабочий режим.
  //Ожижание готовности дисплея.
  while(Lcd12864ReadStatusReg() & LCD12864StatReg_RESETbit){}
  //--------------------
	Lcd12864CS12();										       //Включаем оба чипа
  //Lcd12864FillingScreen(0x00);           //Очистка экрана.
  Lcd12864_Send(LCD12864_X0, LCD12864_CMD);//Ставим X=0
  Lcd12864_Send(LCD12864_Y0, LCD12864_CMD);//Ставим Y=0	
	Lcd12864_Send(0xc0, LCD12864_CMD);	     //Устанавливаем начало отображения.
  Lcd12864_Send(0x3f, LCD12864_CMD);       //Разрешаем отображение
  //Ожижание готовности дисплея.
	while(Lcd12864ReadStatusReg() & LCD12864StatReg_ON_OFFbit){}	
	//--------------------
  //Lcd12864CS12Off();           			//Выключаем оба чипа. 
	Lcd12864LedOn;	                  //Включение подсветки экрана.
}
//-----------------------------------------------------------------------------
uint8_t* Lcd12864_VideoBuf(void){

	return LcdCache;
}
//-----------------------------------------------------------------------------
//Вывод буфера за 16 проходов на дисплей.
uint8_t Lcd12864_Update(void){

         uint8_t      i = 0;
  static uint8_t      j = 0;
  static uint8_t  state = 0;
	static uint16_t index = 0;
	//-------------------- 
  switch(state){
      //--------------------
      case(0):
				Lcd12864CS1();
        Lcd12864_Send(LCD12864_X0 + j, LCD12864_CMD);
        Lcd12864_Send(LCD12864_Y0, LCD12864_CMD);
        for (i=0; i<64; i++)
          {
            //Lcd12864Send((Y0+i), LCD12864_CMD);
            Lcd12864_Send(LcdCache[index++], LCD12864_DATA);	
          }
        index += 64;
        if(++j >= 8) 
          {
                j = 0;
            index = 0;
            state = 1;
          }
      break;
      //--------------------
      case(1):
        Lcd12864CS2();
        Lcd12864_Send(LCD12864_X0 + j, LCD12864_CMD);
        Lcd12864_Send(LCD12864_Y0, LCD12864_CMD);
        index += 64;
        for (i=0; i<64; i++)
          {
            //Lcd12864Send((Y0+i), LCD12864_CMD);
            Lcd12864_Send(LcdCache[index++], LCD12864_DATA);	
          }
        if(++j >= 8) 
          {
                j = 0;
            index = 0;
            state = 0;
            return 1;
          }
      break;
      //-------------------- 
    }
  //---------------------
  return 0;
}
//-----------------------------------------------------------------------------
void  Lcd12864_UpdateAll(void){
	
 while(!Lcd12864_Update()){};
}
//-----------------------------------------------------------------------------

















