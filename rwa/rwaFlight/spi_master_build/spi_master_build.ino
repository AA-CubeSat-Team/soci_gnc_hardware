//SPI MASTER (ARDUINO)

/* testing checklist
    - signal voltages don't exceed 3.3V
    - wires are connected to correct ports
    - wires do not come into contact (short)
*/

#include<SPI.h>

// assigns GPIO pins to be slave-selects
int SS1 = 1;
int SS2 = 2;
int SS3 = 3;
int SS4 = 4;

byte spi_req1_buf[8] = {0}; // creates request array buffer as global variable
byte spi_req2_buf[8] = {0}; // need to allocate max possible size of bytes
byte spi_req3_buf[8] = {0};
byte spi_req4_buf[8] = {0};
int spi_req1_buf_len;
int spi_req2_buf_len;
int spi_req3_buf_len;
int spi_req4_buf_len;
byte req_array_A[16] = {0}; // need to allocate max possible size of bytes
byte req_array_B[16] = {0};
byte req_array_C[16] = {0};
byte req_array_D[16] = {0};
byte req_array_E[16] = {0};
int req_len_A;
int req_len_B;
int req_len_C;
int req_len_D;
int req_len_E;

byte spi_rpl1_buf[8] = {0}; // need to allocate max possible size of bytes
byte spi_rpl2_buf[8] = {0};
byte spi_rpl3_buf[8] = {0};
byte spi_rpl4_buf[8] = {0};
int spi_rpl1_buf_len;
int spi_rpl2_buf_len;
int spi_rpl3_buf_len;
int spi_rpl4_buf_len;
byte rpl_array_Z[16] = {0}; // need to allocate max possible size of bytes
byte rpl_array_Y[16] = {0};
byte rpl_array_X[16] = {0};
byte rpl_array_W[16] = {0};
int rpl_len_Z;
int rpl_len_Y;
int rpl_len_X;
int rpl_len_W;


unsigned int crc_value = 0xFFFF;
unsigned int crc_table[] = {0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
                            0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
                            0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
                            0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
                            0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
                            0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
                            0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
                            0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
                            0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
                            0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
                            0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
                            0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
                            0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
                            0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
                            0x7E97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
                            0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
                            0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
                            0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
                            0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
                            0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
                            0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
                            0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
                            0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
                            0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
                            0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
                            0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
                            0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
                            0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
                            0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
                            0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
                            0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
                            0x6e17, 0x7E36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
                           };

void setup (void) {
  Serial.begin(115200);

//  SPI.begin();
  SPI.beginTransaction(SPISettings(200000, MSBFIRST, SPI_MODE0));

  pinMode(SS1, OUTPUT);
  digitalWrite(2, HIGH);

  Serial.println("master setup complete");
}

void loop(void) {
  Serial.println(" ");
  Serial.println("loop start");

  int32_t new_speed= 65000;
  uint16_t new_ramp_time= 3200;

  byte req_payload_rw1[7] = {0};
  int req_payload_len_rw1 = 0;

              Serial.print("req_payload_rw1:\t");
              for (int yy = 0; yy < 7; yy++){
                Serial.print(req_payload_rw1[yy], HEX);
                Serial.print(" ");
              }
              Serial.println(" ");
              delay(500);


//  payloadWrite_cmd6(&req_payload_rw1[0], &req_payload_len_rw1, new_speed, new_ramp_time);
  payloadWrite_cmd4(&req_payload_rw1[0], &req_payload_len_rw1);

              Serial.print("req_payload_rw1:\t");
              for (int yy = 0; yy < req_payload_len_rw1; yy++){
                Serial.print(req_payload_rw1[yy], HEX);
                Serial.print(" ");
              }
              Serial.println(" ");
              delay(500);
              

  spiSendReq(&req_payload_rw1[0],req_payload_len_rw1,1);
  
  delay(5000);
}

void payloadWrite_cmd4(byte *req_payload_pt, int *req_payload_len_pt) {
  // get reaction wheel status
  byte com_id = 4;

  *req_payload_pt = com_id;

  *req_payload_len_pt = 1;
}


void payloadWrite_cmd6(byte *req_payload_pt, int *req_payload_len_pt, int32_t speed, uint16_t ramp_time) {
  // set reference speed
  byte com_id = 6;

  byte speed0 = speed & 0xFF;
  byte speed1 = (speed >> 8) & 0xFF;
  byte speed2 = (speed >> 16) & 0xFF;
  byte speed3 = (speed >> 24) & 0xFF;

  byte ramp_time0 = ramp_time & 0xFF;
  byte ramp_time1 = (ramp_time >> 8) & 0xFF; 

  *req_payload_pt = com_id;
  
  *(req_payload_pt+1) = speed0;
  *(req_payload_pt+2) = speed1;
  *(req_payload_pt+3) = speed2;
  *(req_payload_pt+4) = speed3;
  
  *(req_payload_pt+5) = ramp_time0;
  *(req_payload_pt+6) = ramp_time1;

  *req_payload_len_pt = 1 + 6;
}


