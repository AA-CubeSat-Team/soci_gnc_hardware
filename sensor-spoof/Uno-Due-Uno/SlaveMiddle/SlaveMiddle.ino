// Slave Code Arduino Due
// Takes 5 from MasterLeft and sends it to MasterRight

#include <Wire.h>
#define SLAVE_ADDR 4
#define ANSWERSIZE 5

String answer = "";

//byte answer;

extern TwoWire Wire1;

void setup() {
  // SDA1 and SCL1
  Wire1.begin(SLAVE_ADDR); //Runs in SLAVE mode
  Wire1.onRequest(requestEvent); 

  // SDA and SCL
  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}

// This method sends back 5 to MasterRight on request
void requestEvent(){ // from Arduino Master 2
  
//  Wire1.write(answer);
}

// This method takes bytes from the Master Left
// Should print 1
void receiveEvent(int numBytes) {
  while(Wire.available()) {
    char b = Wire.read();
    answer += b;
  }
  
  Serial.println(answer);
}

void loop() {

}
