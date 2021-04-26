//SPI MASTER (ARDUINO)

#include "rwa_wrap.h"

bool debug_mode = 0;
int16_t time_0;

void setup (void) {
  delay(100);
  
  Serial.begin(115200);

  rwaArduinoBoot();   

  delay(100);

  systemIden(&rw1);

  Serial.println("--- --- setup complete --- ---");
}

void loop(void) { 
//  rw1.reqSpeed = 0;
//  rw1.rampTime = 10;
//    
//  commandAll(6);
  
  delay(1000);
}

void systemIden(struct rw_data *rwX_pt){
  debug_mode = 0;
  
  rw1.reqSpeed = 0;
  rw1.rampTime = 10;
  commandAll(6);

  rw1.reqClcMode = 0;
  commandAll(7);

  delay(10000);

  Serial.print("currSpeed:\t");
  Serial.println(rwX_pt->currSpeed);
  Serial.print("currClcMode:\t");
  Serial.println(rwX_pt->currClcMode);
  Serial.println("--- --- starting system identification --- ---");

  delay(1000);
  
  rw1.reqSpeed = 10000;
  rw1.rampTime = 10;

  time_0 = millis();
  commandAll(6);

  uint8_t rwX_currSpeed_bytes[sizeof(int16_t)] = {0};
  uint8_t rwX_refSpeed_bytes[sizeof(int16_t)] = {0};
  uint8_t rwX_time_N_bytes[sizeof(int16_t)] = {0};
  uint8_t rwX_systemIden_bytes[3*sizeof(int16_t)] = {0};

  for (int ii=0;ii<100;ii++){
    commandAll(4);

//        Serial.print("time_N (ms): ");
//        Serial.println(rw1.time_N);
//        Serial.print("currSpeed:\t");
//        Serial.println(rwX_pt->currSpeed);
//        Serial.print("refSpeed:\t");
//        Serial.println(rwX_pt->refSpeed);
//        Serial.println(" ");


        rwX_pt->currSpeed = 3200;
        rwX_pt->refSpeed = 1800;
        rwX_pt->time_N = 350;
      
        memcpy(&rwX_currSpeed_bytes[0], rwX_pt->currSpeed, sizeof(int16_t));
        memcpy(&rwX_refSpeed_bytes[0], rwX_pt->refSpeed, sizeof(int16_t));
        memcpy(&rwX_time_N_bytes[0], rwX_pt->time_N, sizeof(int16_t));

        memcpy(&rwX_systemIden_bytes[0], &rwX_currSpeed_bytes[0], sizeof(int16_t));
        memcpy(&rwX_systemIden_bytes[0]+sizeof(int16_t), &rwX_refSpeed_bytes[0], sizeof(int16_t));
        memcpy(&rwX_systemIden_bytes[0]+2*sizeof(int16_t), &rwX_time_N_bytes[0], sizeof(int16_t));

        Serial.write(&rwX_currSpeed_bytes[0],1*sizeof(int16_t));
        
    delay(100);
  }
}
