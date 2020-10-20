
#include "STM32_EEPROM_SPI.h"

//*****************************************************************************

uint8_t EEPROM_StatusByte;

//uint8_t RxBuffer[EEPROM_BUFFER_SIZE] = {0x00};
//uint8_t DebugBuf[1024];
//*****************************************************************************
void EepromErrorHandler(void){

	
}
//*****************************************************************************
/**
 * @brief Init EEPROM SPI
 *
 * @param hspi Pointer to SPI struct handler
 */
void EEPROM_SPI_INIT(void){

	EepM95128Init();
}
//*****************************************************************************
/**
  * @brief  Writes more than one byte to the EEPROM with a single WRITE cycle
  *         (Page WRITE sequence).
  *
  * @note   The number of byte can't exceed the EEPROM page size.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the EEPROM.
  * @param  WriteAddr: EEPROM's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the EEPROM, must be equal
  *         or less than "EEPROM_PAGESIZE" value.
  * @retval EepromOperations value: EEPROM_STATUS_COMPLETE or EEPROM_STATUS_ERROR
  */
EepOpStatEnum_t EEPROM_SPI_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite){
	
	uint8_t header[3];
	uint8_t count;
	//----------------
	//Отправим все команды одним пакетом.
	header[0] = EEPROM_WRITE;   //Send "Write to Memory" instruction
	header[1] = WriteAddr >> 8; //Send 16-bit address
	header[2] = WriteAddr;
	
	// Enables the write access to the EEPROM.
	sEE_WriteEnable();
	// Select the EEPROM: Chip Select low
	EEPROM_CS_LOW();

	EEPROM_SPI_SendInstruction((uint8_t*)header, 3);

	// Make 5 attemtps to write the data
//	for (uint8_t i = 0; i < 5; i++)
//		{
//			spiTransmitStatus = HAL_SPI_Transmit(EEPROM_SPI, pBuffer, NumByteToWrite, 100);

//			if (spiTransmitStatus == HAL_BUSY) osDelay(5);
//			else break;
//		}

	for(count = 0; count < NumByteToWrite; count++)
		{
			EEPROM_TxByte(*(pBuffer + count));
			//DebugBuf[WriteAddr + count] = *(pBuffer + count);
		} 
	
	// Deselect the EEPROM: Chip Select high
	EEPROM_CS_HIGH();

	// Wait the end of EEPROM writing
	EEPROM_SPI_WaitStandbyState();

	// Disable the write access to the EEPROM
	sEE_WriteDisable();

//	if (spiTransmitStatus == HAL_ERROR) return EEPROM_STATUS_ERROR;
																		  return EEPROM_STATUS_COMPLETE;
}
//*****************************************************************************
/**
  * @brief  Writes block of data to the EEPROM. In this function, the number of
  *         WRITE cycles are reduced, using Page WRITE sequence.
  *
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the EEPROM.
  * @param  WriteAddr: EEPROM's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the EEPROM.
  * @retval EepromOperations value: EEPROM_STATUS_COMPLETE or EEPROM_STATUS_ERROR
  */
