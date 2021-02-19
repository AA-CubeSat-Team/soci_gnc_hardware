// Slave Code Arduino Due
// Outline Structure for Sensor Spoof
// Takes data from MasterLeft(Simulink) and sends it to MasterRight(OBC)

#include <Wire.h>
#define SLAVE_ADDR 4 // Dependent on sensor

int num;
int val = 5;
extern TwoWire Wire1; // required for i2c 

void setup() {
  // SDA1 and SCL1
  Wire1.begin(SLAVE_ADDR); //Runs in SLAVE mode
  Wire1.onRequest(requestEvent); 
  Wire1.onReceive(receiveEvent);

  // SDA and SCL
  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  
  Serial.begin(9600);
}

// This method sends back 5 to MasterRight on request
void requestEvent(){ // from Arduino Master 2
  Wire1.write(num);
  Wire.write(val);
}

// This method is called when Simulink is sending data to the sensor.
// It stores the data in a global variable.
void receiveEvent(int numBytes) {
  num = Wire.read();
  val = Wire1.read();
}

void loop() {
}
