#include <Wire.h>
#include <FXAS21002C.h>

FXAS21002C sensor = FXAS21002C(GFSR_250DPS, GODR_50HZ, GBW_L2); // SA0=1 0x21
long waitt = 0;
long intt;
long starttime;
long i = 0;
long ODR;

void setup() {
  Serial.begin(115200);
  Wire.begin();
    // Initialize the FXAS21002C
  ODR = sensor.getODR();
  sensor.setConfigures();
  sensor.calibrate();
  sensor.active();
  intt = micros();
}

void loop() {

  i=i+1;

  // Query the sensor
  sensor.readGyroData();
//  sensor.readTempData();

  // Print out the data
  // Gyroscope
//  Serial.println(sensor.readReg(FXAS21002C_H_CTRL_REG0),BIN);
//  Serial.print("X:");
//  Serial.print((float)sensor.gyroData.x);
//  Serial.print(" Y:");
//  Serial.print((float)sensor.gyroData.y);
//  Serial.print(" Z:");
  Serial.println((float)sensor.gyroData.z);

  waitt = waitt + 1.0/(ODR)*1e6;
  while(micros()-intt<waitt){
  }
if (i == 20*ODR+60){
  while(1){}
}
}
