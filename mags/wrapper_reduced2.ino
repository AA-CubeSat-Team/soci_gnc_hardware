    #include <Wire.h>

    // initialize hardcoded calibration consants
    static float Mag_Cal_vec[3] = {-2.53719700624655, 11.1988547753627, 19.3099210307232};
    static float Mag_Cal_consts[3] = {1, 1, 1};
    // static float T_Mag_Response = ???
    int errorFlag = 0;
    
    // Gauss to micro-Tesla multiplier 
    static int GAUSS_TO_MICROTESLA = 100; 
  
    static float _lsm303Mag_Gauss_LSB_XY = 1100.0F;  // Varies with gain
    static float _lsm303Mag_Gauss_LSB_Z  = 980.0F;   // Varies with gain
    
   //  I2C ADDRESS/BITS
    #define LSM303_ADDRESS_MAG            (0x3C >> 1)         // 0011110x

    // REGISTERS
    typedef enum
    {
      LSM303_REGISTER_MAG_CRA_REG_M             = 0x00,
      LSM303_REGISTER_MAG_CRB_REG_M             = 0x01,
      LSM303_REGISTER_MAG_MR_REG_M              = 0x02,
      LSM303_REGISTER_MAG_OUT_X_H_M             = 0x03,
      LSM303_REGISTER_MAG_SR_REG_Mg             = 0x09,
    } lsm303MagRegisters_t;

    // GAIN SETTINGS
    typedef enum
    {
      LSM303_MAGGAIN_1_3                        = 0x20,  // +/- 1.3
    } lsm303MagGain;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void write8(byte address, byte reg, byte value)
{
  Wire.beginTransmission(address);
  Wire.write((uint8_t)reg);
  Wire.write((uint8_t)value);
  Wire.endTransmission();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

byte read8(byte address, byte reg)
{
  byte value;

  Wire.beginTransmission(address);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();
  Wire.requestFrom(address, (byte)1);
  value = Wire.read();
  Wire.endTransmission();

  return value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void readRegs(uint8_t reg, uint8_t count, uint8_t dest[])
{
  uint8_t i = 0;  
  Wire.beginTransmission((uint8_t)LSM303_ADDRESS_MAG);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)LSM303_ADDRESS_MAG, count);  
  // start timer 
  while (Wire.available()) {
    // increase timer 
    dest[i++] = Wire.read();
    //  if timer > T_Mag_Response {
    //  errorflag = 3;
    //  break;
    //}
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// bool setupMagneto()
void setup()
{
  // Enable I2C
  Wire.begin();
  Serial.begin(9600);
 
  // Enable the magnetometer
  write8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_MR_REG_M, 0x00);

  // LSM303DLHC has no WHOAMI register so read CRA_REG_M to check
  // the default value (0b00010000/0x10)
  uint8_t reg1_a = read8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_CRA_REG_M);
  if (reg1_a != 0x10)
  {
    errorFlag = 1;
  }
 
  // Set the gain to a known level
  write8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_CRB_REG_M, LSM303_MAGGAIN_1_3);
  _lsm303Mag_Gauss_LSB_XY = 1100;
  _lsm303Mag_Gauss_LSB_Z  = 980;
  
  // return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void readMagneto()
{
  uint8_t reg_mg = read8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_SR_REG_Mg);
  if (!(reg_mg & 0x1)) {
    errorFlag = 2;
  }

  // Read the magnetometer
  uint8_t raw_Temp[6];
  readRegs(LSM303_REGISTER_MAG_OUT_X_H_M, (uint8_t)6, raw_Temp);

  // Note high before low (different than accel)
  uint8_t xhi = raw_Temp[0];
  uint8_t xlo = raw_Temp[1];
  uint8_t yhi = raw_Temp[2];
  uint8_t ylo = raw_Temp[3];
  uint8_t zhi = raw_Temp[4];
  uint8_t zlo = raw_Temp[5];

  // Shift values to create properly formed integer (low byte first)
  int16_t raw_x = (int16_t)(xlo | ((int16_t)xhi << 8));
  int16_t raw_y = (int16_t)(ylo | ((int16_t)yhi << 8));
  int16_t raw_z = (int16_t)(zlo | ((int16_t)zhi << 8));
  Serial.print(raw_x);
  Serial.print("  ");
  Serial.print(raw_y);
  Serial.print("  ");
  Serial.println(raw_z);
  
  if ((raw_x >= 2040) | (raw_x <= -2040) |
      (raw_y >= 2040) | (raw_y <= -2040) |
      (raw_z >= 2040) | (raw_z <= -2040) )  {
      errorFlag = 4;
      }

  float cal_x = Mag_Cal_consts[0]*(((float)raw_x/_lsm303Mag_Gauss_LSB_XY*GAUSS_TO_MICROTESLA) - Mag_Cal_vec[0]); 
  float cal_y = Mag_Cal_consts[1]*(((float)raw_y/_lsm303Mag_Gauss_LSB_XY*GAUSS_TO_MICROTESLA) - Mag_Cal_vec[1]);
  float cal_z = Mag_Cal_consts[2]*(((float)raw_z/_lsm303Mag_Gauss_LSB_Z*GAUSS_TO_MICROTESLA) - Mag_Cal_vec[2]);
  
  float magData[3] = {cal_x, cal_y, cal_z};
  Serial.print(cal_x); 
  Serial.print("  "); 
  Serial.print(cal_y);
  Serial.print("  ");  
  Serial.println(cal_z); 
}

void loop()
{
  delay(500);
  readMagneto();
}
