/*
 * MEMORYADDRESSING.h
 *
 *  Created on: Apr 16, 2024
 *      Author: sanga
 */
#pragma once
#define MFM_SPI_HANDLETYPEDEF hspi3
#define SFM_SPI_HANLETYPEDEF hspi4
#ifndef USER_DEFINED_FOLDER_MEMORY_ADDRESSING_MEMORYADDRESSING_H_
#define USER_DEFINED_FOLDER_MEMORY_ADDRESSING_MEMORYADDRESSING_H_
#include "../VARIABLES/variables.h"
#include "../FLASH_MEMORY/internalflash.h"
#include "../FLASH_MEMORY/mt25ql.h"




class MEMORY_ADDRESSING:public internal_flash, public mt25ql {
public:
	const uint8_t CS_HIGH = 1, CS_LOW = 0;
//	uint32_t ERASE_INTERNAL_FLASH, EXTERNAL_FLASH_BLOCK_ADDRESS;
	uint32_t EXTERNAL_FLASH_BLOCK_ADDRESS;
	enum WHO_SAVES_DATA{
		OBC = 0,
		RSV,
		FLAG,
		SAT_LOG,
		COM,
		MSN1,
		MSN2,
		MSN3,
		MSN4
	} who_saves_data;
	uint8_t MUX_CS;
	GPIO_TypeDef *GPIO_PORT;
	uint16_t GPIO_PIN;
	uint32_t teemp;
	uint8_t FM_CS;
	uint8_t SFM_CS;

public:
	MEMORY_ADDRESSING();

	void TOGGLE_CS_PIN(GPIO_TypeDef *CS_GPIO_PORT, uint8_t CS_GPIO_PIN, uint8_t CS_STATE);
	void MUX_ENABLE( GPIO_TypeDef *CS_GPIO_PORT, uint8_t CS_GPIO_PIN);
	void MUX_DISABLE( GPIO_TypeDef *CS_GPIO_PORT, uint8_t CS_GPIO_PIN);
	void SPI_TRANSMIT(SPI_HandleTypeDef *SPI,uint8_t COMMAND ,GPIO_TypeDef *CS_GPIO_PORT, uint8_t CS_GPIO_PIN);
	void SPI_RECEIVE(SPI_HandleTypeDef *SPI, uint8_t COMMAND , uint8_t *DATA, GPIO_TypeDef *CS_GPIO_PORT, uint8_t CS_GPIO_PIN);
	void SPI_TRANSMITRECEIVE(SPI_HandleTypeDef *SPI, uint8_t COMMAND ,uint8_t *DATA, GPIO_TypeDef *CS_GPIO_PORT, uint8_t CS_GPIO_PIN);
	void get_current_address(SPI_HandleTypeDef *SPI,uint8_t choice);
	uint8_t CHECK_DATA_CONSISTENCY_FM(uint8_t block_address, uint8_t choice);
	void CHECK_EACH_ADDRESS_CONSISTENCY();
	void TRANSFER_DATA_BETWEEN_FM(uint8_t choice, uint32_t address, uint32_t difference);


	void SAVE_SAT_LOG();
	void SAVE_FLAG_STATUS();
	void SAVE_COM_DATA();
	void SAVE_MSN1_DATA();
	void SAVE_MSN2_DATA();
	void SAVE_MSN3_DATA();
	void SAVE_MSN4_DATA();

	//void SAVE_DATA(SPI_HandleTypeDef *SPI, uint32_t address, uint8_t sector_size, int16_t *data);
	void SAVE_HK_DATA();
	//void READ_FM_DATA(SPI_HandleTypeDef *SPI, uint32_t address, uint8_t data,uint16_t size);
	void SAVE_DATA(SPI_HandleTypeDef *SPI, uint32_t *address, uint8_t sector_size, int16_t *data);
	void READ_FM_DATA(SPI_HandleTypeDef *SPI, uint32_t address, uint8_t *data,uint16_t size);
//	void Get_Current_Address(); //Get current address data from ext FM
//	void Write_Address_Data();  //Write current address data to FM
//	void Get_Mission_Data_Address();
//	void Check_Flag_Data();
//	void Print_Flag_Data();

	virtual ~MEMORY_ADDRESSING();
};

#endif /* USER_DEFINED_FOLDER_MEMORY_ADDRESSING_MEMORYADDRESSING_H_ */
