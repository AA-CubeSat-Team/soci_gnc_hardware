#include <Wire.h>

// register addresses FXAS21002C_H_
#define FXAS21002C_H_STATUS           0x00
#define FXAS21002C_H_DR_STATUS        0x07
#define FXAS21002C_H_F_STATUS         0x08
#define FXAS21002C_H_OUT_X_MSB        0x01    
#define FXAS21002C_H_OUT_X_LSB        0x02
#define FXAS21002C_H_OUT_Y_MSB        0x03
#define FXAS21002C_H_OUT_Y_LSB        0x04
#define FXAS21002C_H_OUT_Z_MSB        0x05
#define FXAS21002C_H_OUT_Z_LSB        0x06
#define FXAS21002C_H_F_SETUP          0x09
#define FXAS21002C_H_F_EVENT          0x0A
#define FXAS21002C_H_INT_SRC_FLAG     0x0B
#define FXAS21002C_H_WHO_AM_I         0x0C
#define FXAS21002C_H_CTRL_REG0        0x0D  
#define FXAS21002C_H_RT_CFG           0x0E
#define FXAS21002C_H_RT_SRC           0x0F 
#define FXAS21002C_H_RT_THS           0x10
#define FXAS21002C_H_RT_COUNT         0x11
#define FXAS21002C_H_TEMP             0x12
#define FXAS21002C_H_CTRL_REG1        0x13
#define FXAS21002C_H_CTRL_REG2        0x14
#define FXAS21002C_H_CTRL_REG3        0x15

enum GyroODR {
  GODR_800HZ = 0,
  GODR_400HZ,
  GODR_200HZ,
  GODR_100HZ,
  GODR_50HZ,
  GODR_25HZ,
  GODR_12_5HZ,

};
// Set initial input parameters
enum GyroFSR {
  GFSR_2000DPS = 0,
  GFSR_1000DPS,
  GFSR_500DPS,
  GFSR_250DPS,
};

enum GyroBW {
  GBW_L1 = 0,
  GBW_L2,
  GBW_L3,
};

long intt;
double i = 0;
double testtime = 20000;
boolean testing = 0;
GyroFSR FSR = GFSR_250DPS;
GyroODR ODR = GODR_12_5HZ;
GyroBW BW = GBW_L1;
double odrValue;
float tempData;
float gyroX, gyroY, gyroZ;
float gBias[3] = {0,0,0};
byte address = 0x20;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  odrValue = getODR();
  Serial.println("Gyroscope FXAS2100c");
  Serial.print("Output data rate: ");
  Serial.print(odrValue);
  Serial.println(" Hz");
  setConfigures();
  active();
  delay(500);
  readTempData();
  Serial.print("Temperature: ");
  Serial.print(tempData);
  Serial.println(" Degree");
}

void loop() {
  if (i == floor(testtime*ODR)){
    testing = 0;
  }
  
  if (!(testing)){
  int j = 1;
  while(j){
    if(Serial.available())  {
      String value = Serial.readStringUntil('\n');
      Serial.println(value);
      if(value=="start") {
          i = 0;
          testtime = testtime;
          j = 0;
          testing = 1;
          delay(2000);
      }
    }
  }
}
  intt = micros();

  i=i+1;

  // Query the sensor
  readGyroData();
  Serial.print(" ");
  Serial.print((float)gyroX,7);
  Serial.print(" ");
  Serial.print((float)gyroY,7);
  Serial.print(" ");
  Serial.println((float)gyroZ,7);

  while(micros()-intt<1.0/(ODR)*1e6){
  }
}

// writes registration 'reg' with value 'value'
void writeReg(byte reg, byte value)
{
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

// writes a signle field which has last digit at bit 'bit' in registration 'reg' with value 'value'.
// e.g. in CTRL_REG0, bandwidth setting is at bits 7:6, so parameter bit would be 6.
void writeField(byte reg, byte bit, byte value)
{
  byte c = readReg(reg);
  writeReg(FXAS21002C_H_CTRL_REG1, c & ~(0x00<<bit));
  writeReg(FXAS21002C_H_CTRL_REG1, c |   (value<<bit));
}

// Reads register 'reg' and return it as a byte.
byte readReg(byte reg)
{
  byte value;

  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(address, (uint8_t)1);
  value = Wire.read();

  return value;
}

// Reads 'counts' number of values from registration 'reg' and stores the values in 'dest[]'.
void readRegs(byte reg, uint8_t count, byte dest[])
{
  uint8_t i = 0;

  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(address, count);

  while (Wire.available()) {
    dest[i++] = Wire.read();
  }
}

// Reads the temperature data
void readTempData()
{
  tempData = readReg(FXAS21002C_H_TEMP);
}

// Puts the FXAS21002C into standby mode.
// It must be in standby for modifying most registers
void standby()
{
  writeField(FXAS21002C_H_CTRL_REG1, 0, 0x00);
}

// Sets the FXAS21000 to active mode.
// Needs to be in this mode to output data
void ready()
{
  writeField(FXAS21002C_H_CTRL_REG1, 0, 0x01);
}

// Puts the FXAS21002C into active mode.
// Needs to be in this mode to output data.
void active()
{
  writeField(FXAS21002C_H_CTRL_REG1, 0, 0x02);
}

// sets up basic configures, full-scale rage, cut-off frequency and output data rate.
void setConfigures()
{
  standby();
  writeReg(FXAS21002C_H_CTRL_REG0, (BW<<6) | FSR);
  writeReg(FXAS21002C_H_CTRL_REG1, ODR << 2);
}

// Reads the gyroscope data
void readGyroData()
{
  uint8_t rawData[6];  // x/y/z gyro register data stored here
  readRegs(FXAS21002C_H_OUT_X_MSB, 6, &rawData[0]);  // Read the six raw data registers into data array
  gyroX = ((int16_t)(((int16_t)rawData[0]) << 8 | ((int16_t) rawData[1])))*sensitivity()-gBias[0];
  gyroY = ((int16_t)(((int16_t)rawData[2]) << 8 | ((int16_t) rawData[3])))*sensitivity()-gBias[1];
  gyroZ = ((int16_t)(((int16_t)rawData[4]) << 8 | ((int16_t) rawData[5])))*sensitivity()-gBias[2];
}

// return the sensitivity of the senosr depending on the value of the full-scale range
float sensitivity(void)
{
  switch (FSR)
  {
    // Possible gyro scales (and their register bit settings) are:
  // 250 DPS (11), 500 DPS (10), 1000 DPS (01), and 2000 DPS  (00).
    case GFSR_2000DPS:
        return 62.5e-3;
        break;
    case GFSR_1000DPS:
      return   31.25e-3;
      break;
    case GFSR_500DPS:
      return   15.625e-3;
      break;
    case GFSR_250DPS:
      return   7.8125e-3;
      break;
  }
}

// returns the current output data rate as a double (Hz)
double getODR(void)
{
  switch (ODR)
  {
    case  GODR_800HZ:
        return 800;
    case  GODR_400HZ:
        return 400;
    case  GODR_200HZ:
        return 200;
    case  GODR_100HZ:
      return 100;
    case  GODR_50HZ:
      return 50;
    case  GODR_25HZ:
      return 25;
    case  GODR_12_5HZ:
      return 12.5;

  }
}


void reset(){
  writeReg(FXAS21002C_H_CTRL_REG1, 0x20); // set reset bit to 1 to assert software reset to zero at end of boot process
  delay(100);
while(!(readReg(FXAS21002C_H_INT_SRC_FLAG) & 0x08))  { // wait for boot end flag to be set
}
}
