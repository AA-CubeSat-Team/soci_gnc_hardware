
#define SPOOF			1
#define DEBUG_I2C_a		0
#define DEBUG_I2C_b		0
#define PRINT_READINGS	1
/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* Standard C Included Files */
#include <stdio.h>
#include <string.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_lpi2c.h"
#include "fsl_lpi2c_freertos.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define LPI2C_CLOCK_FREQUENCY ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

#define I2C_MASTER_SLAVE_ADDR_7BIT (0x7EU)
#define I2C_BAUDRATE (100000) /* 100K */
#define I2C_DATA_LENGTH (32) /* MAX is 256 */

/* Task priorities. */
#define lpi2c_task_PRIORITY (configMAX_PRIORITIES - 1)

/* clang-format on */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile uint32_t timeout = 100000;

SemaphoreHandle_t lpi2c_sem;
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Task priorities. */
//#define slave_task_PRIORITY  (configMAX_PRIORITIES - 2)
#define master_task_PRIORITY (configMAX_PRIORITIES - 1)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void print_float(float a) {
	if (a < 0) {
		PRINTF("-");
		a = -a;
	}
	long x = (long) a;
	PRINTF("%ld.",x);
	PRINTF("%ld	",(long) (a*10000000) - x*10000000);

}
static void master_task(void *pvParameters);

/*******************************************************************************
 * Code
 ******************************************************************************/

lpi2c_master_config_t masterConfig;
lpi2c_rtos_handle_t master_rtos_handle;

const TickType_t xDelayms = pdMS_TO_TICKS( 2000 );
TickType_t xLastWakeTime;
int main(void)
{
	print_float(3.1415926);
	print_float(-3.1415926);

    /* Init board hardware. */
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /*Clock setting for LPI2C*/
    CLOCK_SetMux(kCLOCK_Lpi2cMux, LPI2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, LPI2C_CLOCK_SOURCE_DIVIDER);

    /* Set IRQ priority for freertos_lpi2c */
    NVIC_SetPriority(LPI2C1_IRQn, 3);
    //NVIC_SetPriority(EXAMPLE_I2C_SLAVE_IRQN, 2);


    if (xTaskCreate(master_task, "Master_task", configMINIMAL_STACK_SIZE + 100, NULL, master_task_PRIORITY, NULL) !=
            pdPASS)
        {
            vTaskSuspend(NULL);
            PRINTF("Failed to create master task");
        }

    vTaskStartScheduler();

    while (1)
    {
    }
}

void I2C_send(lpi2c_rtos_handle_t * handle, uint16_t slave_address, uint8_t subaddress, uint8_t * tx_buffer, size_t tx_size)
{
	lpi2c_master_transfer_t masterXfer;
	status_t status;

#if DEBUG_I2C_a
	PRINTF("Master will send data :");
	int i=0;
	for (i = 0; i < tx_size; i++)
	{
		if (i % 8 == 0)
		{
			PRINTF("\r\n");
		}
		PRINTF("0x%2x  ", tx_buffer[i]);
	}
	PRINTF("\r\n\r\n");
#endif

	memset(&masterXfer, 0, sizeof(masterXfer));
	masterXfer.slaveAddress   = slave_address;
	masterXfer.direction      = kLPI2C_Write;
	masterXfer.subaddress     = ((uint32_t)subaddress);
	masterXfer.subaddressSize = 1;
	masterXfer.data           = tx_buffer;
	masterXfer.dataSize       = tx_size;
	masterXfer.flags          = kLPI2C_TransferDefaultFlag;

	status = LPI2C_RTOS_Transfer(handle, &masterXfer);

	if (status == kStatus_Success)
	{
#if DEBUG_I2C_b
		PRINTF("I2C master transfer completed successfully.\r\n");
#endif
	}
	else
	{
		PRINTF("I2C master transfer completed with ERROR!:      %d\r\n", status);

	}
}

