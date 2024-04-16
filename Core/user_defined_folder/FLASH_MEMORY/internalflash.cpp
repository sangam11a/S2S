/*
 * internalflash.cpp
 *
 *  Created on: Apr 16, 2024
 *      Author: sanga
 */

#include "internalflash.h"

internal_flash::internal_flash() {
	// TODO Auto-generated constructor stub


}

internal_flash::~internal_flash() {
	// TODO Auto-generated destructor stub
}

uint32_t internal_flash::GetSector(uint32_t Address)
{
  uint32_t sector = 0;

  /* BANK 1 */
  if((Address >= 0x08000000) && (Address < 0x08020000))
  {
    sector = FLASH_SECTOR_0;
  }

  else if((Address >= 0x08040000) && (Address < 0x08080000))
  {
    sector = FLASH_SECTOR_1;
  }


  else if((Address >= 0x08080000) && (Address < 0x080C0000))
  {
    sector = FLASH_SECTOR_2;
  }
  else if((Address >= 0x080C0000) && (Address < 0x080F0000))
  {
    sector = FLASH_SECTOR_3;
  }

  else if((Address >= 0x08010000) && (Address < 0x08020000))
  {
    sector = FLASH_SECTOR_4;
  }


  /* BANK 2 */
  if((Address >= 0x08020000) && (Address < 0x08040000))
  {
    sector = FLASH_SECTOR_5;
  }

  else if((Address >= 0x08040000) && (Address < 0x08060000))
  {
    sector = FLASH_SECTOR_6;
  }

  else if((Address >= 0x08060000) && (Address < 0x08080000))
  {
    sector = FLASH_SECTOR_7;
  }

  else if((Address >= 0x08080000) && (Address < 0x080A0000))
  {
    sector = FLASH_SECTOR_8;
  }

  else if((Address >= 0x080A0000) && (Address < 0x080C0000))
  {
    sector = FLASH_SECTOR_9;
  }

  else if((Address >= 0x080C0000) && (Address < 0x080E0000))
  {
    sector = FLASH_SECTOR_10;
  }

  else if((Address >= 0x080E0000) && (Address < 0x08100000))
  {
    sector = FLASH_SECTOR_11;
  }



  return sector;
}
/*
 *
 */

HAL_StatusTypeDef internal_flash::WRITE_TO_INTERNAL_FLASH(uint32_t sector,uint32_t write_address, uint32_t *DATA, uint8_t data_len) {
HAL_StatusTypeDef OPER_STAT;
uint8_t buff[data_len];
for (int i = 0; i < data_len; i++) {
	buff[i] = *DATA++;
}

HAL_FLASH_Unlock();

for (int i = 0; i < data_len; i++) {
	OPER_STAT = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, write_address,(uint8_t) buff[i]);
	HAL_Delay(10);
	if (OPER_STAT != HAL_OK) {
		HAL_FLASH_Lock();
		HAL_Delay(10);
		return OPER_STAT;
	}
	write_address++;
}
HAL_FLASH_Lock();
return OPER_STAT;
}

/*
* @brief Reads data from given address of the internal flash
*
* @param	Address		Address from where data is to be read
* 			Read_Buffer	pointer to the location where address is to be written
* 			len			no of bytes to be read
* @retval	none
*
*/
void internal_flash::READ_FRM_INTERNAL_FLASH(uint32_t address, uint8_t *read_buff, uint32_t len) {

	for (int i = 0; i < len; i++) {
		*(read_buff + i) = *(uint8_t*) address;
		address++;
	}

}

/*
 *
 */

void internal_flash::ERASE_INTERNAL_FLASH(uint32_t SECTOR_NUMBER){
HAL_FLASH_Unlock();
FLASH_Erase_Sector(SECTOR_NUMBER, FLASH_VOLTAGE_RANGE_3);// FLASH_VOLTAGE_RANGE_3 = 0x0000002U i.e. erase flash by 32 bits ffrom the current sector number or block.  default value in stm32xxhal.h
HAL_FLASH_Lock();
}
