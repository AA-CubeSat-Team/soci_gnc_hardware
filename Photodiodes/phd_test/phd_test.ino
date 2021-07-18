
#include "phd_wrap.h"

void setup(){
  Serial.begin (115200); //Declare baude rate
//  Serial.println("Start:");
  while (!Serial) {}
  quickStartPhd(&Phd);
}

void loop(){
  readPhdData(&Phd);
  for (int i = 0; i < 5; i++) {
//    Serial.print("Channel ");
////    Serial.print(i+1);
//    Serial.print(":");
    Serial.print(Phd.current[i]);
    Serial.print("  ");
  }
  Serial.println();
}
