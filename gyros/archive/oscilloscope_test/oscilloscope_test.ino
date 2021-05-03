#include "gyro_wrap.h"
#include <Wire.h>

uint8_t readValue;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Tests begin");
  Serial.println("********************");
  initGyro(&Gyro1);
  Serial.println("finish init");
}

void loop() {
  Serial.println("reading...");
  readRegs(0x0C, &readValue, 1, &Gyro1);
  Serial.println(readValue, HEX);
  delay(500);
}
