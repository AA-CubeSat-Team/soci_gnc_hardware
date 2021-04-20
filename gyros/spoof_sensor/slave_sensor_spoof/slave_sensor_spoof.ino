
#include <Wire.h>

#define FXAS21002C_H_OUT_X_MSB        0x01
#define FXAS21002C_H_CTRL_REG0        0x0D
#define FXAS21002C_H_CTRL_REG1        0x13
#define FXAS21002C_H_TEMP         0x12
#define FXAS21002C_H_INT_SRC_FLAG     0x0B
#define SENSITIVITY                   7.8125e-3
#define ADDRESS                       (uint8_t) 0x20
#define TEMPERATURE                   23
#define DOUBLE                        double

uint8_t tempReg; // variable holding the temporary requested register
uint8_t tempValue = 0; // variable holding the temporary value
uint8_t ctrl_reg0 = 0b00000000;
uint8_t ctrl_reg1 = 0b00000000;
uint8_t gyroByteData[6] = {0b00000001, 0b00000010, 0b00000011, 0b00000100, 0b00000101, 0b00000110};
DOUBLE gyroDoubleData[3];
int     n = 3;



void setup()
{
  Wire.begin(ADDRESS);
  Wire.onReceive(receiveEventOBC);
  Wire.onRequest(requestEventOBC);

  Serial.begin(115200);
}

void loop() {
  serialRecieve(gyroDoubleData, n);
  double2GyroFromat(gyroDoubleData, gyroByteData, n);
}

void receiveEventOBC(int numByte) //recieve from OBC
{  
  if(numByte > 1) {
    tempReg = Wire.read();
    tempValue = Wire.read();
    switch (tempReg) {
      case FXAS21002C_H_CTRL_REG0:
        ctrl_reg0 = tempValue;
        break;
      case FXAS21002C_H_CTRL_REG1:
        ctrl_reg1 = tempValue;
        if (ctrl_reg1>>6) {
          resetGyro();
        }
        break;
    }
  } else {
    tempReg = Wire.read();
  }
}

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
    case FXAS21002C_H_TEMP:
      Wire.write((int8_t)TEMPERATURE);
    case FXAS21002C_H_INT_SRC_FLAG:
      Wire.write(0b1000);
  } 
}

void resetGyro() {
  uint8_t gyroByteDataTemp[6] = {0b00000001, 0b00000010, 0b00000011, 0b00000100, 0b00000101, 0b00000110};
  memcpy(gyroByteData, gyroByteDataTemp, 6);
  ctrl_reg0 = 0b00000000;
  ctrl_reg1 = 0b00000000;
}

void serialRecieve(double * doubleBuffer, int n)
{
  while (Serial.available() < n*sizeof(double)){
  }
  Serial.readBytes((char*)doubleBuffer,n*sizeof(double));
}

void double2GyroFromat(double * gyroDoubleData, uint8_t * gyroByteData, int n)
{
  int16_t gyroIntData;
  for (int ii = 0; ii < n; ii++) {
    gyroIntData = *(gyroDoubleData + ii)/SENSITIVITY;
    *(gyroByteData + 2*ii) = (uint8_t)(gyroIntData>>8);
    *(gyroByteData + 2*ii + 1) = (uint8_t)((gyroIntData<<8)>>8);
  }
}
