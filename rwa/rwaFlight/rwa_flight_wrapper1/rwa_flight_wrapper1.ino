//SPI MASTER (ARDUINO)

/* testing checklist:
    - signal voltages don't exceed 3.3V
    - wires are connected to correct ports
    - wires do not come into contact (short)
*/

/* issues:
 * make reply payload extraction more robust
 */
 
#define DEBUG_MODE 0
#define PRINT_RESULTS 0

#include <SPI.h>
#include "rwa_command_functions1.h"

void setup (void) {
  Serial.begin(115200);

  // pulls SS1,SS2,SS3,SS4 HIGH (disengaged)
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

  Serial.println("--- --- master setup complete --- ---");
  delay(50);
}


void loop(void) {
  Serial.println(" ");
  Serial.println("loop start");

  rw1.reqSpeed = 10000;
  rw1.rampTime = 10;
  
  commandAll_10ping();
//  commandAll_6speed();


  
  delay(3000);
}
