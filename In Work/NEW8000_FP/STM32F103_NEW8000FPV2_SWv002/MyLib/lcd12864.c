
#include "lcd12864.h"

//-----------------------------------------------------------------------------
static uint8_t   LcdTextBuf[32];		      //текстовый буфер для вывода на LCD
static uint8_t   LcdCache[LCD_CACHE_SIZE];//Cache buffer in SRAM 128*64 bits or 1024 bytes
static uint16_t  LcdCacheIdx = 0;         //Cache index


extern const unsigned char AsciiTabWG12864[]; //Таблица символов.

#define TabAscii  AsciiTabWG12864  //Рабочая.
//-----------------------------------------------------------------------------
static void Lcd12864Pause(uint16_t pause){
  
  while(pause != 0) {--pause;}
}
//-----------------------------------------------------------------------------
//Строб.
static void Lcd12864Strob(void){

	Lcd12864EHight;
  Lcd12864Pause(LCD12864PauseConst);
	Lcd12864ELow;
  Lcd12864Pause(LCD12864PauseConst); 
}
//-----------------------------------------------------------------------------
//Передача данных в дисплей.
static void Lcd12864Send(uint8_t data, uint8_t type){
	
  if(type) Lcd12864SetData;//Передаются данные.
  else     Lcd12864SetCmd; //Передаются команды.
  //--------------------
  Lcd12864DataSet(data);  //Вывод данных в порт.
	Lcd12864Pause(LCD12864PauseConst);
  Lcd12864Strob();        //Строб.
}
//-----------------------------------------------------------------------------
//Управление линиями CS1 и CS2.
static void Lcd12864SelCS(uint8_t cs){
	
	if(cs == CS1)
		{
			Lcd12864CS2Off;
			//Lcd12864Pause(50);
			Lcd12864CS1On;
		}
	else if(cs == CS2)
		{
			Lcd12864CS1Off;
			//Lcd12864Pause(50);
			Lcd12864CS2On;
		}
	else if(cs == CS12On)
		{
			Lcd12864CS1On;
			Lcd12864CS2On;
		}
	else if(cs == CS12Off)
		{
			Lcd12864CS1Off;
			Lcd12864CS2Off;
		}
}
//-----------------------------------------------------------------------------
//Заливка экрана.
void Lcd12864FillingScreen(uint8_t data){ 
	
	uint8_t i,j;
	//--------------------
  for (j=0;j<8;j++)
   {
      Lcd12864Send(X0+j, LCD12864_CMD);
      for (i=0;i<64;i++)
        {
          //Lcd12864Pause(0xffff);
          Lcd12864Send(Y0+i, LCD12864_CMD);
          Lcd12864Send(data, LCD12864_DATA);	
        }
   }
	//--------------------
}
//-----------------------------------------------------------------------------
//Вывод буфера на дисплей.
//void LcdUpdate(void){
//  
//  uint8_t  i, j, y;
//  uint16_t index = 0;
//	//-------------------- 
//  for (j=0; j<8; j++)
//    {
//			Lcd12864SelCS(CS1);
//			Lcd12864Send(X0+j, LCD12864_CMD);
//      for (i=0; i<128; i++)
//        {
//          if(i >= 64)
//						{
//							Lcd12864SelCS(CS2);
//							Lcd12864Send(X0+j, LCD12864_CMD);
//							y = (i - 64);
//						}
//          else 
//						{
//							y = i;
//						}
//          Lcd12864Send((Y0+y), LCD12864_CMD);
//					Lcd12864Send(LcdCache[index++], LCD12864_DATA);	
//        }
//		}
//  //LcdClear();//Очистка видеопамяти.
//  //--------------------
//}
//-----------------------------------------------------------------------------
//Вывод буфера за два прохода на дисплей.
//void LcdUpdate(void){