void I2C_request(lpi2c_rtos_handle_t * handle, uint16_t slave_address, uint8_t subaddress, uint8_t * rx_buffer, size_t rx_size)
{
	lpi2c_master_transfer_t masterXfer;
	status_t status;
#if DEBUG_I2C_a
	PRINTF("Master will request data\r\n");
#endif
	memset(&masterXfer, 0, sizeof(masterXfer));
	masterXfer.slaveAddress   = slave_address;
	masterXfer.direction      = kLPI2C_Read;
	masterXfer.subaddress     = ((uint32_t)subaddress);
	masterXfer.subaddressSize = 1;
	masterXfer.data           = rx_buffer;
	masterXfer.dataSize       = rx_size;
	masterXfer.flags          = kLPI2C_TransferDefaultFlag;

	status = LPI2C_RTOS_Transfer(handle, &masterXfer);

	if (status == kStatus_Success)
	{
#if DEBUG_I2C_b
		PRINTF("Received data :\r\n");
		int i;
		for (i = 0; i < rx_size; i++)
		{
			if (i % 8 == 0)
			{
				PRINTF("\r\n");
			}
			PRINTF("0x%2x  ", (rx_buffer)[i]);
		}
		PRINTF("\r\n\r\n");
#endif
	}
	else {
		PRINTF("Failed receive!:      %d\r\n", status);
	}

}
/*
 * gyro_wrap.h
 *
 *  Created on: Feb 17, 2021
 *      Author: Alex Zhen
 */


#define ARDUINO_CODE            0

#if ARDUINO_CODE
#include <Wire.h>
#else
#include "fsl_lpi2c.h"
#include "fsl_lpi2c_freertos.h"
#endif

#define COUNT_TEMP_BIAS       0     // if the code count temperature influence on output
#define MULTI_GYROS         0   // if there are multiple gyroscopes(three)
#define DIFF_TEMP_BIAS_COE      0   // if the gyroscopes have different temperature bias and sensitivity coefficients.


// register addresses FXAS21002C_H_
#define GYRO_OUT_X_MSB        0x01
#define GYRO_CTRL_REG0        0x0D
#define GYRO_TEMP         0x12
#define GYRO_CTRL_REG1        0x13
#define GYRO_INT_SRC_FLAG     0x0B

// gyro parameters

#define GYRO_ODR_NUM        0b101
#define GYRO_FSR_NUM        0b11
#define GYRO_ODR_VALUE        25
#define GYRO_FSR_VALUE        250
#define GYRO_SENSITIVITY      (float)0.0078125
#define GYRO_TEMP_0         23
#define GYRO_ADDRESS        (unsigned short)0x20
/*!
 * @brief Structure contains information about one gyroscope
 *
 */
typedef struct _Gyro
{
  float gyroXYZ[3];           /* measured angular rates*/
  int8_t temperature;           /* measured temperature*/
#if !ARDUINO_CODE
  lpi2c_rtos_handle_t * gyroHandle;   /* gyroscope i2c handle?*/
#endif
  float gyroBias[3];            /* gyroscope zero-off set(bias)*/
  float gyroTempBiasCoe[3];       /* gyroscope temperature bias coefficients*/
  float gyroTempSensCoe[3];       /* gyroscope temperature sensitivity coefficients*/
  char  gyroInitialized; /* gyroscope status */
} gyro_t;


#if MULTI_GYROS
extern gyro_t Gyro2;
extern gyro_t Gyro3;
#endif


// gyroscope struct.
gyro_t Gyro1;


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
#if ARDUINO_CODE
    Wire.beginTransmission(GYRO_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(GYRO_ADDRESS, valueSize);
    int i = 0;
    while (Wire.available()) {
      *(value+i) = Wire.read();
      i++;
    }
#else

    I2C_request(Gyro->gyroHandle, GYRO_ADDRESS, reg, value, valueSize);

#endif
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
#if ARDUINO_CODE
  Wire.beginTransmission(GYRO_ADDRESS);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
#else


  I2C_send(Gyro->gyroHandle, GYRO_ADDRESS, reg, &value, 1);
#endif
}

#if ARDUINO_CODE
/*!
 * @brief Turn on a gyroscope. Initialize all parameters of a gyroscope.
 *
 *
 * @param Gyro The gyroscope wants to be initialized
 * @return void
 *
 */
