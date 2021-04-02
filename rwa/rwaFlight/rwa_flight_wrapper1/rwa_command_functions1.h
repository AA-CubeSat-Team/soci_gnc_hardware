// rwa_command_functions

SPISettings spiSet(125000, MSBFIRST, SPI_MODE0);

// set timeout between request and reply (milliseconds)
#define SPI_TIMEOUT 100

// assigns GPIO pins to be SS pins
#define SS1 3
#define SS2 4
#define SS3 5
#define SS4 6

// assigns GPIO pins to be EN pins
#define EN1 A1
#define EN2 A2
#define EN3 A3
#define EN4 A4

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
                           
#include "rwa_process_functions1.h"                       


// command 4 - get reaction wheel status --- --- --- --- --- ---
void reqPayloadWrite_cmd4(uint8_t *req_payload_pt, uint8_t *req_payload_len_pt) {
  uint8_t com_id = 4;

  *req_payload_pt = com_id;

  *req_payload_len_pt = 1;
}

void rplPayloadRead_cmd4(uint8_t *rpl_payload_pt, uint8_t *rpl_payload_len_pt, uint8_t *result, int32_t *curr_speed, int32_t *ref_speed, uint8_t *state, uint8_t *clc_mode) { 
  // com_id = 4
  // length = 12
  *result = *(rpl_payload_pt+1);
  
  *curr_speed = (*(rpl_payload_pt+5) << 24) | (*(rpl_payload_pt+4) << 16) | (*(rpl_payload_pt+3) << 8) | *(rpl_payload_pt+2);
  
  *ref_speed = (*(rpl_payload_pt+9) << 24) | (*(rpl_payload_pt+8) << 16) | (*(rpl_payload_pt+7) << 8) | *(rpl_payload_pt+6);

  *state = *(rpl_payload_pt+10);

  *clc_mode = *(rpl_payload_pt+11);
}


// command 6 - set reference speed --- --- --- --- --- ---
void reqPayloadWrite_cmd6(uint8_t *req_payload_pt, uint8_t *req_payload_len_pt, int32_t req_speed, uint16_t ramp_time) { 
  uint8_t com_id = 6;

  uint8_t speed0 = req_speed & 0xFF;
  uint8_t speed1 = (req_speed >> 8) & 0xFF;
  uint8_t speed2 = (req_speed >> 16) & 0xFF;
  uint8_t speed3 = (req_speed >> 24) & 0xFF;

  uint8_t ramp_time0 = ramp_time & 0xFF;
  uint8_t ramp_time1 = (ramp_time >> 8) & 0xFF; 

  *req_payload_pt = com_id;
  
  *(req_payload_pt+1) = speed0;
  *(req_payload_pt+2) = speed1;
  *(req_payload_pt+3) = speed2;
  *(req_payload_pt+4) = speed3;
  
  *(req_payload_pt+5) = ramp_time0;
  *(req_payload_pt+6) = ramp_time1;

  *req_payload_len_pt = 1 + 6;
}

void rplPayloadRead_cmd6(uint8_t *rpl_payload_pt, uint8_t *rpl_payload_len_pt, uint8_t *result) {
  // com_id = 6
  // length = 2
  *result = *(rpl_payload_pt+1);
}


// command 10 - ping --- --- --- --- --- ---
void reqPayloadWrite_cmd10(uint8_t *req_payload_pt, uint8_t *req_payload_len_pt) {
  uint8_t com_id = 10;

  *req_payload_pt = com_id;

  *req_payload_len_pt = 1;
}

void rplPayloadRead_cmd10(uint8_t *rpl_payload_pt, uint8_t *rpl_payload_len_pt, uint8_t *result) {
  // com_id = 10
  // length = 2
  *result = *(rpl_payload_pt+1);
}


