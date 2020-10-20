
#ifndef _PowerAndCharge_H
#define _PowerAndCharge_H
//-----------------------------------------------------------------------------

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

#define PowerGPOI     GpioAIDR 
#define DcOkPIN       GPIO_IDR_IDR3  
#define DcFaultPIN    GPIO_IDR_IDR4 
//--------------------
#define BAT_MEAS_NUM    128 //Количество измерений для усреднения
#define BAT_MEAS_SHIFT  7   //Количество сдвигов для деления
//--------------------
#define POWER_ALL_CHECK_ON		0xFF//Все линии питания под контролем.
#define TIMEOUT_AC_CHECK_mS   2000 
//--------------------
//Состояния основного питания 220V и инвертора.
#define POWER_MASK         		0x0F
#define POWER_BLINK     		 (0<<0) //Для мигания.
#define POWER_CONTROL_OFF		 (1<<0) //контроль отключен.
#define POWER_AC_OK      		 (2<<0) //Есть питание 220V
#define POWER_DC_OK      	   (3<<0) //Инвертор исправен.
#define POWER_AC_NOT_CONNECT (4<<0) //Питание 220V отсутствует
#define POWER_DC_FAULT       (5<<0) //Инвертор неисправен.
#define POWER_FAULT          (6<<0) //Неисправны оба блока питания.
//Состояние батареи.
#define BAT_MASK         0xF0
#define BAT_BLINK        (0<<4) //Для мигания.
#define BAT_CONTROL_OFF  (1<<4) //контроль отключен.
#define BAT_OK           (2<<4) //Напряжение на АКБ от 21В до 27,2В.
#define BAT_CHARGE       (3<<4) //Идет заряд АКБ
#define BAT_CHARGE_END   (4<<4) //Заряд окончен, напряжение АКБ 27,2В.
#define BAT_ATTENTION    (5<<4) //Напряжение на АКБ <= 20,5В.
#define BAT_DEEP         (6<<4) //Напряжение на АКБ <= 18,5В - глубокий разряд АКБ.
#define BAT_NOT_CONNECT  (7<<4) //АКБ не подключена.
//-----------------------------------------------------------------------------
//Состояния основного питания 220V, инвертора И УМЗЧ.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
	uint8_t  ACState;
	uint8_t  DCState;
	uint8_t  BatState;
	uint16_t BatMeas;
	//----------
	union
	{
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
void        PowerAndCharge_Init(void);
void        PowerAndCharge_Loop(void);
PowerSTR_t* Power(void);

//Зарядное устройуство.
void Charger_Activate(void);
void Charger_Deactivate(void);

//-----------------------------------------------------------------------------
#endif 





