
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

#define CS1     	      1 
#define CS2     	      2 
#define CS12On          3 
#define CS12Off         4 

#define LCD12864_CMD    0
#define LCD12864_DATA   1

#define LcdBackLightOff 0
#define LcdBackLightOn  1
//---------------------
#define LCD_X_RES       128	 //разрешение экрана    
#define LCD_Y_RES       64

#define LCD_CACHE_SIZE  ((LCD_X_RES*LCD_Y_RES)/8) //Видео буфера.

#define	X0	            0xb8  // 
#define	Y0	            0x40  //

//Режимы отображения пикселя - используются в графических функциях
#define PIXEL_OFF	      0		
#define PIXEL_ON	      1
#define PIXEL_XOR	      2
//-----------------------------------------------------------------------------
void    Lcd12864FillingScreen(uint8_t data);

uint8_t LcdUpdate     (void);
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
void    LcdImage      (const unsigned char *imageData);

uint8_t LcdPutUint      (uint32_t data);
uint8_t LcdBinToDec     (uint16_t var, uint8_t num);
void    LcdBinToDecBold (uint16_t var, uint8_t num);
 
void    Lcd12864GpioInit        (void);
void    Lcd12864BackLightConrtol(uint8_t state);					
void    Lcd12864Init            (void);
//-----------------------------------------------------------------------------
#endif //_lcd12864_H