void commandAll_6speed(int32_t *req_speed_array_pt, uint16_t *ramp_time_array_pt){
  #include "array_def1.h"

  int32_t req_speed_rw1 = *(req_speed_array_pt);
  int32_t req_speed_rw2 = *(req_speed_array_pt+1);
  int32_t req_speed_rw3 = *(req_speed_array_pt+2);
  int32_t req_speed_rw4 = *(req_speed_array_pt+3);

  uint16_t ramp_time_rw1 = *(ramp_time_array_pt);
  uint16_t ramp_time_rw2 = *(ramp_time_array_pt+1);
  uint16_t ramp_time_rw3 = *(ramp_time_array_pt+2);
  uint16_t ramp_time_rw4 = *(ramp_time_array_pt+3);

  int rpl_base_len = 5;
  rpl_packet_len_rw1 = rpl_base_len;
  rpl_packet_len_rw2 = rpl_base_len;
  rpl_packet_len_rw3 = rpl_base_len;
  rpl_packet_len_rw4 = rpl_base_len;

  // rw1
  reqPayloadWrite_cmd6(&req_payload_rw1[0], &req_payload_len_rw1, req_speed_rw1, ramp_time_rw1);
  reqPacketProcess(&req_payload_rw1[0], &req_packet_rw1[0], &req_payload_len_rw1, &req_packet_len_rw1);
  reqSpiTransfer(&req_packet_rw1[0], &req_packet_len_rw1, SS1);

//  // rw2
//  reqPayloadWrite_cmd6(&req_payload_rw2[0], &req_payload_len_rw2, req_speed_rw2, ramp_time_rw2);
//  reqPacketProcess(&req_payload_rw2[0], &req_packet_rw2[0], &req_payload_len_rw2, &req_packet_len_rw2);
//  reqSpiTransfer(&req_packet_rw2[0], &req_packet_len_rw2, SS2);
//
//  // rw3
//  reqPayloadWrite_cmd6(&req_payload_rw3[0], &req_payload_len_rw3, req_speed_rw3, ramp_time_rw3);
//  reqPacketProcess(&req_payload_rw1[0], &req_packet_rw3[0], &req_payload_len_rw3, &req_packet_len_rw3);
//  reqSpiTransfer(&req_packet_rw3[0], &req_packet_len_rw3, SS3);
//
//  // rw4
//  reqPayloadWrite_cmd6(&req_payload_rw4[0], &req_payload_len_rw4, req_speed_rw4, ramp_time_rw4);
//  reqPacketProcess(&req_payload_rw4[0], &req_packet_rw4[0], &req_payload_len_rw4, &req_packet_len_rw4);
//  reqSpiTransfer(&req_packet_rw4[0], &req_packet_len_rw4, SS4);

//  delay(SPI_TIMEOUT);

  // rw1
  rplSpiTransfer(&rpl_packet_rw1[0], &rpl_packet_len_rw1, SS1);
  rplPacketProcess(&rpl_payload_rw1[0], &rpl_packet_rw1[0], &rpl_payload_len_rw1, &rpl_packet_len_rw1);
  rplPayloadRead_cmd6(&rpl_payload_rw1[0], &rpl_payload_len_rw1, &result1);
  
//  // rw2
//  rplSpiTransfer(&rpl_packet_rw2[0], &rpl_packet_len_rw2, SS2);
//  rplPacketProcess(&rpl_payload_rw2[0], &rpl_packet_rw2[0], &rpl_payload_len_rw2, &rpl_packet_len_rw2);
//  rplPayloadRead_cmd6(&rpl_payload_rw2[0], &rpl_payload_len_rw2, &result2);
//
//  // rw3
//  rplSpiTransfer(&rpl_packet_rw3[0], &rpl_packet_len_rw3, SS3);
//  rplPacketProcess(&rpl_payload_rw3[0], &rpl_packet_rw3[0], &rpl_payload_len_rw3, &rpl_packet_len_rw3);
//  rplPayloadRead_cmd6(&rpl_payload_rw3[0], &rpl_payload_len_rw3, &result3);
//
//  // rw4
//  rplSpiTransfer(&rpl_packet_rw4[0], &rpl_packet_len_rw4, SS4);
//  rplPacketProcess(&rpl_payload_rw4[0], &rpl_packet_rw4[0], &rpl_payload_len_rw4, &rpl_packet_len_rw4);
//  rplPayloadRead_cmd6(&rpl_payload_rw4[0], &rpl_payload_len_rw4, &result4);

          Serial.print("req_payload_rw1:\t");
          for (uint8_t yy = 0; yy < req_payload_len_rw1; yy++) {
            Serial.print(req_payload_rw1[yy], HEX);
            Serial.print("\t");
          }
          Serial.println(" ");
          
          Serial.print("req_packet_rw1:\t");
          for (uint8_t yy = 0; yy < req_packet_len_rw1; yy++) {
            Serial.print(req_packet_rw1[yy], HEX);
            Serial.print("\t");
          }
          Serial.println(" ");

          Serial.print("rpl_packet_rw1:\t");
          for (uint8_t yy = 0; yy < rpl_packet_len_rw1; yy++) {
            Serial.print(rpl_packet_rw1[yy], HEX);
            Serial.print("\t");
          }
          Serial.println(" ");

          Serial.print("rpl_payload_rw1:\t");
          for (uint8_t yy = 0; yy < rpl_payload_len_rw1; yy++) {
            Serial.print(rpl_payload_rw1[yy], HEX);
            Serial.print("\t");
          }
          Serial.println(" ");
}


