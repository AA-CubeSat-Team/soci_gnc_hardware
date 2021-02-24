#include <Wire.h>
#include "gyro_wrap.h"
#define PRINT_INFO                    1

long intt;
int i = 0;
double testtime = 2000;
boolean testing = 0;

void setup() {
  Serial.begin(115200);
#if PRINT_INFO
  Serial.println("Gyroscope FXAS2100c");
  Serial.print("Output data rate: ");
  Serial.print(GYRO_ODR_VALUE);
  Serial.println(" Hz");
  Serial.print("Full scale range: ");
  Serial.print(GYRO_FSR_VALUE);
  Serial.println(" Degree/s");
#endif
  startGyro(&Gyro1);
  delay(500);
  readTempData(&Gyro1);
#if PRINT_INFO
  Serial.print("Temperature: ");
  Serial.print(Gyro1.temperature);
  Serial.println(" Degree");
#endif
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

  i = i + 1;
  // Query the sensor
  readGyroData(&Gyro1);

  for (int i = 0; i<3; i++){
    Serial.print(" ");
    Serial.print(Gyro1.gyroXYZ[i], 7);
  }
  Serial.println("");

#if PRINT_INFO
  Serial.print(" ");
  Serial.println(Gyro1.temperature);
#endif
  while (micros() - intt < 1.0 / GYRO_ODR_VALUE * 1e6) {
  }
}
