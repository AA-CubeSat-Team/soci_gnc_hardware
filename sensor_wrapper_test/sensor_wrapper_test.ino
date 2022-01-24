#include "mag_wrap.h"
#include "gyro_wrap.h"
#include <Wire.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("begin");
  initMag(&Mag1);
  startMag(&Mag1);
  initGyro(&Gyro1);
  startGyro(&Gyro1);
}

void loop() {
  double t0 = micros();

  Serial.print("Read Mag");
  readMagData(&Mag1);
  Serial.print("Mag: ");
  for (int i = 0; i < 3; i++) {
    Serial.print(" ");
    Serial.print(Mag1.magXYZ[i],7);
  }
  Serial.println();

//  Serial.print("Read Gyro");
//  readGyroData(&Gyro1);
//  Serial.print("Gyro: ");
//  for (int i = 0; i < 3; i++) {
//    Serial.print(" ");
//    Serial.print(Gyro1.gyroXYZ[i],7);
//  }
//  Serial.println();
  while (micros() - t0 < 1.0 / 25 * 1e6) {
  }
}
