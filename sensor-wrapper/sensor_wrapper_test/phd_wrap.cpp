
  // photodiode wrapper

  /* issues:
   * albedo from Earth may mess up 3*maxV health check
   */
  
  #include "phd_wrap.h"

  // diode struct
  diodes_t Diodes;
  
  // Initialize ADC
  void initADC(){  
    // Enable pullup resistors
    pinMode(4,INPUT_PULLUP);
    pinMode(5,INPUT_PULLUP);
    Wire.begin();
    
    // STEP 1&2: wait until busy status register is read not busy
    uint8_t c = 1;
    while (c) {
      readRegs(BUSY_STATUS_REG, 1, &c);
    }
      
    // STEP 3: choosing internal VREF and mode of operation
    writeReg(ADVANCE_CONFIG_REG,advance_config_value);
    readRegs(ADVANCE_CONFIG_REG,1, &advance_config_value);
  
    // STEP 4: enabling Conversion Rate Register
    writeReg(CONV_RATE_REG,conv_rate_value);  
    readRegs(CONV_RATE_REG,1,&conv_rate_value);
    
    //STEP 5: disabling unused inputs
    writeReg(CONFIG_REG,config_default_value); // Disabling pins in shutdown mode
    readRegs(CONFIG_REG,1,&test);
    writeReg(DISABLE_REG,disable_value);
    readRegs(DISABLE_REG,1,&disable_value);
  
    //STEP 6: enabling interrupt mask (won't go to INT pin)
    writeReg(CONFIG_REG,0b00000001); // Powering ADC back on 
    writeReg(INTERRUPT_MASK_REG,interrupt_mask_value);
    readRegs(INTERRUPT_MASK_REG,1,&interrupt_mask_value);
    
    //STEP7: sending Limits for only enable input
    writeReg(IN_HIGH_REG1,in_high_val);
    writeReg(IN_LOW_REG1,in_low_val);
    readRegs(IN_HIGH_REG1,1,&in_high_val);
    readRegs(IN_LOW_REG1,1,&in_low_val);
    
    in_high_val = 0b101;
    in_low_val = 0;
    writeReg(IN_HIGH_REG2,in_high_val);
    writeReg(IN_LOW_REG2,in_low_val);
    readRegs(IN_HIGH_REG2,1,&in_high_val);
    readRegs(IN_LOW_REG2,1,&in_low_val);
    
    in_high_val = 0b101;
    in_low_val = 0;
    writeReg(IN_HIGH_REG3,in_high_val);
    writeReg(IN_LOW_REG3,in_low_val);
    readRegs(IN_HIGH_REG3,1,&in_high_val);
    readRegs(IN_LOW_REG3,1,&in_low_val);
    
    in_high_val = 0b101;
    in_low_val = 0;
    writeReg(IN_HIGH_REG4,in_high_val);
    writeReg(IN_LOW_REG4,in_low_val);
    readRegs(IN_HIGH_REG4,1,&in_high_val);
    readRegs(IN_LOW_REG4,1,&in_low_val);
    
    in_high_val = 0b101;
    in_low_val = 0;
    writeReg(IN_HIGH_REG5,in_high_val);
    writeReg(IN_LOW_REG5,in_low_val);
    readRegs(IN_HIGH_REG5,1,&in_high_val);
    readRegs(IN_LOW_REG5,1,&in_low_val); 
      
    //STEP 8: setting Start bit to 1   
    writeReg(CONFIG_REG, start_value);
    readRegs(CONFIG_REG, 1, &c);
  
    //STEP 9: setting interrupt to zero
    writeReg(CONFIG_REG,int_clear_val);
    readRegs(CONFIG_REG,1,&int_clear_val);
  }
  
  
  // health check: if any channel exceeds the maxV or all 5 channels exceed 3*maxV (only three side of satellite can face sun) when summed: 
  // 1) resets registers to default values and puts ADC into shutdown mode. 
  // 2) reinitializes ADC to proper configuration (same as quickstart procedure).
  void health(){
    if((current[0] > MAX_CURRENT || current[1] > MAX_CURRENT || current[2] > MAX_CURRENT || current[3] > MAX_CURRENT || current[4] > MAX_CURRENT) || current[0] + current[1] + current[2] + current[3] + current[4] > 3*MAX_CURRENT){
      writeReg(CONFIG_REG, 0b01001000);
      
    initADC();
    }
  }
  
  
  // reads voltages from ADC
  void getVoltage(diodes_t * Diodes){
    // recieve data from ADC
    j = 0;
    for(i=0; i<sizeof(PD_REG_ARRAY); i++){ 
      Wire.beginTransmission(ADC_SER_ADDRESS);            
      Wire.write(PD_REG_ARRAY[i]);     // write register address
      Wire.endTransmission(false);
      
      Wire.requestFrom(ADC_SER_ADDRESS,uint8_t(2));
      while(Wire.available()){
        PDVol[j] = Wire.read();     // data byte 1
        PDVol[j+1] = Wire.read();     // data byte 2
      }
      j = j+2;
    }
    
    // process data and hold current 5 values in 'current' buffer
    j = 0;
    for(i=0;i<5;i++){
      D_out[i] = PDVol[j]<<8 | PDVol[j+1];
      j = j+2;
    }

    Diodes->current_pos_x = uint16_t((((D_out[0]/4096)*V_REF)/R)*1000); // current calculated in uA (micro Amps)
    Diodes->current_neg_x = uint16_t((((D_out[3]/4096)*V_REF)/R)*1000);
    Diodes->current_pos_y = uint16_t((((D_out[1]/4096)*V_REF)/R)*1000);
    Diodes->current_neg_y = uint16_t((((D_out[2]/4096)*V_REF)/R)*1000);
    Diodes->current_pos_z = uint16_t((((D_out[4]/4096)*V_REF)/R)*1000);
  }
  
  
  // reads 'counts' number of values from registration 'reg' and stores the values in 'dest[]'
  void readRegs(uint8_t reg, uint8_t count, uint8_t dest[]){
    i = 0;
    Wire.beginTransmission(ADC_SER_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission(false);
    
    Wire.requestFrom(ADC_SER_ADDRESS, count);
    while (Wire.available()) {
      dest[i++] = Wire.read();    // ISSUE: does this work? should write to a pointer
    }
  }
  
  
  // writes registration 'reg' with value 'value'
  void writeReg(uint8_t reg, uint8_t value){
    Wire.beginTransmission(ADC_SER_ADDRESS);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
  }
