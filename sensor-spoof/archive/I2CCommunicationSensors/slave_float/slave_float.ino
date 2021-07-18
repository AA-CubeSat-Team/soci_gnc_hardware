#include <Wire.h>
#define SLAVE_ADDRESS 3
volatile float value;
const byte dataCount = 8;

void setup()
{
 Wire.begin(SLAVE_ADDRESS);
 Wire.onRequest(requestEvent); // register event
 Wire.onReceive(receiveEvent); 
 Serial.begin(9600);
}

void loop()
{

}

void requestEvent()
{
Wire.write("3.20");

}
void receiveEvent() {
  byte response[dataCount];
   while (Wire.available()) {
    for(byte i = 0; i < dataCount; i++) {
    response[i] = Wire.read();
}
String myString = String((char *)response);
    float y = myString.toFloat();
    Serial.println(y); 
  }
}
