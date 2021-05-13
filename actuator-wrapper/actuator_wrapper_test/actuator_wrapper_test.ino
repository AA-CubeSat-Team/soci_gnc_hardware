//SPI MASTER (ARDUINO)

#include "rwa_wrap.h"

#include <SD.h>
const int chipSelect = 10;
File dataFile;

#include "RTClib.h"
RTC_PCF8523 rtc;

#include <Wire.h>
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219;

bool debug_mode = 0;

int16_t time_0;


void setup (void) {
  delay(100);
  
  Serial.begin(115200);

  /* SD card init */
  if (SD.begin(chipSelect)) {
    Serial.println("card initialized");
  }
  if (!SD.begin(chipSelect)) {
    Serial.println("card failed, or not present");
    while (1);
  }

  /* RTC init */
  if (! rtc.begin()) {
    Serial.println("couldn't find RTC");
    Serial.flush();
    abort();
  }
  if (! rtc.initialized() || rtc.lostPower()) {
    Serial.println("RTC is NOT initialized, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  rtc.start();

  /* INA219 init */
  if (! ina219.begin()) {
    Serial.println("failed to find INA219 chip");
    while (1) { delay(10); }
  }
  ina219.setCalibration_32V_1A();
  
  /* RWA init */
  rwaArduinoBoot();   

  delay(100);

  rwaSysID(&rw1);

  Serial.println("setup complete");
}

void loop(void) {   
  delay(1000);
}


void rwaSysID(struct rw_data *rwX_pt){
  debug_mode = 0;

  /* generating dataFile name */
  DateTime now = rtc.now();
  String monthString = (now.month() < 10) ? "0" + String(now.month()) : String(now.month());
  String dayString = (now.day() < 10) ? "0" + String(now.day()) : String(now.day());
  String hourString = (now.hour() < 10) ? "0" + String(now.hour()) : String(now.hour());
  String minuteString = (now.minute() < 10) ? "0" + String(now.minute()) : String(now.minute());
  String fileString = monthString + dayString + hourString + minuteString + ".CSV";
  fileString = "TESTA";

  /* writing header */
  String headerString = "entry, time_ms, currSpeed_01rpm, refSpeed_01rpm, busVoltage_V, current_mA, power_mW";
  dataFile = SD.open(fileString, FILE_WRITE);
  Serial.println(fileString);
  if (!dataFile){
    Serial.println("error opening dataFile"); 
    while(1);
  }
  if (dataFile) {
    dataFile.println(headerString);
    dataFile.close();
  }

  /* prepping test */
  Serial.println("zeroing reaction wheel");
  rw1.reqSpeed = 0;     // sets RPM to 0
  rw1.rampTime = 10;
//  commandAll(6);

  rw1.reqClcMode = 0;   // sets CLC to low current limit
//  commandAll(7);

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
//  commandAll(6);

  for (int ii=0;ii<100;ii++){
//    commandAll(4);

    busVoltage_V = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    power_mW = ina219.getPower_mW();

        rwX_pt->currSpeed = 3470;
        rwX_pt->refSpeed = 3500;
        rwX_pt->time_N = 120;

        Serial.print(String(ii) + "\t");
        Serial.print(String(rw1.time_N) + "\t");
        Serial.print(String(rwX_pt->currSpeed) + "\t");
        Serial.print(String(rwX_pt->refSpeed));

    /* data recording */
    String dataString = String(ii) + "," + String(rwX_pt->time_N) + "," + String(rwX_pt->currSpeed) + "," + String(rwX_pt->refSpeed)
                        + "," + String(busVoltage_V) + "," + String(current_mA) + "," + String(power_mW);
    dataFile = SD.open(fileString, FILE_WRITE);
    if (!dataFile){
      Serial.println("error opening dataFile"); 
      while(1);
    }
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
    }
        
    delay(1000);
  }
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
