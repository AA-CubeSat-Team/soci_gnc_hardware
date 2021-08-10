//
//  MQT_wrap.cpp
//
//  Created by Josh Perry on 7/21/21.
//

#include "MQT_wrap.h"

#define lowByte(w) ((uint8_t) ((w) & 0xff))

// initialize struct
mqt_t MQT;

// helper functions for initialization
uint8_t read8(uint8_t addr, mqt_t * MQT) {
  #if ARDUINO_CODE
    Wire.beginTransmission(PWM_ADDR);
    Wire.write(addr);
    Wire.endTransmission();
    Wire.requestFrom((uint8_t)PWM_ADDR, (uint8_t)1);
    return Wire.read();
  #else
    I2C_read(MQT->mqtHandle, PWM_ADDR, reg, value, valueSize);
  #endif
}

void write8(uint8_t reg, uint8_t value, mqt_t * MQT) {
  #if ARDUINO_CODE
    Wire.beginTransmission(PWM_ADDR);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
  #else
    I2C_send(MQT->mqtHandle, PWM_ADDR, reg, &value, 1);
  #endif
}

// reset function for PWM chip
void resetPWM(mqt_t * MQT) {
  write8(PCA9685_MODE1, MODE1_RESTART, MQT);
}

void SetPWMFreq(float freq, mqt_t * MQT) {
  // Range output modulation frequency is dependant on oscillator
  if (freq < 1)
    freq = 1;
  if (freq > 3500)
    freq = 3500; // Datasheet limit is 3052=50MHz/(4*4096)
  
  float prescaleval = ((MQT->oscillatorFreq / (freq * 4096.0)) + 0.5) - 1;
  if (prescaleval < PCA9685_PRESCALE_MIN)
    prescaleval = PCA9685_PRESCALE_MIN;
  if (prescaleval > PCA9685_PRESCALE_MAX)
    prescaleval = PCA9685_PRESCALE_MAX;
  uint8_t prescale = (uint8_t)prescaleval;

  uint8_t oldmode = read8(PCA9685_MODE1, MQT);
  uint8_t newmode = (oldmode & ~MODE1_RESTART) | MODE1_SLEEP; // sleep
  write8(PCA9685_MODE1, newmode, MQT);                             // go to sleep
  write8(PCA9685_PRESCALE, prescale, MQT); // set the prescaler
  write8(PCA9685_MODE1, oldmode, MQT);

  // This sets the MODE1 register to turn on auto increment.
  write8(PCA9685_MODE1, oldmode | MODE1_RESTART | MODE1_AI, MQT);
}

void SetOscillatorFrequency(uint32_t freq, mqt_t * MQT) {
  MQT->oscillatorFreq = freq;
}

// set up communications with PWM chip 
void initPWM(mqt_t * MQT) {
  #if ARDUINO_CODE
    Wire.begin();
  #endif
  resetPWM(MQT);

  // set a default frequency
  SetPWMFreq(1000, MQT);
  
  // set the default internal frequency
  SetOscillatorFrequency(FREQUENCY_OSCILLATOR, MQT);
}

// write 4 bytes 
void writeRegs(uint8_t reg, uint8_t values[4], mqt_t * MQT) {
  #if ARDUINO_CODE
    Wire.beginTransmission(PWM_ADDR);
    Wire.write(reg);
    for (int i=0; i<4; i++) {
      Wire.write(values[i]);
    }
    Wire.endTransmission();
  #else
    I2C_send(MQT->mqtHandle, PWM_ADDR, reg, &values, 4);
  #endif
}

// send setpoint commands to PWM chip
void setMoments(float rqt_x, float rqt_y, float rqt_z, mqt_t * MQT)
{
  MQT->requestedMoment[0] = rqt_x;
  MQT->requestedMoment[1] = rqt_y;
  MQT->requestedMoment[2] = rqt_z;
  
  MQT->val_XYZ[0] = 4095*(1 - fabs(rqt_x)/(MQT->maxMomentXY));
  MQT->val_XYZ[1] = 4095*(1 - fabs(rqt_y)/(MQT->maxMomentXY));
  MQT->val_XYZ[2] = 4095*(1 - fabs(rqt_z)/(MQT->maxMomentZ));
  
  uint8_t Xdata[4] = {lowByte(MQT->val_XYZ[0]), (MQT->val_XYZ[0] >> 8), 0x0, 0x0};
  uint8_t Ydata[4] = {lowByte(MQT->val_XYZ[1]), (MQT->val_XYZ[1] >> 8), 0x0, 0x0};
  uint8_t Zdata[4] = {lowByte(MQT->val_XYZ[2]), (MQT->val_XYZ[2] >> 8), 0x0, 0x0};
  uint8_t offData[4] = {0x0, 0x0, 0x0, 0x10};
  
  if (rqt_x > 0) {
    writeRegs(X1_POS_ADDR, Xdata, MQT);
    writeRegs(X1_NEG_ADDR, offData, MQT);
    writeRegs(X2_POS_ADDR, offData, MQT);
    writeRegs(X2_NEG_ADDR, Xdata, MQT);   
  }
  else if (rqt_x < 0) {
    writeRegs(X1_POS_ADDR, offData, MQT);
    writeRegs(X1_NEG_ADDR, Xdata, MQT);
    writeRegs(X2_POS_ADDR, Xdata, MQT);
    writeRegs(X2_NEG_ADDR, offData, MQT);
  }
  else {
    writeRegs(X1_POS_ADDR, offData, MQT);
    writeRegs(X1_NEG_ADDR, offData, MQT);
    writeRegs(X2_POS_ADDR, offData, MQT);
    writeRegs(X2_NEG_ADDR, offData, MQT);
  }

  if (rqt_y > 0) {
    writeRegs(Y1_POS_ADDR, Ydata, MQT);
    writeRegs(Y1_NEG_ADDR, offData, MQT);
    writeRegs(Y2_POS_ADDR, offData, MQT);
    writeRegs(Y2_NEG_ADDR, Ydata, MQT);   
  }
  else if (rqt_y < 0) {
    writeRegs(Y1_POS_ADDR, offData, MQT);
    writeRegs(Y1_NEG_ADDR, Ydata, MQT);
    writeRegs(Y2_POS_ADDR, Ydata, MQT);
    writeRegs(Y2_NEG_ADDR, offData, MQT);
  }
  else {
    writeRegs(Y1_POS_ADDR, offData, MQT);
    writeRegs(Y1_NEG_ADDR, offData, MQT);
    writeRegs(Y2_POS_ADDR, offData, MQT);
    writeRegs(Y2_NEG_ADDR, offData, MQT);
  }
  
  if (rqt_z > 0) {
    writeRegs(Z_POS_ADDR, Zdata, MQT);
    writeRegs(Z_NEG_ADDR, offData, MQT);  
  }
  else if (rqt_z < 0) {
    writeRegs(Z_POS_ADDR, offData, MQT);
    writeRegs(Z_NEG_ADDR, Zdata, MQT);
  }
  else {
    writeRegs(Z_POS_ADDR, offData, MQT);
    writeRegs(Z_NEG_ADDR, offData, MQT);
  }
  
}
