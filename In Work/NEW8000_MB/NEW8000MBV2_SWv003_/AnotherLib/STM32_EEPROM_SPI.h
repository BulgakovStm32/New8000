
#ifndef _STM32_EEPROM_SPI_H
#define _STM32_EEPROM_SPI_H

//*****************************************************************************

#include "EepM95128.h"

//*****************************************************************************
/* M95040 SPI EEPROM defines */
#define EEPROM_WREN  0x06  /*!< Write Enable */
#define EEPROM_WRDI  0x04  /*!< Write Disable */
#define EEPROM_RDSR  0x05  /*!< Read Status Register */
#define EEPROM_WRSR  0x01  /*!< Write Status Register */
#define EEPROM_READ  0x03  /*!< Read from Memory Array */
#define EEPROM_WRITE 0x02  /*!< Write to Memory Array */

#define EEPROM_WIP_FLAG        0x01  /*!< Write In Progress (WIP) flag */

#define EEPROM_PAGE_SIZE      64    /*!< Pagesize according to documentation */
//#define EEPROM_BUFFER_SIZE    1024  /*!< EEPROM Buffer size. Setup to your needs */

#define EEPROM_CS_HIGH()    (GPIOB->BSRR = GPIO_BSRR_BS12) //HAL_GPIO_WritePin(EEPROM_CS_GPIO_Port, EEPROM_CS_Pin, GPIO_PIN_SET)
#define EEPROM_CS_LOW()     (GPIOB->BSRR = GPIO_BSRR_BR12) //HAL_GPIO_WritePin(EEPROM_CS_GPIO_Port, EEPROM_CS_Pin, GPIO_PIN_RESET)

//#define EEPROM_WAIT          ((uint32_t)0x5000)
//#define EEPROM_RxByte()			  Spi2_TxRxByte(0xFF)
//#define EEPROM_TxByte(byte)	  Spi2_TxRxByte(byte)
//#define EEPROM_TxRxByte(byte) Spi2_TxRxByte(byte)
/**
 * @brief EEPROM Operations statuses
 */
typedef enum {
    EEPROM_STATUS_PENDING,
    EEPROM_STATUS_COMPLETE,
    EEPROM_STATUS_ERROR
} EepOpStatEnum_t;
//*****************************************************************************
void 					  EEPROM_SPI_INIT(void);
EepOpStatEnum_t EEPROM_SPI_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
EepOpStatEnum_t EEPROM_WritePage			(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
EepOpStatEnum_t EEPROM_SPI_ReadBuffer	(uint8_t* pBuffer, uint16_t ReadAddr,  uint16_t NumByteToRead);
uint8_t EEPROM_SPI_WaitStandbyState(void);

/* Low layer functions */
uint8_t EEPROM_SendByte(uint8_t byte);
void    sEE_WriteEnable(void);
void    sEE_WriteDisable(void);
void    sEE_WriteStatusRegister(uint8_t regval);
uint8_t sEE_ReadStatusRegister(void);

void  EEPROM_SPI_SendInstruction(uint8_t *instruction, uint8_t size);
//void  EEPROM_SPI_ReadStatusByte(SPI_HandleTypeDef SPIe, uint8_t *statusByte );

//*****************************************************************************
#endif // _STM32_EEPROM_SPI_H
