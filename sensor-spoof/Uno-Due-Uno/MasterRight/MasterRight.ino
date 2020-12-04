// Master Code Arduino Uno
// Should output 5 from Master Left 
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
  Wire.requestFrom(SLAVE_ADDR,1);
  byte response; 
  while(Wire.available()) {
    response = Wire.read();
  }
  Serial.print("Expected: 5; Actual: ");
  Serial.println(response);
}
