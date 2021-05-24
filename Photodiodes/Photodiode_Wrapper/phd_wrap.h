
  // photodiode wrapper

  #include <Arduino.h>
  #include <Wire.h>
  
  #define ADC_SER_ADDRESS (uint8_t)0x1D // ADC address
  
  // Define all registers:
  #define PD1_REG 0x20
  #define PD2_REG 0x21
  #define PD3_REG 0x22
  #define PD4_REG 0x23
  #define PD5_REG 0x24
  #define ADVANCE_CONFIG_REG 0x0B
  #define CONFIG_REG 0x00
  #define CONV_RATE_REG 0x07
  #define DISABLE_REG 0x08
  #define INTERRUPT_MASK_REG 0x03
  #define BUSY_STATUS_REG 0x0C
  #define IN_HIGH_REG1 0x2A
  #define IN_HIGH_REG2 0x2C
  #define IN_HIGH_REG3 0x2E
  #define IN_HIGH_REG4 0x30
  #define IN_HIGH_REG5 0x32
  #define IN_LOW_REG1 0x2B
  #define IN_LOW_REG2 0x2D
  #define IN_LOW_REG3 0x2F
  #define IN_LOW_REG4 0x31
  #define IN_LOW_REG5 0x33
  
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
  uint8_t PD_REG_ARRAY[] = {PD1_REG, PD2_REG, PD3_REG, PD4_REG, PD5_REG}; // Photodiode 1-5 register address'
  uint16_t PDVol[10];
  uint16_t D_out[5];
  uint16_t current[5];   ////// Current reported to flight software in uA (micro Amps) ///////
  
  // Define values for current calculations:
  #define R 10           // Resistance in kilo Ohms
  #define V_REF 5        // Voltage in V
  #define MAX_CURRENT 170
  
  int i;
  int j;

  /*!
 * @brief Structure contains information about one gyroscope
 *
 */
typedef struct _Diodes
{
  uint16_t current_pos_x;   // ADC channel 0           
  uint16_t current_neg_x;   // ADC channel 3
  uint16_t current_pos_y;   // ADC channel 1        
  uint16_t current_neg_y;   // ADC channel 2
  uint16_t current_pos_z;   // ADC channel 4       
} diodes_t;

extern diodes_t Diodes;                

  void health();
  void getVoltage();
  void readRegs(uint8_t reg, uint8_t count, uint8_t dest[]);
  void writeReg(uint8_t reg, uint8_t value);
