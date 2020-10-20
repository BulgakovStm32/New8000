
#ifndef _PowerAndCharge_H
#define _PowerAndCharge_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

#include "gpio_ST.h"
#include "adc_ST.h"

//-----------------------------------------------------------------------------
//Управление и диагностика БП и ЗУ. 
//ZU_On    - PA2 - выход.
//DC_Ok    - PA3 - вход.
//DC_Fault - PA4 - вход.
//AC_Ok    - PA6 - вход.
#define ZuOnGPIO      GPIOA
#define ZuOn         (ZuOnGPIO->BSRR = GPIO_BSRR_BR2)
#define ZuOff        (ZuOnGPIO->BSRR = GPIO_BSRR_BS2)

#define AcGPOI        GPIOA 
#define AcPIN         GPIO_IDR_IDR6 
#define ACmSecConst   500

#define PowerGPOI     GpioAIDR 
#define DcOkPIN       GPIO_IDR_IDR3  
#define DcFaultPIN    GPIO_IDR_IDR4 
//--------------------
//Состояния основного питания 220V и инвертора.
#define PowerMask       0x0F
#define PowerBlink     (0<<0)//Для мигания.
#define PowerACOk      (1<<0)//Есть питание 220V
#define PowerDCOk      (2<<0)//Инвертор исправен.
#define PowerACNo      (3<<0)//Питание 220V отсутствует
#define PowerDCFault   (4<<0)//Инвертор неисправен.
#define PowerFault     (5<<0)//Неисправны оба блока питания.
//Состояние батареи.
#define BatMask         0xF0
#define BatBlink       (0<<4)//Для мигания.
#define BatOk          (1<<4)//Напряжение на АКБ от 21В до 27,2В.
#define BatCharge      (2<<4)//Идет заряд АКБ
#define BatChargeEnd   (3<<4)//Заряд окончен, напряжение АКБ 27,2В.
#define BatAttention   (4<<4)//Напряжение на АКБ <= 20,5В.
#define BatDeep        (5<<4)//Напряжение на АКБ <= 18,5В - глубокий разряд АКБ.
#define BatNo          (6<<4)//АКБ отсутствует.
//-----------------------------------------------------------------------------
void    PowerAndChargeInit(void);
void    PowerGetACStatus  (void);
uint8_t PowerGetStatus    (void);
uint8_t PowerBatGetStatus (void);
//-----------------------------------------------------------------------------
#endif 





