/*
 * internalflash.h
 *
 *  Created on: Apr 16, 2024
 *      Author: sanga
 */

#pragma once
#ifndef USER_DEFINED_FOLDER_FLASH_MEMORY_INTERNALFLASH_H_
#define USER_DEFINED_FOLDER_FLASH_MEMORY_INTERNALFLASH_H_
#include "stdint.h"
#include "stm32f4xx_hal.h"

#define FLASH_USER_START_ADDR   0x8000000   // Adjust this based on the start address of the flash sector you want to use
#define DATA_SIZE               32
#define START_ADD 0x80000000
#define FLASHWORD		8

#include "stm32f4xx_hal_flash.h"
class internal_flash {
public:
	 const uint32_t ADDRESS_LOC_SECTOR = 22;
	uint32_t flashAddress = FLASH_USER_START_ADDR;
	 uint8_t bytes_temp[4];
public:
	internal_flash();
	uint32_t GetSector(uint32_t Address);
	HAL_StatusTypeDef WRITE_TO_INTERNAL_FLASH(uint32_t sector,uint32_t write_address, uint32_t *DATA, uint8_t data_len) ;
	void READ_FRM_INTERNAL_FLASH(uint32_t address, uint8_t *read_buff, uint32_t len);
	void ERASE_DATA_INTERNAL_FLASH(uint32_t );
	void ERASE_INTERNAL_FLASH(uint32_t SECTOR_NUMBER);
	virtual ~internal_flash();
};

#endif /* USER_DEFINED_FOLDER_FLASH_MEMORY_INTERNALFLASH_H_ */
