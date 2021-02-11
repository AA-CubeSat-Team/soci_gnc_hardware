#include <Wire.h>

#define FXAS21002C_H_OUT_X_MSB        0x01  
#define FXAS21002C_H_OUT_X_LSB        0x02  
#define FXAS21002C_H_CTRL_REG0        0x0D

uint8_t address = 0x20;
void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(115200);
  Serial.println("Hello");
}

void loop()
{
//  int referenceIndentifier = 0b01000101;
//  Serial.print("Reference indentifier: ");
//  Serial.println(referenceIndentifier, BIN);  
//  uint8_t readIndentifier;
//  readReg(FXAS21002C_H_CTRL_REG0, &readIndentifier);
//  Serial.print("Read indentifier: ");
//  Serial.println(readIndentifier, BIN);
//  if (readIndentifier == referenceIndentifier) {
//    Serial.println("Pass");
//  } else {
//    Serial.println("Fail");
//  }
//
//  uint8_t referenceRawData[6] = {0b00000001, 0b10000011,0b00000001, 0b10000011,0b00000001, 0b10000011};
//  uint8_t readRawData[6];
//  readRegs(FXAS21002C_H_OUT_X_MSB, 6, &readRawData[0]);
//  
//  Serial.println("Reference raw data  Read raw data Result");
//  
//  for (int i = 0; i < 6; i++) {
//    Serial.print(referenceRawData[i],BIN);
//    Serial.print("  ");
//    Serial.print(readRawData[i],BIN);
//    Serial.print("  ");   
//    if (referenceRawData[i] == readRawData[i]) {
//      Serial.println("Pass");
//    } else {
//      Serial.println("Fail");
//    }
//  }

  uint8_t referenceValue = 2;
  Serial.print("Reference value: ");
  Serial.println(referenceValue);

  writeReg(FXAS21002C_H_CTRL_REG0, referenceValue);
  
  uint8_t readValue;
  readReg(FXAS21002C_H_CTRL_REG0, &readValue);
  Serial.print("Read value: ");
  Serial.println(readValue);
  if (readValue == referenceValue) {
    Serial.println("Pass");
  } else {
    Serial.println("Fail");
  }
  
  delay(1000);
}

// Reads register 'reg' and return it as a byte.
void readReg(uint8_t reg, uint8_t *value)
{
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(address, (uint8_t)1);
  *value = Wire.read();
}

void readRegs(uint8_t reg, uint8_t count, uint8_t dest[])
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

void writeReg(uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}
