// Master Code Arduino Uno Right
// Should output value from Master Left 
#include <Wire.h>
#define SLAVE_ADDR1 4
#define SLAVE_ADDR2 5
#define ANSWERSIZE num

void setup()
{
  //Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
}

void loop()
{
  int response; //integer passed from masterleft
  Wire.begin();
  delay(1000);
  Wire.requestFrom(SLAVE_ADDR1, 1); //pulls data from available wire from slave
  response = Wire.read();
  Serial.print("Value from Slave 1: ");
  Serial.println(response);
  Wire.endTransmission();
  
  delay(1000);
  Wire.begin();
  Wire.requestFrom(SLAVE_ADDR2, 1); //pulls data from available wire from slave
  response = Wire.read();
  Serial.print("Value from Slave 2: ");
  Serial.println(response);
  Wire.endTransmission();

}
