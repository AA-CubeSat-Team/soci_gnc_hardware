// RWA Flight Wrapper, Arduino Version

// Arduino to C
//    need to convert to correct data types

// references:
// 

// to do:
//    create rwa_init routine (current limit, etc.)
//    error recovery and re-init mode
//    make interupt based - don't want to run forever
//    test on simpler SPI slave

#include <SPI.h>

struct rwaCall {
  long refSpeed_rw1 = 0x007e007d;
  long refSpeed_rw2 = 12365;
  long refSpeed_rw3 = 9871;
  long refSpeed_rw4 = 8854;
  unsigned short int rampTime_rw1 = 10;
  unsigned short int rampTime_rw2 = 10;
  unsigned short int rampTime_rw3 = 10;
  unsigned short int rampTime_rw4 = 10;
} rwaCall_var;

struct rwaReturn {
  long currSpeed_rw1 = { };
  long currSpeed_rw2 = { };
  long currSpeed_rw3 = { };
  long currSpeed_rw4 = { };
} rwaReturn_var;


// --- --- --- --- --- --- --- --- --- --- --- ---

void setup() {
  pinMode(10, OUTPUT); // set the SS pin as an output
  SPISettings mySetting(200000, MSBFIRST, SPI_MODE0);   // set SPI settings
  SPI.beginTransaction(SPISettings(mySetting));

  Serial.begin (115200);
  Serial.println("Setup complete");

  
  rwaFunc();
}

void rwaFunc() {
  float t0 = micros();
  
  //reqGen(1,6,rwaCall_var);


  while (true){
    // SPI
    int xferArr[] = {2,4,6,8,10,12};
    int xferLen = 6;
  
    Serial.print("reqArr: ");
    for (int xx = 0; xx < xferLen; xx++){
        Serial.print(xferArr[xx], HEX);
        Serial.print(" ");
    }
    Serial.println(" ");
  
    
    digitalWrite(10, LOW);
    SPI.transfer(xferArr, xferLen);
    digitalWrite(10, HIGH);
  
  
    Serial.print("rplArr: ");
    for (int xx = 0; xx < xferLen; xx++){
        Serial.print(xferArr[xx], HEX);
        Serial.print(" ");
    }
    Serial.println(" ");

    delay(1000);
  }


  
  float t1 = micros();
  float runtime = t1-t0;
  Serial.print("runtime: ");
  Serial.println(runtime);
}



// --- --- --- --- --- --- --- --- --- --- --- ---

void loop() {
}

