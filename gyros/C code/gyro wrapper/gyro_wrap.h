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

#define COUNT_TEMP_BIAS				0 		// if the code count temperature influence on output
#define MULTI_GYROS					0		// if there are multiple gyroscopes(three)
#define DIFF_TEMP_BIAS_COE			0		// if the gyroscopes have different temperature bias and sensitivity coefficients.

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

/*!
 * @brief Structure contains information about one gyroscope
 *
 */
typedef struct _Gyro
{
	float gyroXYZ[3];						/* measured angular rates*/
	int8_t temperature;						/* measured temperature*/
	lpi2c_rtos_handle_t * handle_Gyro;		/* gyroscope i2c handle?*/
	lpi2c_master_transfer_t * transfer;		/* gyroscope i2c transfer structure pointer*/
	float gyroBias[3];						/* gyroscope zero-off set(bias)*/
	float gyroTempBiasCoe[3];				/* gyroscope temperature bias coefficients*/
	float gyroTempSensCoe[3];				/* gyroscope temperature sensitivity coefficients*/
} gyro_t;

extern gyro_t Gyro1;								/* gyroscope 1*/
extern lpi2c_rtos_handle_t * handle_Gyro1;			/* freertos handle of gyroscope 1?*/
extern LPI2C_Type * base1;							/* base of gyroscope 1?*/
extern lpi2c_master_handle_t handle1;				/* master handle of gyroscope 1?*/
extern lpi2c_master_config_t * masterConfig_Gyro1;	/* config. of gyroscope 1?*/

lpi2c_master_transfer_t * transfer1;				/* transfer of gyroscope 1*/
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


/*!
 * @brief read the value of registers of a gyroscope.
 *
 *
 * @param reg The register want to be read.
 * @param value The variable to hold the value of the register.
 * @param valueSize The size of the value of the register.
 * @param Gyro The gyroscope want to be read.
 * @return void
 *
 */
void readRegs(uint8_t reg, uint8_t *value, uint8_t valueSize, gyro_t * Gyro);

/*!
 * @brief write a value to the registers of a gyroscope.
 *
 *
 * @param reg The register want to be written.
 * @param value The value want to assigned to the register.
 * @param valueSize The size of The value want to assigned to the register.
 * @param Gyro The gyroscope want to be written.
 * @return void
 *
 */
void writeReg(uint8_t reg, uint8_t value, gyro_t * Gyro);

/*!
 * @brief Turn on a gyroscope. Initialize all parameters of a gyroscope. Set the gyroscope to desired configurations. Start reading.
 *
 *
 * @param Gyro The gyroscope want to be read.
 * @param handle_Gyro The freertos handle of the gyroscope.
 * @param base_Gyro The base of the gyroscope.
 * @param handle_Master The master handle of the gyroscope.
 * @param masterConfig_Gyro The master config. of the gyroscope.
 * @param transfer_Gyro The transfer information of the gyroscope.
 * @return void
 *
 */
void startGyro(gyro_t * Gyro, lpi2c_rtos_handle_t *handle_Gyro, LPI2C_Type * base_Gyro, lpi2c_master_handle_t handle_Master, lpi2c_master_config_t * masterConfig_Gyro, lpi2c_master_transfer_t *transfer);

/*!
 * @brief Read the temperature of a gyroscope.
 *
 *
 * @param Gyro The gyroscope that its temperature want to be read.
 * @return void
 *
 */
void readTempData(gyro_t * Gyro);

/*!
 * @brief Read the angular rates of a gyroscope (x,y,z axes).
 *
 *
 * @param Gyro The gyroscope that its angular rates  want to be read.
 * @return void
 *
 */
void readGyroData(gyro_t * Gyro);

/*!
 * @brief Reset a gyroscope. The i2c connection won't be reset.
 *
 *
 * @param Gyro The gyroscope to be reset
 * @return void
 *
 */
void reset(gyro_t * Gyro);

#endif /* GYRO_WRAP_H_ */