void commandAll_10ping(){
  #include "array_def1.h"

  int rpl_base_len = 5;

  rpl_packet_len_rw1 = rpl_base_len;
  rpl_packet_len_rw2 = rpl_base_len;
  rpl_packet_len_rw3 = rpl_base_len;
  rpl_packet_len_rw4 = rpl_base_len;

  // rw1
  reqPayloadWrite_cmd10(&req_payload_rw1[0], &req_payload_len_rw1);
  reqPacketProcess(&req_payload_rw1[0], &req_packet_rw1[0], &req_payload_len_rw1, &req_packet_len_rw1);
  reqSpiTransfer(&req_packet_rw1[0], &req_packet_len_rw1, SS1);

//  // rw2
//  reqPayloadWrite_cmd10(&req_payload_rw2[0], &req_payload_len_rw2);
//  reqPacketProcess(&req_payload_rw2[0], &req_packet_rw2[0], &req_payload_len_rw2, &req_packet_len_rw2);
//  reqSpiTransfer(&req_packet_rw2[0], &req_packet_len_rw2, SS2);
//
//  // rw3
//  reqPayloadWrite_cmd10(&req_payload_rw3[0], &req_payload_len_rw3);
//  reqPacketProcess(&req_payload_rw1[0], &req_packet_rw3[0], &req_payload_len_rw3, &req_packet_len_rw3);
//  reqSpiTransfer(&req_packet_rw3[0], &req_packet_len_rw3, SS3);
//
//  // rw4
//  reqPayloadWrite_cmd10(&req_payload_rw4[0], &req_payload_len_rw4);
//  reqPacketProcess(&req_payload_rw4[0], &req_packet_rw4[0], &req_payload_len_rw4, &req_packet_len_rw4);
//  reqSpiTransfer(&req_packet_rw4[0], &req_packet_len_rw4, SS4);

//  delay(SPI_TIMEOUT);

  // rw1
  rplSpiTransfer(&rpl_packet_rw1[0], &rpl_packet_len_rw1, SS1);
  rplPacketProcess(&rpl_payload_rw1[0], &rpl_packet_rw1[0], &rpl_payload_len_rw1, &rpl_packet_len_rw1);
  rplPayloadRead_cmd10(&rpl_payload_rw1[0], &rpl_payload_len_rw1, &result1);
  
//  // rw2
//  rplSpiTransfer(&rpl_packet_rw2[0], &rpl_packet_len_rw2, SS2);
//  rplPacketProcess(&rpl_payload_rw2[0], &rpl_packet_rw2[0], &rpl_payload_len_rw2, &rpl_packet_len_rw2);
//  rplPayloadRead_cmd10(&rpl_payload_rw2[0], &rpl_payload_len_rw2, &result2);
//
//  // rw3
//  rplSpiTransfer(&rpl_packet_rw3[0], &rpl_packet_len_rw3, SS3);
//  rplPacketProcess(&rpl_payload_rw3[0], &rpl_packet_rw3[0], &rpl_payload_len_rw3, &rpl_packet_len_rw3);
//  rplPayloadRead_cmd10(&rpl_payload_rw3[0], &rpl_payload_len_rw3, &result3);
//
//  // rw4
//  rplSpiTransfer(&rpl_packet_rw4[0], &rpl_packet_len_rw4, SS4);
//  rplPacketProcess(&rpl_payload_rw4[0], &rpl_packet_rw4[0], &rpl_payload_len_rw4, &rpl_packet_len_rw4);
//  rplPayloadRead_cmd10(&rpl_payload_rw4[0], &rpl_payload_len_rw4, &result4);

          Serial.print("req_payload_rw1:\t");
          for (uint8_t yy = 0; yy < req_payload_len_rw1; yy++) {
            Serial.print(req_payload_rw1[yy], HEX);
            Serial.print("\t");
          }
          Serial.println(" ");
          
          Serial.print("req_packet_rw1:\t");
          for (uint8_t yy = 0; yy < req_packet_len_rw1; yy++) {
            Serial.print(req_packet_rw1[yy], HEX);
            Serial.print("\t");
          }
          Serial.println(" ");

          Serial.print("rpl_packet_rw1:\t");
          for (uint8_t yy = 0; yy < rpl_packet_len_rw1; yy++) {
            Serial.print(rpl_packet_rw1[yy], HEX);
            Serial.print("\t");
          }
          Serial.println(" ");

          Serial.print("rpl_payload_rw1:\t");
          for (uint8_t yy = 0; yy < rpl_payload_len_rw1; yy++) {
            Serial.print(rpl_payload_rw1[yy], HEX);
            Serial.print("\t");
          }
          Serial.println(" ");
}
