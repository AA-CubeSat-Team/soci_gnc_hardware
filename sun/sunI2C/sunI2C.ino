#include <Wire.h>
#define SLAVE_ADDR 0x60 // Dependent on sensor
extern TwoWire Wire1;

byte Data[8]; // Global Variable
unsigned char error = 0;
byte Command[4];
byte Length = 0x0A;
int i;

void setup() {
  // SDA1 and SCL1
  Wire1.begin(SLAVE_ADDR); //Runs in SLAVE mode
  Wire1.onReceive(receiveEventSimulink); 
  Serial.begin(115200);
}

// This method is called when Simulink is sending data to the sensor.
// It stores the data in a global variable.
void receiveEventSimulink(int numBytes) {
  while (Wire1.available()) {
    for(i=0; i<numBytes; i++){
    Data[i] = Wire1.read();
    }
    for(i=0; i<sizeof(Data); i++){
    Serial.println(Data[i]);
    }
  }
}

// RECIEVE REQUEST FROM OBC AND RESPOND WITH DATA
void requestEventOBC() {
  // Read incoming command packet and populate Command array when serial data is available
    if(Serial.available()) {
    // Wait for entire command package to enter buffer
      delay(10);
    // Incoming address
      Command[0] = Serial.read();
    // Check for correct address before proceeding 
      if (Command[0] == 0x60) {
    // Incoming command
      Command[1] = Serial.read();
    // Incoming length
      Command[2] = Serial.read();
    // Incoming checksum
      Command[3] = Serial.read();
      
// RESPOND TO OBC REQUEST
  // Angular Position
    // Next two lines calculate the checksum by first adding the data sheet specified values, then extracting the least significant byte
    byte Checksum = byte(Command[1] + Length + Data[0] + Data[1] + Data[2] + Data[3] + Data[4] + Data[5] + Data[6] + Data[7] + error);
    Checksum = Checksum & 0xFF;
    // Populate Answer packet
    byte Answer[] = {SLAVE_ADDR,Command[1],Length,Data[0],Data[1],Data[2],Data[3],Data[4],Data[5],Data[6],Data[7],error,Checksum};
    // Write Answer packet to OBC
    Serial.write(Answer,13);
  }
 }
}

void loop() {
  delay(100);
}
