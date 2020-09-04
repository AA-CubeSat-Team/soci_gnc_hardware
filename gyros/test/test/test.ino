#include <Wire.h>
#include <FXAS21002C.h>

FXAS21002C sensor = FXAS21002C(GFSR_2000DPS, GODR_200HZ, GBW_L3); // SA0=1 0x21
long waitt = 0;
long intt;
long starttime;
long i = 0;
long ODR;
int testtime;

void setup() {
  Serial.begin(115200);
  Wire.begin();
    // Initialize the FXAS21002C
  ODR = sensor.getODR();
  sensor.setConfigures();
  sensor.calibrate();
  sensor.active();
  intt = micros();
  testtime = 180;
}

void loop() {

  i=i+1;

  // Query the sensor
  sensor.readGyroData();
  sensor.readTempData();
//  Serial.println(sensor.tempData);
  // Print out the data
  // Gyroscope
//  Serial.print("X:");
//  Serial.print((float)sensor.gyroData.x);
//  Serial.print(" Y:");
//  Serial.print((float)sensor.gyroData.y);
//  Serial.print(" Z:");
  Serial.println((float)sensor.gyroData.z);

  waitt = waitt + 1.0/(ODR)*1e6;
  while(micros()-intt<waitt){
  }
if (i == testtime*ODR+60){
  int j = 1;
  while(j){
    if(Serial.available())  {
  String value = Serial.readStringUntil('\n');
  Serial.println(value);
  if(value=="reset") {
    i = 0;
    waitt = 0;
    intt = micros();
    j = 0;
  }
    }
  }
}
}
