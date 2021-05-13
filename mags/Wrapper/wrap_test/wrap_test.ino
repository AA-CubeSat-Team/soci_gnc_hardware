#include "mag_wrap.h"
#include <Wire.h>

#define TEST_READMAGDATA true
#define TEST_READTEMPDATA false

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  if (TEST_READMAGDATA || TEST_READTEMPDATA) {
    initMag(&Mag1);
  }
}

void loop() {
  double t0 = micros();
  if (TEST_READTEMPDATA) {
    readMagTemp(&Mag1);
    Serial.println(Mag1.temperature);
  }
  if (TEST_READMAGDATA) {
    readMagData(&Mag1);
    for (int i = 0; i < 3; i++) {
      Serial.print(" ");
      Serial.print(Mag1.magXYZ[i],7);
    }
    Serial.println();
  }
  while (micros() - t0 < 1.0 / 25 * 1e6) {
  }
}
