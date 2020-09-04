#include<SoftwareSerial.h>
void setup() {
Serial.begin(115200);
pinMode(13,OUTPUT);
Serial.println("Hello");
}

void loop() {
  // put your main code here, to run repeatedly:
if(Serial.available())  {
  String value = Serial.readStringUntil('\n');
  Serial.println(value);
  if(value=="On") {
    digitalWrite(13,HIGH);
  }
  if(value=="Off")  {
    digitalWrite(13,LOW);
  }
}
}
