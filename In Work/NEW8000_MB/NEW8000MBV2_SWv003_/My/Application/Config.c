
#include "Config.h"

//********************************************************************************************
static DataForFLASH_t DataForFLASHStr;//Конфигурация блока.
//********************************************************************************************
//********************************************************************************************
//Конфигурация блока.
Config_t* Config(void){

	return &DataForFLASHStr.config;
}
//*****************************************************************************
//сохранение конфигурации.
Config_t* Config_Save(void){

	DataForFLASHStr.sector.ChangeConfig += 1;//Признак изменения конфигурации. Это нужно для запись конфигурации после ее изменения.
	return &DataForFLASHStr.config;
}
//*****************************************************************************
void Config_Init(void){
	
	uint32_t pageAddress = FLASH_PAGE_127;
	//uint32_t error	     = 0;
	uint32_t index       = 0;
	//--------------------
	//чтение данных из FLASH памяти.
	while(pageAddress < FLASH_PAGE127_END_ADDR)
		{
			DataForFLASHStr.data32[index] = *(__IO uint32_t*)pageAddress;
			index       += 1;
			pageAddress += 4;
		}
	//--------------------
	//если проверочный ключ не совпадает, значит первый запус блока.
	if(DataForFLASHStr.config.CheckKey != CONFIG_CHECK_KEY_DEFINE)
		{
			//memset(DevNVRAM.data32, 0, sizeof(DevNVRAM.data32));//????????
			//Запись заводских установок во флеш.
			strcpy(DataForFLASHStr.config.SW, "SW01");		
			strcpy(DataForFLASHStr.config.HW, "HW01");		
			DataForFLASHStr.config.Address      = 1;
			DataForFLASHStr.config.Group        = 1;
			DataForFLASHStr.config.PowerCheckOn = POWER_ALL_CHECK_ON;
			DataForFLASHStr.config.SpDeviation  = SP_LINE_DEVIATION_DEFAULT;
			DataForFLASHStr.config.SpCheck      = SP_LINE_ALL_OFF;
			
			for(index = 0; index < FIRE_LINES_NUMBER; index++)
				{
					DataForFLASHStr.config.FireLineConfig[index].Type    = DEFAULT_FIRE_LINES_INPUT_TYPE;
					DataForFLASHStr.config.FireLineConfig[index].Timeout = DEFAULT_FIRE_LINES_INPUT_TIMEOUT;
				}
			//--------------------
			DataForFLASHStr.config.CheckKey = CONFIG_CHECK_KEY_DEFINE;
			DataForFLASHStr.sector.Nwrite   = 0;
			
			MotherBoard_WorkReg()->Address = 1;
			MotherBoard_WorkReg()->Group   = 1;
			//--------------------
			//пишем данные в память.	
			pageAddress = FLASH_PAGE_127;
			//error         = 0;
			index         = 0;
			DataForFLASHStr.sector.Nwrite += 1;
			
			Stm32Flash_Unlock();
			Stm32Flash_ErasePage(FLASH_PAGE_127);
			while(pageAddress < FLASH_PAGE127_END_ADDR)
				{
					Stm32Flash_WriteWord(DataForFLASHStr.data32[index], pageAddress);
					index       += 1;
					pageAddress += 4;			
				}	
			Stm32Flash_Lock();	
			//--------------------
		}	
}
//*****************************************************************************
//Запись конфигурации после ее изменения.
void Config_SaveLoop(void){
	
	uint32_t pageAddress = FLASH_PAGE_127;
	uint32_t index       = 0;
	//uint8_t  sizeConfig  = (uint8_t)sizeof(ConfigSTR_t);
	//--------------------
	//если были редактирования конфигурации, то запишем конфигурацию во FLASH.
	if(DataForFLASHStr.sector.ChangeConfig != 0)
		{
			DataForFLASHStr.sector.ChangeConfig = 0;
			DataForFLASHStr.sector.Nwrite      += 1;
			//--------------------
			//пишем данные в память.			
			//__disable_irq();
			Stm32Flash_Unlock();
			Stm32Flash_ErasePage(FLASH_PAGE_127);
			/*Нужно сделать запись конфигурации в соответствии с ее размером,
			а не перезаписвать всю страницу(1024 байта) памяти*/
			while(pageAddress < FLASH_PAGE127_END_ADDR)
				{
					Stm32Flash_WriteWord(DataForFLASHStr.data32[index], pageAddress);
					index       += 1;
					pageAddress += 4;			
				}	
			Stm32Flash_Lock();	
			//--------------------
			//__enable_irq();
		}
}
//********************************************************************************************
//********************************************************************************************
//********************************************************************************************
