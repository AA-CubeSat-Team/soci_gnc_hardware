#include <Wire.h>
#define SLAVE_ADDR 9
#define ANSWERSIZE 5

void setup() {
  Wire.begin();

  Serial.begin(9600);
}

void loop(){
  delay(100);

  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write("Hi");
  Wire.endTransmission();


  Wire.requestFrom(SLAVE_ADDR,ANSWERSIZE);
  String response = "";
  while(Wire.available()) {
    char b = Wire.read();
    response += b;
  }
  Serial.println(response);
}
