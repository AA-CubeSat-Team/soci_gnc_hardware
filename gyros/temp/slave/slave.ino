#include <Wire.h>

#define FXAS21002C_H_CTRL_REG0        0x0D
// More register here

uint8_t address = 0x20;
uint8_t tempReg; // variable holding the temporary requested register
uint8_t tempValue; // variable holding the temporary value

uint8_t reg0 = 0b00000101;
// More register value here

void setup()
{
  Wire.begin(address);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent);
}

void loop()
{
  delay(100);
}

void receiveEvent(int numByte)
{
  if(numByte > 1) {
    tempReg = Wire.read();
    tempValue = Wire.read();
    * getRegValue(&tempReg) = tempValue;
  } else {
    tempReg = Wire.read();
  }
}

void requestEvent(){
  Wire.write( * getRegValue(&tempReg));
}

int * getRegValue(int reg){
  switch(tempReg) {
    case FXAS21002C_H_CTRL_REG0:
      return &reg0
      break;
  }
}
