#include <Wire.h>
#include "gyro_wrap.h"

long intt;
int i = 0;
double testtime = 2000;
boolean testing = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Gyroscope characterization test.");
  Serial.print("Test time:  ");
  Serial.print(testtime);
  Serial.println("s");
  quickStartGyro(&Gyro1);
  delay(500);
  Serial.println("Ready for testing. Type \"start\" to start...");
}

void loop() {
  if (i == floor(testtime * GYRO_ODR_VALUE)) {
    testing = 0;
  }  

  if (!(testing)) {
    int j = 1;
    while (j) {
      if (Serial.available())  {
        String value = Serial.readStringUntil('\n');
        Serial.println(value);
        if (value == "start") {
          i = 0;
          testtime = testtime;
          j = 0;
          testing = 1;
          delay(2000);
        }
      }
    }
  }

  intt = micros();  

  readGyroData(&Gyro1);  

  for (int i = 0; i<3; i++){
    Serial.print(" ");
    Serial.print(Gyro1.gyroXYZ[i], 7);
  }
  Serial.println("");

  while (micros() - intt < 1.0 / GYRO_ODR_VALUE * 1e6) {
  }
}
