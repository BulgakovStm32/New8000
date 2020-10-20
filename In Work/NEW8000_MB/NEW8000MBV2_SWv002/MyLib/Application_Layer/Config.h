
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
//---------------------------------------
/***************Конфигурация************/
typedef struct {
	//----------
	uint32_t CheckKey;//проверочная последовательность
	//----------
	char	SW[4];//Версия ПО
	char  HW[4];//Версия железа
	//----------
	uint8_t Address;//адрес блока.
	uint8_t Group;	 //группа блока.
	//----------
	//Установленные на контроль линии питания блока.
	uint8_t PowerCheckOn;
	//----------
	//Параметры линий Гр.
	uint16_t SpDeviation;//Отклонение для фиксации неисправности на линии Гр.
	uint16_t SpCheckOn;  //Установленные на контроль линии Гр.
	//----------
	//Параметры входов ШС.
	uint8_t FireLine1Type;
	uint8_t FireLine1Timeout;
	uint8_t FireLine2Type;
	uint8_t FireLine2Timeout;
	uint8_t FireLine3Type;
	uint8_t FireLine3Timeout;
	uint8_t FireLineChSType;
	uint8_t FireLineChSTimeout;
	//----------
}ConfigSTR_t;
//---------------------------------------
struct FLASH_Sector{
	uint8_t  data[1024 - 12];//данные, записываемые в сектор памяти.
	uint32_t Nwrite;         //количество записей в данный секетор памяти.
	uint32_t ChangeConfig;   //используется для отслеживания изменений в конфигурации.
	uint32_t CheckSum;       // 
};
//---------------------------------------
typedef union NVRAM {
	ConfigSTR_t         config;
	struct FLASH_Sector sector;
	uint32_t            data32[CONFIG_DATA_WORD];
}DevNVRAMUnion_t;
//-----------------------------------------------------------------------------
ConfigSTR_t* Config    (void);
ConfigSTR_t* ConfigSave(void);
void         ConfigInit(void);
void         ConfigLoop(void);
//-----------------------------------------------------------------------------
#endif /*_Config_H*/