void reqGen(int rwID, int comID, struct rwaCall rwaCall_loc) {
  long refSpeed_loc;
  unsigned short int rampTime_loc;
  unsigned char reqArr_A[32] = { };
  unsigned char reqArr_B[32] = { };
  unsigned char reqArr_C[32] = { };
  unsigned char reqArr_D[32] = { };
  unsigned char reqArr_E[32] = { };
  int reqLen_A;
  int reqLen_B;
  int reqLen_C;
  int reqLen_D;
  int reqLen_E;

  /*Serial.print("rwID: ");
  Serial.println(rwID);
  Serial.print("comID: ");
  Serial.println(comID);*/
  switch(rwID) {
    case 1:
      refSpeed_loc = rwaCall_loc.refSpeed_rw1;
      rampTime_loc = rwaCall_loc.rampTime_rw1; 
      break;
    case 2:
      refSpeed_loc = rwaCall_loc.refSpeed_rw2;
      rampTime_loc = rwaCall_loc.rampTime_rw2;
      break;
    case 3:
      refSpeed_loc = rwaCall_loc.refSpeed_rw3;
      rampTime_loc = rwaCall_loc.rampTime_rw3;
      break;
    case 4:
      refSpeed_loc = rwaCall_loc.refSpeed_rw4;
      rampTime_loc = rwaCall_loc.rampTime_rw4;
      break;
  }
  /*Serial.print("refSpeed: ");
  Serial.println(refSpeed_loc);
  Serial.print("rampTime: ");
  Serial.println(rampTime_loc);*/

  // reqArr_A
  reqArr_A[0] = comID;
  
  switch(comID) {
    case 6:
      reqArr_A[1] = refSpeed_loc & 0xFF;
      reqArr_A[2] = (refSpeed_loc >> 8) & 0xFF;
      reqArr_A[3] = (refSpeed_loc >> 16) & 0xFF;
      reqArr_A[4] = (refSpeed_loc >> 24) & 0xFF; 
      reqArr_A[5] = rampTime_loc & 0xFF;
      reqArr_A[6] = (rampTime_loc >> 8) & 0xFF;
      reqLen_A = 7;
      break;
  }

  /*Serial.print("reqArr_A: ");
  for (int zz = 0; zz < reqLen_A; zz++){
      Serial.print(reqArr_A[zz], HEX);
      Serial.print(" ");
  }
  Serial.println(" ");*/

  // reqArr_B
  unsigned int crcTable[] = {0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7, 
                            0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
                            0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
                            0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
                            0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
                            0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
                            0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
                            0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,   
                            0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
                            0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
                            0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
                            0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
                            0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
                            0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
                            0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
                            0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
                            0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
                            0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
                            0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
                            0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
                            0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
                            0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
                            0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
                            0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
                            0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
                            0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
                            0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
                            0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
                            0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
                            0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
                            0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
                            0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0};

  unsigned int crcValue = 0xFFFF;
  for (int iterByte = 0; iterByte < reqLen_A; iterByte++) { 
    crcValue = ((crcValue << 8) ^ crcTable[((crcValue >> 8) ^ reqArr_A[iterByte]) & 0x00FF]);
  }
  unsigned int crcSplit[] = {crcValue >> 8, crcValue & 0xFF};
  for (int aa = 0; aa < reqLen_A; aa++) {
      reqArr_B[aa] = reqArr_A[aa];
  }
  reqArr_B[reqLen_A] = crcSplit[1];
  reqArr_B[reqLen_A+1] = crcSplit[0];
  reqLen_B = reqLen_A + 2;

  /*Serial.print("reqArr_B: ");
  for (int yy = 0; yy < reqLen_B; yy++){
      Serial.print(reqArr_B[yy], HEX);
      Serial.print(" ");
  }
  Serial.println(" ");*/
  
  // reqArr_C
  int cc = 0;                                                 
  for (int bb = 0; bb < reqLen_B; bb++){        
    if (reqArr_B[bb] != 0x7d){
      reqArr_C[bb+cc] = reqArr_B[bb];
    }
    if (reqArr_B[bb] == 0x7d){
      reqArr_C[bb+cc] = 0x7d;
      reqArr_C[bb+cc+1] = reqArr_B[bb]^0x20;
      cc++;
    }
  }
  reqLen_C = reqLen_B + cc; 

  /*Serial.print("reqArr_C: ");
  for (int xx = 0; xx < reqLen_C; xx++){
      Serial.print(reqArr_C[xx], HEX);
      Serial.print(" ");
  }
  Serial.println(" ");*/

  // reqArr_D
  int ee = 0; 
  for (int dd = 0; dd < reqLen_C; dd++){
    if (reqArr_C[dd] != 0x7e){
      reqArr_D[dd+ee] = reqArr_C[dd];
    }
    if (reqArr_C[dd] == 0x7e){   
      reqArr_D[dd+ee] = 0x7d;
      reqArr_D[dd+ee+1] = reqArr_C[dd]^0x20;
      ee++;
    }
  }
  reqLen_D = reqLen_C + ee; 

  /*Serial.print("reqArr_D: ");
  for (int vv = 0; vv < reqLen_D; vv++){
      Serial.print(reqArr_D[vv], HEX);
      Serial.print(" ");
  }
  Serial.println(" ");*/

  // reqArr_E
  reqArr_E[0] = 0x7e;
  reqArr_E[reqLen_D+1] = 0x7e;
  for (int ff = 0; ff < reqLen_D; ff++){
    reqArr_E[ff+1] = reqArr_D[ff];
  }
  reqLen_E = reqLen_D + 2;
  
  /*Serial.print("reqArr_E: ");
  for (int uu = 0; uu < reqLen_E; uu++){
      Serial.print(reqArr_E[uu], HEX);
      Serial.print(" ");
  }
  Serial.println(" ");*/
  
}
