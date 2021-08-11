//
//  MQT_wrap.h
//
//  Created by Josh Perry on 7/21/21.
//

#ifndef MQT_wrap_h
#define MQT_wrap_h
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define ARDUINO_CODE            1

#if ARDUINO_CODE
#include <Wire.h>
#else
#include "fsl_lpi2c.h"
#include "fsl_lpi2c_freertos.h"
#endif

#define PCA9685_MODE1     0x00
#define MODE1_RESTART     0x80
#define MODE1_SLEEP       0x10
#define MODE1_EXTCLK      0x40
#define PCA9685_PRESCALE  0xFE
#define MODE1_AI          0x20

#define PCA9685_PRESCALE_MIN 3
#define PCA9685_PRESCALE_MAX 255
#define FREQUENCY_OSCILLATOR 25000000

#define PWM_ADDR       0x47
#define X1_POS_ADDR    0x6
#define X1_NEG_ADDR    0xA
#define X2_POS_ADDR    0xE
#define X2_NEG_ADDR    0x12
#define Y1_POS_ADDR    0x16
#define Y1_NEG_ADDR    0x1A
#define Y2_POS_ADDR    0x1E
#define Y2_NEG_ADDR    0x22
#define Z_POS_ADDR     0x26
#define Z_NEG_ADDR     0x2A

typedef struct _MQT
{
  float maxMomentXY = 2*0.109;
  float maxMomentZ = 0.109;
  float requestedMoment[3];
  uint16_t val_XYZ[3];
  
  uint8_t dataInitialized;
  uint32_t oscillatorFreq;
  
  #if !ARDUINO_CODE
  lpi2c_rtos_handle_t * mqtHandle;
  #endif
} mqt_t;

extern mqt_t MQT;


uint8_t read8(uint8_t addr, mqt_t * MQT);

void write8(uint8_t addr, uint8_t value, mqt_t * MQT);

void resetPWM(mqt_t * MQT);

void initPWM(mqt_t * MQT);

void SetPWMFreq(float freq, mqt_t * MQT);

void SetOscillatorFrequency(uint32_t freq, mqt_t * MQT);

void writeRegs(uint8_t reg, uint8_t values[4], mqt_t * MQT);

void setMoments(float rqt_x, float rqt_y, float rqt_z, mqt_t * MQT);


#endif
