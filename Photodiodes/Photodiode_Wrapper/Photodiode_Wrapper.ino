#include <Wire.h>
uint8_t address = 0x1D; // ADC address
uint8_t PDAdd[] = {0x20,0x21,0x22,0x23,0x24}; // Photodiode 1-5 register address'
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
uint8_t advance_config_reg = 0x0B;
uint8_t advance_config_value = 0b00000000;

// Step 4: Enable Conversion Rate
uint8_t start_reg = 0x00;
uint8_t test;
uint8_t conv_rate_reg = 0x07;
uint8_t conv_rate_value = 0b0000001;

// Step 5: Disable analog inputs (disables IN5,IN6,temp)
uint8_t disable_reg = 0x08;
uint8_t disable_value = 0b11100000; 

// Step 6: Enable interrupt mask 
uint8_t interrupt_mask_reg = 0x03;
uint8_t interrupt_mask_value = 0b00011111;

//Step 7: Program limit registers
//IN0 High limit
uint8_t in_high_reg[] = {0x2A,0x2C,0x2E,0x30,0x32};
uint8_t in_high_val = 0b101 ; // change this (just a test for now)

// IN0 Low limit
uint8_t in_low_reg[] = {0x2B,0x2D,0x2F,0x31,0x33};
uint8_t in_low_val = 0;

// Step 8: Starting the ADC
uint8_t config_reg = 0x00;
uint8_t config_default_value = 0b00001000;
uint8_t start_value = 0b00001001;
uint8_t busy_status_reg = 0x0C;

// Step 9: Setting INT_clear to zero
uint8_t int_clear_reg = 0x00;
uint8_t int_clear_val = 0b00001000;
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

  //_____________________ Quick Start Cont. _______________________________________________________
  // STEP 1&2: wait for at least 33 ms
  uint8_t c = 1;
  while (c) {
    readRegs(busy_status_reg, 1, &c);
    }
    
  // STEP 3: Choosing internal VREF and mode of operation
  writeReg(advance_config_reg,advance_config_value);
  readRegs(advance_config_reg,1, &advance_config_value);
  Serial.println("Step3: operation mode");
  Serial.println(advance_config_value);

  // STEP 4: Enable Conversion Rate Register
  writeReg(conv_rate_reg,conv_rate_value);  
  readRegs(conv_rate_reg,1,&conv_rate_value);
  Serial.println("Step 4b: Conversion Rate value:");
  Serial.println(conv_rate_value);
  
  //STEP 5: Disabling unused inputs
  writeReg(start_reg,config_default_value); // Disabling pins in shutdown mode
  readRegs(start_reg,1,&test);
  writeReg(disable_reg,disable_value);
  readRegs(disable_reg,1,&disable_value);
  Serial.println("Step 5a: Shut down first");
  Serial.println(test,BIN);
  Serial.println("Step5b: Disabling inputs");
  Serial.println(disable_value,BIN);

  //STEP 6: Enabling interrupt mask (won't go to INT pin)
  writeReg(start_reg,0b0000001); // Powering ADC back on 
  writeReg(interrupt_mask_reg,interrupt_mask_value);
  readRegs(interrupt_mask_reg,1,&interrupt_mask_value);
  Serial.println("Enabling interrupt mask");
  Serial.println(interrupt_mask_value);
  
  //STEP7: Sending Limits for only enable input
  writeReg(in_high_reg[0],in_high_val);
  writeReg(in_low_reg[0],in_low_val);
  readRegs(in_high_reg[0],1,&in_high_val);
  readRegs(in_low_reg[0],1,&in_low_val);
  Serial.println("Step7: Limits for IN0");
  Serial.println(in_high_val);
  Serial.println(in_low_val);
  in_high_val = 0b101;
  in_low_val = 0;
  writeReg(in_high_reg[1],in_high_val);
  writeReg(in_low_reg[1],in_low_val);
  readRegs(in_high_reg[1],1,&in_high_val);
  readRegs(in_low_reg[1],1,&in_low_val);
  Serial.println("Step7: Limits for IN1");
  Serial.println(in_high_val);
  Serial.println(in_low_val);
  in_high_val = 0b101;
  in_low_val = 0;
  writeReg(in_high_reg[2],in_high_val);
  writeReg(in_low_reg[2],in_low_val);
  readRegs(in_high_reg[2],1,&in_high_val);
  readRegs(in_low_reg[2],1,&in_low_val);
  Serial.println("Step7: Limits for IN2");
  Serial.println(in_high_val);
  Serial.println(in_low_val);
  in_high_val = 0b101;
  in_low_val = 0;
  writeReg(in_high_reg[3],in_high_val);
  writeReg(in_low_reg[3],in_low_val);
  readRegs(in_high_reg[3],1,&in_high_val);
  readRegs(in_low_reg[3],1,&in_low_val);
  Serial.println("Step7: Limits for IN3");
  Serial.println(in_high_val);
  Serial.println(in_low_val);
  in_high_val = 0b101;
  in_low_val = 0;
  writeReg(in_high_reg[4],in_high_val);
  writeReg(in_low_reg[4],in_low_val);
  readRegs(in_high_reg[4],1,&in_high_val);
  readRegs(in_low_reg[4],1,&in_low_val);
  Serial.println("Step7: Limits for IN4");
  Serial.println(in_high_val);
  Serial.println(in_low_val);    
    
  //STEP 8: Setting Start bit to 1   
  writeReg(config_reg, start_value);
  readRegs(config_reg, 1, &c);
  Serial.println("Setting Start bit to 1");
  Serial.println(c,BIN);

  //STEP 9: Set interrupt to zero
  writeReg(int_clear_reg,int_clear_val);
  readRegs(int_clear_reg,1,&int_clear_val);
  Serial.println("Setting interrupt to zero");
  Serial.println(int_clear_val,BIN);
  Serial.println("------------------------------------------------------------------------------------");
