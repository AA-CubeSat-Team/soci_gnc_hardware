#include <gyro_wrapper/gyro_wrap.h>
#include "dummy_type.h"
#include <sen_wrap.h>
#include "fsl_lpuart_freertos.h"
#include "fsl_lpuart.h"
#include "fsl_lpi2c_freertos.h"
#include "fsl_lpi2c.h"
#include "fsl_debug_console.h"
#include <stdbool.h>
#include "peripherals.h"

extern sensor_bus sens_var;



bool sens_healthcheck(){
  PRINTF("checking sensors health\r\n");
  return true;
}
//do each sensor separately
void sens_readSun(){
  PRINTF("reading sun sensors\r\n");
  sens_var.sun_meas_ss_deg[0] = 180;
}

void sens_readMag(){
  PRINTF("reading magnetometer sensors\r\n");
  sens_var.mag_body_T[0] = 111;
}

void sens_readPhd(){
  PRINTF("reading photodiode sensors\r\n");
  sens_var.photodiodes_A[0] = 222;
}

void sens_readGyr(){
  PRINTF("reading gyroscope sensors\r\n");
  sens_var.gyro_body_radps[0] = 333;
  initGyro(&Gyro1, &i2c1_m_rtos_handle);
  readGyroData(&Gyro1);

}
