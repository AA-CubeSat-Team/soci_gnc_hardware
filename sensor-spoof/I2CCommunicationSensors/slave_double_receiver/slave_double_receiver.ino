#include <Wire.h>
#define SLAVE_ADDRESS 3
char t[10]; //empty array where to put the numbers going to the master
volatile float Val; // variable used by the master to sent data to the slave

void setup() {
  Wire.begin(SLAVE_ADDRESS);  
  Wire.onRequest(requestEvent); 
  Wire.onReceive(receiveEvent); 
  Serial.begin(9600);
}

void loop() {
//  int aRead = analogRead(A0); //plug a potentiometer or a resistor to pin A0, so you can see data being transfer
//  float x = aRead/1024.0*5.0; //generate a float number, with this method you can use any time of data pretty much 

  ftostrf(x, 3, 2, t); //convers the float or integer to a string. (floatVar, minStringWidthIncDecimalPoint, numVarsAfterDecimal, empty array);
  Serial.println(Val); 
 delay(500);
}

// function: what to do when asked for data
void requestEvent() {
Wire.write(t); 
}

// what to do when receiving data from master
void receiveEvent(int howMany) {
   Val = Wire.read();
  }