void payloadWrite_cmd10(byte *req_payload_pt, int *req_payload_len_pt) {
  // ping
  byte com_id = 10;

  *req_payload_pt = com_id;

  *req_payload_len_pt = 1;
}

void spiQueryRpl(byte *spi_rpl_buf_pt, int spi_rpl_buf_len, int rw_id) { //(NEEDS WORK) --- --- --- --- --- --- --- --- ---

  // creates empty flag array as local variable
  byte flag_array[8] = {0}; // need to allocate max possible size of bytes
  for (int ii = 0; ii < 8; ii++) {
    flag_array[ii] = 0x7E;
  }

  // assigns slave select pin to corresponding reaction wheel (WORKS)
  int SS_id;
  switch (rw_id) {
    case 1:
      SS_id = SS1;
      break;
    case 2:
      SS_id = SS2;
      break;
    case 3:
      SS_id = SS3;
      break;
    case 4:
      SS_id = SS4;
      break;
  }

  // assigning back to original buffer variable
  for (int ii = 0; ii < 8; ii++) {
    *(spi_rpl_buf_pt + ii) = flag_array[ii];
  }
  spi_rpl_buf_len = 8;

  //            Serial.print("spi_rpl_buf_pt (MOSI):\t");
  //            for (int yy = 0; yy < spi_rpl_buf_len; yy++){
  //                Serial.print(spi_rpl_buf_pt[yy], HEX);
  //                Serial.print(" ");
  //            }
  //            Serial.println(" ");

  digitalWrite(SS_id, LOW);
  SPI.transfer(spi_rpl_buf_pt, spi_rpl_buf_len);
  digitalWrite(SS_id, HIGH);


  // buf to Z – copying buffer array (WORKS)
  int ii;

  for (ii = 0; ii < spi_rpl_buf_len; ii++) {
    rpl_array_Z[ii] = *(spi_rpl_buf_pt + ii);
  }
  rpl_len_Z = spi_rpl_buf_len;

  byte rpl_array_T[] = {0x7E, 0x03, 0x01, 0x4F, 0x5D, 0x86, 0xBE, 0x7E};
  int rpl_len_T = sizeof(rpl_array_T);

            Serial.print("rpl_array_T:\t");
            for (int yy = 0; yy < rpl_len_T; yy++) {
              Serial.print(rpl_array_T[yy], HEX);
              Serial.print(" ");
            }
            Serial.println(" ");


  // Z to Y – copies request array to new array without frame flags (WORKS)
  for (ii = 0; ii < (rpl_len_T - 1); ii++) {
    rpl_array_Y[ii] = rpl_array_T[ii + 1];
  }
  rpl_len_Y = rpl_len_T - 2;

//            Serial.print("rpl_array_Y:\t");
//            for (int yy = 0; yy < rpl_len_Y; yy++) {
//              Serial.print(rpl_array_Y[yy], HEX);
//              Serial.print(" ");
//            }
//            Serial.println(" ");


  // Y to X – runs XOR process to remove escape bytes from reply array (WORKS)
  int fnd;
  fnd = 0;

  for (int ii = 0; ii < rpl_len_Y; ii++) {
    if (rpl_array_Y[ii + fnd] != 0x7D) {
      rpl_array_X[ii] = rpl_array_Y[ii + fnd];
    }
    if (rpl_array_Y[ii + fnd] == 0x7D) {
      rpl_array_X[ii] = rpl_array_Y[ii + fnd + 1] ^ 0x20;
      fnd++;
    }
  }
  rpl_len_X = rpl_len_Y - fnd;

//            Serial.print("rpl_array_X:\t");
//            for (int yy = 0; yy < rpl_len_X; yy++) {
//              Serial.print(rpl_array_X[yy], HEX);
//              Serial.print(" ");
//            }
//            Serial.println(" ");


  // X to W – removes two CRC bytes from end of array (WORKS)
  for (ii = 0; ii < (rpl_len_X - 2); ii++) {
    rpl_array_W[ii] = rpl_array_X[ii];
  }
  rpl_len_W = rpl_len_X - 2;

//            Serial.print("rpl_array_W:\t");
//            for (int yy = 0; yy < rpl_len_W; yy++) {
//              Serial.print(rpl_array_W[yy], HEX);
//              Serial.print(" ");
//            }
//            Serial.println(" ");


  // compares RWA reply CRC with calculated reply CRC (WORKS, need to add flag)
  unsigned int rpl_crc_rwa[] = {rpl_array_X[rpl_len_X - 2], rpl_array_X[rpl_len_X - 1]};
  
  unsigned int rpl_crc_calc[2] = { };
  crc_value = 0xFFFF;
  
  for (ii = 0; ii < rpl_len_W; ii++) {
    crc_value = ((crc_value << 8) ^ crc_table[((crc_value >> 8) ^ rpl_array_W[ii]) & 0x00FF]);
  }
  rpl_crc_calc[0] = crc_value & 0xFF;
  rpl_crc_calc[1] = crc_value >> 8;

  if (rpl_crc_rwa[0] == rpl_crc_calc[0] && rpl_crc_rwa[1] == rpl_crc_calc[1]) {
    Serial.println("reply CRC good");
  }


//            Serial.print("rpl_crc_rwa:\t");
//            for (int yy = 0; yy < 2; yy++) {
//              Serial.print(rpl_crc_rwa[yy], HEX);
//              Serial.print(" ");
//            }
//            Serial.println(" ");
//
//            Serial.print("rpl_crc_calc:\t");
//            for (int yy = 0; yy < 2; yy++) {
//              Serial.print(rpl_crc_calc[yy], HEX);
//              Serial.print(" ");
//            }
//            Serial.println(" ");


}


