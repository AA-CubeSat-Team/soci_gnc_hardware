
  // photodiode wrapper

  #include <Arduino.h>
  #include <Wire.h>
  
  #define ADC_ser_address (uint8_t)0x1D // ADC address
  
  // Define all registers:
  #define PD1_reg 0x20
  #define PD2_reg 0x21
  #define PD3_reg 0x22
  #define PD4_reg 0x23
  #define PD5_reg 0x24
  #define advance_config_reg 0x0B
  #define config_reg 0x00
  #define conv_rate_reg 0x07
  #define disable_reg 0x08
  #define interrupt_mask_reg 0x03
  #define busy_status_reg 0x0C
  #define in_high_reg1 0x2A
  #define in_high_reg2 0x2C
  #define in_high_reg3 0x2E
  #define in_high_reg4 0x30
  #define in_high_reg5 0x32
  #define in_low_reg1 0x2B
  #define in_low_reg2 0x2D
  #define in_low_reg3 0x2F
  #define in_low_reg4 0x31
  #define in_low_reg5 0x33
  
  // Initialize register values to be used in ADC initialization:
  uint8_t advance_config_value = 0b00000001;
  uint8_t test;
  uint8_t conv_rate_value = 0b00000001;
  uint8_t disable_value = 0b11100000;
  uint8_t interrupt_mask_value = 0b00011111;
  uint8_t in_high_val = 0b101; // change this (just a test for now)
  uint8_t in_low_val = 0;
  uint8_t config_default_value = 0b00001000;
  uint8_t start_value = 0b00001001;
  uint8_t int_clear_val = 0b00000001;
  
  // Initialize PD register array and D_out variables:
  uint8_t PD_reg_array[] = {PD1_reg,PD2_reg,PD3_reg,PD4_reg,PD5_reg}; // Photodiode 1-5 register address'
  uint16_t PDVol[10];
  uint16_t D_out[5];
  uint16_t current[5];   ////// Current reported to flight software in uA (micro Amps) ///////
  
  // Define values for current calculations:
  #define R 10           // Resistance in kilo Ohms
  #define V_ref 5        // Voltage in V
  #define max_current 170
  
  int i;
  int j;

  void health();
  void getVoltage();
  void readRegs(uint8_t reg, uint8_t count, uint8_t dest[]);
  void writeReg(uint8_t reg, uint8_t value);
