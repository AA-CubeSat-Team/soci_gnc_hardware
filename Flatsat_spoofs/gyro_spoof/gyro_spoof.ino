
#include <Wire.h>

#define FXAS21002C_H_OUT_X_MSB        0x01
#define FXAS21002C_H_CTRL_REG0        0x0D
#define FXAS21002C_H_CTRL_REG1        0x13
#define FXAS21002C_H_TEMP             0x12
#define FXAS21002C_H_INT_SRC_FLAG     0x0B
#define SENSITIVITY                   7.8125e-3
#define ADDRESS                       (uint8_t) 0x20
#define TEMPERATURE                   23
#define DOUBLE                        float

uint8_t tempReg; // variable holding the temporary requested register
uint8_t tempValue = 0; // variable holding the temporary value
uint8_t ctrl_reg0 = 0b00000000;
uint8_t ctrl_reg1 = 0b00000000;
byte bytesRecieve[12] = {0xc1, 0xa1, 0xa3, 0xe8, 0xc2, 0xba, 0xa8, 0x24, 0xc2, 0xa1, 0x4a, 0xca};
uint8_t gyroByteData[6] = {0b00000001, 0b00000010, 0b00000011, 0b00000100, 0b00000101, 0b00000110};
DOUBLE gyroDoubleData[3] = {.01, .0625, 81};
int     n = 3;

void setup()
{
  Wire.begin(ADDRESS);
  Wire.onReceive(receiveEventOBC);
  Wire.onRequest(requestEventOBC);

  Serial.begin(115200);
}

void loop() {
  serialRecieve(bytesRecieve, n);
  buffer2float(bytesRecieve, gyroDoubleData, n);
  double2GyroFormat(gyroDoubleData, gyroByteData, n);
}

void receiveEventOBC(int numByte) //recieve from OBC
{  
  if(numByte == 2) {
    tempReg = Wire.read();
    tempValue = Wire.read();
//    Serial.println("OBC want to write:");
//    Serial.println(tempReg,HEX);
//    Serial.println(tempValue, HEX);
    switch (tempReg) {
      case FXAS21002C_H_CTRL_REG0:
        ctrl_reg0 = tempValue;
        break;
      case FXAS21002C_H_CTRL_REG1:
        ctrl_reg1 = tempValue;
        if ((ctrl_reg1&0b1000000)>>6) {
          resetGyro();
        }
        break;
    }
  } else if(numByte == 1){
//    Serial.println("OBC want to read:");
    tempReg = Wire.read();
//    Serial.println(tempReg, HEX);
  } else {
//    Serial.println("too many in the buffer");
  }
}

void requestEventOBC() {  // request event sent from OBC 
//  Serial.println("give OBC what it wants");
//  Serial.println(tempReg, HEX);
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
//  Serial.println("rest gyro");
  uint8_t gyroByteDataTemp[6] = {0b00000001, 0b00000010, 0b00000011, 0b00000100, 0b00000101, 0b00000110};
  memcpy(gyroByteData, gyroByteDataTemp, 6);
  ctrl_reg0 = 0b00000000;
  ctrl_reg1 = 0b00000000;
}

void serialRecieve(byte * Buffer, int n)
{
  while (Serial.available() < n*sizeof(DOUBLE)){
  }
  Serial.readBytes((char*)Buffer, n*sizeof(DOUBLE));
  Serial.write((char*)Buffer, n*sizeof(DOUBLE));
  // delay(10);
  // Serial.write(0x0A);
}

void buffer2float(byte * in_bytes, float * out_Floats, int n)
{
//  for (int i = 0; i < n; i+=4) {
//    uint32_t num;
//    num = (in_bytes[i] << 24) | (in_bytes[i+1] << 16) | (in_bytes[i+2] << 8) | (in_bytes[i+3]);
//    out_Floats[i] = *((float *)&num);
//  }
  for (int ii = 0; ii < n; ii++) {
    union {
      byte temp_bytes[4];
      float temp_f;
    } u;
    for (int iii = 0; iii < 4; iii++) {
      u.temp_bytes[3-iii] = in_bytes[iii+4*ii];
    }
    out_Floats[2-ii] = u.temp_f;
  }
}

void double2GyroFormat(DOUBLE * gyroDoubleData, uint8_t * gyroByteData, int n)
{
  int16_t gyroIntData;
  for (int ii = 0; ii < n; ii++) {
    gyroIntData = *(gyroDoubleData + ii)/SENSITIVITY;
    // Serial.println(gyroIntData);
    *(gyroByteData + 2*ii) = (uint8_t)(gyroIntData>>8);
    *(gyroByteData + 2*ii + 1) = (uint8_t)((gyroIntData<<8)>>8);
  }
}
