// photodiode spoof
#include <Wire.h>
#include <SoftwareSerial.h>
#define ADC_REG 0x1D
#define ADVANCE_config_reg 0x0B
#define config_reg 0x00
#define CONV_RATE_REG 0x07
#define DISABLE_REG 0x08
#define INTERRUPT_MASK_REG 0x03
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
#define config_reg 0x00
#define BUSY_STATUS_REG 0x0C
#define INT_CLEAR_REG 0x00
#define PD1_REG 0x20
#define PD2_REG 0x21
#define PD3_REG 0x22
#define PD4_REG 0x23
#define PD5_REG 0x24
#define ADC_SER_ADDRESS 0x1D 
#define R 10000

uint8_t var;
uint8_t reg;
uint8_t config_storage_val = 0b00001000;
uint8_t advance_config_value = 0b00000000;
uint8_t conv_rate_value  = 0b0000001;
uint8_t disable_value = 0xE0;
uint8_t interrupt_mask_value = 0b00011111;
uint8_t in_high_val = 0b101;
uint8_t in_low_val = 0;
uint8_t int_clear_val = 0b00001000;

uint16_t V_ref = 3.3;
uint16_t D_out_12bit[5];  // Array of spoof outputs
double dVIN = 0;
double D_out_64bit = 0;
uint16_t D_out_16bit = 0;  
double current_array[4];
// Test values (replace with Simulink serial input later)
//double test_val_1 = .000001;
//double test_val_2 = .000002;
//double test_val_3 = .000003;
//double test_val_4 = .000004;
//double test_val_5 = .000005;
//double test_val_array[] = {test_val_1,test_val_2,test_val_3,test_val_4,test_val_5};

// define D_out array as a global to be called by the requestEvent() function
uint8_t D_out_byte_array[10] = {0};


void setup(){
  Serial.begin(9600);
  // this data processing code will be relocated to the Simulink serial input function, once written
  Wire.begin(ADC_SER_ADDRESS);      // join i2c bus with address 0x1D
  Wire.onReceive(receiveEvent); // sets register event, calls function automatically when master sends data
  Wire.onRequest(requestEvent); // sets request event, calls function automatically when master requests data
}

          
void loop(){
  if (Serial.available()) {
    simulinkRecieve();
  }
}


