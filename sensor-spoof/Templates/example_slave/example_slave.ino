#include <Wire.h>
#define SLAVE_ADDR 4
#define ANSWERSIZE 2
extern TwoWire Wire1;

String answer = "Wo";
void setup() {
  // put your setup code here, to run once:
  Wire1.begin(); //Runs in SLAVE mode
  Wire1.onRequest(requestEvent); 
  Wire1.onReceive(receiveEvent);

  Serial.begin(9600);
  Serial.println("I2C Test");

}

// This method takes bytes from the master
void receiveEvent(int numBytes) { // from Arduino Master 1
  String response = "";
  while(Wire1.available()) {
    char b = Wire1.read();
    response += b;
  }
    Serial.println(response);
    Serial.println(numBytes);
}

// This method sends back information to the master on request
void requestEvent(){ // from Arduino Master 2
  byte response[ANSWERSIZE];
  for(byte i=0;i<ANSWERSIZE;i++) {
    response[i] = (byte)answer.charAt(i);
  }
  Wire1.write(response,sizeof(response));
}

void loop() {

}
