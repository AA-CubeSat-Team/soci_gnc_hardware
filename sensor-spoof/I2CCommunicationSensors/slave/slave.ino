#include <Wire.h>
#define SLAVE_ADDR 9
#define ANSWERSIZE 5

void setup() {
  // put your setup code here, to run once:
  Wire.begin(SLAVE_ADDR); //Runs in SLAVE mode
  Wire.onReceive(receiveEvent);

  Serial.begin(9600);
  Serial.println("I2C Test");
}

// This method takes bytes from the master
void receiveEvent() { // from Arduino Master 1
  String answer = "";
  while(Wire.available()) {
    int b = (int) Wire.read();
    answer += b;
    
  }
  Serial.println(response); 
}

// This method sends back information to the master on request
//void requestEvent(){ // from Arduino Master 2
//  byte response[ANSWERSIZE];
//  for(byte i=0;i<ANSWERSIZE;i++) {
//    response[i] = (byte)answer.charAt(i);
//  }
//  Wire.write(response,sizeof(response));
//}
//
//void loop() {
//
//}