void initGyro(gyro_t * Gyro)
#else
/*!
 * @brief Turn on a gyroscope. Initialize all parameters of a gyroscope.
 *
 *
 * @param Gyro The gyroscope wants to be initialized.
 * @param gyroHandle The freertos handle of the gyroscope.
 * @param gyroTransfer The transfer information of the gyroscope.
 * @return void
 *
 */
void initGyro(gyro_t * Gyro, lpi2c_rtos_handle_t *gyroHandle)
#endif
{
  if (!Gyro->gyroInitialized)
  {
#if !ARDUINO_CODE
    Gyro->gyroHandle = gyroHandle;
#endif

#if DIFF_TEMP_BIAS_COE
    switch (base_Gyro){
      case LPI2C1:
        static const float gyroBiasValue = {-0.565375, 0.6173333, -0.0121667};
        static const float gyroTempBiasCoeValue = {0.02, 0.02, 0.01};
        static const float gyroTempSensCoeValue = {0.0008, 0.0008, 0.0001};
        Gyro->gyroBias = gyroBiasValue;
        Gyro->gyroTempBiasCoe = gyroTempBiasCoeValue;
        Gyro->gyroTempSensCoe = gyroTempSensCoeValue;
        break;
      case LPI2C2:
        static const float gyroBiasValue = {0, 0, 0};
        static const float gyroTempBiasCoeValue = {0, 0, 0};
        static const float gyroTempSensCoeValue = {0, 0, 0};
        Gyro->gyroBias = gyroBiasValue;
        Gyro->gyroTempBiasCoe = gyroTempBiasCoeValue;
        Gyro->gyroTempSensCoe = gyroTempSensCoeValue;
        break;
      case LPI2C3:
        static const float gyroBiasValue = {0, 0, 0};
        static const float gyroTempBiasCoeValue = {0, 0, 0};
        static const float gyroTempSensCoeValue = {0, 0, 0};
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

#if ARDUINO_CODE
    Wire.begin();
#endif
    Gyro->gyroInitialized = 1;
  }
}

/*!
 * @brief Set the gyroscope to desired configurations. Start reading.
 *
 *
 * @param Gyro The gyroscope wants to be set.
 * @return void
 *
 */
void startGyro(gyro_t * Gyro)
{
  if (Gyro->gyroInitialized){
  writeReg(GYRO_CTRL_REG0, GYRO_FSR_NUM, Gyro);
  writeReg(GYRO_CTRL_REG1, (GYRO_ODR_NUM<<2 | 0b10), Gyro);
  }
}

/*!
 * @brief initialize the gyroscope and start the gyroscope's reading. This
 * is the function going to be used on the FSW for starting the gyroscope.
 *
 *
 * @param Gyro The gyroscope wants to be set.
 * @return void
 *
 */
#if ARDUINO_CODE
void quickStartGyro(gyro_t * Gyro)
#else
void quickStartGyro(gyro_t * Gyro, lpi2c_rtos_handle_t *gyroHandle)
#endif
{
#if ARDUINO_CODE
  initGyro(Gyro);
#else
  initGyro(Gyro, gyroHandle);
#endif
  startGyro(Gyro);
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
  unsigned char rawData[6];  // x/y/z gyro register data stored here
  readRegs(GYRO_OUT_X_MSB,rawData, 6, Gyro);  // Read the six raw data registers into data array


#if COUNT_TEMP_BIAS
  int8_t tempDelta = Gyro->temperature - GYRO_TEMP_0;
#endif
  for (int i = 0; i<3; i++){
    short tempValue = ((short)(((unsigned short)rawData[2*i]) << 8 | ((unsigned short) rawData[2*i + 1])));
#if COUNT_TEMP_BIAS
    Gyro->gyroXYZ[i] = tempValue*GYRO_SENSITIVITY*(1 + (Gyro->gyroTempSensCoe[i])*(int16_t) tempDelta) - (Gyro->gyroBias[i]) - Gyro->gyroTempBiasCoe[i]*(int16_t) tempDelta;
#else
    Gyro->gyroXYZ[i] = ((float) tempValue)*GYRO_SENSITIVITY  - (Gyro->gyroBias[i]);
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
void resetGyro(gyro_t * Gyro){
  writeReg(GYRO_CTRL_REG1, 0b1000000, Gyro); // set reset bit to 1 to assert software reset to zero at end of boot process

  uint8_t flag;
  readRegs(GYRO_INT_SRC_FLAG, &flag, 1, Gyro);
  while(!(flag & 0x08))  { // wait for boot end flag to be set
      readRegs(GYRO_INT_SRC_FLAG, &flag, 1, Gyro);
  }
}

void test_initGyro() {
  PRINTF("Testing initGyro...\n");
	initGyro(&Gyro1, &master_rtos_handle);
  if (Gyro1.gyroInitialized) {
	  PRINTF("Gyro initialized\n");
	  PRINTF("Pass\n");
  } else {
	  PRINTF("Gryo failed to be initialized\n");
	  PRINTF("Fail\n");
  }
  PRINTF("********************\n");
}

void test_readRegs() {
	PRINTF("Testing raedRegs...\n");
#if SPOOF
  uint8_t referenceRawData[6] = {0b00000001, 0b00000010, 0b00000011, 0b00000100, 0b00000101, 0b00000110};
  uint8_t readRawData[6];
  initGyro(&Gyro1, &master_rtos_handle);
  readRegs(GYRO_OUT_X_MSB, &readRawData[0], 6, &Gyro1);
  PRINTF("Reference raw data  Read raw data Result\n");

  for (int i = 0; i < 6; i++) {
	  PRINTF("%x	%x	",referenceRawData[i], readRawData[i]);
    if (referenceRawData[i] == readRawData[i]) {
    	PRINTF("Pass\n");
    } else {
    	PRINTF("Fail\n");
    }
  }
  PRINTF("********************\n");
#else
  uint8_t referenceRawData[15] = {0, 0, 0, 0, 8, 0xD7, 0, 0,
                                  0, 0, 1, 0, 0, 0,    0};
  uint8_t readRawData[15];
  initGyro(&Gyro1, &master_rtos_handle);
  readRegs(0x07, &readRawData[0], 15, &Gyro1);
  PRINTF("Reference raw data  Read raw data Result\n");

  for (int i = 0; i < 15; i++) {
	  PRINTF("%x	%x	",referenceRawData[i], readRawData[i]);
    if (referenceRawData[i] == readRawData[i]) {
    	PRINTF("Pass\n");
    } else {
    	PRINTF("Fail\n");
    }
  }
  PRINTF("********************\n");
#endif
}

void test_writeReg() {

	PRINTF("Testing writeReg...\n");
  uint8_t referenceValue = 2;
  PRINTF("Reference value:	%d\n", referenceValue);
	initGyro(&Gyro1, &master_rtos_handle);
  writeReg(GYRO_CTRL_REG1, referenceValue, &Gyro1);
  uint8_t readValue;
  readRegs(GYRO_CTRL_REG1, &readValue, 1, &Gyro1);
  PRINTF("Read value: %hhd	", readValue);
  if (readValue == referenceValue) {
	  PRINTF("Pass\n");
  } else {
    PRINTF("Fail\n");
  }
  PRINTF("********************\n");
}

void test_startGyro() {
	PRINTF("Test startGyro...\n");
	PRINTF("Reference CTRL_REG0:	%x\n", GYRO_FSR_NUM);
	PRINTF("Reference CTRL_REG1:	%x\n", (GYRO_ODR_NUM << 2) | 0b10);

	initGyro(&Gyro1, &master_rtos_handle);
	startGyro(&Gyro1);

  uint8_t readCtrlReg0;
  readRegs(GYRO_CTRL_REG0, &readCtrlReg0, 1, &Gyro1);
  PRINTF("Read CTRL_REG0:	%x	", readCtrlReg0);
  if (readCtrlReg0 == GYRO_FSR_NUM) {
	  PRINTF("Pass\n");
  } else {
	  PRINTF("Fail\n");
  }

  uint8_t readCtrlReg1;
  readRegs(GYRO_CTRL_REG1, &readCtrlReg1, 1, &Gyro1);
  PRINTF("Read CTRL_REG1:	%x	", readCtrlReg1);
  if (readCtrlReg1 == ((GYRO_ODR_NUM << 2) | 0b10)) {
	  PRINTF("Pass\n");
  } else {
	  PRINTF("Fail\n");
  }
  PRINTF("********************\n");
}

void test_restGyro() {
#if SPOOF
	PRINTF("Test restGyro...\n");
	initGyro(&Gyro1, &master_rtos_handle);
  startGyro(&Gyro1);
  PRINTF("Start the Gyro\n");
  resetGyro(&Gyro1);
  PRINTF("Rest the gyro\n");

  uint8_t referenceRawData[6] = {0b00000001, 0b00000010, 0b00000011, 0b00000100, 0b00000101, 0b00000110};
  uint8_t readRawData[6];
  readRegs(GYRO_OUT_X_MSB, &readRawData[0], 6, &Gyro1);

  PRINTF("Reference raw data  Read raw data Result\n");

  for (int i = 0; i < 6; i++) {
	  PRINTF("%x	%x	",referenceRawData[i], readRawData[i]);
    if (referenceRawData[i] == readRawData[i]) {
    	PRINTF("Pass\n");
    } else {
    	PRINTF("Fail\n");
    }
  }
#else
	PRINTF("Test restGyro...\n");
	initGyro(&Gyro1, &master_rtos_handle);
  startGyro(&Gyro1);
  PRINTF("Start the Gyro\n");
  resetGyro(&Gyro1);
  PRINTF("Rest the gyro\n");

  uint8_t referenceRawData[15] = {0, 0, 0, 0, 8, 0xD7, 0, 0,
                                  0, 0, 1, 0, 0, 0,    0};
  uint8_t readRawData[15];
  readRegs(0x07, &readRawData[0], 15, &Gyro1);

  PRINTF("Reference raw data  Read raw data Result\n");

  for (int i = 0; i < 15; i++) {
	  PRINTF("%x	%x	",referenceRawData[i], readRawData[i]);
    if (referenceRawData[i] == readRawData[i]) {
    	PRINTF("Pass\n");
    } else {
    	PRINTF("Fail\n");
    }
  }
#endif
}


static void master_task(void *pvParameters)
{

    status_t status;
	/*
	 * masterConfig.debugEnable = false;
	 * masterConfig.ignoreAck = false;
	 * masterConfig.pinConfig = kLPI2C_2PinOpenDrain;
	 * masterConfig.baudRate_Hz = 100000U;
	 * masterConfig.busIdleTimeout_ns = 0;
	 * masterConfig.pinLowTimeout_ns = 0;
	 * masterConfig.sdaGlitchFilterWidth_ns = 0;
	 * masterConfig.sclGlitchFilterWidth_ns = 0;
	 */
	LPI2C_MasterGetDefaultConfig(&masterConfig);
	masterConfig.baudRate_Hz = I2C_BAUDRATE;

	status = LPI2C_RTOS_Init(&master_rtos_handle, (LPI2C_Type *)LPI2C1_BASE, &masterConfig, LPI2C_CLOCK_FREQUENCY);
	if (status != kStatus_Success)
	{
		PRINTF("LPI2C master: Error initializing LPI2C!\r\n");
		vTaskSuspend(NULL);
	}

  PRINTF("Tests begin\n");
  PRINTF("********************\n");

  test_initGyro();
  test_readRegs();
  test_writeReg();
  test_startGyro();
  test_restGyro();

	quickStartGyro(&Gyro1, &master_rtos_handle);

	for(;;) {
#if PRINT_READINGS
		readGyroData(&Gyro1);
		for (int i = 0; i < 3; i++) {
			print_float(Gyro1.gyroXYZ[i]);
		}
		PRINTF("\n");
#endif
	}
}



