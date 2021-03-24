// photodiode wrapper

#include <Wire.h>

uint8_t ADC_ser_address = 0x1D; // ADC address
#define PD1_reg 0x20
#define PD2_reg 0x21
#define PD3_reg 0x22
#define PD4_reg 0x23
#define PD5_reg 0x24
uint8_t PD_reg[] = {PD1_reg,PD2_reg,PD3_reg,PD4_reg,PD5_reg}; // Photodiode 1-5 register address'
uint16_t PDVol[10];
uint16_t PDVoltage1;
uint16_t PDVoltage2;
uint16_t PDVoltage3;
uint16_t PDVoltage4;
uint16_t PDVoltage5;
uint16_t maxV = 2880;
int i;
int j;


//________________Quick Start (Page 33 in ADC Datasheet)_________________________
// Step 3: Configuration 
#define advance_config_reg 0x0B
uint8_t advance_config_value = 0b00000000;

// Step 4: Enable Conversion Rate
#define start_reg 0x00
uint8_t test;
#define conv_rate_reg 0x07
uint8_t conv_rate_value = 0b0000001;

// Step 5: Disable analog inputs (disables IN5,IN6,temp)
#define disable_reg 0x08
uint8_t disable_value = 0b11100000; 

// Step 6: Enable interrupt mask 
#define interrupt_mask_reg 0x03
uint8_t interrupt_mask_value = 0b00011111;

//Step 7: Program limit registers
//IN0 High limit
uint8_t in_high_reg[] = {0x2A,0x2C,0x2E,0x30,0x32};
uint8_t in_high_val = 00000010 ; // change this (just a test for now)

// IN0 Low limit
uint8_t in_low_reg[] = {0x2B,0x2D,0x2F,0x31,0x33};
uint8_t in_low_val = 0;

// Step 8: Starting the ADC
#define config_reg 0x00
uint8_t config_default_value = 0b00001000;
uint8_t start_value = 0b00001001;
uint8_t c = 1;
#define busy_status_reg 0x0C

// Step 9: Setting INT_clear to zero
#define int_clear_reg 0x00
uint8_t int_clear_val = 0b00001001;
//______________________________________________________________________________________________


void setup(){
  // Enable Pullup resistors
  pinMode(4,INPUT_PULLUP);  
  pinMode(5,INPUT_PULLUP);
  Serial.begin (9600); //Declare baude rate
  while (!Serial) 
    {
    }
  Wire.begin();

  initADC();
}



void loop(){
//  getVoltage();

//  health();

  delay(5000);
}


void initADC(){
  // STEP 1&2: wait for at least 33 ms
  while (c) {
    readReg(busy_status_reg, 1, &c);
    }
    
  // STEP 3: Choosing internal VREF and mode of operation
  writeReg(advance_config_reg,advance_config_value);
  readReg(advance_config_reg,1, &advance_config_value);
  Serial.println("Step3: operation mode");
  Serial.println(advance_config_value);

  // STEP 4: Enable Conversion Rate Register
  writeReg(conv_rate_reg,conv_rate_value);  
  readReg(conv_rate_reg,1,&conv_rate_value);
  Serial.println("Step 4b: Conversion Rate value:");
  Serial.println(conv_rate_value);
  
  //STEP 5: Disabling the other seven inputs
  writeReg(disable_reg,disable_value);
  readReg(disable_reg,1,&disable_value);
  writeReg(start_reg,0b00001001);  // Exit shutdown mode
  Serial.println("Step5b: Disabling inputs");
  Serial.println(disable_value,BIN);

  //STEP 6: Enabling interrupt mask (won't go to INT pin)
  writeReg(interrupt_mask_reg,interrupt_mask_value);
  readReg(interrupt_mask_reg,1,&interrupt_mask_value);
  Serial.println("Enabling interrupt mask");
  Serial.println(interrupt_mask_value);
  
  //STEP7: Sending Limits for only enable input
  for(i=0; i<sizeof(in_high_reg); i++){
    writeReg(in_high_reg[i],in_high_val);
    writeReg(in_low_reg[i],in_low_val);
    readReg(in_high_reg[i],1,&in_high_val);
    readReg(in_low_reg[i],1,&in_low_val);
    Serial.println("Step7: Limits for IN0-IN4");
    Serial.println(in_high_val);
    Serial.println(in_low_val);
  }

  //STEP 8: Setting Start bit to 1   
  writeReg(config_reg, start_value);
  readReg(config_reg, 1, &c);
  Serial.println("Setting Start bit to 1");
  Serial.println(c,BIN);

  //STEP 9: Set interrupt to zero
  writeReg(int_clear_reg,int_clear_val);
  readReg(int_clear_reg,1,int_clear_val);
  Serial.println("Setting interrupt to zero");
  Serial.println(int_clear_val,BIN);
  Serial.println("------------------------------------------------------------------------------------");
}

