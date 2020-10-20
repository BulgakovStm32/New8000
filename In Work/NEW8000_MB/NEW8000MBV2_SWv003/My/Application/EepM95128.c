
#include "EepM95128.h"

//**************************************************************************************

static EepInfo_t EepInfoStr;

//**************************************************************************************
//**************************************************************************************
//static void Pause(volatile uint32_t pause){

//  while(pause--);
//}
//*****************************************************************************
static void EepM95128_SendInstruction(uint8_t *instruction, uint8_t size){
	
	uint32_t	count;
	//--------------------
	for(count = 0; count < size; count++)
    {
			EepM95128_TxByte(*(instruction + count));
    }  	 	
}
//*****************************************************************************
//Команда Разрешение записи.
static void EepM95128_WriteEnable(void){
	
  EepM95128_ChipSel();	
  EepM95128_TxByte(EEP_CMD_WREN);	
  EepM95128_ChipUnsel();
}
//*****************************************************************************
//Команда запрещение записи.
static void EepM95128_WriteDisable(void){
	
  EepM95128_ChipSel();	
  EepM95128_TxByte(EEP_CMD_WRDI);	
  EepM95128_ChipUnsel();
}
//*****************************************************************************
//Запись регистра статуса микросхемы памяти.
//static void EepM95128_WriteStatReg(uint8_t regVal){

//	uint8_t command[2];
//	//--------------------
//	command[0] = EEP_CMD_WRSR;
//	command[1] = regVal;
//	
//	EepM95128_WriteEnable();//Разрешение записи.
//	
//	EepM95128_ChipSel();
//	
//	EepM95128_SendInstruction((uint8_t*)command, 2);
//	
//	EepM95128_ChipUnsel();
//	
//	EepM95128_WriteDisable();
//}
//*****************************************************************************
//Чтение регистра статуса микросхемы памяти.
//static uint8_t EepM95128_ReadStatReg(void){
//	
//	uint8_t eepStatus;
//	uint8_t command[1] = { EEP_CMD_RDSR };
//  //-------------------- 
//  EepM95128_ChipSel();
//  
//  EepM95128_SendInstruction((uint8_t*)command, 1);
//	eepStatus = EepM95128_RxByte();
//  
//	EepM95128_ChipUnsel();
//  //-------------------- 
//	return eepStatus;
//}
//*****************************************************************************
//static void EepM95128_ClearAllMem(void){

//}
//*****************************************************************************
static uint8_t EepM95128_WaitStandbyState(void){
	
	uint8_t eepStatus  = 0;
	uint8_t command[1] = { EEP_CMD_RDSR };
	volatile uint32_t waitCount = 0;
	//--------------------
	EepM95128_ChipSel();														// Select the EEPROM: Chip Select low
	EepM95128_SendInstruction((uint8_t*)command, 1);// Send "Read Status Register" instruction
	// Loop as long as the memory is busy with a write cycle
	do{
			eepStatus = EepM95128_RxByte();
			if(++waitCount > EEP_WAIT) break;
		}
	while((eepStatus & EEP_STAT_REG_BIT_WIP) == SET); // Write in progress
		
	EepM95128_ChipUnsel();//Deselect the EEPROM: Chip Select high
	return 0;
}
//*****************************************************************************
//static void EepM95128_WriteInfo(uint8_t *pEepInfo){
//	
//	uint32_t count;
//	uint8_t  command[3];
//	//----------------
//	//Отправим все команды одним пакетом.
//	command[0] = EEP_CMD_WRITE;   			          //Send "Write to Memory" instruction
//	command[1] = (uint8_t)EEP_INFO_START_ADR >> 8;//Send 16-bit address
//	command[2] = (uint8_t)EEP_INFO_START_ADR;
//	
//	EepM95128_WriteEnable();//Enables the write access to the EEPROM.
//	
//	EepM95128_ChipSel();		//Select the EEPROM: Chip Select low
//	
//	EepM95128_SendInstruction((uint8_t*)command, 3);

//	for(count = 0; count < EEP_INFO_SIZE; count++)
//		{
//			EepM95128_TxByte(*(pEepInfo + count));
//		}
//		
//	EepM95128_ChipUnsel(); 			 //Deselect the EEPROM: Chip Select high
//		
//	EepM95128_WaitStandbyState();//Wait the end of EEPROM writing
//		
//	EepM95128_WriteDisable();		 //Disable the write access to the EEPROM	
//}	
//*****************************************************************************
//static void EepM95128_ReadInfo(uint8_t *pEepInfo){
//	
//	uint32_t count;
//	uint8_t  command[3];
//	//--------------------
//	//Отправим все команды одним пакетом.
//	command[0] = EEP_CMD_READ;                    //Send "Read from Memory" instruction
//	command[1] = (uint8_t)EEP_INFO_START_ADR >> 8;//Send 16-bit address
//	command[2] = (uint8_t)EEP_INFO_START_ADR;		  //

