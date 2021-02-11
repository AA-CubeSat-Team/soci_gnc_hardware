#include <Wire.h>

#define FXAS21002C_H_CTRL_REG0        0x0D

uint8_t address = 0x20;
void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(115200);
}

void loop()
{
  int referenceIndentifier = 0b00000101;
  Serial.print("Reference indentifier: ");
  Serial.println(referenceIndentifier, BIN);  
  uint8_t readIndentifier;
  readReg(FXAS21002C_H_CTRL_REG0, &readIndentifier);
  Serial.print("Read indentifier: ");
  Serial.println(readIndentifier, BIN);
  if (readIndentifier == referenceIndentifier) {
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
