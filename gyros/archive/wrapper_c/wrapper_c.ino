#include <Wire.h>
#include "GYR.h"

#define PRINT_INFO                    0
#define DEBUG                         0
#define CALIBRATE                     0


// test var
long intt;
int i = 0;
double testtime = 2000;
boolean testing = 0;

double xBuff[50], yBuff[50], zBuff[50];
//double xLast, yLast, zLast;
double xBiasCum, yBiasCum, zBiasCum;

void setup() {
  xBiasCum = 0;
  yBiasCum = 0;
  zBiasCum = 0;
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
  intt = micros();

  i = i + 1;
  // Query the sensor
  readGyroData(&gyroX, &gyroY, &gyroZ, &tempData);

#if DEBUG
  //  if (i <= 50){
  //    xBuff[(i-1)%50] = gyroX;
  //    yBuff[(i-1)%50] = gyroY;
  //    zBuff[(i-1)%50] = gyroZ;
  //    if (i=50){
  //      for(int j =0; j<50; j++){
  //      xBiasCum += xBuff[j];
  //      yBiasCum += yBuff[j];
  //      zBiasCum += zBuff[j];
  //      }
  //      xBiasCum /= 50;
  //      yBiasCum /= 50;
  //      zBiasCum /= 50;
  //    }
  //  } else {
  //
  //    xBiasCum = (xBiasCum*50+gyroX-xBuff[(i-1)%50])/50;
  //    yBiasCum = (yBiasCum*50+gyroY-yBuff[(i-1)%50])/50;
  //    zBiasCum = (zBiasCum*50+gyroZ-zBuff[(i-1)%50])/50;
  //
  //    xBuff[(i-1)%50] = gyroX;
  //    yBuff[(i-1)%50] = gyroY;
  //    zBuff[(i-1)%50] = gyroZ;
  // }
      xBiasCum = (xBiasCum*50+gyroX-xBuff[(i-1)%50])/50;
      yBiasCum = (yBiasCum*50+gyroY-yBuff[(i-1)%50])/50;
      zBiasCum = (zBiasCum*50+gyroZ-zBuff[(i-1)%50])/50;
  
      xBuff[(i-1)%50] = gyroX;
      yBuff[(i-1)%50] = gyroY;
      zBuff[(i-1)%50] = gyroZ;
#endif

  Serial.print(" ");
  Serial.print(gyroX, 7);
  Serial.print(" ");
  Serial.print(gyroY, 7);
  Serial.print(" ");
  Serial.println(gyroZ, 7);

#if DEBUG
  Serial.print(" ");
  Serial.print(xBiasCum, 7);
  Serial.print(" ");
  Serial.print(yBiasCum, 7);
  Serial.print(" ");
  Serial.println(zBiasCum, 7);
#endif

#if PRINT_INFO
  Serial.print(" ");
  Serial.println(tempData);
#endif
  while (micros() - intt < 1.0 / ODR_VALUE * 1e6) {
  }
}
