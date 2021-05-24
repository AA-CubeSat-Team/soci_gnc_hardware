#include <Wire.h>
#include "gyro_wrap.h"

#define TEST_READGYRODATA false
#define TEST_READTEMPDATA false
#define TEST_PLOTTER         false
void setup() {
  Serial.begin(115200);
  
  if (!TEST_PLOTTER)
  {
    Serial.println("Tests begin");
    Serial.println("********************");
    test_initGyro();
    test_readRegs();
    test_writeReg();
    test_startGyro();
    test_restGyro();
  }
  
  if (TEST_READGYRODATA || TEST_READTEMPDATA) 
  {
    initGyro(&Gyro1);
    startGyro(&Gyro1);
  }
}

void loop() {
  double t0 = micros();
  if (TEST_READTEMPDATA) {
    readTempData(&Gyro1);
    Serial.println(Gyro1.temperature);
  }
  if (TEST_READGYRODATA)
  {
    readGyroData(&Gyro1);
    for (int i = 0; i < 3; i++) {
      Serial.print(" ");
      Serial.print(Gyro1.gyroXYZ[i], 7);
    }
    Serial.println();
  }
  while (micros() - t0 < 1.0 / GYRO_ODR_VALUE * 1e6) {
  }
}

void test_initGyro()
{
  Serial.println("Testing initGyro...");
  Wire.beginTransmission(GYRO_ADDRESS);
  Serial.println(Wire.endTransmission());
  initGyro(&Gyro1);
  if (Gyro1.gyroInitialized)
  {
    Serial.println("Gyro initialized");
    Serial.println("Pass");
  } else
  {
    Serial.println("Gryo failed to be initialized");
    Serial.println("Fail");
  }
  Serial.println("********************");
}

void test_readRegs()
{
  Serial.println("Testing raedRegs...");
  uint8_t referenceRawData[6] = {0b00000001, 0b00000010, 0b00000011, 0b00000100, 0b00000101, 0b00000110};
  uint8_t readRawData[6];
  initGyro(&Gyro1);
  readRegs(GYRO_OUT_X_MSB, &readRawData[0], 6, &Gyro1);
  
  Serial.println("Reference raw data  Read raw data Result");
  
  for (int i = 0; i < 6; i++) {
    Serial.print(referenceRawData[i],BIN);
    Serial.print("  ");
    Serial.print(readRawData[i],BIN);
    Serial.print("  ");   
    if (referenceRawData[i] == readRawData[i]) {
      Serial.println("Pass");
    } else {
      Serial.println("Fail");
    }
  }
  Serial.println("********************");
}

void test_writeReg()
{
  Serial.println("Testing raedRegs...");
  uint8_t referenceValue = 2;
  Serial.print("Reference value: ");
  Serial.println(referenceValue);
  initGyro(&Gyro1);
  writeReg(GYRO_CTRL_REG1, referenceValue, &Gyro1);
  
  uint8_t readValue;
  readRegs(GYRO_CTRL_REG1, &readValue, 1, &Gyro1);
  Serial.print("Read value: ");
  Serial.println(readValue);
  if (readValue == referenceValue) {
    Serial.println("Pass");
  } else {
    Serial.println("Fail");
  }
  Serial.println("********************");  
}

void test_startGyro()
{
  Serial.println("Test startGyro...");
  Serial.print("Reference CTRL_REG0: ");
  Serial.println(GYRO_FSR_NUM, BIN);
  Serial.print("Reference CTRL_REG1: ");
  Serial.println((GYRO_ODR_NUM <<2)|0b10, BIN);

  initGyro(&Gyro1);
  startGyro(&Gyro1);
  
  uint8_t readCtrlReg0;
  readRegs(GYRO_CTRL_REG0, &readCtrlReg0, 1, &Gyro1);
  Serial.print("Read CTRL_REG0: ");
  Serial.println(readCtrlReg0, BIN);
  if (readCtrlReg0 == GYRO_FSR_NUM) {
    Serial.println("Pass");
  } else {
    Serial.println("Fail");
  }
  uint8_t readCtrlReg1;
  readRegs(GYRO_CTRL_REG1, &readCtrlReg1, 1, &Gyro1);
  Serial.print("Read CTRL_REG1: ");
  Serial.println(readCtrlReg1, BIN);
  if (readCtrlReg1 == (GYRO_ODR_NUM <<2)|0b10) {
    Serial.println("Pass");
  } else {
    Serial.println("Fail");
  }
  Serial.println("********************");  
}

void test_restGyro()
{
  Serial.println("Test restGyro...");
  initGyro(&Gyro1);
  startGyro(&Gyro1);
  Serial.println("Start the Gyro");
  resetGyro(&Gyro1);
  Serial.println("Rest the gyro");
  
  uint8_t referenceRawData[6] = {0b00000001, 0b00000010, 0b00000011, 0b00000100, 0b00000101, 0b00000110};
  uint8_t readRawData[6];
  initGyro(&Gyro1);
  readRegs(GYRO_OUT_X_MSB, &readRawData[0], 6, &Gyro1);
  
  Serial.println("Reference raw data  Read raw data Result");
  
  for (int i = 0; i < 6; i++) {
    Serial.print(referenceRawData[i],BIN);
    Serial.print("  ");
    Serial.print(readRawData[i],BIN);
    Serial.print("  ");   
    if (referenceRawData[i] == readRawData[i]) {
      Serial.println("Pass");
    } else {
      Serial.println("Fail");
    }
  }
  Serial.println("********************");
   
}
