#include <Wire.h>
#include "gyro_wrap.h"

bool test_readtempdata = false;
bool test_readgyrodata = false;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Tests begin");
  Serial.println("********************");
  test_initGyro();
  test_readRegs();
  test_writeReg();
  test_startGyro();
  test_restGyro();
  test_readTempData();
  test_readGyroData();
}

void loop() {
  double t0 = micros();
  if (test_readtempdata) {
    readTempData(&Gyro1);
    Serial.println(Gyro1.temperature);
  }
  if (test_readgyrodata)
  {
    readGyroData(&Gyro1);
    for (int i = 0; i<3; i++){
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
  uint8_t referenceRawData[15] = {0, 0, 0, 0, 8, 0xD7, 0, 0, 0, 0, 1, 0, 0, 0, 0};
  uint8_t readRawData[15];
  initGyro(&Gyro1);
  readRegs(0x07, &readRawData[0], 15, &Gyro1);
  
  Serial.println("Reference raw data  Read raw data Result");
  
  for (int i = 0; i < 15; i++) {
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

  uint8_t referenceRawData[15] = {0, 0, 0, 0, 8, 0xD7, 0, 0, 0, 0, 1, 0, 0, 0, 0};
  uint8_t readRawData[15];
  readRegs(0x07, &readRawData[0], 15, &Gyro1);
  
  Serial.println("Reference raw data  Read raw data Result");
  
  for (int i = 0; i < 15; i++) {
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
}

void test_readTempData(){
  initGyro(&Gyro1);
  startGyro(&Gyro1);
  test_readtempdata = true;
  Serial.println("Eyeballing the validity of the data");
  Serial.println("********************");  
}

void test_readGyroData(){
  initGyro(&Gyro1);
  startGyro(&Gyro1);
  test_readgyrodata = true;
  Serial.println("Eyeballing the validity of the data");
  Serial.println("********************");  
}