// Health check: If any channel exceeds the maxV or all 5 channels exceed 3*maxV when summed: 1) resets registers to default values and puts ADC into shutdown mode. 2) Reinitializes ADC to proper configuration (same as quickstart procedure).
void health()
{
  if((PDVoltage1 > maxV || PDVoltage2 > maxV || PDVoltage3 > maxV || PDVoltage4 > maxV || PDVoltage5 > maxV) || PDVoltage1 + PDVoltage2 + PDVoltage3 + PDVoltage4 + PDVoltage5 > 3*maxV){
    writeReg(config_reg, 0b01001000);
    
      // STEP 1&2: wait for at least 33 ms
  while (c) {
    readReg(busy_status_reg, 1, &c);
    }
    
  // STEP 3: Choosing internal VREF and mode of operation
  writeReg(advance_config_reg,advance_config_value);
  readReg(advance_config_reg,1, &advance_config_value);
  Serial.println("Step3: operation mode");
  Serial.println(advance_config_value);

  // STEP 4: Enable Conversion Rate Register
  writeReg(conv_rate_reg,conv_rate_value);  
  readReg(conv_rate_reg,1,&conv_rate_value);
  Serial.println("Step 4b: Conversion Rate value:");
  Serial.println(conv_rate_value);
  
  //STEP 5: Disabling the other seven inputs
  writeReg(disable_reg,disable_value);
  readReg(disable_reg,1,&disable_value);
  writeReg(start_reg,0b00001001);  // Exit shutdown mode
  Serial.println("Step5b: Disabling inputs");
  Serial.println(disable_value,BIN);

  //STEP 6: Enabling interrupt mask (won't go to INT pin)
  writeReg(interrupt_mask_reg,interrupt_mask_value);
  readReg(interrupt_mask_reg,1,&interrupt_mask_value);
  Serial.println("Enabling interrupt mask");
  Serial.println(interrupt_mask_value);
  
  //STEP7: Sending Limits for only enable input
  for(i=0; i<sizeof(in_high_reg); i++){
    writeReg(in_high_reg[i],in_high_val);
    writeReg(in_low_reg[i],in_low_val);
    readReg(in_high_reg[i],1,&in_high_val);
    readReg(in_low_reg[i],1,&in_low_val);
    Serial.println("Step7: Limits for IN0-IN4");
    Serial.println(in_high_val);
    Serial.println(in_low_val);
  }

  //STEP 8: Setting Start bit to 1   
  writeReg(config_reg, start_value);
  readReg(config_reg, 1, &c);
  Serial.println("Setting Start bit to 1");
  Serial.println(c,BIN);

  //STEP 9: Set interrupt to zero
  writeReg(int_clear_reg,int_clear_val);
  readReg(int_clear_reg,1,int_clear_val);
  Serial.println("Setting interrupt to zero");
  Serial.println(int_clear_val,BIN);
  Serial.println("------------------------------------------------------------------------------------");
    
  }
}


// Reads voltages from ADC
void getVoltage()
{
  j = 0;
  for(i=0; i<sizeof(PD_reg); i++){ 
    Wire.beginTransmission(ADC_ser_address);
    Wire.write(PD_reg[i]);     // Write register address
    Wire.endTransmission();

    // EDIT: delay needs to occur outside FOR loop: send 5 registers, wait, request 5 values
    delay(80);    // Delay allowing for completion of conversion (~60ms for 5 voltage channels) 
    Wire.requestFrom(ADC_ser_address,2);
    while(Wire.available()){
      PDVol[j] = Wire.read();     // Data byte 1
      PDVol[j+1] = Wire.read();     // Data byte 2
    }
    j = j+2;
  }
  PDVoltage1 = PDVol[0]<<8 | PDVol[1];
  PDVoltage2 = PDVol[2]<<8 | PDVol[3];
  PDVoltage3 = PDVol[4]<<8 | PDVol[5];
  PDVoltage4 = PDVol[6]<<8 | PDVol[7];
  PDVoltage5 = PDVol[8]<<8 | PDVol[9];
}


// Reads 'counts' number of values from registration
// 'reg' and stores the values in 'dest[]'.
void readReg(uint8_t reg, uint8_t count, uint8_t dest[])
{
  i = 0;
  Wire.beginTransmission(ADC_ser_address);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(ADC_ser_address, count);
  while (Wire.available()) {
    dest[i++] = Wire.read();
  }
}


// writes registration 'reg' with value 'value'
void writeReg(uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(ADC_ser_address);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}
