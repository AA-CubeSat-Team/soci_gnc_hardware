/*
 * gyr_wrap.c
 *
 *  Created on: Feb 17, 2021
 *      Author: Alex Zhen
 */


#include <gyro_wrap.h>


// gyroscope struct.
gyro_t Gyro1;

// i2c stuff
lpi2c_rtos_handle_t * handle_Gyro1;
LPI2C_Type * base1 = LPI2C1; // found this from MIMXRT1021.h, not sure if that's the right value to be used.
lpi2c_master_handle_t handle1; // should this be the i2c1_handler that is talked about in the meeting? do I have to initialize it or it is already there?
lpi2c_master_config_t * masterConfig_Gyro1;

lpi2c_master_transfer_t * transfer1;


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
void readRegs(uint8_t reg, uint8_t *value, uint8_t valueSize, gyro_t * Gyro)
{
	Gyro->transfer->direction = kLPI2C_Read;
	Gyro->transfer->subaddress = reg;
	Gyro->transfer->data = value;
	Gyro->transfer->dataSize = valueSize;
	LPI2C_RTOS_Transfer(Gyro->handle_Gyro, Gyro->transfer);
}

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
void writeReg(uint8_t reg, uint8_t value, gyro_t * Gyro)
{
	Gyro->transfer->direction = kLPI2C_Write;
	Gyro->transfer->subaddress = reg;
	*(uint8_t*)(Gyro->transfer->data) = value;
	Gyro->transfer->dataSize = 1;
	LPI2C_RTOS_Transfer(Gyro->handle_Gyro, Gyro->transfer);
}

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
void startGyro(gyro_t * Gyro, lpi2c_rtos_handle_t *handle_Gyro, LPI2C_Type * base_Gyro, lpi2c_master_handle_t handle_Master, lpi2c_master_config_t * masterConfig_Gyro, lpi2c_master_transfer_t *transfer)
{
	static const float gyroXYZValue[3] = {0, 0, 0};
	memcpy(Gyro->gyroXYZ,gyroXYZValue, 12); /* Question: do I have to initialize all fields of a structure?
	 	 	 	 	 	 	 	 	 	Does this way to initialize a structure even works?
	 	 	 	 	 	 	 	 	 	When is the structure initialized? At line 13 or when the first its field is initialized?
	 	 	 	 	 	 	 	 	 		*/
	Gyro->temperature = 23;


	uint32_t srcClock_Hz_Gyro = 9600000; // not sure what this value should be
	LPI2C_MasterGetDefaultConfig(masterConfig_Gyro); // not sure if this is the right way to initialize it, found this function in fsl_lpi2c.h.
	handle_Gyro->drv_handle = handle_Master;
	LPI2C_RTOS_Init(handle_Gyro, base_Gyro, masterConfig_Gyro, srcClock_Hz_Gyro); // I guess this is the right way to initialize handle_Gyro?
	Gyro->handle_Gyro = handle_Gyro;

	transfer->flags = 0;
	transfer->slaveAddress = GYRO_ADDRESS;
	transfer->direction = kLPI2C_Read;
	transfer->subaddress = 0;
	transfer->subaddressSize = 1;
	static void * data_temp;
	transfer->data = data_temp;
	transfer->dataSize = 1;
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

/*!
 * @brief Read the temperature of a gyroscope.
 *
 *
 * @param Gyro The gyroscope that its temperature want to be read.
 * @return void
 *
 */
void readTempData(gyro_t * Gyro)
{
  uint8_t rawTempData;
  readRegs(GYRO_TEMP, &rawTempData, 1, Gyro);
  Gyro->temperature = (int8_t) rawTempData;
}

/*!
 * @brief Read the angular rates of a gyroscope (x,y,z axes).
 *
 *
 * @param Gyro The gyroscope that its angular rates  want to be read.
 * @return void
 *
 */
void readGyroData(gyro_t * Gyro)
{
  readTempData(Gyro);
  uint8_t rawData[6];  // x/y/z gyro register data stored here
  readRegs(GYRO_OUT_X_MSB,rawData, 6, Gyro);  // Read the six raw data registers into data array
  readRegs(GYRO_OUT_X_MSB, &rawData[0], 6, Gyro);

#if COUNT_TEMP_BIAS
  int8_t tempDelta = Gyro->temperature - GYRO_TEMP_0;
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

/*!
 * @brief Reset a gyroscope. The i2c connection won't be reset.
 *
 *
 * @param Gyro The gyroscope to be reset
 * @return void
 *
 */
void reset(gyro_t * Gyro){
  writeReg(GYRO_CTRL_REG1, 0b1000000, Gyro); // set reset bit to 1 to assert software reset to zero at end of boot process

  uint8_t flag;
  readRegs(GYRO_INT_SRC_FLAG, &flag, 1, Gyro);
  while(!(flag & 0x08))  { // wait for boot end flag to be set
      readRegs(GYRO_INT_SRC_FLAG, &flag, 1, Gyro);
  }
}
