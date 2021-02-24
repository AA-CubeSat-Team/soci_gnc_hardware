// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
}



void loop()
{
  float y;
  int j = 1;
  while (j) {
    while (Serial.available()){
      y = Serial.parseFloat(SKIP_ALL,'\n');
      Serial.println(y,6);
      j = 0;
    }
  }

  char data[sizeof(float)];
  memcpy(data, &y, sizeof y);
  
  Wire.beginTransmission(4);
  Wire.write(data);        
  Wire.endTransmission();    
  delay(500);
}