EepOpStatEnum_t EEPROM_SPI_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite){
	
	uint16_t numOfPage   = 0; 
	uint16_t numOfSingle = 0; 
	uint16_t addr        = 0; 
	uint16_t count 			 = 0; 
	uint16_t temp 			 = 0;
	uint16_t sEE_DataNum = 0;

	EepOpStatEnum_t pageWriteStatus = EEPROM_STATUS_PENDING;
  //--------------------
	addr  = WriteAddr % EEPROM_PAGE_SIZE;// адресс с которого начинается запись.
	count = EEPROM_PAGE_SIZE - addr;     // количество байт для записи.
	
	numOfPage   = NumByteToWrite / EEPROM_PAGE_SIZE;//количество стриниц записи
	numOfSingle = NumByteToWrite % EEPROM_PAGE_SIZE;//количество бат которое нужно будет записать после записи страниц.
	//--------------------
	//Адрес записи выровнен по размеру страницы EEPROM.
	if(addr == 0)
		{
			/* NumByteToWrite < EEPROM_PAGE_SIZE */
 			if(numOfPage == 0)
				{
					sEE_DataNum = NumByteToWrite;
					pageWriteStatus = EEPROM_SPI_WritePage(pBuffer, WriteAddr, sEE_DataNum);
					
					if(pageWriteStatus != EEPROM_STATUS_COMPLETE) return pageWriteStatus;
				} 
			/* NumByteToWrite > EEPROM_PAGESIZE */
			else 
				{ 
					//Запишем нужное количество страниц.
					while(numOfPage--) 
						{
							sEE_DataNum = EEPROM_PAGE_SIZE;
							pageWriteStatus = EEPROM_SPI_WritePage(pBuffer, WriteAddr, sEE_DataNum);

							if(pageWriteStatus != EEPROM_STATUS_COMPLETE) return pageWriteStatus;

							WriteAddr += EEPROM_PAGE_SIZE;
							pBuffer   += EEPROM_PAGE_SIZE;
						}
					//Допишем оставшиеся байты.
					sEE_DataNum = numOfSingle;
					pageWriteStatus = EEPROM_SPI_WritePage(pBuffer, WriteAddr, sEE_DataNum);

					if(pageWriteStatus != EEPROM_STATUS_COMPLETE) return pageWriteStatus;
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
							pageWriteStatus = EEPROM_SPI_WritePage(pBuffer, WriteAddr, sEE_DataNum);
							
							if(pageWriteStatus != EEPROM_STATUS_COMPLETE) return pageWriteStatus;

							WriteAddr += count;
							pBuffer   += count;

							sEE_DataNum = temp;
							pageWriteStatus = EEPROM_SPI_WritePage(pBuffer, WriteAddr, sEE_DataNum);
						} 
					else
						{
							sEE_DataNum = NumByteToWrite;
							pageWriteStatus = EEPROM_SPI_WritePage(pBuffer, WriteAddr, sEE_DataNum);
						}

					if(pageWriteStatus != EEPROM_STATUS_COMPLETE)return pageWriteStatus;
				} 
			/* NumByteToWrite > EEPROM_PAGESIZE */
			else
				{ 
					NumByteToWrite -= count;
					numOfPage   = NumByteToWrite / EEPROM_PAGE_SIZE;
					numOfSingle = NumByteToWrite % EEPROM_PAGE_SIZE;
					sEE_DataNum = count;

					pageWriteStatus = EEPROM_SPI_WritePage(pBuffer, WriteAddr, sEE_DataNum);

					if(pageWriteStatus != EEPROM_STATUS_COMPLETE) return pageWriteStatus;

					WriteAddr += count;
					pBuffer   += count;

					while(numOfPage--)
						{
							sEE_DataNum = EEPROM_PAGE_SIZE;

							pageWriteStatus = EEPROM_SPI_WritePage(pBuffer, WriteAddr, sEE_DataNum);

							if (pageWriteStatus != EEPROM_STATUS_COMPLETE) return pageWriteStatus;

							WriteAddr += EEPROM_PAGE_SIZE;
							pBuffer   += EEPROM_PAGE_SIZE;
						}

					if(numOfSingle != 0)
						{
							sEE_DataNum = numOfSingle;

							pageWriteStatus = EEPROM_SPI_WritePage(pBuffer, WriteAddr, sEE_DataNum);

							if(pageWriteStatus != EEPROM_STATUS_COMPLETE) return pageWriteStatus;
						}
				}
		}
  //--------------------
	return EEPROM_STATUS_COMPLETE;
}
//*****************************************************************************
/**
  * @brief  Reads a block of data from the EEPROM.
  *
  * @param  pBuffer: pointer to the buffer that receives the data read from the EEPROM.
  * @param  ReadAddr: EEPROM's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the EEPROM.
  * @retval None
  */
EepOpStatEnum_t EEPROM_SPI_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead){
	
	uint8_t  header[3];
	uint16_t count;
	//--------------------
	//Отправим все команды одним пакетом.
	header[0] = EEPROM_READ;  // Send "Read from Memory" instruction
	header[1] = ReadAddr >> 8;// Send 16-bit address
	header[2] = ReadAddr;			//

	// Select the EEPROM: Chip Select low
	EEPROM_CS_LOW();

	/* Send WriteAddr address byte to read from */
	EEPROM_SPI_SendInstruction(header, 3);

	for(count = 0; count < NumByteToRead; count++)
		{
			*(pBuffer + count) = EEPROM_RxByte();
			//*(pBuffer + count) = DebugBuf[ReadAddr + count];
		}  
		
	// Deselect the EEPROM: Chip Select high
	EEPROM_CS_HIGH();

	return EEPROM_STATUS_COMPLETE;
}
//*****************************************************************************
/**
  * @brief  Sends a byte through the SPI interface and return the byte received
  *         from the SPI bus.
  *
  * @param  byte: byte to send.
  * @retval The value of the received byte.
  */
