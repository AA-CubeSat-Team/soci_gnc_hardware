// rwa_command_functions

#include "rwa_process_functions.h"

void reqPayloadWrite_cmd4(uint8_t *req_payload_pt, uint8_t *req_payload_len_pt) { // --- --- --- --- --- --- --- --- ---
  // get reaction wheel status
  uint8_t com_id = 4;

  *req_payload_pt = com_id;

  *req_payload_len_pt = 1;
}


void rplPayloadRead_cmd4(uint8_t *rpl_payload_pt, uint8_t *rpl_payload_len_pt, uint8_t *result, int32_t *curr_speed, int32_t *ref_speed, uint8_t *state, uint8_t *clc_mode) { // --- --- --- --- --- --- --- --- ---
  // get reaction wheel status
  // com_id = 4
  // length = 12
  *result = *(rpl_payload_pt+1);
  
  *curr_speed = (*(rpl_payload_pt+5) << 24) | (*(rpl_payload_pt+4) << 16) | (*(rpl_payload_pt+3) << 8) | *(rpl_payload_pt+2);
  
  *ref_speed = (*(rpl_payload_pt+9) << 24) | (*(rpl_payload_pt+8) << 16) | (*(rpl_payload_pt+7) << 8) | *(rpl_payload_pt+6);

  *state = *(rpl_payload_pt+10);

  *clc_mode = *(rpl_payload_pt+11);
}


void reqPayloadWrite_cmd6(uint8_t *req_payload_pt, uint8_t *req_payload_len_pt, int32_t speed, uint16_t ramp_time) { // --- --- --- --- --- --- --- --- ---
  // set reference speed
  uint8_t com_id = 6;

  uint8_t speed0 = speed & 0xFF;
  uint8_t speed1 = (speed >> 8) & 0xFF;
  uint8_t speed2 = (speed >> 16) & 0xFF;
  uint8_t speed3 = (speed >> 24) & 0xFF;

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


void rplPayloadRead_cmd6(uint8_t *rpl_payload_pt, uint8_t *rpl_payload_len_pt, uint8_t *result) { // --- --- --- --- --- --- --- --- ---
  // set reference speed
  // com_id = 6
  // length = 2
  *result = *(rpl_payload_pt+1);
}


void reqPayloadWrite_cmd10(uint8_t *req_payload_pt, uint8_t *req_payload_len_pt) { // --- --- --- --- --- --- --- --- ---
  // ping
  uint8_t com_id = 10;

  *req_payload_pt = com_id;

  *req_payload_len_pt = 1;
}


void rplPayloadRead_cmd10(uint8_t *rpl_payload_pt, uint8_t *rpl_payload_len_pt, uint8_t *result) { // --- --- --- --- --- --- --- --- ---
  // ping
  // com_id = 10
  // length = 2
  *result = *(rpl_payload_pt+1);
//  Serial.println(*result);
}


void runAll_cmd10(){
  #include "array_def.h"

  // RW0-1
  reqPayloadWrite_cmd10(&req_payload_rw1[0], &req_payload_len_rw1);
  reqPacketProcess(&req_payload_rw1[0], &req_packet_rw1[0], &req_payload_len_rw1, &req_packet_len_rw1);
  reqSpiTransfer(&req_packet_rw1[0], &req_packet_len_rw1, SS1);

  // RW0-2
  reqPayloadWrite_cmd10(&req_payload_rw2[0], &req_payload_len_rw2);
  reqPacketProcess(&req_payload_rw2[0], &req_packet_rw2[0], &req_payload_len_rw2, &req_packet_len_rw2);
  reqSpiTransfer(&req_packet_rw2[0], &req_packet_len_rw2, SS2);

  // RW0-3
  reqPayloadWrite_cmd10(&req_payload_rw3[0], &req_payload_len_rw3);
  reqPacketProcess(&req_payload_rw1[0], &req_packet_rw3[0], &req_payload_len_rw3, &req_packet_len_rw3);
  reqSpiTransfer(&req_packet_rw3[0], &req_packet_len_rw3, SS3);

  // RW0-4
  reqPayloadWrite_cmd10(&req_payload_rw4[0], &req_payload_len_rw4);
  reqPacketProcess(&req_payload_rw4[0], &req_packet_rw4[0], &req_payload_len_rw4, &req_packet_len_rw4);
  reqSpiTransfer(&req_packet_rw4[0], &req_packet_len_rw4, SS4);

  delay(100);

  // RW0-1
  rplSpiTransfer(&rpl_packet_rw1[0], &rpl_packet_len_rw1, SS1);
  rplPacketProcess(&rpl_payload_rw1[0], &rpl_packet_rw1[0], &rpl_payload_len_rw1, &rpl_packet_len_rw1);
  rplPayloadRead_cmd10(&rpl_payload_rw1[0], &rpl_payload_len_rw1, &result1);
  
  // RW0-2
  rplSpiTransfer(&rpl_packet_rw2[0], &rpl_packet_len_rw2, SS2);
  rplPacketProcess(&rpl_payload_rw2[0], &rpl_packet_rw2[0], &rpl_payload_len_rw2, &rpl_packet_len_rw2);
  rplPayloadRead_cmd10(&rpl_payload_rw2[0], &rpl_payload_len_rw2, &result2);

  // RW0-3
  rplSpiTransfer(&rpl_packet_rw3[0], &rpl_packet_len_rw3, SS3);
  rplPacketProcess(&rpl_payload_rw3[0], &rpl_packet_rw3[0], &rpl_payload_len_rw3, &rpl_packet_len_rw3);
  rplPayloadRead_cmd10(&rpl_payload_rw3[0], &rpl_payload_len_rw3, &result3);

  // RW0-4
  rplSpiTransfer(&rpl_packet_rw4[0], &rpl_packet_len_rw4, SS4);
  rplPacketProcess(&rpl_payload_rw4[0], &rpl_packet_rw4[0], &rpl_payload_len_rw4, &rpl_packet_len_rw4);
  rplPayloadRead_cmd10(&rpl_payload_rw4[0], &rpl_payload_len_rw4, &result4);

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

//          Serial.println(result1);
}
