#include<SPI.h>  

// max length of rplArrCRC_XF = 2*(rplLen_T + 2) + 2
volatile byte reqArrCRC_XF[16] = { };
byte reqArrCRC_X[32] = { };
unsigned long reqArrCRC_T[32] = { };  
    
byte rplArr_T[32] = { }; 
byte rplArrCRC_T[32] = { };
byte rplArrCRC_D[32] = { };
byte rplArrCRC_X[32] = { };
byte rplArrCRC_XF[32] = { };
int rplLenCRC_XF;

int qq;  
int ee;
int ff;

volatile byte reqB_new;
volatile byte reqB_old;
volatile byte rplB;
volatile byte flag;
volatile byte kk;
volatile byte yy;
volatile byte readReg;

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


// CURRENT STATE: sends follow-up reply package based on request contents
// NEXT STEP: 

void setup (void){
  Serial.begin (115200);
  
  pinMode(MISO, OUTPUT);

  SPCR = SPCR | bit(SPE);           // sets SPE bit in SPCR to 1, enabling SPI
  SPCR = SPCR | bit(SPIE);          // sets SPIE bit in SPCR to 1, enabling SPI interrupts
  
  SPDR = 0;
  flag = 0;
  kk = 0;
  yy = 0;
  reqB_old = 126;

  Serial.println("Setup complete");
}  


ISR (SPI_STC_vect){
  reqB_new = SPDR;

  if ( (reqB_old == 126) && (reqB_new == 126) ){      // master querying reply   
    if (yy < rplLenCRC_XF){
      SPDR = rplArrCRC_XF[yy];
      yy++;
    }
    if (yy >= rplLenCRC_XF){
      SPDR = 126;
    }
  }
  if ( (reqB_old == 126) && (reqB_new != 126) ){      // beginning of request
    reqArrCRC_XF[kk] = reqB_old;
    kk++;
    reqArrCRC_XF[kk] = reqB_new;
    kk++;
    SPDR = 126;
  }
  if ( (reqB_old != 126) && (reqB_new != 126) ){      // during request
    reqArrCRC_XF[kk] = reqB_new;
    kk++;
    SPDR = 126;
  }
  if ( (reqB_old != 126) && (reqB_new == 126) ){      // end of request
    SPI.detachInterrupt();   
    reqArrCRC_XF[kk] = reqB_new;
    kk++;       
    flag = 1;
  }
  
  reqB_old = reqB_new;
}

                                     
void loop (void){
  if (flag == 1){
    
    qq = 0;
    int reqLenCRC_XF = kk;

    // copies request array to new array without flags at front and back
    for (int ff = 0; ff < (reqLenCRC_XF - 1); ff++){
      reqArrCRC_X[ff] = reqArrCRC_XF[ff+1];
    }
    int reqLenCRC_X = reqLenCRC_XF - 2;

    // runs XOR process to remove escape bytes from request array
    for (int xx = 0; xx < reqLenCRC_X; xx++){
      if (reqArrCRC_X[xx+qq] != 0x7d){
        reqArrCRC_T[xx] = reqArrCRC_X[xx+qq];
      }
      if (reqArrCRC_X[xx+qq] == 0x7d){
        reqArrCRC_T[xx] = reqArrCRC_X[xx+qq+1]^0x20;
        qq++;
      }
    }
    
    genRpl(reqArrCRC_T[0]);                
    yy = 0;

    Serial.print("reqArrCRC_XF: ");
    for (int jj = 0; jj < sizeof(reqArrCRC_XF); jj++){
      Serial.print(reqArrCRC_XF[jj], HEX);
      Serial.print(" ");
    }
    Serial.println();    
    Serial.print("rplArrCRC_XF: ");
    for (int jj = 0; jj < sizeof(rplArrCRC_XF); jj++){
      Serial.print(rplArrCRC_XF[jj], HEX);
      Serial.print(" ");
    }
    Serial.println();

    // resets rplArr arrays
    for (int jj = 0; jj < sizeof(reqArrCRC_XF); jj++){
      rplArrCRC_T[jj] = 0;
      reqArrCRC_X[jj] = 0;
      reqArrCRC_XF[jj] = 0;
    }

    SPDR = 0;
    flag = false;
    kk = 0;
    reqB_old = 126;
    
    readReg = SPSR;
    readReg = SPDR;
    SPCR = SPCR | bit(SPIE);            
  }
}