//___________________________________________________________________________________________________________
}



void loop(){
  PD_voltage_read();
  
//  PDVoltage1 = ;
//  PDVoltage2 = ;
//  PDVoltage3 = ;
//  PDVoltage4 = ;
//  PDVoltage5 = ;
  
  health_check();
}



// Health check: If any channel exceeds the maxV or all 5 channels exceed 3*maxV when summed: 1) resets registers to default values and puts ADC into shutdown mode. 2) Reinitializes ADC to proper configuration (same as quickstart procedure).
void health_check()
{
  if((PDVoltage1 > maxV || PDVoltage2 > maxV || PDVoltage3 > maxV || PDVoltage4 > maxV || PDVoltage5 > maxV) || PDVoltage1 + PDVoltage2 + PDVoltage3 + PDVoltage4 + PDVoltage5 > 3*maxV){
    writeReg(config_reg, 0b01001000);
    
  // STEP 1&2: wait for at least 33 ms
      uint8_t c = 1;
  while (c) {
    readRegs(busy_status_reg, 1, &c);
    }
    
  // STEP 3: Choosing internal VREF and mode of operation
  writeReg(advance_config_reg,advance_config_value);
  readRegs(advance_config_reg,1, &advance_config_value);
  Serial.println("Step3: operation mode");
  Serial.println(advance_config_value);

  // STEP 4: Enable Conversion Rate Register
  writeReg(conv_rate_reg,conv_rate_value);  
  readRegs(conv_rate_reg,1,&conv_rate_value);
  Serial.println("Step 4b: Conversion Rate value:");
  Serial.println(conv_rate_value);
  
  //STEP 5: Disabling unused inputs
  writeReg(start_reg,config_default_value); // Disabling pins in shutdown mode
  readRegs(start_reg,1,&test);
  writeReg(disable_reg,disable_value);
  readRegs(disable_reg,1,&disable_value);
  Serial.println("Step 5a: Shut down first");
  Serial.println(test,BIN);
  Serial.println("Step5b: Disabling inputs");
  Serial.println(disable_value,BIN);

  //STEP 6: Enabling interrupt mask (won't go to INT pin)
  writeReg(start_reg,0b0000001); // Powering ADC back on 
  writeReg(interrupt_mask_reg,interrupt_mask_value);
  readRegs(interrupt_mask_reg,1,&interrupt_mask_value);
  Serial.println("Enabling interrupt mask");
  Serial.println(interrupt_mask_value);
  
  //STEP7: Sending Limits for only enable input
    writeReg(in_high_reg[0],in_high_val);
    writeReg(in_low_reg[0],in_low_val);
    readRegs(in_high_reg[0],1,&in_high_val);
    readRegs(in_low_reg[0],1,&in_low_val);
    Serial.println("Step7: Limits for IN0");
    Serial.println(in_high_val);
    Serial.println(in_low_val);
    in_high_val = 0b101;
    in_low_val = 0;
    writeReg(in_high_reg[1],in_high_val);
    writeReg(in_low_reg[1],in_low_val);
    readRegs(in_high_reg[1],1,&in_high_val);
    readRegs(in_low_reg[1],1,&in_low_val);
    Serial.println("Step7: Limits for IN1");
    Serial.println(in_high_val);
    Serial.println(in_low_val);
    in_high_val = 0b101;
    in_low_val = 0;
    writeReg(in_high_reg[2],in_high_val);
    writeReg(in_low_reg[2],in_low_val);
    readRegs(in_high_reg[2],1,&in_high_val);
    readRegs(in_low_reg[2],1,&in_low_val);
    Serial.println("Step7: Limits for IN2");
    Serial.println(in_high_val);
    Serial.println(in_low_val);
    in_high_val = 0b101;
    in_low_val = 0;
    writeReg(in_high_reg[3],in_high_val);
    writeReg(in_low_reg[3],in_low_val);
    readRegs(in_high_reg[3],1,&in_high_val);
    readRegs(in_low_reg[3],1,&in_low_val);
    Serial.println("Step7: Limits for IN3");
    Serial.println(in_high_val);
    Serial.println(in_low_val);
    in_high_val = 0b101;
    in_low_val = 0;
    writeReg(in_high_reg[4],in_high_val);
    writeReg(in_low_reg[4],in_low_val);
    readRegs(in_high_reg[4],1,&in_high_val);
    readRegs(in_low_reg[4],1,&in_low_val);
    Serial.println("Step7: Limits for IN4");
    Serial.println(in_high_val);
    Serial.println(in_low_val);    
    
  //STEP 8: Setting Start bit to 1   
  writeReg(config_reg, start_value);
  readRegs(config_reg, 1, &c);
  Serial.println("Setting Start bit to 1");
  Serial.println(c,BIN);

  //STEP 9: Set interrupt to zero
  writeReg(int_clear_reg,int_clear_val);
  readRegs(int_clear_reg,1,&int_clear_val);
  Serial.println("Setting interrupt to zero");
  Serial.println(int_clear_val,BIN);
  Serial.println("------------------------------------------------------------------------------------"); 
  }
}


