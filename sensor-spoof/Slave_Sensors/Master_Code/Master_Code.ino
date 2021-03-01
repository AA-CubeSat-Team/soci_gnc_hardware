// Master Code Simulink

#include <Wire.h>
#define SLAVE_ADDR1 4
#define SLAVE_ADDR2 5
#define ANSWERSIZE num

void setup()
{
  //Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
}

void loop()
{
 int response; //integer passed from masterleft
 Wire.begin();
 delay(250);
 Wire.requestFrom(SLAVE_ADDR1, 1); //pulls data from available wire from slave
 response = Wire.read();
 Serial.print("Value from Slave 1: ");
 Serial.println(response);
 Wire.endTransmission();
 delay(250);
 Wire.beginTransmission(SLAVE_ADDR2);
 Wire.write(response + 1); //pulls data from available wire from slave
 Wire.endTransmission();

}
