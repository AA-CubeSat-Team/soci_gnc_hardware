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
float phdFloatData[5];  // Array of spoof outputs
uint8_t phdByteData[10];

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
  serialRecieve(phdFloatData, n);
  double2PhdFormat(phdFloatData, phdByteData, n);
}

void receiveEventOBC (int numByte)
{
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
// this function is used when the master uses Wire.requestFrom()
// IFF the wrapper is requesting a value, must first check current register in reg variable, which corresponds to the last sent register
// switch case takes the form:
  // evaluate register previously sent
    // write the value that corresponds to the register
      // else print "unknown register received" 

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

void serialRecieve(float * floatBuffer, int n)
{
  while (Serial.available() < n*sizeof(float)){
  }
  Serial.readBytes((char*)floatBuffer,n*sizeof(float));
  Serial.write((char*)floatBuffer,n*sizeof(float));
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


  
