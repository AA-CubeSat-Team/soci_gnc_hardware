#include <Wire.h>

#define FXAS21002C_H_OUT_X_MSB        0x01  
#define FXAS21002C_H_OUT_X_LSB        0x02  
#define FXAS21002C_H_CTRL_REG0        0x0D
// More register here

uint8_t address = 0x20;
uint8_t tempReg; // variable holding the temporary requested register
uint8_t tempValue; // variable holding the temporary value

uint8_t ctrl_reg0 = 0b01000101; // just some testing values
uint8_t x_msb[6] = {0b00000001, 0b10000011,0b00000001, 0b10000011,0b00000001, 0b10000011};
// all six bytes of gyro raw data are stored in this array
// More register value here

void setup()
{
  Wire.begin(address);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent);
  Serial.begin(115200);
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
    switch(tempReg) {
      case FXAS21002C_H_CTRL_REG0:
        ctrl_reg0 = tempValue;
        break;

       // more registers need to be added here
    }
  } else {
    tempReg = Wire.read();
  }
}

void requestEvent(){
  switch(tempReg) {
    case FXAS21002C_H_CTRL_REG0:
      Wire.write(ctrl_reg0);
      break;
    case FXAS21002C_H_OUT_X_MSB:
      // need to request the right Arduino slave(the simulation) for the value
      Wire.write(x_msb,6);
      break;

   // more registers need to be added here
  }
}