void genRpl(byte comID1){ 

  // PROCESS REQUEST DATA --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  long reqRefSpeed;
  int reqRampTime;
  byte reqClcMode;
  
  if (comID1 == 6){
    reqRefSpeed = (reqArrCRC_T[4] << 24) | (reqArrCRC_T[3] << 16) | (reqArrCRC_T[2] << 8) | reqArrCRC_T[1];
    reqRampTime = (reqArrCRC_T[6] << 8) | reqArrCRC_T[5];
  }
  if (comID1 == 7){
    reqClcMode = reqArrCRC_T[1];
  }

  
  // PROCESS REPLY DATA --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  byte rplLastResetStatus;
  long rplCurrSpeed;
  long rplRefSpeed;
  byte rplRwState;
  byte rplClcMode;
  long rplMcuTemp;

  // resets rplArr arrays
  for (int jj = 0; jj < sizeof(rplArrCRC_XF); jj++){
    rplArr_T[jj] = 0;
    rplArrCRC_T[jj] = 0;
    rplArrCRC_D[jj] = 0;
    rplArrCRC_X[jj] = 0;
    rplArrCRC_XF[jj] = 0;
  }

  // set reply values
  rplLastResetStatus = 6;
  rplCurrSpeed = 0.95 * reqRefSpeed;
  rplRefSpeed = reqRefSpeed;
  rplRwState = 4;
  rplClcMode = 1;
  rplMcuTemp = 35;
                       
  // generates reply array contents  
  int rplLen_T;
  
  rplArr_T[0] = comID1;
  rplArr_T[1] = 1;
  switch(comID1) {
    case 1 :
      rplLen_T = 2;
      break;
    case 2 :
      rplArr_T[2] = rplLastResetStatus;
      rplLen_T = 3;
      break;
    case 3 :
      rplLen_T = 2;
      break;
    case 4 :
      rplArr_T[2] = rplCurrSpeed & 0xFF;
      rplArr_T[3] = (rplCurrSpeed >> 8) & 0xFF;
      rplArr_T[4] = (rplCurrSpeed >> 16) & 0xFF;
      rplArr_T[5] = (rplCurrSpeed >> 24) & 0xFF;
      rplArr_T[6] = rplRefSpeed & 0xFF;
      rplArr_T[7] = (rplRefSpeed >> 8) & 0xFF;
      rplArr_T[8] = (rplRefSpeed >> 16) & 0xFF;
      rplArr_T[9] = (rplRefSpeed >> 24) & 0xFF;
      rplArr_T[10] = rplRwState;
      rplArr_T[11] = rplClcMode;
      rplLen_T = 12;
      break;
    case 5 :
      rplLen_T = 2;
      break;
    case 6 :
      rplLen_T = 2;
      break;
    case 7 :
      rplLen_T = 2;
      break;
    case 8 :
      rplArr_T[2] = rplMcuTemp & 0xFF;
      rplArr_T[3] = (rplMcuTemp >> 8) & 0xFF;
      rplArr_T[4] = (rplMcuTemp >> 16) & 0xFF;
      rplArr_T[5] = (rplMcuTemp >> 24) & 0xFF;
      rplLen_T = 6;
      break;
    case 9 :
    
      rplLen_T = 81;
      break;
    case 10 :
      rplLen_T = 2;
      break;
    case 11 :
    
      rplLen_T = 22;
      break;
  }   


  // CRC/XOR/FRAME REPLY ARRAY --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
                                              
  // runs CRC algorithm on rplArr_T, adds CRC bytes on to end
  unsigned int crcValue;
  crcValue = 0xFFFF;
  for (int iterByte = 0; iterByte < rplLen_T; iterByte++){ 
    crcValue = ((crcValue << 8) ^ crcTable[((crcValue >> 8) ^ rplArr_T[iterByte]) & 0x00FF]);
  }
  unsigned int crcSplit[] = {crcValue >> 8, crcValue & 0xFF};
  int rplLenCRC_T = rplLen_T + 2;
  
  int ss = 0;
  for (int cc = 0; cc < rplLenCRC_T; cc++){
    if (cc < rplLen_T){
      rplArrCRC_T[cc] = rplArr_T[cc];
    }
    rplArrCRC_T[rplLen_T] = crcSplit[1];
    rplArrCRC_T[rplLen_T+1] = crcSplit[0];
  }
    
  // runs XOR process to add escape bytes to reply array
  ee = 0;
  ff = 0;                                                  
  for (int tt = 0; tt < rplLenCRC_T; tt++){        
    if (rplArrCRC_T[tt] != 0x7d){
      rplArrCRC_D[tt+ee] = rplArrCRC_T[tt];
    }
    if (rplArrCRC_T[tt] == 0x7d){
      rplArrCRC_D[tt+ee] = 0x7d;
      rplArrCRC_D[tt+ee+1] = rplArrCRC_T[tt]^0x20;
      ee++;
    }
  } 
  int rplLenCRC_D = rplLenCRC_T + ee;                                           
  for (int dd = 0; dd < rplLenCRC_D; dd++){
    if (rplArrCRC_D[dd] != 0x7e){
      rplArrCRC_X[dd+ff] = rplArrCRC_D[dd];
    }
    if (rplArrCRC_D[dd] == 0x7e){   
      rplArrCRC_X[dd+ff] = 0x7d;
      rplArrCRC_X[dd+ff+1] = rplArrCRC_D[dd]^0x20;
      ff++;
    }
  }
  int rplLenCRC_X = rplLenCRC_D + ff;

  // copies reply array to new array with flags at front and back
  rplArrCRC_XF[0] = 0x7e;
  rplArrCRC_XF[rplLenCRC_X+1] = 0x7e;
  for (ff = 0; ff < rplLenCRC_X; ff++){
    rplArrCRC_XF[ff+1] = rplArrCRC_X[ff];
  }
  rplLenCRC_XF = rplLenCRC_X + 2;
}
