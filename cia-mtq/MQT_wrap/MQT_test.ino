#include "MQT_wrap.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Wire.h>

void setup() {
  // Serial.begin(9600);
  // Wire.begin();
  initPWM(&MQT);
}

void loop() {
  // input requested moments
  float m_x = .2;
  float m_y = .1;
  float m_z = .025;

  setMoments(m_x, m_y, m_z, &MQT);
  resetPWM(&MQT);
  
  delay(1000);
}
