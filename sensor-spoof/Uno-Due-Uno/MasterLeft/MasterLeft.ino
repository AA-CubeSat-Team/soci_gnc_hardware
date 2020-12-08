// Master Code Arduino Uno
// Sends 5 to Arduino Due
#include <Wire.h>
#define SLAVE_ADDR 4
#define ANSWERSIZE 5

char test[] = "123456789123456789123456789\0";
void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
}

void loop()
{
  delay(100);
//  char test[sentence.length() + 1];
//  sentence.toCharArray(test, sentence.length() + 1);
  int size = sizeof(test);
  for (int i = 0; i < size; i++) {
    if (i % 32 == 0) {
      Wire.beginTransmission(SLAVE_ADDR);
    }
    Wire.write(test[i]);
    if (i % 32 == 31 || i == size - 1) {      
      Wire.endTransmission();
    }
  } 
}
//void loop()
//{
//  delay(100);
//  Wire.beginTransmission(SLAVE_ADDR);
//  Wire.write(5);
//  Wire.endTransmission();
//}
