// Master Code Arduino Uno Right
// Should output value from Master Left 
#include <Wire.h>
#define SLAVE_ADDR 4
#define ANSWERSIZE num

int val = 5;

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
}

void loop()
{
  delay(100);
  Wire.requestFrom(SLAVE_ADDR,1); //pulls data from available wire from slave
  int response; //integer passed from masterleft
  while(Wire.available()) {
    response = Wire.read();
  }
  Serial.print("Value from MasterLeft: ");
  Serial.println(response);
//}
//
//void loop()
//{
  delay(100);
  //opens communication with middle slave
  Wire.beginTransmission(SLAVE_ADDR); 
  Wire.write(val);
  Wire.endTransmission();
}
