//
//  mag_wrap.h
//  wrapper test
//
//  Created by Josh Perry on 3/21/21.
//

#ifndef mag_wrap_h
#define mag_wrap_h
#include <stdio.h>

#define ARDUINO_CODE            0
#define MULTI_MAGS              0

#if ARDUINO_CODE
#include <Wire.h>
#else
#include "fsl_lpi2c.h"
#include "fsl_lpi2c_freertos.h"
#endif

// Gain multipliers
#ifndef GAUSS_TO_MICROTESLA
    #define GAUSS_TO_MICROTESLA 100
#endif
#ifndef _lsm303Mag_Gauss_LSB_XY
    #define _lsm303Mag_Gauss_LSB_XY 1100
#endif
#ifndef _lsm303Mag_Gauss_LSB_Z
    #define _lsm303Mag_Gauss_LSB_Z 980
#endif
// Address
#ifndef LSM303_ADDRESS_MAG
    #define LSM303_ADDRESS_MAG (0x3C >> 1)   // 0011110x
#endif

// Registers
#ifndef lsm303MagRegisters_t
typedef enum
{
    LSM303_REGISTER_MAG_CRA_REG_M             = 0x00,
    LSM303_REGISTER_MAG_CRB_REG_M             = 0x01,
    LSM303_REGISTER_MAG_MR_REG_M              = 0x02,
    LSM303_REGISTER_MAG_OUT_X_H_M             = 0x03,
    LSM303_REGISTER_MAG_SR_REG_Mg             = 0x09,
    LSM303_MAGGAIN_1_3                        = 0x20,
    LSM303_REGISTER_MAG_TEMP_OUT_X_H_M        = 0x31,
} lsm303MagRegisters_t;
#endif

// Calibration constants
static const float magCalVec[3] = {0, 0, 0};
static const float magCalCoe[3] = {1, 1, 1};
int errorFlag = 0;

/*!
 * @brief Structure contains information about one magnetometer
 */

typedef struct _Mag
{
    float magXYZ[3];            /* measured field strengths*/
    int8_t temperature;         /* measured temperature*/
#if !ARDUINO_CODE
lpi2c_rtos_handle_t * magHandle;        /* i2c handle?*/
lpi2c_master_transfer_t * magTransfer;  /* i2c transfer structure pointer*/
#endif
    float magCalOffset[3];
    float magCalCoe[3];
    float T_Mag_Response;
    char magInitialized;
    int errorFlag;
} mag_t;

extern mag_t Mag1;
lpi2c_rtos_handle_t * magHandle1 /*freertos handle 1?*/
lpi2c_master_transfer_t magTransfer1 /*i2c transfer structure pointer*/

#if MULTI_MAGS
extern mag_t Mag2;
extern mag_t Mag3;
lpi2c_rtos_handle_t * magHandle2 /*freertos handle 2?*/
lpi2c_rtos_handle_t * magHandle3 /*freertos handle 3?*/
lpi2c_master_transfer_t magTransfer2 /*i2c transfer structure pointer*/
lpi2c_master_transfer_t magTransfer3 /*i2c transfer structure pointer*/
#endif

void readRegs(uint8_t reg, uint8_t *value, uint8_t valueSize, mag_t * Mag);
void write8(uint8_t address, uint8_t reg, uint8_t value, mag_t * Mag);
uint8_t read8(uint8_t address, uint8_t reg, mag_t * Mag);

#if ARDUINO_CODE
void initMag(mag_t * Mag);
#else
void initMag(mag_t * Mag, lpi2c_rtos_handle_t *magHandle, lpi2c_master_transfer_t *transfer);
#endif

void readMagData(mag_t * Mag);
void readMagTemp(mag_t * Mag);

#endif /* mag_wrap_h */

