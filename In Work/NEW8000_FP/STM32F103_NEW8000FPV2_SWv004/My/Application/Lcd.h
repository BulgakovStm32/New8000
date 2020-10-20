
#ifndef _Lcd_H
#define _Lcd_H
//-----------------------------------------------------------------------------

#include "lcd12864.h"

#include "Rtc.h"
//-----------------------------------------------------------------------------
#define LCD_CACHE_SIZE	 LCD12864_CACHE_SIZE
#define LCD_X_RES				 LCD12864_X_RES
#define LCD_Y_RES				 LCD12864_Y_RES

//Режимы отображения пикселя - используются в графических функциях
#define PIXEL_OFF	      0		
#define PIXEL_ON	      1
#define PIXEL_XOR	      2
//Номера строк. Первая строка в верху дисплея.
#define String1   0 
#define String2   1 
#define String3   2 
#define String4   3 
#define String5   4 
#define String6   5 
#define String7   6 
#define String8   7
//Номера символов в строке. Первый символ слева.
#define Char1     1 
#define Char2     2
#define Char3     3
#define Char4     4
#define Char5     5
#define Char6     6
#define Char7     7
#define Char8     8
#define Char9     9
#define Char10    10
#define Char11    11
#define Char12    12
#define Char13    13
#define Char14    14
#define Char15    15
//-----------------------------------------------------------------------------
void LcdInit     (void);
void LcdBackLight(uint8_t state);
void LcdUpdate   (void);
void LcdUpdateAll(void);


void    LcdFilling    (void);
void    LcdClear      (void);
void    LcdPixel      (uint8_t x, uint8_t y, uint8_t mode);
void    LcdLine       (int x1, int y1, int x2, int y2, uint8_t mode);
void    LcdCircle     (uint8_t x, uint8_t y, uint8_t radius, uint8_t mode);
void    LcdBar        (int x1, int y1, int x2, int y2, uint8_t persent);
void    LcdSetCursor  (uint8_t x, uint8_t y);
void    LcdChr        (uint16_t ch);
//void    LcdString     (uint8_t x, uint8_t y);
void    LcdChrBold    (int ch);
void    LcdStringBold (uint8_t x, uint8_t y);
void    LcdChrBig     (int ch);
void    LcdStringBig  (uint8_t x, uint8_t y);

uint8_t LcdOutStr     (char *txt);
void    LcdImage      (const uint8_t *imageData);

uint8_t LcdPutUint        (uint32_t data);
uint8_t LcdBinToDec       (uint16_t var, uint8_t num);
uint8_t LcdBinToDecWithDot(uint16_t var, uint8_t positionDot, char *txt);
void    LcdBinToDecBold   (uint16_t var, uint8_t num);

void 		LcdPrint   (uint8_t posChar, uint8_t posStr, char *text);
void    LcdPrintHEX(uint32_t varHex);
//-----------------------------------------------------------------------------
#endif /*_Lcd_H*/
