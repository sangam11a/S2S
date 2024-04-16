/*
 * IMU.cpp
 *
 *  Created on: Apr 8, 2024
 *      Author: sanga
 */

#include "IMU.h"

IMU::IMU() {
	// TODO Auto-generated constructor stub
	aScaleFactor = 1;
	aFullScaleRange = 1;
}

IMU::~IMU() {
	// TODO Auto-generated destructor stub
}

/**
 * @brief  Read data from Specific Register address of LIS3MDL
 * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
 *               the configuration information for SPI module.
 * @param  add Register address from which data is to be read
 */
void IMU::ReadReg(SPI_HandleTypeDef *hspi, uint8_t add, uint8_t *val, GPIO_TypeDef *GPIO_PORT, uint8_t GPIO_PIN) {
//	uint8_t val;
	add |= 0x80;  // set the MSB to indicate a read operation
	HAL_GPIO_WritePin(GPIO_PORT, GPIO_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi, &add, 1, 100);
	HAL_SPI_Receive(hspi, val, 1, 100);
	HAL_GPIO_WritePin(GPIO_PORT, GPIO_PIN, GPIO_PIN_SET);
//	return val;
}

/**
 * @brief  Write on Specific Register address of LIS3MDL
 * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
 *               the configuration information for SPI module.
 * @param  add Register address where certain value is to be written
 */
void IMU::WriteReg(SPI_HandleTypeDef *hspi, uint8_t add, uint8_t val, GPIO_TypeDef *GPIO_PORT, uint8_t GPIO_PIN) {
	add &= 0x7F;  // clear the MSB to indicate a write operation
	HAL_GPIO_WritePin(GPIO_PORT, GPIO_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi, &add, 1, 100);
	HAL_SPI_Transmit(hspi, &val, 1, 100);
	HAL_GPIO_WritePin(GPIO_PORT, GPIO_PIN, GPIO_PIN_SET);

}


uint8_t IMU::SPIx_WriteRead(uint8_t Byte) {
	uint8_t receivedbyte = 0;
	if (HAL_SPI_TransmitReceive(&IMU_SPI, (uint8_t*) &Byte,
			(uint8_t*) &receivedbyte, 1, 0x1000) != HAL_OK) {
		return -1;
	} else {
	}
	return receivedbyte;
}

void IMU::TransmitReceive(SPI_HandleTypeDef *hspi, uint8_t *add, uint8_t val, GPIO_TypeDef *GPIO_PORT, uint8_t GPIO_PIN, uint8_t NumByteToWrite){
	SPIx_WriteRead(*add);
		while (NumByteToWrite >= 0x01) {
			SPIx_WriteRead(*add);
			NumByteToWrite--;
			add++;
		}
}

/**
 * @brief  Initialize LIS3MDL to work in 16-bit, 1.25Hz ODR, ±4 Gauss and Continuous conversion Mode
 * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
 *               the configuration information for SPI module.
 * @param  ctrl2 Control Register Value to choose LIS3MDL sensor Scale
 */
void IMU::LIS3MDL_Init(SPI_HandleTypeDef *hspi, uint8_t ctrl2) {
	uint8_t ctrl1 = 0x74; // set the magnetic resolution to 16-bit, 20 Hz ODR, UHP mode in X-Y axis
	WriteReg(hspi, LIS3MDL_CTRL_REG1, ctrl1, MAG_CS_PORT, MAG_CS_PIN );

	/* Change the full-scale range to ±4 Gauss */

	//value to set the full-scale range
	WriteReg(hspi, LIS3MDL_CTRL_REG2, ctrl2, MAG_CS_PORT, MAG_CS_PIN);

	/* Change the control register 3 to continuous conversion mode */

	uint8_t ctrl3 = 0x00; // value to set the continuous conversion mode
	WriteReg(hspi, LIS3MDL_CTRL_REG3, ctrl3, MAG_CS_PORT, MAG_CS_PIN);

	uint8_t ctrl4 = 0x0C; // value to set the UHP mode on Z-axis
	WriteReg(hspi, LIS3MDL_CTRL_REG4, ctrl4, MAG_CS_PORT, MAG_CS_PIN);
}


