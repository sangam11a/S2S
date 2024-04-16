/*
 * variables.cpp
 *
 *  Created on: Apr 8, 2024
 *      Author: sanga
 */

#include "variables.h"

variables::variables() {
	// TODO Auto-generated constructor stub

}

variables::~variables() {
	// TODO Auto-generated destructor stub
}


SatelliteHealth::SatelliteHealth(){
	this->HEAD = 0;
	this->TIM_DAYS = 0;
	this->TIM_HOUR = 0;
	this->TIM_MINUTE = 0;
	this->TIM_SECOND = 0;
	this->ANT_DEPLOYED = 0;
	this->OPERATION_MODE = 0;
//	this->MSN_FLAG = 0;
//	this->RSV_FLAG = 0;
//	this->MSN1_FLAG = 0;
//	this->MSN2_FLAG = 0;
//	this->MSN3_FLAG = 0;
	this->BATTERY_VOLTAGE = 0;
	this->BATTERY_TEMPERATURE = 0;
	this->BATTERY_CURRENT = 0;
	this->ax = 0;
	this->ay = 0;
	this->az = 0;
	this->gx = 0;
	this->gy = 0;
	this->gz = 0;
	this->magx = 0;
	this->magy = 0;
	this->magz = 0;
	this->altitude = 0;
	this->solar_panel_y_current[2] ;
	this->solar_panel_z_current[2] ;
	this->rst_3v3_current  = 0;
	this->raw_current = 0;
	this->_3v3_1_current = 0;
	this->_3v3_2_current  = 0;
	this->_5v_current = 0;
	this->unreg1_current = 0;
	this->solar_panel_y_voltage[2];
	this->solar_panel_z_voltage[2];
	this->OBC_ABNL_RST = 0;
	this->RST_ABNL_RST = 0;
	this->RST_TIME = 0;
	this->CHK_SUM = 0;
}
