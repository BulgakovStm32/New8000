
#ifndef _EepM95128_H
#define _EepM95128_H

//*********************************************************************************

#include "spi_ST.h"

//*********************************************************************************
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
	//--------
	uint32_t BadSectorsCount;//Количество битых секторов. 
	uint32_t WriteCount;		 //Количество записей
	uint32_t ReadCount;			 //Количество чтений.	
//	uint8_t  StatusReg; 		 //Регистр состояния.
	//--------
}EepInfo_t;
#pragma pack(pop)//вернули предыдущую настройку.
//*********************************************************************************
//*******************************EEPROM M95128W************************************
//Управление EEPROM. 
//EEP_CS - PB12.
#define EepM95128_ChipUnsel() 		(GPIOB->BSRR = GPIO_BSRR_BS12) 
#define EepM95128_ChipSel()   		(GPIOB->BSRR = GPIO_BSRR_BR12)

#define EepM95128_RxByte()				Spi2_TxRxByte(0xFF)
#define EepM95128_TxByte(byte)		Spi2_TxRxByte(byte)
#define EepM95128_TxRxByte(byte)	Spi2_TxRxByte(byte)
//-----------------------------------------------------------------------------
//Биты Read Status Register(RDSR).
#define EEP_STAT_REG_BIT_SRWD (1<<7) //Status Register Write Protect
#define EEP_STAT_REG_BIT_BP1  (1<<3) //Block Protect bits
#define EEP_STAT_REG_BIT_BP0  (1<<2)
#define EEP_STAT_REG_BIT_WEL  (1<<1) //Write Enable Latch bit
#define EEP_STAT_REG_BIT_WIP  (1<<0) //Write In Progress bit
//Команды микросхемы памяти M95128-W.
#define EEP_CMD_WREN   0x06 //Write Enable
#define EEP_CMD_WRDI   0x04 //Write Disable
#define EEP_CMD_RDSR   0x05 //Read Status Register 
#define EEP_CMD_WRSR   0x01 //Write Status Register
#define EEP_CMD_READ   0x03 //Read from Memory Array
#define EEP_CMD_WRITE  0x02 //Write to Memory Array 

#define EEP_MEM_START_ADR 0x0000
#define EEP_MEM_END_ADR   0x3FFF
#define EEP_MEM_SIZE      (EEP_MEM_END_ADR - EEP_MEM_START_ADR + 1)
#define EEP_MEM_PAGE_SIZE  64 

#define EEP_WAIT           ((uint32_t)0x5000)

#define EEP_INFO_SIZE			 (uint8_t)sizeof(EepInfo_t)
#define EEP_INFO_START_ADR (EEP_MEM_END_ADR - EEP_INFO_SIZE)   
/**
 * @brief EEPROM Operations statuses
 */
typedef enum {
    EEP_STATUS_PENDING,
    EEP_STATUS_COMPLETE,
    EEP_STATUS_ERROR
}EepStatusEnum_t;
//-----------------------------------------------------------------------------
void     	      EepM95128_Init(void);
EepInfo_t*      EepM95128_Info(void);
EepStatusEnum_t EepM95128_ReadBuffer (uint8_t *pBuffer, uint16_t readAddr, uint16_t numByteToRead);
EepStatusEnum_t EepM95128_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
//-----------------------------------------------------------------------------
#endif 














