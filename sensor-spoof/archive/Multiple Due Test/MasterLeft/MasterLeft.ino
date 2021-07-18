// Master Code Arduino Uno Left
// Sends values to middle slaves
#include <Wire.h>
#define SLAVE_ADDR1 4
#define SLAVE_ADDR2 5
int slave1 = 2;
int slave2 = 4;

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
}

void loop()
{
  delay(100);
  //opens communication with Slave1
  Wire.beginTransmission(SLAVE_ADDR1); 
  Wire.write(slave1);
  Wire.endTransmission();
  
  delay(100);
  //opens communication with Slave2
  Wire.beginTransmission(SLAVE_ADDR2); 
  Wire.write(slave2);
  Wire.endTransmission();
}
