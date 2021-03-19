// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.

#include <Wire.h>
#define ADDRESS                       (uint8_t) 0x20
#define GYRO_FSR_VALUE        250
#define GYRO_ODR_VALUE        12.5
#define DOUBLE                        float
DOUBLE gyroSimRead[3];
char data[sizeof(gyroSimRead)];
DOUBLE time0, time1;
void setup()
{
  Serial.begin(115200);
  Serial.println("Size of gyroSimRead");
  Serial.println(sizeof(gyroSimRead));
  Serial.println("--------------------------------");
  Wire.begin();
  time0 = micros();
  for (int i = 0; i < 3; i++)
  {
    gyroSimRead[i] = GYRO_FSR_VALUE*sin(PI/3*i);
  }
}



void loop()
{
  time1 = micros();
  gyroSimUpdate();
  memcpy(data, &gyroSimRead, 3*sizeof(DOUBLE));  
  Wire.beginTransmission(ADDRESS);
  Serial.println(Wire.write(data, 3*sizeof(DOUBLE)));    
  Wire.endTransmission(); 
  while (micros() - time1 < 1.0 / GYRO_ODR_VALUE * 1e6) {
  }
}

void gyroSimUpdate()
{
  for (int i = 0; i < 3; i++)
  {
    gyroSimRead[i] = GYRO_FSR_VALUE*sin((micros() - time0)*1e-6 + PI/3*i);
//    gyroSimRead[i]= PI;
    Serial.println(gyroSimRead[i],7);
  }
}
