#include <Wire.h>
extern TwoWire Wire1;


#define LSM303_REGISTER_MAG_CRA_REG_M             = 0x00,
#define LSM303_REGISTER_MAG_CRB_REG_M             = 0x01,
#define LSM303_REGISTER_MAG_MR_REG_M              = 0x02,
#define LSM303_REGISTER_MAG_OUT_X_H_M             = 0x03,
#define LSM303_REGISTER_MAG_SR_REG_Mg             = 0x09,
#define LSM303_MAGGAIN_1_3                        = 0x20,
#define LSM303_REGISTER_MAG_TEMP_OUT_X_H_M        = 0x31,
// Gain multipliers
#ifndef GAUSS_TO_MICROTESLA
  #define GAUSS_TO_MICROTESLA 100
#endif
#ifndef _lsm303Mag_Gauss_LSB_XY
  #define _lsm303Mag_Gauss_LSB_XY 1100
#endif
#ifndef _lsm303Mag_Gauss_LSB_Z
  #define _lsm303Mag_Gauss_LSB_Z 980
#endif
// Address
#define LSM303_ADDRESS_MAG                        = 0x3C,


uint8_t tempReg; // variable holding the temporary requested register
uint8_t tempValue = 0; // variable holding the temporary value
uint8_t cra_reg_m = 0b00000000;
uint8_t crb_reg_m = 0b00000000;
uint8_t mr_reg_m = 0b00000000;
uint8_t MagData[6] = {0b00000001, 0b10000011, 0b00000001, 0b10000011, 0b00000001, 0b10000011};
DOUBLE magDoubleData;
int16_t magRAWint;
uint8_t doubleConvertor[sizeof(DOUBLE)];

void setup()
{
  Wire.begin(LSM303_ADDRESS_MAG);
  Wire.onReceive(receiveEventOBC);
  Wire.onRequest(requestEventOBC);

  Wire1.begin(LSM303_ADDRESS_MAG);
  Wire1.onReceive(receiveEventSimulink);

  Serial.begin(115200);
}

void loop()
{
  delay(100);
}

void receiveEventOBC(int numByte) //recieve from OBC
{  
  if(numByte > 1) {
    tempReg = Wire.read();
    tempValue = Wire.read();
    switch (tempReg) {
      case LSM303_REGISTER_MAG_CRA_REG_M:
        cra_reg_m = tempValue;
        break;
      case LSM303_REGISTER_MAG_CRB_REG_M:
        crb_reg_m = tempValue;
        if (crb_reg_m>>6) {
          resetMag();
        }
        break;
    }
  } else {
    tempReg = Wire.read();
  }
}


void receiveEventSimulink(int numByte) //recieve from Simulink
// convert double for x and y axis 
{
  int ii = 0;
  while (Wire1.available()) {
    for (int jj = 0; jj<sizeof(doubleConvertor); jj++){
      doubleConvertor[jj] = Wire1.read();
    }
    memcpy(&magDoubleData, doubleConvertor, sizeof(doubleConvertor));
    magRAWData = magDoubleData/GAUSS_TO_MICROTESLA*_lsm303Mag_Gauss_LSB_XY
    magByteData[ii] = (uint8_t)(magRAWData>>8);
    ii++;
    magByteData[ii] = (uint8_t)((magRAWData<<8)>>8);
    ii++;    
  }
 //convert double for z axis
  int ii = 0;
  while (Wire1.available()) {
    for (int jj = 0; jj<sizeof(doubleConvertor); jj++){
      doubleConvertor[jj] = Wire1.read();
    }
    memcpy(&magDoubleData, doubleConvertor, sizeof(doubleConvertor));
    magRAWData = magDoubleData/GAUSS_TO_MICROTESLA*_lsm303Mag_Gauss_LSB_Z
    magByteData[ii] = (uint8_t)(magRAWData>>8);
    ii++;
    magByteData[ii] = (uint8_t)((magRAWData<<8)>>8);
    ii++;    
  }
}

void requestEventOBC() {  // request event sent from OBC 
  switch (tempReg) {
    case LSM303_REGISTER_MAG_CRA_REG_M:
      Wire.write(cra_reg_m);
      break;
    case LSM303_REGISTER_MAG_CRB_REG_M:
      Wire.write(crb_reg_m);
      break;
    case LSM303_REGISTER_MAG_OUT_X_H_M:
      Wire.write(magByteData, 6);
      break;
  } 
}

void resetMag() {
  uint8_t magByteDataTemp[6] = {0b00000001, 0b00000010, 0b00000011, 0b00000100, 0b00000101, 0b00000110};
  memcpy(magByteData, magByteDataTemp, 6);
  cra_reg_m = 0b00000000;
  crb_reg_m = 0b00000000;
}
  
