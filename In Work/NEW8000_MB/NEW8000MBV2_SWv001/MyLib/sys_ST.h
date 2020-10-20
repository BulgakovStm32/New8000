
#ifndef _sys_ST_H
#define _sys_ST_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "gpio_ST.h"

//-----------------------------------------------------------------------------
//Как же определить свободную страницу? Это зависит от размера прошивки. 
//Обычно используют последнюю страницу в памяти. 
//Её адрес можно вычислить как [начальный адрес] + [номер страницы - 1] * [размер страницы].
//Выберем 16 страницу: 0x0800 0000 + 15 * 0x0000 0400 = 0x0800 3C00

#define FlashPage1 0x08003C00  
#define FlashPage2 0x08003C00

//-----------------------------------------------------------------------------
uint32_t InitRCC(void);
void     SysInit(void);

void     Stm32FlashLock    (void);
void     Stm32FlashUnlock  (void);
void     Stm32FlashEraseAll(void);//неотлажено

void     Stm32FlashErasePage(uint32_t PageAddress);
void     Stm32FlashWritePage(unsigned char* data, uint32_t Address, uint16_t size);
uint32_t Stm32FlashRead     (uint32_t Address);
void     Stm32FlashWriteBuf (unsigned char* buf, uint16_t size);
//-----------------------------------------------------------------------------
#endif /*_sys_ST_H*/