void spiSendReq(byte *req_payload_pt, int req_payload_len, int rw_id) { //(FIX POINTERS) --- --- --- --- --- --- --- --- ---


  // buf to A – copying variable array (WORKS)
  int ii;

  for (ii = 0; ii < req_payload_len; ii++) {
    req_array_A[ii] = *(req_payload_pt + ii);
  }
  req_len_A = req_payload_len;


  // A to B – adding two CRC bytes to end of array (WORKS)
  unsigned int req_crc[2] = { };
  
  for (ii = 0; ii < req_len_A; ii++) {
    crc_value = ((crc_value << 8) ^ crc_table[((crc_value >> 8) ^ req_array_A[ii]) & 0x00FF]);
  }
  req_crc[0] = crc_value & 0xFF;
  req_crc[1] = crc_value >> 8;

  for (ii = 0; ii < req_len_A; ii++) {
    req_array_B[ii] = req_array_A[ii];
  }
  req_array_B[req_len_A] = req_crc[0];
  req_array_B[req_len_A + 1] = req_crc[1];
  req_len_B = req_len_A + 2;


  // B to C – checking for 0x7D bytes (WORKS)
  int fnd;
  fnd = 0;

  for (ii = 0; ii < req_len_B; ii++) {
    if (req_array_B[ii] != 0x7D) {
      req_array_C[ii + fnd] = req_array_B[ii];
    }
    if (req_array_B[ii] == 0x7D) {
      req_array_C[ii + fnd] = 0x7D;
      req_array_C[ii + fnd + 1] = req_array_B[ii] ^ 0x20;
      fnd++;
    }
  }
  req_len_C = req_len_B + fnd;


  // C to D – checking for 0x7E bytes (WORKS)
  fnd = 0;

  for (ii = 0; ii < req_len_C; ii++) {
    if (req_array_C[ii] != 0x7E) {
      req_array_D[ii + fnd] = req_array_C[ii];
    }
    if (req_array_C[ii] == 0x7E) {
      req_array_D[ii + fnd] = 0x7D;
      req_array_D[ii + fnd + 1] = req_array_C[ii] ^ 0x20;
      fnd++;
    }
  }
  req_len_D = req_len_C + fnd;


  // D to E – adding 0x7E bytes to front and back of array (WORKS)
  for (ii = 0; ii < req_len_D; ii++) {
    req_array_E[ii + 1] = req_array_D[ii];
  }
  req_array_E[0] = 0x7E;
  req_array_E[req_len_D + 1] = 0x7E;
  req_len_E = req_len_D + 2;


  // assigning to buffer variable (ISSUE)
  byte spi_req_buf[16] = {0};
  int spi_req_buf_len;
  
  for (ii = 0; ii < req_len_E; ii++) {
    spi_req_buf[ii] = req_array_E[ii];
  }
  spi_req_buf_len = req_len_E;


  // assigns slave select pin to corresponding reaction wheel (WORKS)
  int SS_id;
  switch (rw_id) {
    case 1:
      SS_id = SS1;
      break;
    case 2:
      SS_id = SS2;
      break;
    case 3:
      SS_id = SS3;
      break;
    case 4:
      SS_id = SS4;
      break;
  }

          Serial.print("spi_req_buf (MOSI):\t");
          for (int yy = 0; yy < spi_req_buf_len; yy++) {
            Serial.print(spi_req_buf[yy], HEX);
            Serial.print(" ");
          }
          Serial.println(" ");
          delay(500);

  digitalWrite(SS_id, LOW);
  SPI.transfer(spi_req_buf, spi_req_buf_len);
  digitalWrite(SS_id, HIGH);

          Serial.print("spi_req_buf (MISO):\t");
          for (int yy = 0; yy < spi_req_buf_len; yy++) {
            Serial.print(spi_req_buf[yy], HEX);
            Serial.print(" ");
          }
          Serial.println(" ");
          delay(500);


  // process reply to detect errors
  // should recieve all 0x7E in this function
}
