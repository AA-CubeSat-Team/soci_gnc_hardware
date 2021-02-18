/*
 * gyr_wrap.c
 *
 *  Created on: Feb 17, 2021
 *      Author: Alex Zhen
 */


#include <gyro_wrap.h>


// gyroscope related variables
gyro_t Gyro1;

// i2c stuff
lpi2c_rtos_handle_t * handle_Gyro1;
LPI2C_Type * base1 = LPI2C1;
lpi2c_master_handle_t handle1;
lpi2c_master_config_t * masterConfig_Gyro1;

lpi2c_master_transfer_t * transfer1;


// Reads register 'reg' and return it as a byte.
void readRegs(uint8_t reg, uint8_t *value, uint8_t valueSize, gyro_t * Gyro)
{
	Gyro->transfer->direction = kLPI2C_Read;
	Gyro->transfer->subaddress = reg;
	Gyro->transfer->data = value;
	Gyro->transfer->dataSize = valueSize;
	LPI2C_RTOS_Transfer(Gyro->handle_Gyro, Gyro->transfer);
}

// writes registration 'reg' with value 'value'
void writeReg(uint8_t reg, uint8_t value, gyro_t * Gyro)
{
	Gyro->transfer->direction = kLPI2C_Write;
	Gyro->transfer->subaddress = reg;
	*(uint8_t*)(Gyro->transfer->data) = value;
	Gyro->transfer->dataSize = 1;
	LPI2C_RTOS_Transfer(Gyro->handle_Gyro, Gyro->transfer);
}

// sets up basic configures, full-scale rage,
// cut-off frequency and output data rate.
void startGyro(gyro_t * Gyro, lpi2c_rtos_handle_t *handle_Gyro, LPI2C_Type * base_Gyro, lpi2c_master_handle_t handle_Master, lpi2c_master_config_t * masterConfig_Gyro, lpi2c_master_transfer_t *transfer)
{
	static const float gyroXYZValue[3] = {0, 0, 0};
	memcpy(Gyro->gyroXYZ,gyroXYZValue, 12);
	Gyro->temperature = 23;

	uint32_t srcClock_Hz_Gyro = 9600000;
	LPI2C_MasterGetDefaultConfig(masterConfig_Gyro);
	handle_Gyro->drv_handle = handle_Master;
	LPI2C_RTOS_Init(handle_Gyro, base_Gyro, masterConfig_Gyro, srcClock_Hz_Gyro);
	Gyro->handle_Gyro = handle_Gyro;

	transfer->flags = 0;
	transfer->slaveAddress = GYRO_ADDRESS;
	Gyro->transfer = transfer;

#if DIFF_TEMP_BIAS_COE
	switch (base_Gyro){
		case LPI2C1:
			static const gyroBiasValue = {-0.565375, 0.6173333, -0.0121667};
			static const gyroTempBiasCoeValue = {0.02, 0.02, 0.01};
			static const gyroTempSensCoeValue = {0.0008, 0.0008, 0.0001};
			Gyro->gyroBias = gyroBiasValue;
			Gyro->gyroTempBiasCoe = gyroTempBiasCoeValue;
			Gyro->gyroTempSensCoe = gyroTempSensCoeValue;
			break;
		case LPI2C2:
			static const gyroBiasValue = {0, 0, 0};
			static const gyroTempBiasCoeValue = {0, 0, 0};
			static const gyroTempSensCoeValue = {0, 0, 0};
			Gyro->gyroBias = gyroBiasValue;
			Gyro->gyroTempBiasCoe = gyroTempBiasCoeValue;
			Gyro->gyroTempSensCoe = gyroTempSensCoeValue;
			break;
		case LPI2C3:
			static const gyroBiasValue = {0, 0, 0};
			static const gyroTempBiasCoeValue = {0, 0, 0};
			static const gyroTempSensCoeValue = {0, 0, 0};
			Gyro->gyroBias = gyroBiasValue;
			Gyro->gyroTempBiasCoe = gyroTempBiasCoeValue;
			Gyro->gyroTempSensCoe = gyroTempSensCoeValue;
			break;
	}
#else
	static const float gyroBiasValue[3] = {-0.565375, 0.6173333, -0.0121667};
	static const float gyroTempBiasCoeValue[3] = {0.02, 0.02, 0.01};
	static const float gyroTempSensCoeValue[3] = {0.0008, 0.0008, 0.0001};
	memcpy(Gyro->gyroBias,gyroBiasValue, 12);
	memcpy(Gyro->gyroTempBiasCoe,gyroTempBiasCoeValue, 12);
	memcpy(Gyro->gyroTempSensCoe,gyroTempSensCoeValue, 12);
#endif

	writeReg(GYRO_CTRL_REG0, GYRO_FSR_NUM, Gyro);
	writeReg(GYRO_CTRL_REG1, (GYRO_ODR_NUM<<2 | 0b10), Gyro);
}

// Reads the temperature data
void readTempData(gyro_t * Gyro)
{
  uint8_t rawTempData;
  readRegs(GYRO_TEMP, &rawTempData, 1, Gyro);
  Gyro->temperature = (int8_t) rawTempData;
}

// Reads the gyroscope data
void readGyroData(gyro_t * Gyro)
{
  readTempData(Gyro);
  uint8_t rawData[6];  // x/y/z gyro register data stored here
  readRegs(GYRO_OUT_X_MSB,rawData, 6, Gyro);  // Read the six raw data registers into data array
  readRegs(GYRO_OUT_X_MSB, &rawData[0], 6, Gyro);

#if COUNT_TEMP_BIAS
  int8_t tempDelta = *tempData - tempData0;
#endif
  for (int i = 0; i<3; i++){
	  Gyro->gyroXYZ[i] = ((int16_t)(((int16_t)rawData[2*i]) << 8 | ((int16_t) rawData[2*i + 1])));
#if COUNT_TEMP_BIAS
	  Gyro->gyroXYZ[i] = (Gyro->gyroXYZ[i])*GYRO_SENSITIVITY*(1 + (Gyro->gyroTempSensCoe[i])*(int16_t) tempDelta) - (Gyro->gyroBias[i]) - Gyro->gyroTempBiasCoe[i]*(int16_t) tempDelta;
#else
	  Gyro->gyroXYZ[i] = (Gyro->gyroXYZ[i])*GYRO_SENSITIVITY - (Gyro->gyroBias[i]);
#endif
  }


}

void reset(gyro_t * Gyro){
  writeReg(GYRO_CTRL_REG1, 0b1000000, Gyro); // set reset bit to 1 to assert software reset to zero at end of boot process

  uint8_t flag;
  readRegs(GYRO_INT_SRC_FLAG, &flag, 1, Gyro);
  while(!(flag & 0x08))  { // wait for boot end flag to be set
      readRegs(GYRO_INT_SRC_FLAG, &flag, 1, Gyro);
  }
}
