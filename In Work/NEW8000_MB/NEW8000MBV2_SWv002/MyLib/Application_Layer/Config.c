
#include "Config.h"

//-----------------------------------------------------------------------------
static DevNVRAMUnion_t DevNVRAM;//Конфигурация блока.
//-----------------------------------------------------------------------------
//Конфигурация блока.
ConfigSTR_t* Config(void){

	return &DevNVRAM.config;
}
//-----------------------------------------------------------------------------
//сохранение конфигурации.
ConfigSTR_t* ConfigSave(void){

	DevNVRAM.sector.ChangeConfig += 1;//Это нужно для запись конфигурации после ее изменения.
	return &DevNVRAM.config;
}
//-----------------------------------------------------------------------------
void ConfigInit(void){
	
	uint32_t pageAddress = FLASH_PAGE_127;
	//uint32_t error	     = 0;
	uint32_t index       = 0;
	//--------------------
	//чтение данных из FLASH памяти.
	while(pageAddress < FLASH_PAGE127_END_ADDR)
		{
			DevNVRAM.data32[index] = *(__IO uint32_t*)pageAddress;
			index       += 1;
			pageAddress += 4;
		}
	//--------------------
	//если проверочный ключ не совпадает, значит первый запус блока.
	if(DevNVRAM.config.CheckKey != CONFIG_CHECK_KEY_DEFINE)
		{
			//memset(DevNVRAM.data32, 0, sizeof(DevNVRAM.data32));//????????
			//Запись заводских установок во флеш.
			strcpy(DevNVRAM.config.SW, "SW01");		
			strcpy(DevNVRAM.config.HW, "HW01");		
			DevNVRAM.config.Address      = 1;
			DevNVRAM.config.Group        = 1;
			DevNVRAM.config.PowerCheckOn = PowerCheckOnDefault;
			DevNVRAM.config.SpDeviation  = LcDeviationDefaultConst;
			DevNVRAM.config.SpCheckOn    = SpAllLinesOff;
			
			DevNVRAM.config.FireLine1Type      = 1;
			DevNVRAM.config.FireLine1Timeout   = 10;
			DevNVRAM.config.FireLine2Type      = 1;
			DevNVRAM.config.FireLine2Timeout   = 10;
			DevNVRAM.config.FireLine3Type      = 1;
			DevNVRAM.config.FireLine3Timeout   = 10;
			DevNVRAM.config.FireLineChSType    = 1;
			DevNVRAM.config.FireLineChSTimeout = 10;
			//--------------------
			DevNVRAM.config.CheckKey = CONFIG_CHECK_KEY_DEFINE;
			DevNVRAM.sector.Nwrite   = 0;
			
			SetAddress(1);
			SetGroup(1);
			//--------------------
			//пишем данные в память.	
			pageAddress = FLASH_PAGE_127;
			//error         = 0;
			index         = 0;
			DevNVRAM.sector.Nwrite += 1;
			
			Stm32FlashUnlock();
			Stm32FlashErasePage(FLASH_PAGE_127);
			while(pageAddress < FLASH_PAGE127_END_ADDR)
				{
					Stm32FlashWriteWord(DevNVRAM.data32[index], pageAddress);
					index       += 1;
					pageAddress += 4;			
				}	
			Stm32FlashLock();	
			//--------------------
		}	
}
//-----------------------------------------------------------------------------
//Запись конфигурации после ее изменения.
void ConfigLoop(void){
	
	uint32_t pageAddress = FLASH_PAGE_127;
	uint32_t index       = 0;
	//uint8_t  sizeConfig  = (uint8_t)sizeof(ConfigSTR_t);
	//--------------------
	//если были редактирования конфигурации, то запишем конфигурацию во FLASH.
	if(DevNVRAM.sector.ChangeConfig != 0)
		{
			DevNVRAM.sector.ChangeConfig = 0;
			DevNVRAM.sector.Nwrite      += 1;
			//--------------------
			//пишем данные в память.			
			//__disable_irq();
			Stm32FlashUnlock();
			Stm32FlashErasePage(FLASH_PAGE_127);
			/*Нужно сделать запись конфигурации в соответствии с ее размером,
			а не перезаписвать всю страницу(1024 байта) памяти*/
			while(pageAddress < FLASH_PAGE127_END_ADDR)
				{
					Stm32FlashWriteWord(DevNVRAM.data32[index], pageAddress);
					index       += 1;
					pageAddress += 4;			
				}	
			Stm32FlashLock();	
			//--------------------
			//__enable_irq();
		}
}
//-----------------------------------------------------------------------------