//	EepM95128_ChipSel();
//	
//	EepM95128_SendInstruction((uint8_t*)command, 3);
//	
//	for(count = 0; count < EEP_INFO_SIZE; count++)
//		{
//			*(pEepInfo + count) = EepM95128_RxByte();
//		}  
//		
//	EepM95128_ChipUnsel();
//}
//*****************************************************************************
///**
//  * @brief  Writes more than one byte to the EEPROM with a single WRITE cycle
//  *         (Page WRITE sequence).
//  *
//  * @note   The number of byte can't exceed the EEPROM page size.
//  * @param  pBuffer: pointer to the buffer  containing the data to be written
//  *         to the EEPROM.
//  * @param  WriteAddr: EEPROM's internal address to write to.
//  * @param  NumByteToWrite: number of bytes to write to the EEPROM, must be equal
//  *         or less than "EEPROM_PAGESIZE" value.
//  * @retval EepromOperations value: EEPROM_STATUS_COMPLETE or EEPROM_STATUS_ERROR
//  */
EepStatusEnum_t EepM95128_WritePage(uint8_t* pBuffer, uint16_t writeAddr, uint16_t numByteToWrite){
	
	uint32_t count;
	uint8_t  command[3];
	//----------------
	//Отправим все команды одним пакетом.
	command[0] = EEP_CMD_WRITE; //Send "Write to Memory" instruction
	command[1] = writeAddr >> 8;//Send 16-bit address
	command[2] = writeAddr;
	
	EepM95128_WriteEnable();//Enables the write access to the EEPROM.
	
	EepM95128_ChipSel();		//Select the EEPROM: Chip Select low
	
	EepM95128_SendInstruction((uint8_t*)command, 3);

	for(count = 0; count < numByteToWrite; count++)
		{
			EepM95128_Info()->WriteCount++;
			EepM95128_TxByte(*(pBuffer + count));
		} 
	
	EepM95128_ChipUnsel(); 			 //Deselect the EEPROM: Chip Select high
		
	EepM95128_WaitStandbyState();//Wait the end of EEPROM writing

	EepM95128_WriteDisable();		 //Disable the write access to the EEPROM
		
//	if (spiTransmitStatus == HAL_ERROR) return EEPROM_STATUS_ERROR;
																		  return EEP_STATUS_COMPLETE;
}
//**************************************************************************************
//**************************************************************************************
//Управление EEPROM. PA8(EEP_HOLD), PB11(EEP_WP), PB12(EEP_CS).
//Включение тактирования порта А и В в GpioInit().
void EepM95128_Init(void){
  
	//Включаем тактирование порта B.
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	//Настройка пина PB12(EEP_CS). EEP_CS подтянут через 100кОм к +3,3В.
	//После сброса CNF[1:0]=01b 
	GPIOB->CRH &= ~GPIO_CRH_CNF12;
	GPIOB->CRH |=  GPIO_CRH_MODE12; //PB12(SPI2_NSS/EPP_CS) - выход, Push-pull, 50МГц.

	EepM95128_ChipUnsel();	 
  //--------------------
	Spi2_Init();
	//--------------------
	//EepM95128Info()->BadSectorsCount = 1;
	//EepM95128Info()->WriteCount      = 2;
	//EepM95128Info()->ReadCount       = 3;
	//EepM95128()->StatusReg       = EepM95128ReadStatReg();
}
//*****************************************************************************
EepInfo_t* EepM95128_Info(void){

	return &EepInfoStr;
}
//*****************************************************************************
EepStatusEnum_t EepM95128_ReadBuffer(uint8_t *pBuffer, uint16_t readAddr, uint16_t numByteToRead){

	uint32_t count;
	uint8_t  command[3];
	//--------------------
	//Отправим все команды одним пакетом.
	command[0] = EEP_CMD_READ; //Send "Read from Memory" instruction
	command[1] = readAddr >> 8;//Send 16-bit address
	command[2] = readAddr;		 //

	EepM95128_ChipSel();
	
	EepM95128_SendInstruction((uint8_t*)command, 3);
	
	for(count = 0; count < numByteToRead; count++)
		{
			*(pBuffer + count) = EepM95128_RxByte();
		}  
		
	EepM95128_ChipUnsel();
  //--------------------		
	return EEP_STATUS_COMPLETE;		
}
//*****************************************************************************
//Если количество байтов данных, отправленных на устройство, превышает границу страницы, 
//счетчик внутреннего адреса переключается на начало страницы и предыдущие данные там 
//перезаписываются входящими данными. Размер страницы этих устройств составляет 64 байта!!!
///**
//  * @brief  Writes block of data to the EEPROM. In this function, the number of
//  *         WRITE cycles are reduced, using Page WRITE sequence.
//  *
//  * @param  pBuffer: pointer to the buffer  containing the data to be written
//  *         to the EEPROM.
//  * @param  WriteAddr: EEPROM's internal address to write to.
//  * @param  NumByteToWrite: number of bytes to write to the EEPROM.
//  * @retval EepromOperations value: EEPROM_STATUS_COMPLETE or EEPROM_STATUS_ERROR
//  */
EepStatusEnum_t EepM95128_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite){
	
	uint16_t numOfPage   = 0; 
	uint16_t numOfSingle = 0; 
	uint16_t addr        = 0; 
	uint16_t count 			 = 0; 
	uint16_t temp 			 = 0;
	uint16_t sEE_DataNum = 0;

	EepStatusEnum_t pageWriteStatus = EEP_STATUS_PENDING;
  //--------------------
	addr  = WriteAddr % EEP_MEM_PAGE_SIZE;// адресс с которого начинается запись.
	count = EEP_MEM_PAGE_SIZE - addr;     // количество байт для записи.
	
	numOfPage   = NumByteToWrite / EEP_MEM_PAGE_SIZE;//количество стриниц записи
	numOfSingle = NumByteToWrite % EEP_MEM_PAGE_SIZE;//количество бат которое нужно будет записать после записи страниц.
	//--------------------
	//Адрес записи выровнен по размеру страницы EEPROM.
	if(addr == 0)
		{
			/* NumByteToWrite < EEPROM_PAGE_SIZE */
 			if(numOfPage == 0)
				{
					sEE_DataNum = NumByteToWrite;
					pageWriteStatus = EepM95128_WritePage(pBuffer, WriteAddr, sEE_DataNum);
					
					if(pageWriteStatus != EEP_STATUS_COMPLETE) return pageWriteStatus;
				} 
			/* NumByteToWrite > EEPROM_PAGESIZE */
			else 
				{ 
					//Запишем нужное количество страниц.
					while(numOfPage--) 
						{
							sEE_DataNum = EEP_MEM_PAGE_SIZE;
							pageWriteStatus = EepM95128_WritePage(pBuffer, WriteAddr, sEE_DataNum);

							if(pageWriteStatus != EEP_STATUS_COMPLETE) return pageWriteStatus;

							WriteAddr += EEP_MEM_PAGE_SIZE;
							pBuffer   += EEP_MEM_PAGE_SIZE;
						}
					//Допишем оставшиеся байты.
					sEE_DataNum = numOfSingle;
					pageWriteStatus = EepM95128_WritePage(pBuffer, WriteAddr, sEE_DataNum);

					if(pageWriteStatus != EEP_STATUS_COMPLETE) return pageWriteStatus;
				}
		} 
	//--------------------
	//Адрес записи НЕ выровнен по размеру страницы EEPROM.		
	/* WriteAddr is not EEPROM_PAGESIZE aligned  */
	else
		{ 
			/* NumByteToWrite < EEPROM_PAGESIZE */
			if(numOfPage == 0) 
				{ 
					/* (NumByteToWrite + WriteAddr) > EEPROM_PAGESIZE */
					if(numOfSingle > count)
						{ 
							temp = numOfSingle - count;
							sEE_DataNum = count;
							pageWriteStatus = EepM95128_WritePage(pBuffer, WriteAddr, sEE_DataNum);
							
							if(pageWriteStatus != EEP_STATUS_COMPLETE) return pageWriteStatus;

							WriteAddr += count;
							pBuffer   += count;

							sEE_DataNum = temp;
							pageWriteStatus = EepM95128_WritePage(pBuffer, WriteAddr, sEE_DataNum);
						} 
					else
						{
							sEE_DataNum = NumByteToWrite;
							pageWriteStatus = EepM95128_WritePage(pBuffer, WriteAddr, sEE_DataNum);
						}

					if(pageWriteStatus != EEP_STATUS_COMPLETE)return pageWriteStatus;
				} 
			/* NumByteToWrite > EEPROM_PAGESIZE */
			else
				{ 
					NumByteToWrite -= count;
					numOfPage   = NumByteToWrite / EEP_MEM_PAGE_SIZE;
					numOfSingle = NumByteToWrite % EEP_MEM_PAGE_SIZE;
					sEE_DataNum = count;

					pageWriteStatus = EepM95128_WritePage(pBuffer, WriteAddr, sEE_DataNum);

					if(pageWriteStatus != EEP_STATUS_COMPLETE) return pageWriteStatus;

					WriteAddr += count;
					pBuffer   += count;

					while(numOfPage--)
						{
							sEE_DataNum = EEP_MEM_PAGE_SIZE;

							pageWriteStatus = EepM95128_WritePage(pBuffer, WriteAddr, sEE_DataNum);

							if (pageWriteStatus != EEP_STATUS_COMPLETE) return pageWriteStatus;

							WriteAddr += EEP_MEM_PAGE_SIZE;
							pBuffer   += EEP_MEM_PAGE_SIZE;
						}

					if(numOfSingle != 0)
						{
							sEE_DataNum = numOfSingle;

							pageWriteStatus = EepM95128_WritePage(pBuffer, WriteAddr, sEE_DataNum);

							if(pageWriteStatus != EEP_STATUS_COMPLETE) return pageWriteStatus;
						}
				}
		}
  //--------------------
	return EEP_STATUS_COMPLETE;
}
//**************************************************************************************
//**************************************************************************************












