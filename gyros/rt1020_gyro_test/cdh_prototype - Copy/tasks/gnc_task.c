/*******************************************************************************
 * Variables
 ******************************************************************************/
#define I2C_DATA_LENGTH (32) /* MAX is 256 */



#define GYR_TEST	1
#define OSC_TEST    0

#include "dummy_type.h"
#include "wrappers/gyro_wrapper/gyro_wrap.h"
#include <gnc_task.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "clock_config.h"

//#include "FSW_Lib_types.h"
#include "act_wrap.h"
#include "sen_wrap.h"


//#include <gnc_build/FSW_Lib_ert_rtw/FSW_Lib_types.h>
//#include <gnc_build/FSW_Lib_ert_rtw/FSW_Lib.h>

//	temp struct variables
sensor_bus sens_var;
actuator_meas actu_var;
fsw_out fsw_var;

extern bool g_sunSensActive, g_magSensActive, g_phdSensActive, g_mtqSensActive, g_rwaSensActive;

//// gyro data
//lpi2c_rtos_handle_t *gyroHandle;
//lpi2c_master_transfer_t *transfer;
//gyro_t * Gyro;


void test_initGyro() {
  PRINTF("Testing initGyro...\n");
	initGyro(&Gyro1, &i2c1_m_rtos_handle);
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
  initGyro(&Gyro1, &i2c1_m_rtos_handle);
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
  initGyro(&Gyro1, &i2c1_m_rtos_handle);
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
	initGyro(&Gyro1, &i2c1_m_rtos_handle);
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

	initGyro(&Gyro1, &i2c1_m_rtos_handle);
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
	initGyro(&Gyro1, &i2c1_m_rtos_handle);
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
	initGyro(&Gyro1, &i2c1_m_rtos_handle);
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
void gnc_task(void *pvParameters)
{
  PRINTF("Tests begin\n");
  PRINTF("********************\n");

  test_initGyro();
  test_readRegs();
  test_writeReg();
  test_startGyro();
  test_restGyro();

	quickStartGyro(&Gyro1, &i2c1_m_rtos_handle);

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