uint8_t EEPROM_SendByte(uint8_t byte){
	
//	uint8_t answerByte;
//	//--------------------
//	/* Loop while DR register in not empty */
//	while(EEPROM_SPI->State == HAL_SPI_STATE_RESET)
//		{
//			osDelay(1);
//		}

//	/* Send byte through the SPI peripheral */
//	if (HAL_SPI_Transmit(EEPROM_SPI, &byte, 1, 200) != HAL_OK)
//		{
//			Error_Handler();
//		}

//	/* Wait to receive a byte */
//	while(EEPROM_SPI->State == HAL_SPI_STATE_RESET)
//		{
//			osDelay(1);
//		}

//	/* Return the byte read from the SPI bus */
//	if (HAL_SPI_Receive(EEPROM_SPI, &answerByte, 1, 200) != HAL_OK)
//		{
//			Error_Handler();
//		}

	return (uint8_t)EEPROM_TxRxByte(byte);
}
//*****************************************************************************
/**
  * @brief  Enables the write access to the EEPROM.
  *
  * @param  None
  * @retval None
  */
void sEE_WriteEnable(void) {
	
	uint8_t command[1] = { EEPROM_WREN };
	//-------------------- 
	// Select the EEPROM: Chip Select low
	EEPROM_CS_LOW();

	/* Send "Write Enable" instruction */
	EEPROM_SPI_SendInstruction((uint8_t*)command, 1);

	// Deselect the EEPROM: Chip Select high
	EEPROM_CS_HIGH();
}
//*****************************************************************************
/**
  * @brief  Disables the write access to the EEPROM.
  *
  * @param  None
  * @retval None
  */
void sEE_WriteDisable(void) {

	uint8_t command[1] = { EEPROM_WRDI };
	//-------------------- 
	// Select the EEPROM: Chip Select low
	EEPROM_CS_LOW();

	/* Send "Write Disable" instruction */
	EEPROM_SPI_SendInstruction((uint8_t*)command, 1);

	// Deselect the EEPROM: Chip Select high
	EEPROM_CS_HIGH();
}
//*****************************************************************************
/**
  * @brief  Write new value in EEPROM Status Register.
  *
  * @param  regval : new value of register
  * @retval None
  */
void sEE_WriteStatusRegister(uint8_t regval){
	
	uint8_t command[2];
	//--------------------
	command[0] = EEPROM_WRSR;
	command[1] = regval;

	// Enable the write access to the EEPROM
	sEE_WriteEnable();

	// Select the EEPROM: Chip Select low
	EEPROM_CS_LOW();

	// Send "Write Status Register" instruction
	// and Regval in one packet
	EEPROM_SPI_SendInstruction((uint8_t*)command, 2);

	// Deselect the EEPROM: Chip Select high
	EEPROM_CS_HIGH();

	sEE_WriteDisable();
}
//*****************************************************************************
/**
  * @brief  Polls the status of the Write In Progress (WIP) flag in the EEPROM's
  *         status register and loop until write operation has completed.
  *
  * @param  None
  * @retval None
  */
uint8_t EEPROM_SPI_WaitStandbyState(void){
	
	uint8_t eepStatus  = 0;
	uint8_t command[1] = { EEPROM_RDSR };
	volatile uint32_t eepromWaitCount = 0;
	//--------------------
	// Select the EEPROM: Chip Select low
	EEPROM_CS_LOW();

	// Send "Read Status Register" instruction
	EEPROM_SPI_SendInstruction((uint8_t*)command, 1);

	// Loop as long as the memory is busy with a write cycle
	do{
			eepStatus = EEPROM_RxByte();
			if(++eepromWaitCount > EEPROM_WAIT) break;
		
		} while((eepStatus & EEPROM_WIP_FLAG) == SET); // Write in progress
	
	// Deselect the EEPROM: Chip Select high
	EEPROM_CS_HIGH();

	return 0;
}
//*****************************************************************************
/**
 * @brief Low level function to send header data to EEPROM
 *
 * @param instruction array of bytes to send
 * @param size        data size in bytes
 */
void EEPROM_SPI_SendInstruction(uint8_t *instruction, uint8_t size){
	
	uint8_t	count;
	//--------------------
	for(count = 0; count < size; count++)
    {
			EEPROM_TxByte(*(instruction + count));
    }  	 	
}
//*****************************************************************************













