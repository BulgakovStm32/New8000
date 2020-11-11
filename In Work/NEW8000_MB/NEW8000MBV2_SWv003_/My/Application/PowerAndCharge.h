
#ifndef _PowerAndCharge_H
#define _PowerAndCharge_H
//-----------------------------------------------------------------------------

#include "gpio_ST.h"
#include "adc_ST.h"
//--------------------
//Программные таймеры.
#include "SwTimers.h"
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
//Состояние основного питания 220V
#define POWER_AC_CHECK_OFF	0 //контроль отключен.
#define POWER_AC_OK		  		1 //Есть питание 220V
#define POWER_AC_FAULT  		2 //Питание 220V отсутствует
//Состояние инвертора.
#define POWER_DC_CHECK_OFF	0 //контроль отключен.
#define POWER_DC_OK      		1 //Инвертор исправен.
#define POWER_DC_FAULT   		2 //Инвертор неисправен.
//Состояние батареи.
#define BAT_CHECK_OFF   0 //контроль отключен.
#define BAT_CONNECT     1 //Напряжение на АКБ от 21В до 27,2В.
#define BAT_CHARGE      2 //Идет заряд АКБ, Напряжение на АКБ от 21В до 27,2В.
#define BAT_CHARGE_END  3 //Заряд окончен, напряжение АКБ 27,2В.
#define BAT_ATTENTION   4 //Напряжение на АКБ <= 20,5В.
#define BAT_DEEP        5 //Напряжение на АКБ <= 18,5В - глубокий разряд АКБ.
#define BAT_NOT_CONNECT 6 //АКБ не подключена, напряжение на АКБ < 15В.
//-----------------------------------------------------------------------------
#pragma pack(push, 1)//размер выравнивания в 1 байт

typedef union{
	struct{	
			uint8_t AC  :2;//Состояние основного питания 220V
			uint8_t DC  :2;//Состояние инвертора.
			uint8_t Bat :4;//Состояние батареи.
	}bits;
	uint8_t Byte;
}PowerState_t;	
//**************************************************
//Состояния основного питания 220V, инвертора И УМЗЧ.
typedef struct{	
	uint16_t BatMeas;//Напряжение на АКБ.
	//----------
	PowerState_t State;
	PowerState_t CheckConfig;
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
void Charger_FSMLoop(void);
//-----------------------------------------------------------------------------
#endif 





