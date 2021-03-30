//
//  mag_wrap.c
//  wrapper test
//
//  Created by Josh Perry on 3/21/21.
//

#include "mag_wrap.h"
#if !ARDUINO_CODE
#include <peripherals.h>
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
    Mag->magTransfer->direction = kLPI2C_Read;
    Mag->magTransfer->subaddress = reg;
    Mag->magTransfer->data = value;
    LPI2C_RTOS_Transfer(Mag->magHandle, Mag->magTransfer);
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
    Mag->magTransfer->direction = kLPI2C_Read;
    Mag->magTransfer->subaddress = reg;
    Mag->magTransfer->data = value;
    LPI2C_RTOS_Transfer(Mag->magHandle, Mag->magTransfer);
    return value;
#endif
}

void readMagTemp(mag_t * Mag)
{
  uint8_t rawTempData;
  readRegs(LSM303_REGISTER_MAG_TEMP_OUT_X_H_M, &rawTempData, 1, Mag);
  Mag->temperature = (int8_t) rawTempData;
}

#if ARDUINO_CODE
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
        if (reg1_a != 0x10) { errorFlag = 1; }
       
        // Set the gain to a known level
        write8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_CRB_REG_M, LSM303_MAGGAIN_1_3, Mag);
    }
    Mag->magInitialized = 1;
}
#else
void initMag(mag_t * Mag, lpi2c_rtos_handle_t *magHandle, lpi2c_master_transfer_t *magTransfer)
{
    if (!Mag->magInitialized)
    {
        static const float magCalVec[3] = {0, 0, 0};
        static const float magCalCoe[3] = {1, 1, 1};
        int errorFlag = 0;
 
        Mag->magHandle = magHandle;
        magTransfer->slaveAddress = LSM303_ADDRESS_MAG;
        magTransfer->subaddressSize = 1;
        Mag->magTransfer = magTransfer;
    }
    Mag->magInitialized = 1;
}
#endif

void readMagData(mag_t * Mag)
{
    uint8_t reg_mg = read8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_SR_REG_Mg, Mag);
    if (!(reg_mg & 0x1)) {
      errorFlag = 2;
    }

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
}
