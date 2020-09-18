#include <Wire.h>
#include <FXAS21002C.h>

FXAS21002C sensor = FXAS21002C(GFSR_250DPS, GODR_200HZ, GBW_L1); // SA0=1 0x21
double waitt = 0;
long intt;
double i = 0;
double ODR;
double testtime = 20000;
boolean testing = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  ODR = sensor.getODR();
  Serial.println("Gyroscope FXAS2100c");
  Serial.print("Output data rate: ");
  Serial.print(ODR);
  Serial.println(" Hz");
  sensor.setConfigures();
  sensor.active();
  delay(500);
  sensor.readTempData();
  Serial.print("Temperature: ");
  Serial.print(sensor.tempData);
  Serial.println(" Degree");
}

void loop() {
  
  if (i == floor(testtime*ODR)){
    testing = 0;
  }
  
  if (!(testing)){
  int j = 1;
  while(j){
    if(Serial.available())  {
      String value = Serial.readStringUntil('\n');
      Serial.println(value);
      if(value=="start") {
          i = 0;
          waitt = 0;
          testtime = testtime;
          j = 0;
          testing = 1;
          delay(2000);
          intt = micros();
      }
    }
  }
}

  i=i+1;

  // Query the sensor
  sensor.readGyroData();
  Serial.print(" ");
  Serial.print((float)sensor.gyroData.x,7);
  Serial.print(" ");
  Serial.print((float)sensor.gyroData.y,7);
  Serial.print(" ");
  Serial.println((float)sensor.gyroData.z,7);

  waitt = waitt + 1.0/(ODR)*1e6;
  while(micros()-intt<waitt){
  }


}
