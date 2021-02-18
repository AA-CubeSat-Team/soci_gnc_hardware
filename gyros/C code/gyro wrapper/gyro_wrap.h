/*
 * gyr_wrap.h
 *
 *  Created on: Feb 17, 2021
 *      Author: Alex Zhen
 */

#ifndef GYRO_WRAP_H_
#define GYRO_WRAP_H_

#include "fsl_lpi2c.h"
#include "fsl_lpi2c_freertos.h"

#define COUNT_TEMP_BIAS				0
#define MULTI_GYROS					0
#define DIFF_TEMP_BIAS_COE			0

// register addresses FXAS21002C_H_
#define GYRO_OUT_X_MSB				0x01
#define GYRO_CTRL_REG0				0x0D
#define GYRO_TEMP					0x12
#define GYRO_CTRL_REG1				0x13
#define GYRO_INT_SRC_FLAG			0x0B

// gyro parameters
#define GYRO_ODR_VALUE				12.5
#define GYRO_ODR_NUM				0b110
#define GYRO_FSR_VALUE				250
#define GYRO_FSR_NUM				0b11
#define GYRO_SENSITIVITY			7.8125e-3
#define GYRO_TEMP_0					23
#define GYRO_ADDRESS				(uint8_t)0x20

typedef struct _Gyro
{
	float gyroXYZ[3];
	int8_t temperature;
	lpi2c_rtos_handle_t * handle_Gyro;
	lpi2c_master_transfer_t * transfer;
	float gyroBias[3];
	float gyroTempBiasCoe[3];
	float gyroTempSensCoe[3];
} gyro_t;

extern gyro_t Gyro1;
extern LPI2C_Type * base1;
extern lpi2c_master_handle_t handle1;
extern lpi2c_master_config_t * masterConfig_Gyro1;

lpi2c_master_transfer_t * transfer1;
#if MULTI_GYROS
extern gyro_t Gyro2;
extern gyro_t Gyro3;
extern LPI2C_Type * base2;
extern lpi2c_master_handle_t handle2;
extern lpi2c_master_config_t * masterConfig_Gyro2;
extern LPI2C_Type * base3;
extern lpi2c_master_handle_t handle3;
extern lpi2c_master_config_t * masterConfig_Gyro3;
#endif



void readRegs(uint8_t reg, uint8_t *value, uint8_t valueSize, gyro_t * Gyro);
void writeReg(uint8_t reg, uint8_t value, gyro_t * Gyro);
void startGyro(gyro_t * Gyro, lpi2c_rtos_handle_t *handle_Gyro, LPI2C_Type * base_Gyro, lpi2c_master_handle_t handle_Master, lpi2c_master_config_t * masterConfig_Gyro, lpi2c_master_transfer_t *transfer);
void readTempData(gyro_t * Gyro);
void readGyroData(gyro_t * Gyro);
void reset(gyro_t * Gyro);

#endif /* GYRO_WRAP_H_ */
