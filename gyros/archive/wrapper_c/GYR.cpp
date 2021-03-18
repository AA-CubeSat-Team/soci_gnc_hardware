/*
 * FXAS21002Ca.cpp
 *
 *  Created on: Aug 7, 2020
 *      Author: Alex Zhen
 */
#include <Wire.h>
#if !(FIXED_BIAS)
#include <Arduino.h>
#endif
#include "GYR.h"

int8_t tempData;
int8_t tempData0;
float gyroX, gyroY, gyroZ;
float gBiasX, gBiasY, gBiasZ;
uint8_t address = 0x20;

// Reads register 'reg' and return it as a byte.
void readReg(uint8_t reg, uint8_t *value)
{
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(address, (uint8_t)1);
  *value = Wire.read();
}

// Reads 'counts' number of values from registration
// 'reg' and stores the values in 'dest[]'.
void readRegs(uint8_t reg, uint8_t count, uint8_t dest[])
{
  uint8_t i = 0;

  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(address, count);

  while (Wire.available()) {
    dest[i++] = Wire.read();
  }
}

// writes registration 'reg' with value 'value'
void writeReg(uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

// writes a signle field which has last digit at bit 'bit' 
// wiht length 'numBit' in registration 'reg' with value 'value'.
// e.g. in CTRL_REG1, ODR setting is at bits 4:2, so parameter
// 'bit' would be 2, 'numBit' would be 3.
void writeField(uint8_t reg, int bit, int numBit, uint8_t value)
{
  uint8_t c;
  readReg(reg, &c);
  for (int i = 0; i < numBit; i++) {
    c &= ~(1 << (bit + i));
  }
  writeReg(reg, c | (value<<bit));
}

// Reads the temperature data
void readTempData(int8_t *tempData)
{
  uint8_t rawTempData;
  readReg(FXAS21002C_H_TEMP, &rawTempData);
  *tempData = (int8_t) rawTempData;
}

// Puts the FXAS21002C into standby mode.
void standby()
{
  writeField(FXAS21002C_H_CTRL_REG1, 0, 2, 0b00);
}

// Sets the FXAS21000 to active mode.
void ready()
{
  writeField(FXAS21002C_H_CTRL_REG1, 0, 2, 0b01);
}

// Puts the FXAS21002C into active mode.
void active()
{
  writeField(FXAS21002C_H_CTRL_REG1, 0, 2, 0b10);
}

// sets up basic configures, full-scale rage, 
// cut-off frequency and output data rate.
void setConfigures()
{
  ready();
  writeField(FXAS21002C_H_CTRL_REG0, 0, 2, FSR_NUM);
  writeField(FXAS21002C_H_CTRL_REG1, 2, 3, ODR_NUM);
}

// Reads the gyroscope data
void readGyroData(float *gyroX, float *gyroY, float *gyroZ, int8_t *tempData)
{
  readTempData(tempData);
  int8_t tempDelta = *tempData - tempData0;
  uint8_t rawData[6];  // x/y/z gyro register data stored here
  readRegs(FXAS21002C_H_OUT_X_MSB, 6, rawData);  // Read the six raw data registers into data array
  *gyroX = ((int16_t)(((int16_t)rawData[0]) << 8 | ((int16_t) rawData[1])));
  *gyroY = ((int16_t)(((int16_t)rawData[2]) << 8 | ((int16_t) rawData[3])));
  *gyroZ = ((int16_t)(((int16_t)rawData[4]) << 8 | ((int16_t) rawData[5])));
  #if COUNT_TEMP_BIAS
  *gyroX = (*gyroX)*SENSITIVITY*(1 + TEMP_SENS_COE_X*(int16_t) tempDelta) - gBiasX - TEMP_BIAS_COE_X*(int16_t) tempDelta;
  *gyroY = (*gyroY)*SENSITIVITY*(1 + TEMP_SENS_COE_Y*(int16_t) tempDelta) - gBiasY - TEMP_BIAS_COE_Y*(int16_t) tempDelta;
  *gyroZ = (*gyroZ)*SENSITIVITY*(1 + TEMP_SENS_COE_X*(int16_t) tempDelta) - gBiasZ - TEMP_BIAS_COE_Z*(int16_t) tempDelta;
  #else
  *gyroX = (*gyroX)*SENSITIVITY - gBiasX;
  *gyroY = (*gyroY)*SENSITIVITY - gBiasY;
  *gyroZ = (*gyroZ)*SENSITIVITY - gBiasZ;  
  #endif
}

#if FIXED_BIAS
void calibrate(float *gBiasX, float *gBiasY, float *gBiasZ)
{
  *gBiasX = -0.565375;
  *gBiasY = 0.6173333;
  *gBiasZ = -0.0121667;
}
#else
void calibrate(float *gBiasX, float *gBiasY, float *gBiasZ)
{
  uint16_t ii, fcount = 5*ODR_VALUE;
  uint8_t rawData[6];
  int16_t temp[3];
  float gyro_bias[3] = {0, 0, 0};

  active();  // Set to active to start collecting data
  
  for(ii = 0; ii < fcount+60; ii++)
  {
    double intt = micros();
    readRegs(FXAS21002C_H_OUT_X_MSB, 6, &rawData[0]);
    temp[0] = ((int16_t)( ((int16_t) rawData[0]) << 8 | ((int16_t) rawData[1])));
    temp[1] = ((int16_t)( ((int16_t) rawData[2]) << 8 | ((int16_t) rawData[3])));
    temp[2] = ((int16_t)( ((int16_t) rawData[4]) << 8 | ((int16_t) rawData[5])));
  
    if (ii>60) {
      gyro_bias[0] += (int32_t) temp[0];
      gyro_bias[1] += (int32_t) temp[1];
      gyro_bias[2] += (int32_t) temp[2];
    }
    while(micros()-intt < 1.0/(ODR_VALUE)*1e6){
    }
  }

  gyro_bias[0] /= (int32_t) fcount; // get average values
  gyro_bias[1] /= (int32_t) fcount;
  gyro_bias[2] /= (int32_t) fcount;

  *gBiasX = gyro_bias[0]* SENSITIVITY;
  *gBiasY = gyro_bias[1]* SENSITIVITY;
  *gBiasZ = gyro_bias[2]* SENSITIVITY;

  ready();
}
#endif

void reset(){
  writeReg(FXAS21002C_H_CTRL_REG1, 0b1000000); // set reset bit to 1 to assert software reset to zero at end of boot process
  delay(100);

  uint8_t flag;
  readReg(FXAS21002C_H_INT_SRC_FLAG, &flag);
  while(!(flag & 0x08))  { // wait for boot end flag to be set
      readReg(FXAS21002C_H_INT_SRC_FLAG, &flag);
  }
}
