#include <Wire.h>
#define SLAVE_ADDRESS 3
byte data[8];
volatile float value;
const byte dataCount = 8;

void setup() {
  Wire.begin();
  Serial.begin(9600);
}
void loop()
{

  Wire.requestFrom(SLAVE_ADDRESS, dataCount);

  int i = 0;
  while (Wire.available()) 
  {
    data[i] = Wire.read();
    i++;
  }

  String myString = String((char *)data);
    float y = myString.toFloat();
    Serial.println(y); 


   Wire.beginTransmission (SLAVE_ADDRESS); 
   Wire.write("2.20");
   Wire.endTransmission();
}