uint8_t IMU::MPU_begin(SPI_HandleTypeDef *SPIx) {
	// Initialize variables
	uint8_t addr, val,check;
//	 Confirm device
	ReadReg(&IMU_SPI, WHO_AM_I, &check, IMU_CS_PORT, IMU_CS_PIN);
	if( check == WHO_AM_I_6500_ANS){

//	 Startup / reset the sensor
		addr = PWR_MGMT_1;
		val = 0x00;
		WriteReg(SPIx, addr, val, IMU_CS_PORT, IMU_CS_PIN);
//		writeRegister(addr, val);

		// Disable I2C (SPI only)
		addr = USER_CTRL;
		val = 0x10;
//		writeRegister(addr, val);

		WriteReg(SPIx, addr, val, IMU_CS_PORT, IMU_CS_PIN);

		// Configure DLPF value()
		val = 0x11;
		MPU6500_SetDLPFBandwidth(DLPF_BANDWIDTH_20HZ);

		// Set the full scale ranges
		MPU_writeAccFullScaleRange(aFullScaleRange);
		MPU_writeGyroFullScaleRange(gFullScaleRange);
		return 1;
	} else {
		return 0;
	}
}

void IMU::MPU_SPI_Write(uint8_t *pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite) {
//	MPU_CS(CS_SEL);
	SPIx_WriteRead(WriteAddr);
	while (NumByteToWrite >= 0x01) {
		SPIx_WriteRead(*pBuffer);
		NumByteToWrite--;
		pBuffer++;
	}
//	MPU_CS(CS_DES);
}


/* sets the DLPF bandwidth to values other than default */
void IMU::MPU6500_SetDLPFBandwidth(DLPFBandwidth bandwidth) {
//	writeRegister(ACCEL_CONFIG2, bandwidth);
//	TransmitReceive(&IMU_SPI, ACCEL_CONFIG2, bandwidth, IMU_CS_PORT, IMU_CS_PIN, 1);
//	TransmitReceive(&IMU_SPI, &CONFIG, bandwidth, IMU_CS_PORT, IMU_CS_PIN, 1);
	WriteReg(&IMU_SPI, ACCEL_CONFIG2, bandwidth, IMU_CS_PORT, IMU_CS_PIN);
	WriteReg(&IMU_SPI, CONFIG, bandwidth, IMU_CS_PORT, IMU_CS_PIN);
//	writeRegister(CONFIG, bandwidth);
}
/// @brief Set the accelerometer full scale range
/// @param SPIx Pointer to SPI structure config
/// @param pMPU6500 Pointer to master MPU6500 struct
/// @param aScale Set 0 for ±2g, 1 for ±4g, 2 for ±8g, and 3 for ±16g
void IMU::MPU_writeAccFullScaleRange(uint8_t aScale) {
    // Variable init
    uint8_t addr = ACCEL_CONFIG;
    uint8_t val;

    // Set the value
    switch (aScale) {
    case AFSR_2G:
        aScaleFactor = 16384.0;
        val = 0x00;
//        WriteReg(&IMU_SPI, addr, val, &IMU_CS_PORT, IMU_CS_PIN);
        WriteReg(&IMU_SPI, addr, val, IMU_CS_PORT, IMU_CS_PIN);
        break;
    case AFSR_4G:
        aScaleFactor = 8192.0;
        val = 0x08;
        WriteReg(&IMU_SPI, addr, val, IMU_CS_PORT, IMU_CS_PIN);

        break;
    case AFSR_8G:
        aScaleFactor = 4096.0;
        val = 0x10;
        WriteReg(&IMU_SPI, addr, val, IMU_CS_PORT, IMU_CS_PIN);

        break;
    case AFSR_16G:
        aScaleFactor = 2048.0;
        val = 0x18;
        WriteReg(&IMU_SPI, addr, val, IMU_CS_PORT, IMU_CS_PIN);
        break;
    default:
        aScaleFactor = 8192.0;
        val = 0x08;
        WriteReg(&IMU_SPI, addr, val, IMU_CS_PORT, IMU_CS_PIN);

        break;
    }
}

/// @brief Set the gyroscope full scale range
/// @param SPIx Pointer to SPI structure config
/// @param pMPU6500 Pointer to master MPU6500 struct
/// @param gScale Set 0 for ±250°/s, 1 for ±500°/s, 2 for ±1000°/s, and 3 for ±2000°/s
void IMU::MPU_writeGyroFullScaleRange(uint8_t gScale) {
	// Variable init
	uint8_t addr = GYRO_CONFIG;
	uint8_t val;

	// Set the value
	switch (gScale) {
	case GFSR_250DPS:
		 gScaleFactor = 131.0;
		val = 0x00;
		 WriteReg(&IMU_SPI, addr, val, IMU_CS_PORT, IMU_CS_PIN);
		break;
	case GFSR_500DPS:
		 gScaleFactor = 65.5;
		val = 0x08;
		 WriteReg(&IMU_SPI, addr, val, IMU_CS_PORT, IMU_CS_PIN);
		break;
	case GFSR_1000DPS:
		 gScaleFactor = 32.8;
		val = 0x10;
		 WriteReg(&IMU_SPI, addr, val, IMU_CS_PORT, IMU_CS_PIN);
		break;
	case GFSR_2000DPS:
		 gScaleFactor = 16.4;
		val = 0x18;
		 WriteReg(&IMU_SPI, addr, val, IMU_CS_PORT, IMU_CS_PIN);
		break;
	default:
		 gScaleFactor = 65.5;
		val = 0x08;
		 WriteReg(&IMU_SPI, addr, val, IMU_CS_PORT, IMU_CS_PIN);
		break;
	}
}

