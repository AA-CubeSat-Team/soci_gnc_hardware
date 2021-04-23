//SPI MASTER (ARDUINO)

#include "rwa_wrap.h"

void setup (void) {
  delay(100);
  
  Serial.begin(115200);

  rwaArduinoBoot();

  delay(100);

  systemIden(&rw1);
}

void loop(void) { 
  delay(1000);
}

void systemIden(struct rw_data *rwX_pt){
  rw1.reqSpeed = 10000;
  rw1.rampTime = 10;

  commandAll(6);

  for (int ii=0;ii<10;ii++){
    commandAll(4);

    Serial.print("currSpeed:\t");
    Serial.println(rwX_pt->currSpeed);
    Serial.print("refSpeed:\t");
    Serial.println(rwX_pt->refSpeed);
  }
}
