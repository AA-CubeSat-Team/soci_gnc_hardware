
#include <stdio.h>

#define ARDUINO_CODE            1
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
   //LSM303_MAGGAIN_8_1                        = 0xE0,
    LSM303_REGISTER_MAG_TEMP_OUT_H_M          = 0x31,
    LSM303_REGISTER_MAG_TEMP_OUT_L_M          = 0x32,
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
    /* lpi2c_rtos_handle_t * magHandle; */       /* i2c handle?*/
    /*lpi2c_master_transfer_t * magTransfer; */  /* i2c transfer structure pointer*/
#endif
    float magCalOffset[3];
    float magCalCoe[3];
    float T_Mag_Response;
    char magInitialized;
    int errorFlag;
} mag_t;

extern mag_t Mag1;
/* lpi2c_rtos_handle_t * magHandle1 */ /*freertos handle 1?*/
/* lpi2c_master_transfer_t magTransfer1 */ /*i2c transfer structure pointer*/

#if MULTI_MAGS
extern mag_t Mag2;
extern mag_t Mag3;
/* lpi2c_rtos_handle_t * magHandle2 */ /*freertos handle 2?*/
/* lpi2c_rtos_handle_t * magHandle3 */ /*freertos handle 3?*/
/* lpi2c_master_transfer_t magTransfer2 */ /*i2c transfer structure pointer*/
/* lpi2c_master_transfer_t magTransfer3 */ /*i2c transfer structure pointer*/
#endif

// initialize struct.
mag_t Mag1;

#if ARDUINO_CODE
#include <Wire.h>
#else
// i2c stuff, should be defined in peripherals.c?
lpi2c_rtos_handle_t * magHandle1;
lpi2c_master_transfer_t * magTransfer1;
#endif

void readRegs(uint8_t reg, uint8_t *value, uint8_t valueSize, mag_t * Mag)
{
#if ARDUINO_CODE
    Wire.beginTransmission(LSM303_ADDRESS_MAG);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(LSM303_ADDRESS_MAG, valueSize);
    int i = 0;
    while (Wire.available()) {
      *(value+i) = Wire.read();
      i++;
    }
#else
    Mag->magTransfer->direction = kLPI2C_Read;
    Mag->magTransfer->subaddress = reg;
    Mag->magTransfer->data = value;
    Mag->magTransfer->dataSize = valueSize;
    LPI2C_RTOS_Transfer(Mag->magHandle, Mag->magTransfer);
#endif
}

void write8(uint8_t address, uint8_t reg, uint8_t value, mag_t * Mag)
{
#if ARDUINO_CODE
    Wire.beginTransmission(address);
    Wire.write((uint8_t)reg);
    Wire.write((uint8_t)value);
    Wire.endTransmission();
#else
    /* rtos stuff */
#endif
}

uint8_t read8(uint8_t address, uint8_t reg, mag_t * Mag)
{
    uint8_t value;
#if ARDUINO_CODE
    Wire.beginTransmission(address);
    Wire.write((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom(address, (uint8_t)1);
    value = Wire.read();
    Wire.endTransmission();
    return value;
#else
    /* rtos stuff */
    return value;
#endif
}

void readMagTemp(mag_t * Mag)
{
    uint8_t rawTempData[2];
    readRegs(LSM303_REGISTER_MAG_TEMP_OUT_H_M, rawTempData, 2, Mag);
    uint8_t thi = rawTempData[0];
    uint8_t tlo = rawTempData[1];
    int16_t tempData = (int16_t)(tlo | ((int16_t)thi << 8));
    Mag->temperature = (int8_t) tempData;
    Serial.println(tempData);
}

//#if ARDUINO_CODE
void initMag(mag_t * Mag)
{
    if (!Mag->magInitialized)
    {
        Wire.begin();
        Serial.begin(9600);

        // Enable the magnetometer
        write8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_MR_REG_M, 0x00, Mag);

        // LSM303DLHC has no WHOAMI register so read CRA_REG_M to check
        // the default value (0b00010000/0x10)
        uint8_t reg1_a = read8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_CRA_REG_M, Mag);
        Serial.println(reg1_a, BIN);
        if (reg1_a != 0x10) { errorFlag = 1; }
       
        // Set the gain to a known level
        write8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_CRB_REG_M, LSM303_MAGGAIN_1_3, Mag);
    }
    Mag->magInitialized = 1;
}
//#else
//void initMag(mag_t * Mag, lpi2c_rtos_handle_t *magHandle, lpi2c_master_transfer_t *magTransfer)
//{
//    if (!Mag->magInitialized)
//    {
//        static const float magCalVec[3] = {0, 0, 0};
//        static const float magCalCoe[3] = {1, 1, 1};
//        int errorFlag = 0;
// 
//        Mag->magHandle = magHandle;
//        magTransfer->slaveAddress = LSM303_ADDRESS_MAG;
//        magTransfer->subaddressSize = 1;
//        Mag->magTransfer = magTransfer;
//    }
//    Mag->magInitialized = 1;
//}
//#endif

