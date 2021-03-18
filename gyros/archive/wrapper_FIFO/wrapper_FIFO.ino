#include <Wire.h>
#include "GYR.h"

#define PRINT_INFO                    1
#define DEBUG                         0
#define CALIBRATE                     1


// test var
long intt;
int i = 0;
double testtime = 2000;
boolean testing = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  setConfigures();
#if PRINT_INFO
  Serial.println("Gyroscope FXAS2100c");
  Serial.print("Output data rate: ");
  Serial.print(ODR_VALUE);
  Serial.println(" Hz");
  Serial.print("Full scale range: ");
  Serial.print(FSR_VALUE);
  Serial.println(" Degree/s");
#endif
  active();
  delay(500);
  readTempData(&tempData);
  tempData0 = tempData;
  ready();
#if PRINT_INFO
  Serial.print("Temperature: ");
  Serial.print(tempData);
  Serial.println(" Degree");
#endif

#if CALIBRATE
#if PRINT_INFO
  Serial.println("Calibrating...");
#endif
  calibrate(&gBiasX, &gBiasY, &gBiasZ);
#if PRINT_INFO
  Serial.println("Calibration finished");
#endif
#endif

  active();
intt = micros();
}

void loop() {
  if (i == floor(testtime * ODR_VALUE)) {
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

  i = i + 1;
  // Query the sensor
  readGyroData(gyroX, gyroY, gyroZ, &tempData);

  for (int i = 0; i<5; i++){
    Serial.print(" ");
    Serial.print(gyroX[i], 7);
    Serial.print(" ");
    Serial.print(gyroY[i], 7);
    Serial.print(" ");
    Serial.println(gyroZ[i], 7);
  }

#if PRINT_INFO
  Serial.print(" ");
  Serial.println(tempData);
#endif
  while (micros() - intt < 1.0 / ODR_VALUE*5 * 1e6) {
  }
intt = micros();
}
