#include <Wire.h>

void setup()  
{ 
  Wire.begin(0x48);               // set address for feedback
  Wire.onReceive(receiveEvent);   // register event
  Serial.begin(115200);           // start serial for output
}

void loop()
{
  delay(100);  
}

float bytesToFloat(uint8_t inBytes[5])
{
  float fOut;
  uint8_t outBytes[4];
  for (int i = 0; i < 4; i++) {
    outBytes[i] = inBytes[i+1];
  }
  memcpy(&fOut, &outBytes, sizeof(fOut));
  return fOut;
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  uint8_t byteArray[howMany];
  int i = 0;
  while(Wire.available()) // loop through all but the last
  {
    uint8_t b = Wire.read(); // receive byte as a character
    if (b != 0xF7) {  // exclude reg
      byteArray[i] = b;
      i += 1;
    }
//    Serial.println(b, HEX);
//    if (i > howMany-1) {
//      break;
//    }
  }
  
  Serial.write(byteArray, howMany);

//  for (int ii = 0; ii < howMany; ii++) {
//    byteArray[ii] = 0;
//  }
  
//  for (int i = 0; i < howMany; i++) {
//    Serial.println(byteArray[i], HEX);
//  }
//  Serial.println(" ");
  
  //Serial.print(byteArray, HEX);
  //Serial.print(" ");
  //Serial.println(bytesToFloat(byteArray));
}