void readMagData(mag_t * Mag)
{
//    uint8_t reg_mg = read8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_SR_REG_Mg, Mag);
//    if (!(reg_mg & 0x1)) {
//      errorFlag = 2;
//    }

    // Read the magnetometer
    uint8_t raw[6];
    readRegs(LSM303_REGISTER_MAG_OUT_X_H_M, raw, (uint8_t)6, Mag);

    // Note high before low (different than accel)
    uint8_t xhi = raw[0];
    uint8_t xlo = raw[1];
    uint8_t yhi = raw[2];
    uint8_t ylo = raw[3];
    uint8_t zhi = raw[4];
    uint8_t zlo = raw[5];
    Serial.print(xhi, BIN);
    Serial.print(" ");
    Serial.print(xlo, BIN);
    Serial.print(" ");
    Serial.print(yhi, BIN);
    Serial.print(" ");
    Serial.print(ylo, BIN);
    Serial.print(" ");
    Serial.print(zhi, BIN);
    Serial.print(" ");
    Serial.println(zlo, BIN);

    // Shift values to create properly formed integer (low uint8_t first)
    int16_t raw_x = (int16_t)(xlo | ((int16_t)xhi << 8));
    int16_t raw_y = (int16_t)(ylo | ((int16_t)yhi << 8));
    int16_t raw_z = (int16_t)(zlo | ((int16_t)zhi << 8));
    
    if ((raw_x >= 2040) | (raw_x <= -2040) |
        (raw_y >= 2040) | (raw_y <= -2040) |
        (raw_z >= 2040) | (raw_z <= -2040) )  {errorFlag = 4; }

    Mag->magXYZ[0] = magCalCoe[0]*(((float)raw_x/_lsm303Mag_Gauss_LSB_XY*GAUSS_TO_MICROTESLA) - magCalVec[0]);
    Mag->magXYZ[1] = magCalCoe[1]*(((float)raw_y/_lsm303Mag_Gauss_LSB_XY*GAUSS_TO_MICROTESLA) - magCalVec[1]);
    Mag->magXYZ[2] = magCalCoe[2]*(((float)raw_z/_lsm303Mag_Gauss_LSB_Z*GAUSS_TO_MICROTESLA) - magCalVec[2]);
    
    float cal_x = magCalCoe[0]*(((float)raw_x/_lsm303Mag_Gauss_LSB_XY*GAUSS_TO_MICROTESLA) - magCalVec[0]); 
    float cal_y = magCalCoe[1]*(((float)raw_y/_lsm303Mag_Gauss_LSB_XY*GAUSS_TO_MICROTESLA) - magCalVec[1]);
    float cal_z = magCalCoe[2]*(((float)raw_z/_lsm303Mag_Gauss_LSB_Z*GAUSS_TO_MICROTESLA) - magCalVec[2]);
//    Serial.print(cal_x); 
//    Serial.print("  "); 
//    Serial.print(cal_y);
//    Serial.print("  "); 
//    Serial.println(cal_z); 
    if (errorFlag != 0) {
      Serial.println(errorFlag);
    }
}

void setup() {
  // put your setup code here, to run once:
  initMag(&Mag1);
}

void loop() {
  // put your main code here, to run repeatedly:
  readMagData(&Mag1);
  delay(2000);
  
}
