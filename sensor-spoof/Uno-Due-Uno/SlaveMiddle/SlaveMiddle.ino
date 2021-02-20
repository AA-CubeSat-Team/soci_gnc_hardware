// Slave Code Arduino Due
// Outline Structure for Sensor Spoof
// Takes data from MasterLeft(Simulink) and sends it to MasterRight(OBC)

#include <Wire.h>
#define SLAVE_ADDR 4 // Dependent on sensor

int num; // Global Variable

extern TwoWire Wire1; // WireRequest - required for SDA1/SCL1 port
                      // Wire        - used for regular i2c port

void setup() {
  // SDA1 and SCL1
  Wire1.begin(SLAVE_ADDR); //Runs in SLAVE mode
  Wire1.onRequest(requestEvent); 

  // SDA and SCL
  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(receiveEvent);
  
  Serial.begin(9600);
}

// This method is called when OBC requests data from the sensor.
// It sends the value held in the global variable.
void requestEvent(){ 
   Wire1.write(num);
}

// This method is called when Simulink is sending data to the sensor.
// It stores the data in a global variable.
void receiveEvent(int numBytes) {
  while (Wire.available()) {
    num = Wire.read();
    Serial.println(num);
  }
}

void loop() {
}