// Reads voltages from ADC
void PD_voltage_read()
{
  j = 0;
  for(i=0; i<sizeof(PDAdd); i++){ 
    Wire.beginTransmission(address);
    Wire.write(PDAdd[i]);     // Write register address
    Wire.endTransmission(false);
    delay(80);    // Delay allowing for completion of conversion (~60ms for 5 voltage channels)
    Wire.requestFrom(address,uint8_t(2));
    while(Wire.available()){
      PDVol[j] = Wire.read();     // Data byte 1
      PDVol[j+1] = Wire.read();     // Data byte 2
    }
    j = j+2;
  }
  Serial.println("Unprocessed PD Voltage data:");
  Serial.println(PDVol[0]);
  Serial.println(PDVol[1]);
  
  PDVoltage1 = PDVol[0]<<8 | PDVol[1];
  PDVoltage2 = PDVol[2]<<8 | PDVol[3];
  PDVoltage3 = PDVol[4]<<8 | PDVol[5];
  PDVoltage4 = PDVol[6]<<8 | PDVol[7];
  PDVoltage5 = PDVol[8]<<8 | PDVol[9];
}


// Reads 'counts' number of values from registration
// 'reg' and stores the values in 'dest[]'.
void readRegs(uint8_t reg, uint8_t count, uint8_t dest[])
{
  i = 0;
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(address, count);
  while (Wire.available()) {
    dest[i++] = Wire.read();
  }
}


// writes registration 'reg' with value 'value'
void writeReg(uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}
