/*
 * mt25ql.cpp
 *
 *  Created on: Apr 9, 2024
 *      Author: sanga
 */

#include "mt25ql.h"

mt25ql::mt25ql() {
	// TODO Auto-generated constructor stub

}

mt25ql::~mt25ql() {
	// TODO Auto-generated destructor stub
}


/*
 * @brief	to read the FM identification data
 *
 * @param	SPI			pointer to the handle of SPI connected to Flash from which to read data
 * @param	*rxData		typedef pointer to the data to be read
 *
 * @retval	20 bytes of specific data can be read
 */
void mt25ql::Read_ID(SPI_HandleTypeDef *SPI){
	uint8_t cmd = READ_ID;
	FM_Enable(SPI);
	HAL_Delay(1);
	HAL_SPI_Transmit(SPI, &cmd, 1, 300);
	HAL_SPI_Receive(SPI, data, 20, 1000);
	HAL_Delay(1);
	FM_Disable(SPI);
	HAL_Delay(500);
//	buff.MAN_ID = data[0];
//	buff.M_TYPE = data[1];
//	buff.M_CAP = data[2];
//	buff.REM_BYTES = data[3];
//	buff.EXT_ID = data[4];
//	buff.DEV_INFO = data[5];
//	for (int i = 6; i < 20; i++) {
//		buff.UID[i] = data[i];
//	}
//	memcpy(&buff, data, 20);
//	*rxData = buff;
	return;
}

/*
 * @brief  use 4-byte addressing mode for flash operation
 *
 * @param	uint8_t flag to specify weather to enter 4-byte mode or exit 4-byte mode
 * 			0: exit 4-byte addresssing mode [to 3-BYTE mode]
 * 			1: enter 4-byte addressing mode
 * @retval	none
 *
 */
void mt25ql::ADDRESSING_MODE(SPI_HandleTypeDef *SPI, uint8_t mode){

}


void mt25ql::CS_LOW(GPIO_TypeDef *GPIO_PORT, uint16_t GPIO_PIN){
	HAL_GPIO_WritePin(GPIO_PORT, GPIO_PIN, GPIO_PIN_RESET);
}

void mt25ql::CS_HIGH(GPIO_TypeDef *GPIO_PORT, uint16_t GPIO_PIN){
	HAL_GPIO_WritePin(GPIO_PORT, GPIO_PIN, GPIO_PIN_SET);
}

void mt25ql::SPI_Operation(SPI_HandleTypeDef *SPI, uint8_t cmd, uint32_t address, GPIO_TypeDef *GPIO_PORT, uint16_t GPIO_PIN, SPI_OPERATIONS spi_operations, uint8_t *data_buffer, uint8_t addressing_mode){
    uint8_t command[addressing_mode];

    CS_LOW(GPIO_PORT, GPIO_PIN);
    HAL_Delay(2);
    for(int i =0 ;i < addressing_mode; i++ ){
        command[i] = (uint8_t) (address >> 8 * (addressing_mode - i - 1) & 0xff );
    }
    switch(spi_operations){
        case SPI_READ:
            command[0] = cmd;
            HAL_SPI_Transmit(SPI, command, addressing_mode, 100);
            HAL_SPI_Receive(SPI, data_buffer, sizeof(*data_buffer), 200);
            break;
        case SPI_WRITE:
        	Write_Enable(SPI);
            HAL_SPI_Transmit(SPI, &cmd, 1, 100);
            HAL_SPI_Transmit(SPI, command, addressing_mode, 200);
            HAL_SPI_Transmit(SPI, (uint8_t*)data_buffer, sizeof(*data_buffer), 200);
            HAL_Delay(4);
            break;
        case SPI_ERASE:
        	Write_Enable(SPI);
            HAL_SPI_Transmit(SPI, &cmd, 1, 100);
            HAL_SPI_Transmit(SPI, command, addressing_mode, 200);
            break;
    }
    CS_HIGH(GPIO_PORT, GPIO_PIN);
    HAL_Delay(2);
}

void mt25ql::Bulk_Read(SPI_HandleTypeDef *SPI, uint32_t address, uint8_t *data, uint16_t size){
    uint8_t cmd = READ;
    GPIO_TypeDef *PORT;
    uint16_t PIN;
    PORT = MAIN_FM_CS_PORT;
    PIN = MAIN_FM_CS_PIN;
    if(SPI == &SHARED_FM_SPI){
    	PORT = SHARED_FM_CS_PORT;
		PIN = SHARED_FM_CS_PIN;
    }
    SPI_Operation(SPI, cmd, address, PORT, PIN, SPI_READ, data, size);
}

