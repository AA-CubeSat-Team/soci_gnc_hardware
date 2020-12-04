// Master Code Arduino Uno

#include <Wire.h>
#define SLAVE_ADDR 4
#define ANSWERSIZE 2

String sentence = "abcdefghijklmnopqrstuvwxyz";
void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
}

byte x = 0;

void loop()
{
  delay(100);
  Wire.beginTransmission(SLAVE_ADDR);
  char test[sentence.length()];
  sentence.toCharArray(test, sentence.length());

  int size = sentence.length();
  for (int i = 0; i < size; i++) {
    Wire.write(test[i]);  // the data that is being sent
  }
  
  Wire.endTransmission();

  Wire.requestFrom(SLAVE_ADDR,ANSWERSIZE);
  String response = "";
  while(Wire.available()) {
    char b = Wire.read();
    response += b;
  }
  Serial.println(response);
}