void IMU::IMU_Setup() {
	char serialBuf[8];
	LIS3MDL_Init(&IMU_SPI, 0x60); // ±16 Gauss full scale, 20Hz ODR, Continuous conversion mode
	if (MPU_begin(&IMU_SPI ) != TRUE) {
		sprintf((char*) serialBuf, "ERROR!\r\n");
//		HAL_UART_Transmit(&DEBUG_STREAM, serialBuf, strlen((char*) serialBuf),
//				100);
	} else {
		sprintf((char*) serialBuf, "DDD!\r\n");
//		myprintf("MPU6500 Initialization Complete");
		//	 Calibrate the IMU
//		myprintf("Calibrating sensors.\r\n");
//		MPU_calibrateGyro(&MPU6500, 1000);
//		myprintf("Calibration  Complete");
	}

}

uint8_t IMU::MPU6500_ReadReg(SPI_HandleTypeDef *hspi, uint8_t add) {
	uint8_t val;
	add |= 0x80;  // set the MSB to indicate a read operation
	HAL_GPIO_WritePin(IMU_CS_PORT, IMU_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi, &add, 1, 100);
	HAL_SPI_Receive(hspi, &val, 1, 100);
	HAL_GPIO_WritePin(IMU_CS_PORT, IMU_CS_PIN, GPIO_PIN_SET);
	return val;
}
/**
 * @brief  Read data from Specific Register address of LIS3MDL
 * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
 *               the configuration information for SPI module.
 * @param  add Register address from which data is to be read
 */
uint8_t LIS3MDL_ReadReg(SPI_HandleTypeDef *hspi, uint8_t add) {
	uint8_t val;
	add |= 0x80;  // set the MSB to indicate a read operation
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi, &add, 1, 100);
	HAL_SPI_Receive(hspi, &val, 1, 100);
	HAL_GPIO_WritePin(GPIOF,  GPIO_PIN_12, GPIO_PIN_SET);
	return val;
}

void IMU::HouseKeeping() {

	//Gyro, Accel and Mag register data

	IMU_Data[0] = MPU6500_ReadReg(&IMU_SPI, ACCEL_XOUT_H);
	IMU_Data[1] = MPU6500_ReadReg(&IMU_SPI, ACCEL_XOUT_L);
	IMU_Data[2] = MPU6500_ReadReg(&IMU_SPI, ACCEL_YOUT_H);
	IMU_Data[3] = MPU6500_ReadReg(&IMU_SPI, ACCEL_YOUT_L);
	IMU_Data[4] = MPU6500_ReadReg(&IMU_SPI, ACCEL_ZOUT_H);
	IMU_Data[5] = MPU6500_ReadReg(&IMU_SPI, ACCEL_ZOUT_L);

	IMU_Data[6] = MPU6500_ReadReg(&IMU_SPI, GYRO_XOUT_H);
	IMU_Data[7] = MPU6500_ReadReg(&IMU_SPI, GYRO_XOUT_L);
	IMU_Data[8] = MPU6500_ReadReg(&IMU_SPI, GYRO_YOUT_H);
	IMU_Data[9] = MPU6500_ReadReg(&IMU_SPI, GYRO_YOUT_L);
	IMU_Data[10] = MPU6500_ReadReg(&IMU_SPI, GYRO_ZOUT_H);
	IMU_Data[11] = MPU6500_ReadReg(&IMU_SPI, GYRO_ZOUT_L);

	IMU_Data[12] = LIS3MDL_ReadReg(&IMU_SPI, LIS3MDL_OUTX_L);
	IMU_Data[13] = LIS3MDL_ReadReg(&IMU_SPI, LIS3MDL_OUTX_H);
	IMU_Data[14] = LIS3MDL_ReadReg(&IMU_SPI, LIS3MDL_OUTY_L);
	IMU_Data[15] = LIS3MDL_ReadReg(&IMU_SPI, LIS3MDL_OUTY_H);
	IMU_Data[16] = LIS3MDL_ReadReg(&IMU_SPI, LIS3MDL_OUTZ_L);
	IMU_Data[17] = LIS3MDL_ReadReg(&IMU_SPI, LIS3MDL_OUTZ_H);
}
