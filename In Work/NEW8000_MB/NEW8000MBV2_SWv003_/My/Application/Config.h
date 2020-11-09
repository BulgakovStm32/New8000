
#ifndef _Config_H
#define _Config_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"
#include <string.h>

#include "sys_ST.h"
//--------------------
//AppLayer.
#include "LineCheck.h"
//--------------------
//Boards.
#include "MB_MotherBoard.h"
//-----------------------------------------------------------------------------
#define CONFIG_CHECK_KEY_DEFINE		0x12345678 
#define CONFIG_DATA_WORD         (FLASH_PAGE_SIZE / 4) //количество 32-х слов.
//***************************************
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct {
	//----------
	//Параметры входов ШС.
	uint16_t Type;
	uint16_t Timeout;
	//----------
}FireLineConfig_t;
#pragma pack(pop)//вернули предыдущую настройку.
//***************************************
/***************Конфигурация************/
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct {
	//----------
	uint32_t CheckKey;//проверочная последовательность
	//----------
	char SW[4];//Версия ПО
	char HW[4];//Версия железа
	//----------
	uint8_t Address;//адрес блока.
	uint8_t Group;	 //группа блока.
	//----------
	//Установленные на контроль линии питания блока.
	uint8_t PowerCheck;
	//----------
	//Параметры линий Гр.
	uint16_t SpDeviation;//Отклонение для фиксации неисправности на линии Гр.
	uint8_t  SpCheck;    //Установленные на контроль линии Гр.
	//----------
	//Параметры входов ШС.
	FireLineConfig_t FireLineConfig[FIRE_LINES_NUMBER];
	//----------
}Config_t;
#pragma pack(pop)//вернули предыдущую настройку.
//---------------------------------------
struct FLASH_Sector{
	uint8_t  data[1024 - 12];//данные, записываемые в сектор памяти.
	uint32_t Nwrite;         //количество записей в данный секетор памяти.
	uint32_t ChangeConfig;   //используется для отслеживания изменений в конфигурации.
	uint32_t CheckSum;       // 
};
//---------------------------------------
typedef union NVRAM {
	Config_t            config;
	struct FLASH_Sector sector;
	uint32_t            data32[CONFIG_DATA_WORD];
}DataForFLASH_t;
//-----------------------------------------------------------------------------
Config_t* Config     (void);
Config_t* Config_Save(void);
void      Config_Init(void);
void      Config_SaveLoop(void);
//-----------------------------------------------------------------------------
#endif /*_Config_H*/














