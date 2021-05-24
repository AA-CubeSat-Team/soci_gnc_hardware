// ACTUATOR FEEDBACK SLAVE (ARDUINO) - FLATSAT CODE

// Due: receives current actuator values from OBC over I2C (as slave), transmits to Simulink over UART

#include <Wire.h>

#define ACT_DUE_ADDRESS 0x18
#define DOUBLE_SIZE 8

uint8_t rwa_currSpeed_bytes[4*DOUBLE_SIZE] = {0};
uint8_t mtq_dipole_bytes[3*DOUBLE_SIZE] = {0};
uint8_t act_feedback_bytes[7*DOUBLE_SIZE] = {0};

void setup(){
  Serial.begin(115200);
  
  Wire.begin(ACT_DUE_ADDRESS);
  Wire.onReceive(receiveEvent);
}

void loop(){
  memcpy(&act_feedback_bytes[0],&rwa_currSpeed_bytes[0],4*DOUBLE_SIZE);
  memcpy(&act_feedback_bytes[0]+4*DOUBLE_SIZE,&mtq_dipole_bytes[0],3*DOUBLE_SIZE);
  
  Serial.write(&act_feedback_bytes[0],7*DOUBLE_SIZE);
          
  delay(1000);
}

void receiveEvent(int howMany){
  int rcv = 0;
  if (howMany == 4*DOUBLE_SIZE){
    while(Wire.available() > 0){
      rwa_currSpeed_bytes[rcv] = Wire.read(); 
      rcv++;
    }
  }
  if (howMany == 3*DOUBLE_SIZE){
    while(Wire.available() > 0){
      mtq_dipole_bytes[rcv] = Wire.read(); 
      rcv++;
    }
  }
}
