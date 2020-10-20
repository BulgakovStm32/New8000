
#include "Lcd.h"

//**************************************************************************************************
static uint8_t*  pVideoBuf;			 //Указетель на видеопамять дисплея.
static uint8_t   TextBuf[32];		 //текстовый буфер для вывода на LCD
static uint16_t  LcdCacheIdx = 0;//Cache index

extern const uint8_t AsciiTabWG12864[];//Таблица символов.
#define TabAscii     AsciiTabWG12864   //Рабочая.
//**************************************************************************************************
//**************************************************************************************************
void LcdInit(void){
	
	Lcd12864_Init();
	pVideoBuf = Lcd12864_VideoBuf();
}
//-----------------------------------------------------------------------------
void LcdUpdate(void){

	Lcd12864_Update();
}
//-----------------------------------------------------------------------------
void LcdUpdateAll(void){

	Lcd12864_UpdateAll();
}
//-----------------------------------------------------------------------------
void LcdBackLight(uint8_t state){

	Lcd12864_BackLight(state);
}
//**************************************************************************************************
//**************************************************************************************************
//Clears the display
void LcdClear(void){
  
  uint16_t i;
  //--------------------
  for (i = 0; i < LCD_CACHE_SIZE; i++) pVideoBuf[i] = 0;	//забиваем всю память 0
}
//-----------------------------------------------------------------------------
void LcdFilling(void){
  
  uint16_t i;
  //--------------------
  for (i = 0; i < LCD_CACHE_SIZE; i++) pVideoBuf[i] = 0xFF;	//забиваем всю память 1
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
	index  = (((int)(y)/8)*128)+x;//считаем номер байта в массиве памяти дисплея
	offset = y-((y/8)*8);         //считаем номер бита в этом байте
  data   = pVideoBuf[index];    //берем байт по найденному индексу.
  //редактируем бит в этом байте
       if(mode == PIXEL_OFF) data &= (~(0x01 << offset));	
  else if(mode == PIXEL_ON ) data |= (  0x01 << offset);
  else if(mode == PIXEL_XOR) data ^= (  0x01 << offset);
  //--------------------
  pVideoBuf[index] = data;		//загружаем байт назад
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
void LcdSetCursor(uint8_t x, uint8_t y){
  
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
	for (i=0; i<32; i++) TextBuf[i] = 0;
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
      pVideoBuf[LcdCacheIdx++] = TabAscii[ch*5+i];	
    }
  pVideoBuf[LcdCacheIdx++] = 0x00;	//добавляем пробел между символами
}
//-----------------------------------------------------------------------------
//Печать строки.
//void LcdString(uint8_t x, uint8_t y){
//  
//	unsigned char i;
//	//--------------------
//	//if (x > 15 || y > 8) return;
//  if (x > 22 || y > 8) 
//    {
//      x = 22;
//      y = 8;
//    }
//  //--------------------
//	LcdSetCursor(x, y);
// 
//	for (i=0; i<(22-x); i++)
//    {
//      if(TextBuf[i]) 
//        {
//          LcdChr(TextBuf[i]);
//        }
//    }
//	clean_lcd_buf(); 
//}
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

      pVideoBuf[LcdCacheIdx]   = b;	  //копируем байты в экранный буфер
      pVideoBuf[LcdCacheIdx+1] = b;  //дублируем для получения жирного шрифта
      pVideoBuf[LcdCacheIdx+128] = a;
      pVideoBuf[LcdCacheIdx+129] = a;
      LcdCacheIdx = LcdCacheIdx+2;
    }

  pVideoBuf[LcdCacheIdx++] = 0x00;	//для пробела между символами
  pVideoBuf[LcdCacheIdx++] = 0x00;
}
//-----------------------------------------------------------------------------
//Печатает символ на текущем месте, большой и жирный.
void LcdStringBold (unsigned char x, unsigned char y){
  
	unsigned char i;
	//-------------------- 
	LcdSetCursor(x, y);
  
	for ( i = 0; i < 12-x; i++ ) 
		{
			if (TextBuf[i]) LcdChrBold(TextBuf[i]); 
		}
		
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
      pVideoBuf[LcdCacheIdx] = b;
      pVideoBuf[LcdCacheIdx+128] = a;
      LcdCacheIdx = LcdCacheIdx+1;
    }
  pVideoBuf[LcdCacheIdx++] = 0x00;
}
//-----------------------------------------------------------------------------
//Displays a string at current cursor location
void LcdStringBig (unsigned char x, unsigned char y){
  
	unsigned char i;
	//-------------------- 
	if (x > 22 && y > 5) return;
  
	LcdSetCursor(x, y);
  
	for (i=0; i<(22-x); i++) if (TextBuf[i]) LcdChrBig(TextBuf[i]); 
  
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
void LcdImage(const uint8_t* imageData){
  
  uint16_t i;
  //--------------------
  Lcd12864_Send(LCD12864_X0, LCD12864_CMD);//ставим указатель на 0,0
  Lcd12864_Send(LCD12864_Y0, LCD12864_CMD);
  for (i = 0; i < LCD_CACHE_SIZE; i++) 
    {
      pVideoBuf[i] = imageData[i];	//грузим данные
    }
}
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
					
//					if(DecArray[(num - 1) - i] == 0)
//						{
//							LcdChr(' ');
//						}
//					else
//						{
//							LcdChr(0x30 + DecArray[(num - 1) - i]);
//						}
        }
    }
  return num+1;
}
//-----------------------------------------------------------------------------
uint8_t LcdBinToDecWithDot(uint16_t var, uint8_t positionDot, char *txt){
  
  uint8_t   DecArray[5];
  uint8_t   i;
  //--------------------
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
	for(i=0; i < 4; i++)//4 - это количество цифр, на которое разбивается var.
		{
			LcdChr(0x30 + DecArray[(4 - 1) - i]);
			if(positionDot > 0)
				{
					if(i == (positionDot - 1))LcdChr(',');
				}
		}
	LcdOutStr(txt);		
	//--------------------
		return 7;//занимаемое количество знокомест.
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
void LcdPrint(uint8_t posChar, uint8_t posStr, char *text){

	LcdSetCursor(posChar, posStr+1);
	LcdOutStr(text);
}
//-----------------------------------------------------------------------------
void LcdPrintOneHEX(char hexChar){

	if(hexChar <= 9) LcdChr(hexChar + '0');
	if((hexChar >= 10) && (hexChar <= 15)) LcdChr(hexChar + 'a' - 10);
}
//-----------------------------------------------------------------------------
void LcdPrintHEX(uint32_t varHex){

	uint8_t char_1;
	//--------------------
	LcdOutStr("0x");
	
	char_1 = (uint8_t)((varHex & 0xF0000000) >> 28);
	LcdPrintOneHEX(char_1);
	
	char_1 = (uint8_t)((varHex & 0x0F000000) >> 24);
	LcdPrintOneHEX(char_1);
	
	char_1 = (uint8_t)((varHex & 0x00F00000) >> 20);
	LcdPrintOneHEX(char_1);
	
	char_1 = (uint8_t)((varHex & 0x000F0000) >> 16);
	LcdPrintOneHEX(char_1);
	
	char_1 = (uint8_t)((varHex & 0x0000F000) >> 12);
	LcdPrintOneHEX(char_1);
	
	char_1 = (uint8_t)((varHex & 0x00000F00) >> 8);
	LcdPrintOneHEX(char_1);
	
	char_1 = (uint8_t)((varHex & 0x000000F0) >> 4);
	LcdPrintOneHEX(char_1);
	
	char_1 = (uint8_t)varHex & 0x0000000F;
	LcdPrintOneHEX(char_1);
}	
//**************************************************************************************************
//**************************************************************************************************





