//SPI MASTER (ARDUINO)

/* having issue with SRAM usage, need ~600 bytes free for SD card */

#include "rwa_wrap.h"

//#include <SD.h>
//const int chipSelect = 10;
//File dataFile;

//#include <Wire.h>
//#include <Adafruit_INA219.h>
//Adafruit_INA219 ina219;

bool debug_mode = 1;

int16_t time_0;

String dateString = "15162318";


int freeRam() {
  extern int __heap_start,*__brkval;
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int) __brkval);  
}


void setup (void) {
  delay(100);
  
  Serial.begin(115200);

  /* RWA init */
  rwaArduinoBoot();   // What does this do?

  delay(100);

  Serial.print(F("SRAM = "));
  Serial.println(freeRam());
  rw1.reqClcMode = 0;   // sets CLC to low current limit
  commandAll(7);
  Serial.println("setup complete");
  Serial.println("Ping");
  commandAll(10);

  delay(10000);

  Serial.println("End");
 
//  Serial.println("1000");
//  rw1.reqSpeed = 10000;
//  rw1.rampTime = 10;
//  commandAll(6);
//
//  delay(5000);
//
//  Serial.println("2000");
//  rw1.reqSpeed = 20000;
//  rw1.rampTime = 10;
//  commandAll(6);
//
//  delay(5000);
//
//  Serial.println("1000");
//  rw1.reqSpeed = 10000;
//  rw1.rampTime = 10;
//  commandAll(6);
//
//  delay(5000);
//
//  Serial.println("2000");
//  rw1.reqSpeed = 20000;
//  rw1.rampTime = 10;
//  commandAll(6);
//
//  delay(5000);
//  Serial.println("1000");
//  rw1.reqSpeed = 10000;
//  rw1.rampTime = 10;
//  commandAll(6);
//
//  delay(5000);
//
//  Serial.println("2000");
//  rw1.reqSpeed = 20000;
//  rw1.rampTime = 10;
//  commandAll(6);
//
//  delay(5000);
//  
//  Serial.println("1000");
//  rw1.reqSpeed = 10000;
//  rw1.rampTime = 10;
//  commandAll(6);
//
//  delay(5000);
//
//  Serial.println("End");






  
//  rwaSysID(&rw1);
}

void loop(void) {  
  commandAll(10);
   
//  Serial.println(rw1.currSpeed);
  
  delay(3000);
}


void rwaSysID(struct rw_data *rwX_pt){
  debug_mode = 0;

//  /* writing header */
//  String fileString = dateString + ".CSV";
//  String headerString = "entry, time_ms, currSpeed_01rpm, refSpeed_01rpm, busVoltage_V, current_mA, power_mW";
//  dataFile = SD.open(fileString, FILE_WRITE);  
//  if (!dataFile){
//    Serial.println("error opening dataFile"); 
//    while(1);
//  }
//  if (dataFile) {
//    dataFile.println(headerString);
//    dataFile.close();
//  }

  /* prepping test */
  Serial.println("zeroing reaction wheel");
  rw1.reqSpeed = 0;     // sets RPM to 0
  rw1.rampTime = 10;
  commandAll(6);

  rw1.reqClcMode = 0;   // sets CLC to low current limit
  commandAll(7);

  float busVoltage_V = 0;
  float current_mA = 0;
  float power_mW = 0;

  delay(10000);
  Serial.println("starting system identification");

  delay(1000);

  /* starting test */
  rw1.reqSpeed = 10000;
  rw1.rampTime = 10;

  time_0 = millis();
  commandAll(6);

  for (int ii=0;ii<100;ii++){
    commandAll(4);

//    busVoltage_V = ina219.getBusVoltage_V();
//    current_mA = ina219.getCurrent_mA();
//    power_mW = ina219.getPower_mW();

//    /* data recording */
//    String dataString = String(ii) + "," + String(rwX_pt->time_N) + "," + String(rwX_pt->currSpeed) + "," + String(rwX_pt->refSpeed)
//                        + "," + String(busVoltage_V) + "," + String(current_mA) + "," + String(power_mW);
//    dataFile = SD.open(fileString, FILE_WRITE);
//    if (!dataFile){
//      Serial.println("error opening dataFile"); 
//      while(1);
//    }
//    if (dataFile) {
//      dataFile.println(dataString);
//      dataFile.close();
//    }
        
    delay(1000);
  }
}

void rwaMagInterference(){
  Serial.println("zeroing reaction wheel");
  rw1.reqClcMode = 0;   // sets CLC to low current limit
  commandAll(7);
  
  rw1.reqSpeed = 0;
  rw1.rampTime = 10;
  commandAll(6);

  delay(10000);
  
  Serial.println("-6500");
  rw1.reqSpeed = -65000;
  rw1.rampTime = 10;
  commandAll(6);

  delay(10000);

  Serial.println("-4500");
  rw1.reqSpeed = -45000;
  rw1.rampTime = 10;
  commandAll(6);

  delay(10000);

  Serial.println("-2500");
  rw1.reqSpeed = -25000;
  rw1.rampTime = 10;
  commandAll(6);

  delay(10000);

  Serial.println("-1000");
  rw1.reqSpeed = -10000;
  rw1.rampTime = 10;
  commandAll(6);

  delay(10000);

  Serial.println("-500");
  rw1.reqSpeed = -5000;
  rw1.rampTime = 10;
  commandAll(6);

  delay(10000);

  Serial.println("500");
  rw1.reqSpeed = 5000;
  rw1.rampTime = 10;
  commandAll(6);

  delay(10000);

  Serial.println("1000");
  rw1.reqSpeed = 10000;
  rw1.rampTime = 10;
  commandAll(6);

  delay(10000);

  Serial.println("2500");
  rw1.reqSpeed = 25000;
  rw1.rampTime = 10;
  commandAll(6);

  delay(10000);

  Serial.println("4500");
  rw1.reqSpeed = 45000;
  rw1.rampTime = 10;
  commandAll(6);

  delay(10000);

  Serial.println("6500");
  rw1.reqSpeed = 65000;
  rw1.rampTime = 10;
  commandAll(6);

  delay(10000);

  Serial.println("test end");
}


void rwaArduinoBoot(){
  // pulls SS1,SS2,SS3,SS4 HIGH
  SPI.begin();
  pinMode(SS1, OUTPUT);
  digitalWrite(SS1, HIGH);
  pinMode(SS2, OUTPUT);
  digitalWrite(SS2, HIGH);
  pinMode(SS3, OUTPUT);
  digitalWrite(SS3, HIGH);
  pinMode(SS4, OUTPUT);
  digitalWrite(SS4, HIGH);

  // powers on RW0-1,2,3,4
  pinMode(EN1, OUTPUT);
  digitalWrite(EN1, HIGH);
  pinMode(EN2, OUTPUT);
  digitalWrite(EN2, HIGH);
  pinMode(EN3, OUTPUT);
  digitalWrite(EN3, HIGH);
  pinMode(EN4, OUTPUT);
  digitalWrite(EN4, HIGH);
}
