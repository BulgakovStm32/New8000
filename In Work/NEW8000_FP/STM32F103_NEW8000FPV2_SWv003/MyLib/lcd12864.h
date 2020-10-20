
#ifndef _lcd12864_H
#define _lcd12864_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "Rtc.h"

//-----------------------------------------------------------------------------
//Сигналы DB0-DB7.
#define Lcd12864DataPort	  GPIOA
#define Lcd12864DataSet(x)	Lcd12864DataPort->ODR = (Lcd12864DataPort->ODR & 0xFF00) | (uint16_t)(x)
//Сигнал LED.
#define Lcd12864LedOn     	(GPIOB->BSRR = GPIO_BSRR_BS11) 
#define Lcd12864LedOff    	(GPIOB->BSRR = GPIO_BSRR_BR11) 
#define Lcd12864LedToggel 	(GPIOB->ODR  = GPIOB->ODR ^ GPIO_ODR_ODR11)
//Сигнал D/I.
#define Lcd12864SetData    	(GPIOB->BSRR = GPIO_BSRR_BS2)
#define Lcd12864SetCmd     	(GPIOB->BSRR = GPIO_BSRR_BR2) 
//Сигнал R/W.
#define Lcd12864Read     		(GPIOB->BSRR = GPIO_BSRR_BS1) 
#define Lcd12864Write    		(GPIOB->BSRR = GPIO_BSRR_BR1)
//Сигнал E.
#define Lcd12864EHight     	(GPIOB->BSRR = GPIO_BSRR_BS10) 
#define Lcd12864ELow    		(GPIOB->BSRR = GPIO_BSRR_BR10)
//Сигнал CS1.
#define Lcd12864CS1On     	(GPIOC->BSRR = GPIO_BSRR_BS4) 
#define Lcd12864CS1Off   		(GPIOC->BSRR = GPIO_BSRR_BR4)
//Сигнал CS2.
#define Lcd12864CS2On     	(GPIOC->BSRR = GPIO_BSRR_BS5) 
#define Lcd12864CS2Off   		(GPIOC->BSRR = GPIO_BSRR_BR5)

#define Lcd12864CS1()     Lcd12864CS2Off; Lcd12864CS1On
#define Lcd12864CS2()     Lcd12864CS1Off; Lcd12864CS2On

#define Lcd12864CS12()    Lcd12864CS1On; Lcd12864CS2On
#define Lcd12864CS12Off() Lcd12864CS1Off; Lcd12864CS2Off

//Сигнал /RES.
#define Lcd12864ResHiht  		(GPIOB->BSRR = GPIO_BSRR_BS0) 
#define Lcd12864ResLow   		(GPIOB->BSRR = GPIO_BSRR_BR0)
//---------------------
//Биты регистра STATUS.
#define LCD12864StatReg_BUSYbit   (1<<7)
#define LCD12864StatReg_ON_OFFbit (1<<5)
#define LCD12864StatReg_RESETbit  (1<<4)
//---------------------
#define LCD12864PauseConst 20 //

#define LCD12864_CMD    0
#define LCD12864_DATA   1

#define LcdBackLightOff 0
#define LcdBackLightOn  1
//-----------------------------------------------------------------------------
#define LCD_X_RES       128	 //разрешение экрана    
#define LCD_Y_RES       64
#define LCD_CACHE_SIZE  ((LCD_X_RES*LCD_Y_RES)/8) //Видео буфера.

#define	X0	            0xb8  // 
#define	Y0	            0x40  //

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
void    Lcd12864BackLightConrtol(uint8_t state);					
void    Lcd12864Init            (void);

uint8_t LcdUpdate     (void);
void    LcdUpdateAll  (void);

void    LcdFilling    (void);
void    LcdClear      (void);
void    LcdPixel      (uint8_t x, uint8_t y, uint8_t mode);
void    LcdLine       (int x1, int y1, int x2, int y2, uint8_t mode);
void    LcdCircle     (uint8_t x, uint8_t y, uint8_t radius, uint8_t mode);
void    LcdBar        (int x1, int y1, int x2, int y2, uint8_t persent);
void    LcdGotoXYFont (uint8_t x, uint8_t y);
void    LcdChr        (uint16_t ch);
void    LcdString     (uint8_t x, uint8_t y);
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

void 		LcdPrint(uint8_t posChar, uint8_t posStr, char *text);
void    LcdPrintHEX(uint32_t varHex);
//-----------------------------------------------------------------------------
#endif //_lcd12864_H


