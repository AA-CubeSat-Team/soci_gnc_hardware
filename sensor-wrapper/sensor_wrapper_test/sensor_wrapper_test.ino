
#include "gyro_wrap.h"
#include "mag_wrap.h"
#include "sun_wrap.h"

#include <Wire.h>

void setup(){
  Serial.begin(115200);

  // init functions
  // start functions
}

void loop(){
  readGyroData(&Gyro1);
  readGyroData(&Gyro2);
  readGyroData(&Gyro3);

  readMagData(&Mag1);
  readMagData(&Mag2);
  readMagData(&Mag3);

  getSunAngles(double* angles);
}
