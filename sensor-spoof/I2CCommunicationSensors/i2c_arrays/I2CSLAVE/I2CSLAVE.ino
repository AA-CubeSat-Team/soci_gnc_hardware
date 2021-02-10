#include <Wire.h>
#define SLAVE_ADDR 4
#define ANSWERSIZE 5

char t[10]; //empty array where to put the data going to the master
int pulse[12]={245,255,265,275,285,295,305,315,325,335,345,355}; //Store random data to send to Master

volatile int Address = 10; // variable used by the master to sent data to the slave

void setup() {
  Wire.begin(SLAVE_ADDR);           //slave address
  Wire.onRequest(requestEvent); //Create an Interrup when Master request data
  Wire.onReceive(receiveEvent); //Create an Interrup when receiving data from the Master
  Serial.begin(9600); 
 

}

void loop() {
  delay(500);
}


void receiveEvent()
{Address = Wire.read();    //Read the data requested by the Master
}   


void requestEvent() {
   dtostrf(pulse[Address], 3, 0, t);   //Convert the Data requested to ASII String   dtostrf(floatVar or intVar, minStringWidthIncDecimalPoint, numVarsAfterDecimal, empty array)         
   Wire.write(t);         //Send ASII String
}
