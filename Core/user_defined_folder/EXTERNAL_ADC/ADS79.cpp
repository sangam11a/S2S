/*
 * ADS79.cpp
 *
 *  Created on: Apr 10, 2024
 *      Author: sanga
 */

#include "ADS79.h"
ADS79::ADS79() {
	// TODO Auto-generated constructor stub

}

ADS79::~ADS79() {
	// TODO Auto-generated destructor stub
}



/*
 * @brief		function to enable the ADC
 *
 * @retval 		null
 *
 * @param 		null
 */
void ADS79::ADC_Enable() {
	HAL_GPIO_WritePin(GPIOD, ADC_CS_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
}

/*
 * @brief 		function to disable the ADC
 *
 * @retval		null
 *
 * @param		null
 */
void ADS79::ADC_Disable() {
	HAL_GPIO_WritePin(GPIOD, ADC_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
}

/*
 * @brief		function to select MANUAL mode for ADC
 *
 * @retval		null
 *
 * @param		null
 */
void ADS79::MANUAL_Select() {
	uint8_t command[2];
	uint16_t temp;

	temp = MANUAL_MODE;
	command[0] = temp >> 8;
	command[1] = temp & 0xff;

	ADC_Enable();
	HAL_Delay(1);
	HAL_SPI_TransmitReceive(&E_ADC, command, adc_data, 2, 1000);
	HAL_Delay(1);
	ADC_Disable();
	HAL_Delay(1);
}


/*
 *
 */
void ADS79::SPI_Transmit_Receive(SPI_HandleTypeDef *SPI, uint8_t *command,uint8_t *adc_data){
	ADC_Enable();
	HAL_Delay(1);
	HAL_SPI_TransmitReceive(SPI, command, adc_data, sizeof(command), 1000);
	HAL_Delay(1);
	ADC_Disable();
	HAL_Delay(1);
}

/*
 *
 */
void ADS79::SPI_Transmit_Receive(SPI_HandleTypeDef *SPI, uint8_t *command){
	ADC_Enable();
	HAL_Delay(1);
	HAL_SPI_Transmit(SPI, command, sizeof(command), 1000);
	HAL_Delay(1);
	ADC_Disable();
	HAL_Delay(1);
}

/*
 * @brief		function to select AUTO-2 mode for ADC
 *
 * @retval 		null
 *
 * @param		null
 */
void ADS79::AUTO_2_Select_ADC(operation_modes mode) {
	uint8_t command[2];
	uint16_t temp;
	switch (mode) {
	case ADC_SELECT:
		temp = AUTO_2_MODE;
		command[0] = temp >> 8;
		command[1] = temp & 0xff;
//		ADC_Enable();
//		HAL_Delay(1);
//		HAL_SPI_Transmit(&E_ADC, command, 2, 1000);
//		HAL_Delay(1);
//		ADC_Disable();
//		HAL_Delay(1);
		SPI_Transmit_Receive(&E_ADC, command);
		break;
	case ADC_READ:
		temp = AUTO_2_MODE2;
		command[0] = temp >> 8;
		command[1] = temp & 0xff;
//		ADC_Enable();
//		HAL_Delay(1);
		SPI_Transmit_Receive(&E_ADC, command, adc_data);
//		HAL_SPI_TransmitReceive(&E_ADC, command, adc_data, 2, 1000);
//		HAL_Delay(1);
//		ADC_Disable();
//		HAL_Delay(1);
//		SPI_Transmit()
		break;
	default:
		break;
	}
}

/*
 * @brief 		funtion to program AUTO-2 mode for ADC
 *
 * @retval		null
 *
 * @param		null
 */
void ADS79::ADC_AUTO_2_Program() {
	uint8_t command[2];
	uint16_t temp;

	temp = ADC_AUTO_2_PROGRAM;
	command[0] = temp >> 8;
	command[1] = temp & 0xff;
	SPI_Transmit_Receive(&E_ADC, command);
//	ADC_Enable();
//	HAL_Delay(1);
//	HAL_SPI_Transmit(&E_ADC, command, 2, 1000);
//	HAL_Delay(1);
//	ADC_Disable();
//	HAL_Delay(1);
}

/*
 * @brief 		function to combine the ADC data to one variable
 *
 * @retval		null
 *
 * @param		float pointer to the buffer where the converted data is to be stored
 */
void ADS79::ADC_CombineData(uint16_t *adc_channels, uint16_t *adc1_channels) {
	int i = 0, j = 0;

	if (i < E_ADC_DATA_LEN) {
		adc_channels[i] = ((adc_data[0] << 8) | adc_data[1]);
		i++;
		if (i == E_ADC_DATA_LEN) {
			for (int x = 0; x < E_ADC_DATA_LEN; x++) {
				adc_channels[x] = adc_channels[x] & 0x0fff;
			}
		}
	}
}

/*
 * @brief		function to convert the raw data into actual data for ADC
 *
 * @retval		null
 *
 * @param		floating pointer to the buffer where the converted data is to be stored,
 * 				uint16_t pointer to the buffer which contains the raw data
 */
void ADS79::ADC_Conv_Data(float *adc_conv_buf, uint16_t *adc_buf) {

	for (int x = 0; x < E_ADC_DATA_LEN; x++) {
		adc_conv_buf[x] = (2.5 * adc_buf[x]) / 4095.0;
	}
}

/*
 * @brief		function to convert the raw data into actual data for ADC1
 *
 * @retval		null
 *
 * @param		floating pointer to the buffer where the converted data is to be stored,
 * 				uint16_t pointer to the buffer which contains the raw data
 */
void ADS79::ADC1_Conv_Data(float *adc1_conv_buf, uint16_t *adc1_buf) {
	float dummy;
	float root;

	for (int x = 6; x < E_ADC_DATA_LEN; x++) {
		switch (x) {
		case 8:		// Solar Panel Total Current
			dummy = (2.5 * adc1_buf[x]) / 4095.0;
			adc1_conv_buf[x] = ((dummy - 1.65) / 264) * 1000;//converting voltage to current. In actual code need to keep separate variable for current
			break;
		case 9:		// Battery current
			dummy = (2.5 * adc1_buf[x]) / 4095.0;
			adc1_conv_buf[x] = ((dummy - 1.65) / 264) * 1000;//converting voltage to current. In actual code need to keep separate variable for current

			break;
		case 10:  // COM Board Temp data
			dummy = (2.5 * adc1_buf[x]) / 4095.0;
			root = sqrtf(
					(5.506 * 5.506) + (4 * 0.00176 * (870.6 - (dummy * 1000))));
			adc1_conv_buf[x] = ((5.506 - root) / (2 * (-0.00176))) + 30;
			break;
		case 11:	// COM RSSI analog data
			adc1_conv_buf[x] = adc1_buf[x];
			break;
		default:		// baki sabai
			dummy = (2.5 * adc1_buf[x]) / 4095.0;
			adc1_conv_buf[x] = (dummy * (1100.0 + 931.0)) / 931.0;
			break;
		}
	}
}

/*
 * @brief		function to run the whole ADC operation
 *
 * @param		floating pointers to the buffer which stores the converted data.
 *
 * @retval		null
 */
void ADS79::ADC_Operate() {
	MANUAL_Select();
	AUTO_2_Select_ADC(ADC_SELECT); // programming cycle of ADC 0
	ADC_AUTO_2_Program();

	for (int x = 0; x < E_ADC_DATA_LEN; x++) {
		if (x < E_ADC_DATA_LEN) {
			AUTO_2_Select_ADC(ADC_READ);			// Data read cycle of ADC 0

		}				// Data  Read cycle from ADC 1
		ADC_CombineData(adc_channels, adc1_channels);
		ADC1_Conv_Data((float*)adc_channels, adc1_channels);
	}
}

/*
 * @brief		function to convert the voltage data of ADC1 to temperature data
 *
 * @param		float adc1_conv_buf		pointer to the buffer which is storing the converted raw data(voltage) of ADC1
 * 				float temp_buf			pointer to the buffer which stores the voltage converted to the temperature data.
 * 										The temperature converted is multiplied by 10 to remove floating point of 1 decimal place
 *
 * @retval		null
 */
void ADS79::ADC_Temp_Conv(float *adc_conv_buf, float *temp_buf) {
	float root = 0;

	for (int i = 0; i < E_ADC_DATA_LEN; i++) {

		if (i == 6) {		// Battery temperature channel
			float res = (adc_conv_buf[i] * 10000) / (2.5 - adc_conv_buf[i]);
			float tempK = (3976 * 298) / (3976 - (298 * log(10000 / res)));
			temp_buf[i] = (tempK - 273) * 100;
		} else {
			root =
					sqrtf(
							(5.506 * 5.506)
									+ (4 * 0.00176
											* (870.6 - (adc_conv_buf[i] * 1000))));
			temp_buf[i] = ((((5.506 - root) / (2 * (-0.00176))) + 30))*100;
		}
	}
}
