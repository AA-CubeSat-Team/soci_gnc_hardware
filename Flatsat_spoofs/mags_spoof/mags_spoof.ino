
#include <Wire.h>
extern TwoWire Wire1;


#define LSM303_REGISTER_MAG_CRA_REG_M             0x00
#define LSM303_REGISTER_MAG_CRB_REG_M             0x01
#define LSM303_REGISTER_MAG_MR_REG_M              0x02
#define LSM303_REGISTER_MAG_OUT_X_H_M             0x03
#define LSM303_REGISTER_MAG_SR_REG_Mg             0x09
#define LSM303_MAGGAIN_1_3                        0x20
#define LSM303_REGISTER_MAG_TEMP_OUT_X_H_M        0x31
// Gain multipliers
#ifndef GAUSS_TO_MICROTESLA
  #define GAUSS_TO_MICROTESLA 100
#endif
int gauss_LSB[3] = {1100, 980, 1100};
// Address
#define LSM303_ADDRESS_MAG           (0x3C>>1)
#define DOUBLE                        float

uint8_t tempReg; // variable holding the temporary requested register
uint8_t tempValue = 0; // variable holding the temporary value
uint8_t cra_reg_m = 0b00010000;
uint8_t crb_reg_m = 0b0010000;
uint8_t mr_reg_m = 0b00000011;
uint8_t sr_reg_mg = 0b00000000;
byte bytesRecieve[12] = {0xc1, 0xa1, 0xa3, 0xe8, 0xc2, 0xba, 0xa8, 0x24, 0xc2, 0xa1, 0x4a, 0xca};
uint8_t magByteData[6] = {0b00000001, 0b00000010, 0b00000011, 0b00000101, 0b00000110, 0b00000111};
DOUBLE magDoubleData[3] = {13, 44, 27};
int n=3;

void setup()
{
  Wire.begin(LSM303_ADDRESS_MAG);
  Wire.onReceive(receiveEventOBC);
  Wire.onRequest(requestEventOBC);

  Serial.begin(115200);
}

void loop()
{
  serialRecieve(bytesRecieve, n);
  buffer2float(bytesRecieve, magDoubleData, n);
  double2MagFormat(magDoubleData, magByteData, n);
}

void receiveEventOBC(int numByte) //recieve from OBC
{  
//  Serial.println("recieve");
  double2MagFormat(magDoubleData, magByteData, n);
  
  if(numByte > 1) {
    tempReg = Wire.read();
    tempValue = Wire.read();

//    Serial.println(tempReg, HEX);
    switch (tempReg) {
      case LSM303_REGISTER_MAG_CRA_REG_M:
        cra_reg_m = tempValue;
        break;
      case LSM303_REGISTER_MAG_CRB_REG_M:
        crb_reg_m = tempValue;
        break;
      case LSM303_REGISTER_MAG_SR_REG_Mg:
        sr_reg_mg = tempValue;
        break;
    }
  } else {
    tempReg = Wire.read();
  }
}

void requestEventOBC() {  // request event sent from OBC 
//  Serial.println("request");
  double2MagFormat(magDoubleData, magByteData, n);

//  Serial.println(tempReg, HEX);
  switch (tempReg) {
    case LSM303_REGISTER_MAG_CRA_REG_M:
      Wire.write(cra_reg_m);
      break;
    case LSM303_REGISTER_MAG_CRB_REG_M:
      Wire.write(crb_reg_m);
      break;
    case LSM303_REGISTER_MAG_OUT_X_H_M:
      Wire.write(magByteData, 6);
      for (int i = 0; i < 6; i++) {
        Serial.print(magByteData[i], HEX);
      }
      Serial.println(" ");
      break;
    case LSM303_REGISTER_MAG_SR_REG_Mg:
      Wire.write(sr_reg_mg);
      break;
  } 
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

void double2MagFormat(float * magDoubleData, uint8_t * magByteData, int n)
{
  int16_t magIntData;
  for (int ii = 0; ii < n; ii++) {
    magIntData = *(magDoubleData + ii)/GAUSS_TO_MICROTESLA*gauss_LSB[ii];
    *(magByteData + 2*ii) = (uint8_t)(magIntData>>8);
    *(magByteData + 2*ii + 1) = (uint8_t)((magIntData<<8)>>8);
  }
}
