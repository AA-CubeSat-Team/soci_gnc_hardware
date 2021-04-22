
  // photodiode wrapper
  
  #include "phd_wrap.h"
  
  // Initialize ADC
  void initADC(){  
    // Enable Pullup resistors
//    pinMode(4,INPUT_PULLUP);
//    pinMode(5,INPUT_PULLUP);
    Wire.begin();
    
    // STEP 1&2: wait until busy status register is read not busy
    uint8_t c = 1;
    while (c) {
      readRegs(busy_status_reg, 1, &c);
      }
      
    // STEP 3: Choosing internal VREF and mode of operation
    writeReg(advance_config_reg,advance_config_value);
    readRegs(advance_config_reg,1, &advance_config_value);
  
    // STEP 4: Enable Conversion Rate Register
    writeReg(conv_rate_reg,conv_rate_value);  
    readRegs(conv_rate_reg,1,&conv_rate_value);
    
    //STEP 5: Disabling unused inputs
    writeReg(config_reg,config_default_value); // Disabling pins in shutdown mode
    readRegs(config_reg,1,&test);
    writeReg(disable_reg,disable_value);
    readRegs(disable_reg,1,&disable_value);
  
    //STEP 6: Enabling interrupt mask (won't go to INT pin)
    writeReg(config_reg,0b00000001); // Powering ADC back on 
    writeReg(interrupt_mask_reg,interrupt_mask_value);
    readRegs(interrupt_mask_reg,1,&interrupt_mask_value);
    
    //STEP7: Sending Limits for only enable input
    writeReg(in_high_reg1,in_high_val);
    writeReg(in_low_reg1,in_low_val);
    readRegs(in_high_reg1,1,&in_high_val);
    readRegs(in_low_reg1,1,&in_low_val);
    
    in_high_val = 0b101;
    in_low_val = 0;
    writeReg(in_high_reg2,in_high_val);
    writeReg(in_low_reg2,in_low_val);
    readRegs(in_high_reg2,1,&in_high_val);
    readRegs(in_low_reg2,1,&in_low_val);
    
    in_high_val = 0b101;
    in_low_val = 0;
    writeReg(in_high_reg3,in_high_val);
    writeReg(in_low_reg3,in_low_val);
    readRegs(in_high_reg3,1,&in_high_val);
    readRegs(in_low_reg3,1,&in_low_val);
    
    in_high_val = 0b101;
    in_low_val = 0;
    writeReg(in_high_reg4,in_high_val);
    writeReg(in_low_reg4,in_low_val);
    readRegs(in_high_reg4,1,&in_high_val);
    readRegs(in_low_reg4,1,&in_low_val);
    
    in_high_val = 0b101;
    in_low_val = 0;
    writeReg(in_high_reg5,in_high_val);
    writeReg(in_low_reg5,in_low_val);
    readRegs(in_high_reg5,1,&in_high_val);
    readRegs(in_low_reg5,1,&in_low_val); 
      
    //STEP 8: Setting Start bit to 1   
    writeReg(config_reg, start_value);
    readRegs(config_reg, 1, &c);
  
    //STEP 9: Set interrupt to zero
    writeReg(config_reg,int_clear_val);
    readRegs(config_reg,1,&int_clear_val);
  }
  
  
  // Health check: If any channel exceeds the maxV or all 5 channels exceed 3*maxV when summed: 1) resets registers to default values and puts ADC into shutdown mode. 2) Reinitializes ADC to proper configuration (same as quickstart procedure).
  void health() {
    if((current[0] > max_current || current[1] > max_current || current[2] > max_current || current[3] > max_current || current[4] > max_current) || current[0] + current[1] + current[2] + current[3] + current[4] > 3*max_current){
      writeReg(config_reg, 0b01001000);
      
    initADC();
    }
  }
  
  
  // Reads voltages from ADC
  void getVoltage()
  {
    // Recieve data from ADC
    j = 0;
    for(i=0; i<sizeof(PD_reg_array); i++){ 
      Wire.beginTransmission(ADC_ser_address);            
      Wire.write(PD_reg_array[i]);     // Write register address
      Wire.endTransmission(false);
      Wire.requestFrom(ADC_ser_address,uint8_t(2));
      while(Wire.available()){
        PDVol[j] = Wire.read();     // Data byte 1
        PDVol[j+1] = Wire.read();     // Data byte 2
      }
      j = j+2;
    }
    
    // Process Data and hold current 5 values in 'current' buffer
    j = 0;
    for(i=0; i<sizeof(current); i++){
      D_out[i] = PDVol[j]<<8 | PDVol[j+1];
      current[i] = uint16_t((((D_out[i]/4096)*V_ref)/R)*1000);  // current calculated in uA (micro Amps)
      j = j+2;
    }
  }
  
  
  // Reads 'counts' number of values from registration
  // 'reg' and stores the values in 'dest[]'.
  void readRegs(uint8_t reg, uint8_t count, uint8_t dest[])
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
