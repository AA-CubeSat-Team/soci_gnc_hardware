// ACTUATOR FEEDBACK SLAVE (ARDUINO) - FLATSAT CODE

// Due: receives current actuator values from OBC over I2C (as slave), transmits to Simulink over UART

#include <Wire.h>

#define ACT_DUE_ADDRESS 0x18
#define DOUBLE_SIZE 8

void setup(){
  Serial.begin(115200);
  
  Wire.begin(ACT_DUE_ADDRESS);
  Wire.onReceive(receiveEvent);
  
  Serial.println("--- --- slave setup complete --- ---");
  delay(100);
}

void loop(){
  // UART transmit function call (need to turn off interrupts?)
  
  delay(100);

//  if (rcv < DOUBLE_SIZE){
//      rw1_currSpeed_bytes[rw1_ct] = sda_byte;
//      rw1_ct++;
//    }
//    
//    if (rcv >= DOUBLE_SIZE && rcv < 2*DOUBLE_SIZE){
//      rw2_currSpeed_bytes[rw2_ct] = sda_byte;
//      rw2_ct++;
//    }
//
////    if (rcv >= 2*DOUBLE_SIZE && rcv < 3*DOUBLE_SIZE){
////      rw3_currSpeed_bytes[rw3_ct] = sda_byte;
////      rw3_ct++;
////    }
}

void receiveEvent(int howMany){
  uint8_t act_feedback_bytes[7*DOUBLE_SIZE] = {0};
  
  int rcv = 0;
  while(Wire.available() > 0){
    act_feedback_bytes[rcv] = Wire.read(); 
    rcv++;
  }

//          Serial.print("rw1_currSpeed_bytes:\t");
//          for(int ii=0; ii<DOUBLE_SIZE; ii++){
//            Serial.print(rw1_currSpeed_bytes[ii],HEX);
//            Serial.print(" ");
//          }
//          Serial.println(" ");
//        
//          Serial.print("rw2_currSpeed_bytes:\t");
//          for(int ii=0; ii<DOUBLE_SIZE; ii++){
//            Serial.print(rw2_currSpeed_bytes[ii],HEX);
//            Serial.print(" ");
//          }
//          Serial.println(" ");
//
//          Serial.print("rw3_currSpeed_bytes:\t");
//          for(int ii=0; ii<DOUBLE_SIZE; ii++){
//            Serial.print(rw3_currSpeed_bytes[ii],HEX);
//            Serial.print(" ");
//          }
//          Serial.println(" ");
}
