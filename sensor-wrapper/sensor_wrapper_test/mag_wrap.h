//
//  mag_wrap.h
//  wrapper test
//
//  Created by Josh Perry on 3/21/21.
//


#ifndef mag_wrap_h
#define mag_wrap_h

#define ARDUINO_CODE            1
#define MULTI_MAGS              1

#if ARDUINO_CODE
#include <Wire.h>
#else
#include "fsl_lpi2c.h"
#include "fsl_lpi2c_freertos.h"
#include "peripherals.h"
#endif


/*!
 * @brief Structure contains information about one magnetometer
 */

typedef struct _Mag
{
  float magXYZ[3];            /* measured field strengths*/
  int8_t temperature;         /* measured temperature*/
#if !ARDUINO_CODE
  lpi2c_rtos_handle_t * magHandle;        /* i2c handle?*/
#endif
  float magCalVec[3];
  float magCalCoe[3];
  char magInitialized;
  int errorFlag;
} mag_t;

extern mag_t Mag1, Mag2, Mag3;

void readRegs(uint8_t reg, uint8_t *value, uint8_t valueSize, mag_t * Mag);
void write8(uint8_t reg, uint8_t value, mag_t * Mag);

#if ARDUINO_CODE
void initMag(mag_t * Mag);
#else
void initMag(mag_t * Mag, lpi2c_rtos_handle_t *magHandle);
#endif

void startMag(mag_t * Mag);

#if ARDUINO_CODE
void quickStartMag(mag_t * Mag);
#else
void quickStartMag(mag_t * Mag, lpi2c_rtos_handle_t * magHandle);
#endif

void readMagData(mag_t * Mag);
void readMagTemp(mag_t * Mag);

#endif /* mag_wrap_h */
