// Master Code Arduino Uno Right
// Should output value from Master Left 
// 2/16/21
#include <Wire.h>
#define SLAVE_ADDR 4

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
}

void loop()
{
  delay(1000);
  Wire.requestFrom(SLAVE_ADDR,1); //pulls data from available wire from slave
  int response;                   //integer passed from masterleft
  while(Wire.available()) {
    response = Wire.read();
  }
  Serial.print("Value from MasterLeft: ");
  Serial.println(response);
}
