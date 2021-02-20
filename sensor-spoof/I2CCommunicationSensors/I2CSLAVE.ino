#include <Wire.h>
#define SLAVE_ADDR 9
#define ANSWERSIZE 5

String answer = "Hello";
void setup() {
  // put your setup code here, to run once:
  Wire.begin(SLAVE_ADDR); //Runs in SLAVE mode
  Wire.onRequest(requestEvent); 
  Wire.onReceive(receiveEvent);

   Serial.begin(9600);
   Serial.println("I2C Test");
}

void receiveEvent() {
    while (Wire.available()) {
    byte x = Wire.read();
  }   
}

void requestEvent(){
  byte response[ANSWERSIZE];

  for(byte i=0;i<ANSWERSIZE;i++) {
    response[i] = (byte)answer.charAt(i);
}

  Wire.write(response,sizeof(response));
}
void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
}