//  uint8_t         i, j;
//  uint16_t        index = 0;
//  static uint8_t  state = 0;
//	//-------------------- 
//  switch(state){
//      //--------------------
//      case(0):
//        Lcd12864SelCS(CS1);
//        for (j=0; j<8; j++)
//          {
//            Lcd12864Send(X0+j, LCD12864_CMD);
//            for (i=0; i<64; i++)
//              {
//                Lcd12864Send((Y0+i), LCD12864_CMD);
//                Lcd12864Send(LcdCache[index++], LCD12864_DATA);	
//              }
//            index += 64;
//          }
//        state = 1;
//      break;
//      //--------------------
//      case(1):
//        Lcd12864SelCS(CS2);
//        for (j=0; j<8; j++)
//          {
//            Lcd12864Send(X0+j, LCD12864_CMD);
//            index += 64;
//            for (i=0; i<64; i++)
//              {
//                Lcd12864Send((Y0+i), LCD12864_CMD);
//                Lcd12864Send(LcdCache[index++], LCD12864_DATA);	
//              }
//          }
//        state = 0;
//      break;
//      //-------------------- 
//      case(2):
//        LcdClear();
//        state = 0;
//      break;
//      //-------------------- 
//    }
//  //---------------------
//}
//-----------------------------------------------------------------------------
//Вывод буфера за 16 проходов на дисплей.
uint8_t LcdUpdate(void){

         uint8_t      i = 0;
  static uint8_t      j = 0;
  static uint16_t index = 0;
  static uint8_t  state = 0;
	//-------------------- 
  switch(state){
      //--------------------
      case(0):
        //Lcd12864SelCS(CS1);
        Lcd12864CS2Off;
        Lcd12864CS1On;

        Lcd12864Send(X0+j, LCD12864_CMD);
        Lcd12864Send(Y0  , LCD12864_CMD);
        for (i=0; i<64; i++)
          {
            //Lcd12864Send((Y0+i), LCD12864_CMD);
            Lcd12864Send(LcdCache[index++], LCD12864_DATA);	
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
        //Lcd12864SelCS(CS2);
        Lcd12864CS1Off;
        Lcd12864CS2On;

        Lcd12864Send(X0+j, LCD12864_CMD);
        Lcd12864Send(Y0  , LCD12864_CMD);
        index += 64;
        for (i=0; i<64; i++)
          {
            //Lcd12864Send((Y0+i), LCD12864_CMD);
            Lcd12864Send(LcdCache[index++], LCD12864_DATA);	
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
//      case(2):
//        LcdClear();
//        state = 0;
//      break;
      //-------------------- 
    }
  //---------------------
  return 0;
}
//-----------------------------------------------------------------------------
//Clears the display
void LcdClear(void){
  
  uint16_t i;
  //--------------------
  for (i = 0; i < LCD_CACHE_SIZE; i++) LcdCache[i] = 0;	//забиваем всю память 0
  //LcdUpdate ();
}
//-----------------------------------------------------------------------------
void LcdFilling(void){
  
  uint16_t i;
  //--------------------
  for (i = 0; i < LCD_CACHE_SIZE; i++) LcdCache[i] = 0xFF;	//забиваем всю память 0
  //LcdUpdate ();
}
//-----------------------------------------------------------------------------
//управление пикселем с координатами x,y. mode -> Off, On or Xor.
void LcdPixel(uint8_t x, uint8_t y, uint8_t mode){
  
  uint16_t index;
  uint16_t offset, data;
  //--------------------
  //если передали в функцию некорректные данные - выходим.
  if ( (x > LCD_X_RES) || (y > LCD_Y_RES) ) return;	
  //-------------------- 
	index = (((int)(y)/8)*128)+x;//считаем номер байта в массиве памяти дисплея
	offset  = y-((y/8)*8);       //считаем номер бита в этом байте
  data = LcdCache[index];      //берем байт по найденному индексу.
  //редактируем бит в этом байте
       if( mode == PIXEL_OFF ) data &= (~(0x01 << offset));	
  else if( mode == PIXEL_ON  ) data |= (  0x01 << offset);
  else if( mode == PIXEL_XOR ) data ^= (  0x01 << offset);
  //--------------------
  LcdCache[index] = data;		//загружаем байт назад
}
//-----------------------------------------------------------------------------
//Draws a line between two points on the display - по Брезенхейму
void LcdLine(int x1, int y1, int x2, int y2, uint8_t mode){
  
  signed int dy = 0;
  signed int dx = 0;
  signed int stepx = 0;
  signed int stepy = 0;
  signed int fraction = 0;
  //--------------------
  //если передали в функцию некорректные данные - выходим.
  if (x1>LCD_X_RES || x2>LCD_X_RES || y1>LCD_Y_RES || y2>LCD_Y_RES) return;
 //--------------------
  dy = y2 - y1;
  dx = x2 - x1;
  if (dy < 0) 
    {
      dy = -dy;
      stepy = -1;
    }
  else stepy = 1;
  if (dx < 0)
    {
      dx = -dx;
      stepx = -1;
    }
  else stepx = 1;
  dy <<= 1;
  dx <<= 1;
  LcdPixel(x1,y1,mode);
  if (dx > dy)
    {
      fraction = dy - (dx >> 1); 
      while (x1 != x2)
        {
          if (fraction >= 0)
            {
              y1 += stepy;
              fraction -= dx;
            }
          x1 += stepx;
          fraction += dy;  
          LcdPixel(x1,y1,mode);
        }
    }
  else
    {
      fraction = dx - (dy >> 1);
      while (y1 != y2)
        {
          if (fraction >= 0)
            {
              x1 += stepx;
              fraction -= dy;
            }
          y1 += stepy;
          fraction += dx;
          LcdPixel(x1,y1,mode);
        }
    }
}
//-----------------------------------------------------------------------------
//рисуем круг по координатам с радиусом - по Брезенхейму
void LcdCircle(uint8_t x, uint8_t y, uint8_t radius, uint8_t mode){
  
  signed char xc = 0;
  signed char yc = 0;
  signed char p = 0;
  //--------------------
  if (x > LCD_X_RES && y > LCD_Y_RES) return;
 //--------------------
  yc = radius;
  p  = 3 - (radius<<1);
  while (xc <= yc)  
    {
      LcdPixel(x + xc, y + yc, mode);
      LcdPixel(x + xc, y - yc, mode);
      LcdPixel(x - xc, y + yc, mode);
      LcdPixel(x - xc, y - yc, mode);
      LcdPixel(x + yc, y + xc, mode);
      LcdPixel(x + yc, y - xc, mode);
      LcdPixel(x - yc, y + xc, mode);
      LcdPixel(x - yc, y - xc, mode);
        if (p < 0) p += (xc++ << 2) + 6;
      else  p += ((xc++ - yc--)<<2) + 10;
    }
}
//-----------------------------------------------------------------------------
//рисуем батарейку с заполнением в %
void LcdBar(int x1, int y1, int x2, int y2, uint8_t persent){
  
  unsigned char horizon_line,horizon_line2,i;
  //--------------------
  if(persent>100)return;
  
  LcdLine(x1,y2,x2,y2,1);  //down
  LcdLine(x2,y1,x2,y2,1);  //right
  LcdLine(x1,y1,x1,y2,1);  //left
  LcdLine(x1,y1,x2,y1,1);  //up
  LcdLine(x1+7,y1-1,x2-7,y1-1,1);
  LcdLine(x1+7,y1-2,x2-7,y1-2,1);

  horizon_line = persent*(y2-y1-3)/100;
  for(i=0;i<horizon_line;i++) LcdLine(x1+2,y2-2-i,x2-2,y2-2-i,1);

  horizon_line2 = (y2-y1-3);
  for(i=horizon_line2;i>horizon_line;i--) LcdLine(x1+2,y2-2-i,x2-2,y2-2-i,0);
}
//-----------------------------------------------------------------------------
//Установка курсора в положение Х,У. Диапазон значений Х,У: 1,1 .. 14,8.
void LcdGotoXYFont(uint8_t x, uint8_t y){
  
  if ((x <= 24) && (y <= 8)) 
    {
      LcdCacheIdx = (uint16_t)((y-1) * 128) + (uint16_t)((x-1) * 6);
    }
}
//-----------------------------------------------------------------------------
//Очистка текстового буфера.
void clean_lcd_buf(void){
  
	uint8_t i;
	//-------------------- 
	for (i=0; i<32; i++) LcdTextBuf[i] = 0;
}   
//----------------------------------------------------------------------------- 
//Displays a character at current cursor location and increment cursor location
void LcdChr(uint16_t ch){
  
  unsigned char i;
  //--------------------
  //Проверка на максимум.
  //if(ch > LCD_CACHE_SIZE) return;
  //--------------------
  for (i=0; i<5; i++)
    {
      //выделяем байт-столбик из символа и грузим в массив - 5 раз
      LcdCache[LcdCacheIdx++] = TabAscii[ch*5+i];	
    }
  LcdCache[LcdCacheIdx++] = 0x00;	//добавляем пробел между символами
}
//-----------------------------------------------------------------------------
//Печать строки.
void LcdString(uint8_t x, uint8_t y){
  
	unsigned char i;
	//--------------------
	//if (x > 15 || y > 8) return;
  if (x > 22 || y > 8) 
    {
      x = 22;
      y = 8;
    }
  //--------------------
	LcdGotoXYFont (x, y);
 
	for (i=0; i<(22-x); i++)
    {
      if(LcdTextBuf[i]) 
        {
          LcdChr(LcdTextBuf[i]);
        }
    }
	clean_lcd_buf(); 
}
//-----------------------------------------------------------------------------
//Displays a bold character at current cursor location and increment cursor location
void LcdChrBold(int ch){
  
  unsigned char i;
  unsigned char a = 0, b = 0, c = 0;
  //--------------------    	
  for (i = 0; i < 5; i++)
    {
      c = TabAscii[(ch*5+i)];		//выделяем столбец из символа

      b  = (c & 0x01) * 3;    //"растягиваем" столбец на два байта 
      b |= (c & 0x02) * 6;
      b |= (c & 0x04) * 12;
      b |= (c & 0x08) * 24;

      c >>= 4;
      a  = (c & 0x01) * 3;
      a |= (c & 0x02) * 6;
      a |= (c & 0x04) * 12;
      a |= (c & 0x08) * 24;

      LcdCache[LcdCacheIdx]   = b;	  //копируем байты в экранный буфер
      LcdCache[LcdCacheIdx+1] = b;  //дублируем для получения жирного шрифта
      LcdCache[LcdCacheIdx+128] = a;
      LcdCache[LcdCacheIdx+129] = a;
      LcdCacheIdx = LcdCacheIdx+2;
    }

  LcdCache[LcdCacheIdx++] = 0x00;	//для пробела между символами
  LcdCache[LcdCacheIdx++] = 0x00;
}
//-----------------------------------------------------------------------------
//Печатает символ на текущем месте, большой и жирный.
void LcdStringBold (unsigned char x, unsigned char y){
  
	unsigned char i;
	//-------------------- 
	//if (x > 11 && y > 5) return;
  
	LcdGotoXYFont (x, y);
  
	for ( i = 0; i < 12-x; i++ ) if (LcdTextBuf[i]) LcdChrBold(LcdTextBuf[i]); 
	clean_lcd_buf();
}
//-----------------------------------------------------------------------------	
//Displays a character at current cursor location and increment cursor location
void LcdChrBig (int ch){
  
  unsigned char i;
  unsigned char a = 0, b = 0, c = 0;
  //-------------------- 
  for ( i = 0; i < 5; i++ )
    {
      c = TabAscii[(ch*5+i)];		//выделяем столбец из символа

      b  = (c & 0x01) * 3;            //"растягиваем" столбец на два байта 
      b |= (c & 0x02) * 6;
      b |= (c & 0x04) * 12;
      b |= (c & 0x08) * 24;

      c >>= 4;
      a  = (c & 0x01) * 3;
      a |= (c & 0x02) * 6;
      a |= (c & 0x04) * 12;
      a |= (c & 0x08) * 24;
      LcdCache[LcdCacheIdx] = b;
      LcdCache[LcdCacheIdx+128] = a;
      LcdCacheIdx = LcdCacheIdx+1;
    }
  LcdCache[LcdCacheIdx++] = 0x00;
}
//-----------------------------------------------------------------------------
//Displays a string at current cursor location
void LcdStringBig (unsigned char x, unsigned char y){
  
	unsigned char i;
	//-------------------- 
	if (x > 22 && y > 5) return;
  
	LcdGotoXYFont (x, y);
  
	for (i=0; i<(22-x); i++) if (LcdTextBuf[i]) LcdChrBig(LcdTextBuf[i]); 
  
	clean_lcd_buf();
}
//-----------------------------------------------------------------------------
uint8_t LcdOutStr(char *txt){
  
  uint8_t i = 0;
  //-------------------- 
	clean_lcd_buf();
  while(*txt)
    {
      LcdChr(*txt++);
      i++;  
    } 
  return i;
}
//-----------------------------------------------------------------------------
//вывод изображения.
void LcdImage(const unsigned char *imageData){
  
  uint16_t i;
  //--------------------
  Lcd12864Send(X0, LCD12864_CMD);//ставим указатель на 0,0
  Lcd12864Send(Y0, LCD12864_CMD);
  for (i = 0; i < LCD_CACHE_SIZE; i++) 
    {
      //LcdCache[i] = imageData[1023 - i];	//грузим данные
      LcdCache[i] = imageData[i];	//грузим данные
    }
}
//-----------------------------------------------------------------------------
//void LcdTime(uint8_t x, uint8_t y, RtcTimeSTR_Type *time){

//  LcdString(x, y);
//  LcdChr(time->hour);
//  LcdChr(':');
//  LcdChr(time->min);
//  LcdChr(':');
//  LcdChr(time->sec);
//}
//-----------------------------------------------------------------------------
uint8_t LcdPutUint(uint32_t data){
  
  uint8_t temp[10];
  uint8_t count = 0;
  volatile uint8_t countTemp;
  //--------------------
  if(data)
    {
      while(data)
        {
          temp[count++]=data%10+'0';
          data/=10;        
          countTemp = count;     
        } 
      while(count)   
        {
          LcdChr(temp[--count]);                
        }
    }
  else 
    {
      LcdChr('0');
      ++countTemp;
    }
  return countTemp;
}
//-----------------------------------------------------------------------------
uint8_t LcdBinToDec(uint16_t var, uint8_t num){
  
  uint8_t   DecArray[5];
  uint8_t   i;
  //--------------------
  if((var <= 0xFFFF) && (num < 5))
    {
      DecArray[4] = (uint8_t)(var / 10000);
      var  %= 10000;

      DecArray[3] = (uint8_t)(var / 1000);
      var  %= 1000;

      DecArray[2] = (uint8_t)(var / 100);
      var  %= 100;

      DecArray[1] = (uint8_t)(var / 10);
      DecArray[0] = (uint8_t)(var % 10);   
      //--------------------
      //Вывод на дисплей
      for(i=0; i<num; i++)
        {
          LcdChr(0x30 + DecArray[(num - 1) - i]);
        }
    }
  return num+1;
}
//-----------------------------------------------------------------------------
void LcdBinToDecBold(uint16_t var, uint8_t num){
  
  uint8_t   DecArray[5];
  uint8_t   i;
  //--------------------
  if((var <= 0xFFFF) && (num < 5))
    {
      DecArray[4] = (uint8_t)(var / 10000);
      var  %= 10000;

      DecArray[3] = (uint8_t)(var / 1000);
      var  %= 1000;

      DecArray[2] = (uint8_t)(var / 100);
      var  %= 100;

      DecArray[1] = (uint8_t)(var / 10);
      DecArray[0] = (uint8_t)(var % 10);   
      //--------------------
      //Вывод на дисплей
      for(i=0; i < num; i++)
        {
          LcdChrBold(0x30 + DecArray[(num - 1) - i]);
        }
    }
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
void Lcd12864GpioInit(void){
	
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
//Управление подсветкой дисплея.
void Lcd12864BackLightConrtol(uint8_t state){

  if(state) Lcd12864LedOn;
  else      Lcd12864LedOff;
}
//-----------------------------------------------------------------------------
void Lcd12864Init(void){
	
  Lcd12864GpioInit();               //Инициализация портов  
	Lcd12864ResHiht;                  //Перевод в рабочий режим.
  //Ожижание готовности дисплея.
  while(Lcd12864ReadStatusReg() & LCD12864StatReg_RESETbit){}
  //--------------------
	Lcd12864SelCS(CS12On);            //Включаем оба чипа
  Lcd12864FillingScreen(0x00);      //Очистка экрана.
  Lcd12864Send(X0, LCD12864_CMD);   //Ставим X=0
  Lcd12864Send(Y0, LCD12864_CMD);   //Ставим Y=0	
	Lcd12864Send(0xc0, LCD12864_CMD);	//Устанавливаем начало отображения.
  Lcd12864Send(0x3f, LCD12864_CMD); //Разрешаем отображение
  //Ожижание готовности дисплея.
	while(Lcd12864ReadStatusReg() & LCD12864StatReg_ON_OFFbit){}	
	//--------------------
  Lcd12864SelCS(CS12Off);           //Выключаем оба чипа. 
	Lcd12864LedOn;	                  //Включение подсветки экрана.
		
	//clean_lcd_buf();
}
//-----------------------------------------------------------------------------



