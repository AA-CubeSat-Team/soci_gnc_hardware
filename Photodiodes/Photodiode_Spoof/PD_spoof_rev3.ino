  
// photodiode spoof

#include <Wire.h>


// Step 3: Configuration 
#define advance_config_reg 0x0B
#define advance_config_value 0b00000000

// Step 4: Enable Conversion Rate
#define start_reg 0x00
uint8_t test;
#define conv_rate_reg 0x07
uint8_t conv_rate_value  = 0b0000001;

// Step 5: Disable analog inputs (disables IN5,IN6,temp)
#define disable_reg 0x08
#define disable_value 0xE0

// Step 6: Enable interrupt mask 
#define interrupt_mask_reg 0x03
uint8_t interrupt_mask_value = 0b00011111;

//Step 7: Program limit registers
//IN0 High limit
uint8_t in_high_reg[] = {0x2A,0x2C,0x2E,0x30,0x32};
uint8_t in_high_val = 0b101 ; // change this (just a test for now)

// IN0 Low limit
uint8_t in_low_reg[] = {0x2B,0x2D,0x2F,0x31,0x33};
uint8_t in_low_val = 0;

// Step 8: Starting the ADC
#define config_reg 0x00
uint8_t config_default_value = 0b00001000;
uint8_t start_value = 0b00001001;
#define busy_status_reg 0x0C

// Step 9: Setting INT_clear to zero
#define int_clear_reg 0x00
uint8_t int_clear_val = 0b00001000;



// Givens
#define R 10000
#define V_ref 3.3
#define ADC_ser_address 0x1D // ADC address

#define PD1_reg 0x20
#define PD2_reg 0x21
#define PD3_reg 0x22
#define PD4_reg 0x23
#define PD5_reg 0x24

// Function Variables 
double PD_comp;           //array of inputs
uint16_t D_out_12bit[5];  // Array of spoof outputs
uint8_t req_PD_reg;
  
// Test values (replace with Simulink serial input later)
double test_val_1 = .000001;
double test_val_2 = .000002;
double test_val_3 = .000003;
double test_val_4 = .000004;
double test_val_5 = .000005;
double test_val_array[] = {test_val_1,test_val_2,test_val_3,test_val_4,test_val_5};

// define D_out array as a global to be called by the requestEvent() function
uint8_t D_out_byte_array[10] = {0};


void setup(){
  Serial.begin(9600);
  Serial.println("function start");

  // this data processing code will be relocated to the Simulink serial input function, once written
  int ii = 0;
  double dVIN = 0;
  double D_out_64bit = 0;
  uint16_t D_out_16bit = 0;
  
  for(ii=0; ii<5; ii++){
    // split the calculation step into multiple lines for clarity
    // D_out_16bit = desired 12bit value, but more clear that it still takes up 16 bits of storage
    dVIN = test_val_array[ii]*R;
    D_out_64bit = dVIN/V_ref * pow(2,12);
    D_out_16bit = int(D_out_64bit);

    // weird error where the script restarts at this point, can probably ignore it for now

    // Wire.write() requires the data to be split into individual bytes, 
    // so these two lines split each 16 bit D_out into two bytes
    // bytes are arranged in little endian: [PD1_LSB, PD1_MSB, PD2_LSB, PD2_MSB, ...]
    D_out_byte_array[2*ii] = D_out_16bit & 0xFF;
    D_out_byte_array[2*ii+1] = (D_out_16bit >> 8) & 0xFF;
  }
  delay(1000);

  // printing out D_out bytes for demonstration
  Serial.print("PD1 D_out:\t");
  Serial.print(D_out_byte_array[1],HEX);
  Serial.println(D_out_byte_array[0],HEX);

  Serial.print("PD2 D_out:\t");
  Serial.print(D_out_byte_array[3],HEX);
  Serial.println(D_out_byte_array[2],HEX);

  Serial.print("PD3 D_out:\t");
  Serial.print(D_out_byte_array[5],HEX);
  Serial.println(D_out_byte_array[4],HEX);

  Serial.print("PD4 D_out:\t");
  Serial.print(D_out_byte_array[7],HEX);
  Serial.println(D_out_byte_array[6],HEX);

  Serial.print("PD5 D_out:\t");
  Serial.print(D_out_byte_array[9],HEX);
  Serial.println(D_out_byte_array[8],HEX);

  Wire.begin(ADC_ser_address);      // join i2c bus with address 0x1D
  Wire.onReceive(receiveEvent); // sets register event, calls function automatically when master sends data
  Wire.onRequest(requestEvent); // sets request event, calls function automatically when master requests data
}

          
void loop(){
  // arbitrary, slave just waits for master to send/request data
  delay(100);
}


void receiveEvent(int num_bytes){
// this function is used when the master uses Wire.beginTransmission()->Wire.write()->Wire.endTransmission()
  
  // need to write code to process/sort values sent by master, this is a very rough draft
  uint8_t received_array[3] = {0};    // creates array of three 0 that is reset every loop
  int ii = 0;
  while(Wire.available() > 0){ // loop through all but the last
    received_array[ii] = Wire.read(); // receive byte as a character
    ii++;
  }

  
  
}


void requestEvent(){
// this function is used when the master uses Wire.requestFrom()

  // need a higher level switch case to respond to all different requests



switch (var){ // where var will be the data  sent by the master 
  case advance_config_reg
    Wire.write(advance_config_value);
    break;
  case conv_rate_reg
    Wire.write(conv_rate_value);
    break;
  case disable_reg
    Wire.write(disable_value);
    break;
  case interrupt_mask_reg
    Wire.write(interrupt_mask_value);
    break;
  case in_high_reg
    switch (in_high_reg){
      case 0x2A
        Wire.write(in_high_val);
        break;
      case 0x2C
        Wire.write(in_high_val);
        break;
      case 0x2E
        Wire.write(in_high_val);
        break;
      case 0x30
        Wire.write(in_high_val);
        break;
      case 0x32
        Wire.write(in_high_val);
        break;
    }
  case in_low_reg
    switch (in_low_reg){
      case 0x2B
        Wire.write(in_low_val);
        break;
      case 0x2D
        Wire.write(in_low_val);
        break;
      case 0x2F
        Wire.write(in_low_val);
        break;
      case 0x31
        Wire.write(in_low_val);
        break;
      case 0x33
        Wire.write(in_low_val);
        break;
    }
  case config_reg
    Wire.write(config_default_value);
    break;
  case int_clear_reg
    Wire.write(int_clear_val);
    break;
  case PD_vol_request   
    // beginning of channel voltage request code
    // when master wants a channel voltage, it sends reg address then requests reg value (two transfers)
    // request_reg is the register address sent by master in line 214 of Photodiode_Wrapper
    // request_reg needs to be a global passed between written by receiveEvent() and used by requestEvent()
      switch (req_PD_reg){
        case PD1_reg: 
          Wire.write(&D_out_byte_array[0],2);
          break;
       case PD2_reg:
          Wire.write(&D_out_byte_array[2],2);
          break;
       case PD3_reg:
          Wire.write(&D_out_byte_array[4],2);
          break;
       case PD4_reg:
          Wire.write(&D_out_byte_array[6],2);
          break;
       case PD5_reg:
          Wire.write(&D_out_byte_array[8],2);
          break;
    }
}




  
