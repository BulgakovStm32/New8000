
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
#define ZuOn()        (ZuOnGPIO->BSRR = GPIO_BSRR_BR2)
#define ZuOff()       (ZuOnGPIO->BSRR = GPIO_BSRR_BS2)

#define AcGPOI        GPIOA 
#define AcPIN         GPIO_IDR_IDR6 
#define ACmSecConst   500

#define PowerGPOI     GpioAIDR 
#define DcOkPIN       GPIO_IDR_IDR3  
#define DcFaultPIN    GPIO_IDR_IDR4 
//--------------------
#define BatAdcMeasQuantity   128 //Количество измерений для усреднения
#define BatAdcMeasShiftDev   7   //Количество сдвигов для деления
//--------------------
#define PowerCheckOnDefault  0xFF//Все линии питания под контролем.
//--------------------
//Состояния основного питания 220V и инвертора.
#define PowerMask        0x0F
#define PowerBlink     	(0<<0) //Для мигания.
#define PowerControlOff	(1<<0) //контроль отключен.
#define PowerACOk      	(2<<0) //Есть питание 220V
#define PowerDCOk      	(3<<0) //Инвертор исправен.
#define PowerACNo       (4<<0) //Питание 220V отсутствует
#define PowerDCFault    (5<<0) //Инвертор неисправен.
#define PowerFault      (6<<0) //Неисправны оба блока питания.
//Состояние батареи.
#define BatMask          0xF0
#define BatBlink        (0<<4) //Для мигания.
#define BatControlOff   (1<<4) //контроль отключен.
#define BatOk           (2<<4) //Напряжение на АКБ от 21В до 27,2В.
#define BatCharge       (3<<4) //Идет заряд АКБ
#define BatChargeEnd    (4<<4) //Заряд окончен, напряжение АКБ 27,2В.
#define BatAttention    (5<<4) //Напряжение на АКБ <= 20,5В.
#define BatDeep         (6<<4) //Напряжение на АКБ <= 18,5В - глубокий разряд АКБ.
#define BatNo           (7<<4) //АКБ отсутствует.
//-----------------------------------------------------------------------------
//Состояния основного питания 220V, инвертора И УМЗЧ.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
	uint8_t  PowerState;
	uint8_t  BatState;
	uint16_t BatMeas;
	//----------
	union {
		struct 
		{	
			uint8_t MainPower :1;
			uint8_t Bat       :1;
			uint8_t :6;
		}bit;
		uint8_t Byte;
	}Check;
	//----------
}PowerSTR_t;
#pragma pack(pop)//вернули предыдущую настройку.
//-----------------------------------------------------------------------------
void        PowerAndChargeInit(void);
void        PowerAndChargeLoop(void);
PowerSTR_t* Power(void);
//-----------------------------------------------------------------------------
#endif 