void simulinkRecieve() {
  int ii = 0;
  uint8_t serial_received[10];    // need to double check size
  
  if (Serial.available() > 0) {
    serial_received[ii] = Serial.read();
    ii++;
  }
     
  for(ii=0; ii<5; ii++){
    // split the calculation step into multiple lines for clarity
    // D_out_16bit = desired 12bit value, but more clear that it still takes up 16 bits of storage
    dVIN = serial_received[ii]*R;
    D_out_64bit = dVIN/V_ref * pow(2,12);
    D_out_16bit = int(D_out_64bit);
    D_out_byte_array[2*ii] = D_out_16bit & 0xFF;
    D_out_byte_array[2*ii+1] = (D_out_16bit >> 8) & 0xFF;
  }
}

    
void receiveEvent(int num_bytes){
// this function is used when the master uses Wire.beginTransmission()->Wire.write()->Wire.endTransmission()
  uint8_t received_array[3] = {0};    // creates array of three 0 that is reset every loop
  int ii = 0;
  while(Wire.available() > 0){ // loop through all but the last
    received_array[ii] = Wire.read(); // receive byte as a character
    ii++;    
  }

// for loop to assess received_array
for(ii=0; ii<3; ii++){
 var = received_array[ii]; 

// Two cases: Wrapper is writing an address or a value. The switch case evaluates possibilities in the form:

// If the received byte is a register, it will be stored in a global variable named reg,
  // otherwise the data byte written is a value being prescribed to the "ADC," 
    // check the current register stored in global variable named reg
      // Once the correct register is determined, the data byte associated with the register is updated to the value written from the wrapper

    switch (var){
      case ADC_REG:
        break;
      case ADVANCE_config_reg:
        reg = var;
        break;
      case CONV_RATE_REG:
        reg = var;
        break;
        // Start Reg
      case DISABLE_REG:
        reg = var;
        break;
      case config_reg:
        reg = var;
        break;
      case INTERRUPT_MASK_REG:
        reg = var;
        break;
      case IN_HIGH_REG1: 
        reg = var;
        break;
      case IN_HIGH_REG2:
        reg = var;
        break;
      case IN_HIGH_REG3:
        reg = var;
        break;
      case IN_HIGH_REG4:
        reg = var;
        break;
      case IN_HIGH_REG5:
        reg = var;
        break;
      case IN_LOW_REG1:
        reg = var;
        break;
      case IN_LOW_REG2:
        reg = var;
        break;
      case IN_LOW_REG3:
        reg = var;
        break;
      case IN_LOW_REG4:
        reg = var;
        break;
      case IN_LOW_REG5:
        reg = var;
        break;
      case PD1_REG:
        reg = var;
        break;
      case PD2_REG:
        reg = var;
        break;
      case PD3_REG:
        reg = var;
        break;
      case PD4_REG:
        reg = var;
        break;
      case PD5_REG:
        reg = var;
        break;
      default:
        switch (reg){
            case ADVANCE_config_reg:
              advance_config_value = var;
              break;
            case CONV_RATE_REG:
              conv_rate_value = var;
            case DISABLE_REG:
              disable_value = var;
              break;
            case config_reg:
              config_storage_val = var;
              break;
            case INTERRUPT_MASK_REG:
              interrupt_mask_value = var;
              break;
            case IN_HIGH_REG1:
              in_high_val = var;
              break;
            case IN_HIGH_REG2:
              in_high_val = var;
              break;
            case IN_HIGH_REG3:
              in_high_val = var;
              break;
            case IN_HIGH_REG4:
              in_high_val = var;
              break;
            case IN_HIGH_REG5:
              in_high_val = var;
              break;
            case IN_LOW_REG1:
              in_low_val = var;
              break;
            case IN_LOW_REG2:
              in_low_val = var;
              break;
            case IN_LOW_REG3:
              in_low_val = var;
              break;
            case IN_LOW_REG4:
              in_low_val = var;
              break;
            case IN_LOW_REG5:
              in_low_val = var;
              break;
            default:
              Serial.println("Unknown Value received"); // print " unknown value received"
              break;
      }
    break;
  }
}  
}


void requestEvent(){
// this function is used when the master uses Wire.requestFrom()
// IFF the wrapper is requesting a value, must first check current register in reg variable, which corresponds to the last sent register
// switch case takes the form:
  // evaluate register previously sent
    // write the value that corresponds to the register
      // else print "unknown register received" 

switch (reg){
  case ADVANCE_config_reg:
    Wire.write(advance_config_value);
    break;
  case CONV_RATE_REG:
    Wire.write(conv_rate_value);
    break;
  case DISABLE_REG:
    Wire.write(disable_value);
    break;
  case config_reg:
    Wire.write(config_storage_val);      
    break;
  case INTERRUPT_MASK_REG:
    Wire.write(interrupt_mask_value);
    break;
  case IN_HIGH_REG1:
    Wire.write(in_high_val);
    break;
  case IN_HIGH_REG2:
    Wire.write(in_high_val);
    break;
  case IN_HIGH_REG3:
    Wire.write(in_high_val);
    break;
  case IN_HIGH_REG4:
    Wire.write(in_high_val);
    break;
  case IN_HIGH_REG5:
    Wire.write(in_high_val);
    break;
  case IN_LOW_REG1:
    Wire.write(in_low_val);
    break;
  case IN_LOW_REG2:
    Wire.write(in_low_val);
    break;
  case IN_LOW_REG3:
    Wire.write(in_low_val);
    break;
  case IN_LOW_REG4:
    Wire.write(in_low_val);
    break;
  case IN_LOW_REG5:
    Wire.write(in_low_val);
    break;
  case PD1_REG:
    Wire.write(&D_out_byte_array[0],2);
    break;
  case PD2_REG:
    Wire.write(&D_out_byte_array[2],2);
    break;
  case PD3_REG:
    Wire.write(&D_out_byte_array[4],2);
    break;
  case PD4_REG:
    Wire.write(&D_out_byte_array[6],2);
    break;
  case PD5_REG:
    Wire.write(&D_out_byte_array[8],2);
    break;
  default:
    // Unknown register stored
    Serial.println("Unknown register stored");
    break;
}
}




  
