
#include "gyro_wrap.h"
#include "mag_wrap.h"
#include "sun_wrap.h"

#include <Wire.h>

void setup(){
  Serial.begin(115200);
  Serial1.begin(115200);

  // init functions
  // start functions
}

void loop(){
//  readGyroData(&Gyro1);
//  readGyroData(&Gyro2);
//  readGyroData(&Gyro3);
//
//  readMagData(&Mag1);
//  readMagData(&Mag2);
//  readMagData(&Mag3);

  getSunAngles(&Sun1);
  Serial.print("angle1  ");
  Serial.print(Sun1.angles[0]);
  Serial.print("  ");
  Serial.print("angle2  ");
  Serial.print(Sun1.angles[1]);
  Serial.print("  ");
  Serial.print("detection  ");
  Serial.print(Sun1.angles[2]);
  Serial.print("  ");
  Serial.print("error  ");
  Serial.println(Sun1.angles[3]);
}
