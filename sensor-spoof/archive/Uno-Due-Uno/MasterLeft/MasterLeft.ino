// Master Code Arduino Uno Left
// Sends value to Arduino Due (the middle slave)

#include <Wire.h>
#define SLAVE_ADDR 4 

int num = 10;
void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
}

void loop()
{
  delay(100);
  //opens communication with middle slave
  Wire.beginTransmission(SLAVE_ADDR); 
  Wire.write(num);
  Wire.endTransmission();
}
