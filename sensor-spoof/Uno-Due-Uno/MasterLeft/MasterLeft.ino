// Master Code Arduino Uno
// Sends 5 to Arduino Due
#include <Wire.h>
#define SLAVE_ADDR 4
#define ANSWERSIZE 5

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
}

void loop()
{
  delay(100);
  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(5);
  Wire.endTransmission();
}
