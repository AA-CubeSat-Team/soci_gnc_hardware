//Sender Code
#include <Wire.h>
#define SLAVE_ADDRESS 3
char t[10]={};//empty array where to put the numbers comming from the slave
volatile float Val; // varaible used by the master to sent data to the slave

void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
   Wire.requestFrom(SLAVE_ADDRESS, 3);    // request 3 bytes from slave device

//gathers data comming from slave
int i = 0; //counter for each byte as it arrives
  while (Wire.available()) { 
    t[i] = Wire.read(); // every character that arrives it put in order in the empty array "t"
    i++;
  }

Serial.println(t);   //shows the data in the array t
delay(500); //give some time to relax

// send data to slave. here I am just sending the number 2
  float Val = 2.20;
  Wire.beginTransmission (SLAVE_ADDRESS);
  Wire.write ((byte)Val);
  Wire.endTransmission ();
}
