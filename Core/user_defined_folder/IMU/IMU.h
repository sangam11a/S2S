/*
 * IMU.h
 *
 *  Created on: Apr 8, 2024
 *      Author: sanga
 */
#pragma once
#include "../../Core/user_defined_folder/VARIABLES/variables.h"

#ifndef USER_DEFINED_FOLDER_IMU_IMU_H_
#define USER_DEFINED_FOLDER_IMU_IMU_H_

#define TRUE 1

#define LIS3MDL_ADDRESS    0x1C
#define LIS3MDL_CTRL_REG1  0x20
#define LIS3MDL_CTRL_REG2 0x21
#define LIS3MDL_CTRL_REG3  0x22
#define LIS3MDL_CTRL_REG4  0x23
#define LIS3MDL_SENSITIVITY  0.000488281  //±16 Gauss and a resolution of 16-bits

#define LIS3MDL_OUTX_L 0x28
#define LIS3MDL_OUTY_L 0x2A
#define LIS3MDL_OUTZ_L 0x2C
#define LIS3MDL_OUTX_H 0x29
#define LIS3MDL_OUTY_H 0x2B
#define LIS3MDL_OUTZ_H 0x2D

#define RAD2DEG 57.2957795131

#define ACCEL_XOUT_H      0x3B
#define ACCEL_XOUT_L      0x3C
#define ACCEL_YOUT_H      0x3D
#define ACCEL_YOUT_L      0x3E
#define ACCEL_ZOUT_H      0x3F
#define ACCEL_ZOUT_L      0x40

#define GYRO_XOUT_H      0x43
#define GYRO_XOUT_L      0x44
#define GYRO_YOUT_H      0x45
#define GYRO_YOUT_L      0x46
#define GYRO_ZOUT_H      0x47
#define GYRO_ZOUT_L      0x48
#define WHO_AM_I_6500_ANS 0x70
#define WHO_AM_I          0x75
#define USER_CTRL         0x6A
#define PWR_MGMT_1        0x6B

#define READWRITE         0x80
#define SPI_TIMOUT_MS     1000

class IMU {
public:

	uint8_t IMU_Data[18];

	const uint8_t ACCEL_OUT = 0x3B;
	const uint8_t GYRO_OUT = 0x43;
	const uint8_t TEMP_OUT = 0x41;

	const uint8_t EXT_SENS_DATA_00 = 0x49;

	const uint8_t GYRO_CONFIG = 0x1B;
	const uint8_t ACCEL_CONFIG = 0x1C;
	const uint8_t ACCEL_CONFIG2 = 0x1D;

	const uint8_t ACCEL_FS_SEL_2G = 0x00;
	const uint8_t ACCEL_FS_SEL_4G = 0x08;
	const uint8_t ACCEL_FS_SEL_8G = 0x10;
	const uint8_t ACCEL_FS_SEL_16G = 0x18;

	const uint8_t GYRO_FS_SEL_250DPS = 0x00;
	const uint8_t GYRO_FS_SEL_500DPS = 0x08;
	const uint8_t GYRO_FS_SEL_1000DPS = 0x10;
	const uint8_t GYRO_FS_SEL_2000DPS = 0x18;

	const uint8_t CONFIG = 0x1A;

	uint8_t aScaleFactor, aFullScaleRange, gScaleFactor, gFullScaleRange;

	enum gyroscopeFullScaleRange {
		GFSR_250DPS, GFSR_500DPS, GFSR_1000DPS, GFSR_2000DPS
	};

	enum accelerometerFullScaleRange {
		AFSR_2G, AFSR_4G, AFSR_8G, AFSR_16G
	};
	typedef enum DLPFBandwidth_ {
		DLPF_BANDWIDTH_184HZ = 0,
		DLPF_BANDWIDTH_92HZ,
		DLPF_BANDWIDTH_41HZ,
		DLPF_BANDWIDTH_20HZ,
		DLPF_BANDWIDTH_10HZ,
		DLPF_BANDWIDTH_5HZ
	} DLPFBandwidth;
public:
	IMU();
	void ReadReg(SPI_HandleTypeDef *hspi, uint8_t add, uint8_t *val, GPIO_TypeDef *GPIO_PORT, uint8_t GPIO_PIN);
	void WriteReg(SPI_HandleTypeDef *hspi, uint8_t add, uint8_t val, GPIO_TypeDef *GPIO_PORT, uint8_t GPIO_PIN);
	float LIS3MDL_ReadData(SPI_HandleTypeDef *hspi, uint8_t add);
	void LIS3MDL_Init(SPI_HandleTypeDef *hspi, uint8_t ctrl2);

	void SensortoSat_Mag();
	/* Reads register value of Gyro and Accel */
	uint8_t MPU6500_ReadReg(SPI_HandleTypeDef *hspi, uint8_t add);

	/* read the data, each argument should point to a array for x, y, and x */
//	void MPU6500_GetData(MPU6500_t *pMPU6500);

	/* sets the DLPF bandwidth to values other than default */
//	void MPU6500_SetDLPFBandwidth(DLPFBandwidth bandwidth);

	//WHO AM I register MPU
	uint8_t whoAmI();
	uint8_t SPIx_WriteRead(uint8_t Byte);
	void MPU_SPI_Write(uint8_t *pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
	void MPU_SPI_Read(uint8_t *pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);
//	void writeRegister(uint8_t subAddress, uint8_t data);
	void readRegisters(uint8_t subAddress, uint8_t count, uint8_t *dest);

	void MPU6500_SetDLPFBandwidth(DLPFBandwidth bandwidth);
	void MPU_writeAccFullScaleRange(uint8_t aScale);

//	void MPU_calibrateGyro(MPU6500_t *pMPU6500, uint16_t numCalPoints);
//	void MPU_calcAttitude(MPU6500_t *pMPU6500);
	void MPU_writeGyroFullScaleRange(uint8_t gScale);
//	void MPU_writeAccFullScaleRange(MPU6500_t *pMPU6500, uint8_t aScale);
	uint8_t MPU_begin(SPI_HandleTypeDef *SPIx);
	void TransmitReceive(SPI_HandleTypeDef *hspi, uint8_t *add, uint8_t val, GPIO_TypeDef *GPIO_PORT, uint8_t GPIO_PIN, uint8_t NumByteToWrite);
//	void MPU_readProcessedData(MPU6500_t *pMPU6500);

	void IMU_Setup();
	void Collect_IMU_Data();
	void HouseKeeping();

	virtual ~IMU();
};

#endif /* USER_DEFINED_FOLDER_IMU_IMU_H_ */
