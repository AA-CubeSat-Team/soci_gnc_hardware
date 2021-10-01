// photodiode spoof
#include <Wire.h>

#define PHD_ADC_SER_ADDRESS (uint8_t)0x1D // ADC address

#define PHD1_REG            0x20
#define PHD2_REG            0x21
#define PHD3_REG            0x22
#define PHD4_REG            0x23
#define PHD5_REG            0x24

#define ADVANCE_CONFIG_REG  0x0B
#define CONFIG_REG          0x00
#define CONV_RATE_REG       0x07
#define DISABLE_REG         0x08
#define BUSY_STATUS_REG     0x0C


#define R             10      // Resistance in kilo Ohms
#define V_REF         3.3       // Voltage in V
#define SENSITIVITY   (V_REF/R*1000/4096)

uint8_t advance_config_value = 0;
uint8_t conv_rate_value = 0;
uint8_t disable_value = 0;
uint8_t config_value = 0;
uint8_t start_value = 0;

//uint16_t V_ref = 3.3;
int n = 5;
int bytesPerFloat = 4;
//byte bytesRecieve[20] = {0xc1, 0xa1, 0xa3, 0xe8, 0xc2, 0xba, 0xa8, 0x24, 0xc2, 0xa1, 0x4a, 0xca, 0x41, 0x77, 0x46, 0x87, 0x42, 0xae, 0xa7, 0x0a};
byte bytesRecieve[20] = {0x42, 0xb5, 0x27, 0x2b, 0x42, 0x66, 0x9a, 0xba, 0x42, 0xf0, 0x07, 0x2b, 0x41, 0x77, 0x46, 0x87, 0x42, 0xae, 0xa7, 0x0a};
float phdFloatData[5] = {90, 110, 130, 150, 170};  // Array of spoof outputs
byte phdByteData[10];

uint8_t tempReg = 0; // variable holding the temporary requested register
uint8_t tempValue = 0; // variable holding the temporary value

void setup(){
  Serial.begin(115600);
  // this data processing code will be relocated to the Simulink serial input function, once written
  Wire.begin(PHD_ADC_SER_ADDRESS);      // join i2c bus with address 0x1D
  Wire.onReceive(receiveEventOBC); // sets register event, calls function automatically when master sends data
  Wire.onRequest(requestEventOBC); // sets request event, calls function automatically when master requests data
}

void loop(){
//  for (int ii = 0; ii < 5; ii++) {
//    Serial.print(phdFloatData[ii]);
//    Serial.print(" ");
//  }
//  Serial.println(" ");
  
  serialRecieve(bytesRecieve, n);
  buffer2float(bytesRecieve, phdFloatData, n);
  double2PhdFormat(phdFloatData, phdByteData, n);
  //delay(1000);
}

void receiveEventOBC (int numByte)
{
  //buffer2float(bytesRecieve, phdFloatData, n);
  double2PhdFormat(phdFloatData, phdByteData, n);
  
  if(numByte == 2) {
    tempReg = Wire.read();
    tempValue = Wire.read();
    switch (tempReg) {
      case ADVANCE_CONFIG_REG:
        advance_config_value = tempValue;
        break;
      case CONFIG_REG:
        config_value = tempValue;
        break;
      case CONV_RATE_REG:
        conv_rate_value = tempValue;
        break;
      case DISABLE_REG:
        disable_value = tempValue;
        break;
      case BUSY_STATUS_REG:
        //TODO
        break;
      default:
        // Unknown register stored
        Serial.println("Unknown register stored");
        break;
    }
  } else if(numByte == 1){
//    Serial.println("OBC want to read:");
    tempReg = Wire.read();
//    Serial.println(tempReg, HEX);
  } else {
//    Serial.println("too many in the buffer");
  }
}


void requestEventOBC(){
//  Serial.println("request");
// this function is used when the master uses Wire.requestFrom()
// IFF the wrapper is requesting a value, must first check current register in reg variable, which corresponds to the last sent register
// switch case takes the form:
  // evaluate register previously sent
    // write the value that corresponds to the register
      // else print "unknown register received" 
      
//buffer2float(bytesRecieve, phdFloatData, n);
double2PhdFormat(phdFloatData, phdByteData, n);

switch (tempReg){
  case ADVANCE_CONFIG_REG:
    Wire.write(advance_config_value);
    break;
  case CONV_RATE_REG:
    Wire.write(conv_rate_value);
    break;
  case DISABLE_REG:
    Wire.write(disable_value);
    break;
  case CONFIG_REG:
    Wire.write(config_value);
    break;
  case PHD1_REG:
    Wire.write(&phdByteData[0],2);
    break;
  case PHD2_REG:
    Wire.write(&phdByteData[2],2);
    break;
  case PHD3_REG:
    Wire.write(&phdByteData[4],2);
    break;
  case PHD4_REG:
    Wire.write(&phdByteData[6],2);
    break;
  case PHD5_REG:
    Wire.write(&phdByteData[8],2);
    break;
  default:
    // Unknown register stored
    Serial.println("Unknown register stored");
    break;
  }
}

void serialRecieve(byte * Buffer, int n)
{
  while (Serial.available() < 20){
  }
  Serial.readBytesUntil(0x0A, (char*)Buffer, 20);
  Serial.write((char*)Buffer, 20);
  Serial.write(0x0A);
}

void buffer2float(byte * in_bytes, float * out_Floats, int n)
{
  for (int ii = 0; ii < n; ii++) {
    union {
      byte temp_bytes[4];
      float temp_f;
    } u;
    for (int iii = 0; iii < 4; iii++) {
      u.temp_bytes[3-iii] = in_bytes[iii+4*ii];
    }
    out_Floats[4-ii] = u.temp_f;
  }
}

void double2PhdFormat(float * phdFloatData, uint8_t * phdByteData, int n)
{
  int16_t phdIntData;
  for (int ii = 0; ii < n; ii++) {
    phdIntData = *(phdFloatData + ii)/SENSITIVITY;
    phdIntData = phdIntData << 4;
    *(phdByteData + 2*ii) = (uint8_t)(phdIntData>>8);
    *(phdByteData + 2*ii + 1) = (uint8_t)((phdIntData<<8)>>8);
  }
}


  
