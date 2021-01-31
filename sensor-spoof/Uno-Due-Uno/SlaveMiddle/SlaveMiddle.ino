// Slave Code Arduino Due
// Takes int from MasterLeft and sends it to MasterRight

#include <Wire.h>
#define SLAVE_ADDR 4
#define ANSWERSIZE num

int num;

extern TwoWire Wire1; // required for i2c 

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
  Wire1.write(num);
}

// This method takes bytes from the Master Left
// Should print integer from MasterLeft
void receiveEvent(int numBytes) {
  num = Wire.read();
}

void loop() {
}
