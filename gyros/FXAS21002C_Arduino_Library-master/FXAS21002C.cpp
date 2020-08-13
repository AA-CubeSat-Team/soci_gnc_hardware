/*
 * FXAS21002Ca.cpp
 *
 *  Created on: Aug 7, 2020
 *      Author: Alex Zhen
 */
#include <Wire.h>
#include <math.h>

#include <FXAS21002C.h>

// Public Methods //////////////////////////////////////////////////////////////

// sets up the sensor with full-scale range at 'fsr', output data rate at 'odr' and cut-off frequency at 'bw'.
FXAS21002C::FXAS21002C(GyroFSR fsr, GyroODR odr, GyroBW bw)
{
	gyroODR = odr; // In hybrid mode, accel/mag data sample rates are half of this value
	gyroFSR = fsr;
	gyroBW = bw;
}

// writes registration 'reg' with value 'value'
void FXAS21002C::writeReg(byte reg, byte value)
{
	Wire.beginTransmission(address);
	Wire.write(reg);
	Wire.write(value);
	Wire.endTransmission();
}

// writes a signle field which has last digit at bit 'bit' in registration 'reg' with value 'value'.
// e.g. in CTRL_REG0, bandwidth setting is at bits 7:6, so parameter bit would be 6.
void FXAS21002C::writeField(byte reg, byte bit, byte value)
{
	byte c = readReg(reg);
	writeReg(FXAS21002C_H_CTRL_REG1, c & ~(0x00<<bit));
	writeReg(FXAS21002C_H_CTRL_REG1, c |   (value<<bit));
}

// Reads register 'reg' and return it as a byte.
byte FXAS21002C::readReg(byte reg)
{
	byte value;

	Wire.beginTransmission(address);
	Wire.write(reg);
	Wire.endTransmission(false);
	Wire.requestFrom(address, (uint8_t)1);
	value = Wire.read();

	return value;
}

// Reads 'counts' number of values from registration 'reg' and stores the values in 'dest[]'.
void FXAS21002C::readRegs(byte reg, uint8_t count, byte dest[])
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

// Reads the temperature data
void FXAS21002C::readTempData()
{
	tempData = readReg(FXAS21002C_H_TEMP);
}

// Puts the FXAS21002C into standby mode.
// It must be in standby for modifying most registers
void FXAS21002C::standby()
{
	writeField(FXAS21002C_H_CTRL_REG1, 0, 0x00);
}
// Sets the FXAS21000 to active mode.
// Needs to be in this mode to output data
void FXAS21002C::ready()
{
	writeField(FXAS21002C_H_CTRL_REG1, 0, 0x01);
}

// Puts the FXAS21002C into active mode.
// Needs to be in this mode to output data.
void FXAS21002C::active()
{
	writeField(FXAS21002C_H_CTRL_REG1, 0, 0x02);
}

// sets up basic configures, full-scale rage, cut-off frequency and output data rate.
void FXAS21002C::setConfigures()
{
	standby();
	writeReg(FXAS21002C_H_CTRL_REG0, (gyroBW<<6) | gyroFSR);
	writeReg(FXAS21002C_H_CTRL_REG1, gyroODR << 2);
}

// Reads the gyroscope data
void FXAS21002C::readGyroData()
{
	uint8_t rawData[6];  // x/y/z gyro register data stored here
	readRegs(FXAS21002C_H_OUT_X_MSB, 6, &rawData[0]);  // Read the six raw data registers into data array
	gyroData.x = ((int16_t)(((int16_t)rawData[0]) << 8 | ((int16_t) rawData[1])))*sensitivity()-gBias[0];
	gyroData.y = ((int16_t)(((int16_t)rawData[2]) << 8 | ((int16_t) rawData[3])))*sensitivity()-gBias[1];
	gyroData.z = ((int16_t)(((int16_t)rawData[4]) << 8 | ((int16_t) rawData[5])))*sensitivity()-gBias[2];
}

// return the sensitivity of the senosr depending on the value of the full-scale range
float FXAS21002C::sensitivity(void)
{
	switch (gyroFSR)
	{
		// Possible gyro scales (and their register bit settings) are:
  // 250 DPS (11), 500 DPS (10), 1000 DPS (01), and 2000 DPS  (00).
    case GFSR_2000DPS:
        return 62.5e-3;
        break;
    case GFSR_1000DPS:
    	return   31.25e-3;
    	break;
    case GFSR_500DPS:
    	return   15.625e-3;
    	break;
    case GFSR_250DPS:
    	return   7.8125e-3;
    	break;
	}
}

// returns the current output data rate as a int (Hz)
int FXAS21002C::getODR(void)
{
	switch (gyroODR)
	{
    case 	GODR_800HZ:
    		return 800;
    case	GODR_400HZ:
    		return 400;
    case	GODR_200HZ:
    		return 200;
    case	GODR_100HZ:
			return 100;
    case	GODR_50HZ:
			return 50;
    case	GODR_25HZ:
			return 25;
    case	GODR_12_5HZ:
			return 12.5;

	}
}

// calibrates the sensor under current configurations. stores the bias of each axis in gBias[].
void FXAS21002C::calibrate()
{
  int32_t gyro_bias[3] = {0, 0, 0};
  uint16_t ii, fcount = getODR();
  int16_t temp[3];

  active();  // Set to active to start collecting data
  double intt = micros();
  double waitt = 0;
  uint8_t rawData[6];
  for(ii = 0; ii < fcount+60; ii++)
  {
  readRegs(FXAS21002C_H_OUT_X_MSB, 6, &rawData[0]);
  temp[0] = ((int16_t)( ((int16_t) rawData[0]) << 8 | ((int16_t) rawData[1])));
  temp[1] = ((int16_t)( ((int16_t) rawData[2]) << 8 | ((int16_t) rawData[3])));
  temp[2] = ((int16_t)( ((int16_t) rawData[4]) << 8 | ((int16_t) rawData[5])));

//  Serial.print("X:");
//  Serial.print((float)temp[0]);
//  Serial.print(" Y:");
//  Serial.print((float)temp[1]);
//  Serial.print(" Z:");
//  Serial.println((float)temp[2]);
  if (ii>60) {
  gyro_bias[0] += (int32_t) temp[0];
  gyro_bias[1] += (int32_t) temp[1];
  gyro_bias[2] += (int32_t) temp[2];
  }
  waitt = waitt + 1.0/(fcount)*1e6;
    while(micros()-intt<waitt){
    }
  }

  gyro_bias[0] /= (int32_t) fcount; // get average values
  gyro_bias[1] /= (int32_t) fcount;
  gyro_bias[2] /= (int32_t) fcount;

  gBias[0] = (float)gyro_bias[0]*(float) sensitivity(); // get average values
  gBias[1] = (float)gyro_bias[1]*(float) sensitivity(); // get average values
  gBias[2] = (float)gyro_bias[2]*(float) sensitivity(); // get average values

  ready();  // Set to ready
}

void FXAS21002C::reset()
{
	writeReg(FXAS21002C_H_CTRL_REG1, 0x20); // set reset bit to 1 to assert software reset to zero at end of boot process
	delay(100);
while(!(readReg(FXAS21002C_H_INT_SRC_FLAG) & 0x08))  { // wait for boot end flag to be set
}

}
// Private Methods //////////////////////////////////////////////////////////////