/*
 * @brief	to write to a page(256 bytes) of the FM 3-byte-addressing mode
 *
 * @param	address		4 byte starting address from which data is to be read
 * 			*data		data to be written into the address
 * 			size		size of the data
 *@retval	none
 */
void mt25ql::Page_Write(SPI_HandleTypeDef *SPI, uint32_t address, uint8_t *data, uint8_t addressing_mode, uint16_t size) {
	uint8_t cmd;
	if(addressing_mode == 5) cmd = PAGE_PROGRAM;
	else cmd = BYTE_4_PAGE_PROGRAM;
	GPIO_TypeDef *PORT;
	uint16_t PIN;
	PORT = MAIN_FM_CS_PORT;
	PIN = MAIN_FM_CS_PIN;
	if(SPI == &SHARED_FM_SPI){
		PORT = SHARED_FM_CS_PORT;
		PIN = SHARED_FM_CS_PIN;
	}
	while (size > PAGE_SIZE){
		SPI_Operation(SPI, cmd, address, PORT, PIN, SPI_WRITE, data, addressing_mode);
		size = size - PAGE_SIZE;
		data = data + 256;
		address = address + 256;
	}
	SPI_Operation(SPI, cmd, address, PORT, PIN, SPI_WRITE, data, addressing_mode);
}

/*
 * @brief	similar function as Bulk_Read but 4 byte address mode for the FM
 *
 *
 * @param	SPI			pointer to the handle of SPI connected to Flash from which to read data
 * * @param	address		address from which the data is to be read
 * 			*data		pointer to store the data which is read from the corresponding address
 * 			size		size of the data to be read
 */
void mt25ql::Bulk_Read_4B(SPI_HandleTypeDef *SPI, uint32_t address, uint8_t *data,
		uint16_t size) {
	uint8_t cmd[5];
	cmd[0] = BYTE_4_READ;
	cmd[1] = (uint8_t) (address >> 24 & 0xff);
	cmd[2] = (uint8_t) (address >> 16 & 0xff);
	cmd[3] = (uint8_t) (address >> 8);
	cmd[4] = (uint8_t) (address);
	FM_Enable(SPI);
	HAL_SPI_Transmit(SPI, cmd, 5, 100);
	HAL_SPI_Receive(SPI, data, size, 200);
	FM_Disable(SPI);
	HAL_Delay(5);
	return;
}

uint8_t mt25ql::Read_Byte_4B(SPI_HandleTypeDef *SPI, uint32_t address){
	uint8_t data_buffer = 0;
	SPI_Operation(&MAIN_FM_SPI, BYTE_4_READ, address, MAIN_FM_CS_PORT, MAIN_FM_CS_PIN, SPI_READ, &data_buffer, 5 );
	return data_buffer;
}

void mt25ql::FM_Enable(SPI_HandleTypeDef *SPI) {
	if (SPI == &MAIN_FM_SPI) {
		HAL_GPIO_WritePin(MAIN_FM_CS_PORT, MAIN_FM_CS_PIN, GPIO_PIN_RESET);
		HAL_Delay(1);
	} else if (SPI == &SHARED_FM_SPI) {
		HAL_GPIO_WritePin(SHARED_FM_CS_PORT, SHARED_FM_CS_PIN, GPIO_PIN_RESET);
		HAL_Delay(1);
	}
}

void mt25ql::FM_Disable(SPI_HandleTypeDef *SPI) {
	if (SPI == &MAIN_FM_SPI) {
		HAL_GPIO_WritePin(MAIN_FM_CS_PORT, MAIN_FM_CS_PIN, GPIO_PIN_SET);
		HAL_Delay(1);
	} else if (SPI == &SHARED_FM_SPI) {
		HAL_GPIO_WritePin(SHARED_FM_CS_PORT, SHARED_FM_CS_PIN, GPIO_PIN_SET);
		HAL_Delay(1);
	}

}
void mt25ql::Write_Enable(SPI_HandleTypeDef *SPI) {
	uint8_t cmd = WRITE_ENABLE;
	FM_Enable(SPI);
	HAL_Delay(1);
	HAL_SPI_Transmit(SPI, &cmd, 1, 500);
	FM_Disable(SPI);
	HAL_Delay(10);
}


