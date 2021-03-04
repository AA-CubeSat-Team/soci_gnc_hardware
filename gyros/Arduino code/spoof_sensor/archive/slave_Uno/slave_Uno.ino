#include <Wire.h>

#define DUE                           1

#if DUE
extern TwoWire Wire1;
#endif

#define FXAS21002C_H_OUT_X_MSB        0x01
#define FXAS21002C_H_CTRL_REG0        0x0D
#define FXAS21002C_H_CTRL_REG1        0x13
#define SENSITIVITY                   0.1
#define ADDRESS                       (uint8_t) 0x20

uint8_t tempReg; // variable holding the temporary requested register
uint8_t tempValue; // variable holding the temporary value
uint8_t ctrl_reg0 = 0b00000000;
uint8_t ctrl_reg1 = 0b00000000;
uint8_t gyroByteData[6] = {0b00000000, 0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
double gyroDoubleData;
int16_t gyroIntData;
uint8_t doubleConvertor[sizeof(double)];

// just some testing values
// all six bytes of gyro raw data are stored in this array
void setup()
{
  Wire.begin(ADDRESS);
  Wire.onReceive(receiveEventOBC);
  Wire.onRequest(requestEventOBC);
#if DUE
  Wire1.begin(ADDRESS);
  Wire1.onReceive(receiveEventSimulink);
#endif
  Serial.begin(115200);
}

void loop()
{
  delay(100);
}

void receiveEventOBC(int numByte) //recieve from OBC
{
#if !DUE
  if (numByte > 10) {
    int ii = 0;
    while (Wire.available()) {
      for (int jj = 0; jj<sizeof(doubleConvertor); jj++){
        doubleConvertor[jj] = Wire.read();
      }
      memcpy(&gyroDoubleData, doubleConvertor, sizeof(doubleConvertor));
      gyroIntData = gyroDoubleData/SENSITIVITY;
      gyroByteData[ii] = (uint8_t)(gyroIntData>>8);
      gyroByteData[ii++] = (uint8_t)((gyroIntData<<8)>>8);
      ii++;
    }
  } else
#endif
  if(numByte > 1) {
    tempReg = Wire.read();
    tempValue = Wire.read();
    switch (tempReg) {
      case FXAS21002C_H_CTRL_REG0:
        ctrl_reg0 = tempValue;
        break;
      case FXAS21002C_H_CTRL_REG1:
        ctrl_reg1 = tempValue;
        break;
    }
  } else {
    tempReg = Wire.read();
  }
}

#if DUE
void receiveEventSimulink(int numByte) //recieve from Simulink
{
//  if (numByte != sizeof(double)*6) {
//    Serial.println("the data should be six double");
//  }

  int ii = 0;
  while (Wire1.available()) {
    for (int jj = 0; jj<sizeof(doubleConvertor); jj++){
      doubleConvertor[jj] = Wire1.read();
    }
    memcpy(&gyroDoubleData, doubleConvertor, sizeof(doubleConvertor));
    gyroIntData = gyroDoubleData/SENSITIVITY;
    gyroByteData[ii] = (uint8_t)(gyroIntData>>8);
    gyroByteData[ii++] = (uint8_t)((gyroIntData<<8)>>8);
    ii++;
  }
}
#endif

void requestEventOBC() {  // request event sent from OBC
  switch (tempReg) {
    case FXAS21002C_H_CTRL_REG0:
      Wire.write(ctrl_reg0);
      break;
    case FXAS21002C_H_CTRL_REG1:
      Wire.write(ctrl_reg1);
      break;
    case FXAS21002C_H_OUT_X_MSB:
      Wire.write(gyroByteData, 6);
      break;
  }
}
