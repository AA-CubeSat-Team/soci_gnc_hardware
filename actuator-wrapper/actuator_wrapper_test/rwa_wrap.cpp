
/* issues:
 * add init function (clear reset, set current limit)
 * add CRC error processing
 * possibly can't handle all 0x7E reply
 * add double conversion
 * flight wrapper should request reqSpeed and return currSpeed
 * make reply data read more robust
 */

 /* notes:
  * tested SPI_TIMEOUT down to 1 ms on Arduino Uno with com_id 6
  */

#include <Arduino.h>                     
#include "rwa_wrap.h"  

SPISettings spiSet(125000, MSBFIRST, SPI_MODE0); 

struct rw_data rw1;
//struct rw_data rw1, rw2, rw3, rw4;    saving SRAM

void reqPacketProcess(uint8_t *req_payload_pt, uint8_t *req_packet_pt, uint8_t *req_payload_len_pt, uint8_t *req_packet_len_pt) { // --- --- --- --- --- --- --- --- ---
  uint8_t req_array_A[MAX_REQ_PACKET] = {0}; // need to allocate max possible size of uint8_t
  uint8_t req_array_B[MAX_REQ_PACKET] = {0};
  uint8_t req_array_C[MAX_REQ_PACKET] = {0};
  uint8_t req_array_D[MAX_REQ_PACKET] = {0};
  uint8_t req_array_E[MAX_REQ_PACKET] = {0};
  uint8_t req_len_A;
  uint8_t req_len_B;
  uint8_t req_len_C;
  uint8_t req_len_D;
  uint8_t req_len_E;
  
  uint8_t ii;

  for (ii = 0; ii < 14; ii++) {
    *(req_packet_pt+ii) = 0;
  }
  *req_packet_len_pt = 0;
  
  // buf to A – copying variable array (WORKS)
  for (ii = 0; ii < *req_payload_len_pt; ii++) {
    req_array_A[ii] = *(req_payload_pt + ii);
  }
  req_len_A = *req_payload_len_pt;

        // A to B – adding two CRC uint8_t s to end of array (ISSUE)
        uint8_t req_crc[2] = {0};
        uint16_t crc_value = CRC_VAL; 
        
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

  // B to C – checking for 0x7D uint8_t s (WORKS)
  uint8_t fnd;
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

  // C to D – checking for 0x7E uint8_t s (WORKS)
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

  // D to E – adding 0x7E uint8_t s to front and back of array (WORKS)
  for (ii = 0; ii < req_len_D; ii++) {
    req_array_E[ii + 1] = req_array_D[ii];
  }
  req_array_E[0] = 0x7E;
  req_array_E[req_len_D + 1] = 0x7E;
  req_len_E = req_len_D + 2;

  // assigning to packet array
  for (ii = 0; ii < req_len_E; ii++) {
    *(req_packet_pt+ii) = req_array_E[ii];
  }
  *req_packet_len_pt = req_len_E;
}


void reqSpiTransfer(uint8_t *req_packet_pt, uint8_t *req_packet_len_pt, uint8_t SS_id) { // --- --- --- --- --- --- --- --- ---
  int ii;
  
  uint8_t spi_buffer[MAX_REQ_PACKET] = {0};
  
  for (ii = 0; ii < *req_packet_len_pt; ii++) {
    spi_buffer[ii] = *(req_packet_pt+ii);
  }
  
  SPI.beginTransaction(spiSet);
  digitalWrite(SS_id, LOW);
  SPI.transfer(spi_buffer, *req_packet_len_pt);
  digitalWrite(SS_id, HIGH);
  SPI.endTransaction();
}


void rplSpiTransfer(uint8_t *rpl_packet_pt, uint8_t *rpl_packet_len_pt, uint8_t SS_id) { // --- --- --- --- --- --- --- --- ---
  int ii;
  
  uint8_t spi_buffer[MAX_RPL_PACKET] = {0};

  ii = 0;
  for (ii = 0; ii < MAX_RPL_PACKET; ii++) {
    spi_buffer[ii] = 0x7E;
  }
  
  SPI.beginTransaction(spiSet);
  digitalWrite(SS_id, LOW);
  SPI.transfer(spi_buffer, *rpl_packet_len_pt);
  digitalWrite(SS_id, HIGH);
  SPI.endTransaction();

  for (ii = 0; ii < *rpl_packet_len_pt; ii++) {
    *(rpl_packet_pt+ii) = spi_buffer[ii];
  }
}


void rplPacketProcess(uint8_t *rpl_payload_pt, uint8_t *rpl_packet_pt, uint8_t *rpl_payload_len_pt, uint8_t *rpl_packet_len_pt) { //(NEEDS WORK) --- --- --- --- --- --- --- --- ---
  uint8_t rpl_array_Z[MAX_RPL_PACKET] = {0}; 
  uint8_t rpl_array_Y[MAX_RPL_PACKET] = {0};
  uint8_t rpl_array_X[MAX_RPL_PACKET] = {0};
  uint8_t rpl_array_W[MAX_RPL_PACKET] = {0};
  uint8_t rpl_len_Z;
  uint8_t rpl_len_Y;
  uint8_t rpl_len_X;
  uint8_t rpl_len_W;
  
  // copying packet array (WORKS)
  uint8_t ii;

  for (ii = 0; ii < *rpl_packet_len_pt; ii++) {
    rpl_array_Z[ii] = *(rpl_packet_pt + ii);
  }
  rpl_len_Z = *rpl_packet_len_pt;

  // Z to Y – copies request array to new array without frame flags (0x7E) (WORKS)
  uint8_t fnd;
  fnd = 0;
  
  for (ii = 0; ii < rpl_len_Z; ii++) {
    if (rpl_array_Z[ii] != 0x7E) {
      rpl_array_Y[ii - fnd] = rpl_array_Z[ii];
    }
    if (rpl_array_Z[ii] == 0x7E) {
      fnd++;
    }
  }
  rpl_len_Y = rpl_len_Z - fnd;

  // in case reply is made entirely of flag bytes
  if (fnd == rpl_len_Z){
    for (ii = 0; ii < rpl_len_Z; ii++) {
      *(rpl_payload_pt + ii) = rpl_array_Z[ii];
    }
    *rpl_payload_len_pt = rpl_len_Z;
    return;
  }

  // Y to X – runs reverse XOR process to remove escape bytes (0x7D) from reply array (WORKS)
  fnd = 0;

  for (ii = 0; ii < rpl_len_Y; ii++) {
    if (rpl_array_Y[ii + fnd] != 0x7D) {
      rpl_array_X[ii] = rpl_array_Y[ii + fnd];
    }
    if (rpl_array_Y[ii + fnd] == 0x7D) {
      rpl_array_X[ii] = rpl_array_Y[ii + fnd + 1] ^ 0x20;
      fnd++;
    }
  }
  rpl_len_X = rpl_len_Y - fnd;

  // X to W – removes two CRC uint8_t s from end of array (WORKS)
  for (ii = 0; ii < (rpl_len_X - 2); ii++) {
    rpl_array_W[ii] = rpl_array_X[ii];
  }
  rpl_len_W = rpl_len_X - 2;

  // compares RWA reply CRC with calculated reply CRC (WORKS, need to add flag)
  uint8_t rpl_crc_rwa[] = {rpl_array_X[rpl_len_X - 2], rpl_array_X[rpl_len_X - 1]};
  
  uint8_t rpl_crc_calc[2] = { };
  uint16_t crc_value = CRC_VAL; 
  
  for (ii = 0; ii < rpl_len_W; ii++) {
    crc_value = ((crc_value << 8) ^ crc_table[((crc_value >> 8) ^ rpl_array_W[ii]) & 0x00FF]);
  }
  rpl_crc_calc[0] = crc_value & 0xFF;
  rpl_crc_calc[1] = crc_value >> 8;

  if (rpl_crc_rwa[0] == rpl_crc_calc[0] && rpl_crc_rwa[1] == rpl_crc_calc[1]) {
//    do something
  }


  // copies to rpl_payload_pt
  for (ii = 0; ii < rpl_len_W; ii++) {
    *(rpl_payload_pt + ii) = rpl_array_W[ii];
  }
  *rpl_payload_len_pt = rpl_len_W;
}

// command 4 - get reaction wheel status --- --- --- --- --- ---
void reqPayloadWrite_cmd4(uint8_t *req_payload_pt, uint8_t *req_payload_len_pt, struct rw_data *rwX_pt) {
  uint8_t com_id = 4;

  *req_payload_pt = com_id;

  *req_payload_len_pt = 1;
}

void rplPayloadRead_cmd4(uint8_t *rpl_payload_pt, uint8_t *rpl_payload_len_pt, struct rw_data *rwX_pt) { 
  // com_id = 4
  // length = 12
  rwX_pt->result = *(rpl_payload_pt+1);
  
  rwX_pt->currSpeed = (*(rpl_payload_pt+5) << 24) | (*(rpl_payload_pt+4) << 16) | (*(rpl_payload_pt+3) << 8) | *(rpl_payload_pt+2);
  
  rwX_pt->refSpeed = (*(rpl_payload_pt+9) << 24) | (*(rpl_payload_pt+8) << 16) | (*(rpl_payload_pt+7) << 8) | *(rpl_payload_pt+6);

  rwX_pt->state = *(rpl_payload_pt+10);

  rwX_pt->currClcMode = *(rpl_payload_pt+11);

  // system ID only
  rwX_pt->time_N = millis() - time_0;
}


// command 6 - set reference speed --- --- --- --- --- ---
void reqPayloadWrite_cmd6(uint8_t *req_payload_pt, uint8_t *req_payload_len_pt, struct rw_data *rwX_pt) { 
  uint8_t com_id = 6;

  *req_payload_pt = com_id;
  
  *(req_payload_pt+1) = rwX_pt->reqSpeed & 0xFF;
  *(req_payload_pt+2) = (rwX_pt->reqSpeed >> 8) & 0xFF;
  *(req_payload_pt+3) = (rwX_pt->reqSpeed >> 16) & 0xFF;
  *(req_payload_pt+4) = (rwX_pt->reqSpeed >> 24) & 0xFF;
  
  *(req_payload_pt+5) = rwX_pt->rampTime & 0xFF;
  *(req_payload_pt+6) = (rwX_pt->rampTime >> 8) & 0xFF; 

  *req_payload_len_pt = 1 + 6;
}

void rplPayloadRead_cmd6(uint8_t *rpl_payload_pt, uint8_t *rpl_payload_len_pt, struct rw_data *rwX_pt) {
  // com_id = 6
  // length = 2
  rwX_pt->result = *(rpl_payload_pt+1);
}


// command 7 - set current limit control mode --- --- --- --- --- ---
void reqPayloadWrite_cmd7(uint8_t *req_payload_pt, uint8_t *req_payload_len_pt, struct rw_data *rwX_pt) { 
  uint8_t com_id = 7;

  *req_payload_pt = com_id;
  
  *(req_payload_pt+1) = rwX_pt->reqClcMode;

  *req_payload_len_pt = 1 + 1;
}

void rplPayloadRead_cmd7(uint8_t *rpl_payload_pt, uint8_t *rpl_payload_len_pt, struct rw_data *rwX_pt) {
  // com_id = 7
  // length = 2
  rwX_pt->result = *(rpl_payload_pt+1);
}


// command 10 - ping --- --- --- --- --- ---
void reqPayloadWrite_cmd10(uint8_t *req_payload_pt, uint8_t *req_payload_len_pt, struct rw_data *rwX_pt) {
  uint8_t com_id = 10;

  *req_payload_pt = com_id;

  *req_payload_len_pt = 1;
}

void rplPayloadRead_cmd10(uint8_t *rpl_payload_pt, uint8_t *rpl_payload_len_pt, struct rw_data *rwX_pt) {
  // com_id = 10
  // length = 2
  rwX_pt->result = *(rpl_payload_pt+1);
}


void reqPayloadWriteSwitcher(uint8_t com_id, uint8_t *req_payload_pt, uint8_t *req_payload_len_pt, struct rw_data *rwX_pt){
  switch (com_id){
    case 4:
      reqPayloadWrite_cmd4(req_payload_pt, req_payload_len_pt, rwX_pt);
      break;
    case 6:
      reqPayloadWrite_cmd6(req_payload_pt, req_payload_len_pt, rwX_pt);
      break;
    case 7:
      reqPayloadWrite_cmd7(req_payload_pt, req_payload_len_pt, rwX_pt);
      break;
    case 10:
      reqPayloadWrite_cmd10(req_payload_pt, req_payload_len_pt, rwX_pt);
      break;
  }
}

void rplPayloadReadSwitcher(uint8_t com_id, uint8_t *rpl_payload_pt, uint8_t *rpl_payload_len_pt, struct rw_data *rwX_pt){
  switch (com_id){
    case 4:
      rplPayloadRead_cmd4(rpl_payload_pt, rpl_payload_len_pt, rwX_pt);
      break;
    case 6:
      rplPayloadRead_cmd6(rpl_payload_pt, rpl_payload_len_pt, rwX_pt);
      break;
    case 7:
      rplPayloadRead_cmd7(rpl_payload_pt, rpl_payload_len_pt, rwX_pt);
      break;
    case 10:
      rplPayloadRead_cmd10(rpl_payload_pt, rpl_payload_len_pt, rwX_pt);
      break;
  }
}


void commandRWA(uint8_t com_id){
  uint8_t req_payload_rw1[MAX_REQ_PAYLOAD] = {0};
  uint8_t req_payload_len_rw1 = MAX_REQ_PAYLOAD;
  uint8_t req_packet_rw1[MAX_REQ_PACKET] = {0};
  uint8_t req_packet_len_rw1 = MAX_REQ_PACKET;

  uint8_t req_payload_rw2[MAX_REQ_PAYLOAD] = {0};
  uint8_t req_payload_len_rw2 = MAX_REQ_PAYLOAD;
  uint8_t req_packet_rw2[MAX_REQ_PACKET] = {0};
  uint8_t req_packet_len_rw2 = MAX_REQ_PACKET;

  uint8_t req_payload_rw3[MAX_REQ_PAYLOAD] = {0};
  uint8_t req_payload_len_rw3 = MAX_REQ_PAYLOAD;
  uint8_t req_packet_rw3[MAX_REQ_PACKET] = {0};
  uint8_t req_packet_len_rw3 = MAX_REQ_PACKET;

  uint8_t req_payload_rw4[MAX_REQ_PAYLOAD] = {0};
  uint8_t req_payload_len_rw4 = MAX_REQ_PAYLOAD;
  uint8_t req_packet_rw4[MAX_REQ_PACKET] = {0};
  uint8_t req_packet_len_rw4 = MAX_REQ_PACKET;
  
  uint8_t rpl_payload_rw1[MAX_RPL_PAYLOAD] = {0};
  uint8_t rpl_payload_len_rw1 = MAX_RPL_PAYLOAD;
  uint8_t rpl_packet_rw1[MAX_RPL_PACKET] = {0};
  uint8_t rpl_packet_len_rw1 = MAX_RPL_PACKET;

  uint8_t rpl_payload_rw2[MAX_RPL_PAYLOAD] = {0};
  uint8_t rpl_payload_len_rw2 = MAX_RPL_PAYLOAD;
  uint8_t rpl_packet_rw2[MAX_RPL_PACKET] = {0};
  uint8_t rpl_packet_len_rw2 = MAX_RPL_PACKET;

  uint8_t rpl_payload_rw3[MAX_RPL_PAYLOAD] = {0};
  uint8_t rpl_payload_len_rw3 = MAX_RPL_PAYLOAD;
  uint8_t rpl_packet_rw3[MAX_RPL_PACKET] = {0};
  uint8_t rpl_packet_len_rw3 = MAX_RPL_PACKET;

  uint8_t rpl_payload_rw4[MAX_RPL_PAYLOAD] = {0};
  uint8_t rpl_payload_len_rw4 = MAX_RPL_PAYLOAD;
  uint8_t rpl_packet_rw4[MAX_RPL_PACKET] = {0};
  uint8_t rpl_packet_len_rw4 = MAX_RPL_PACKET;

  int rpl_data_len;

  switch (com_id){
    case 4:
      rpl_data_len = 10;
      break;
    case 6:
      rpl_data_len = 0;
      break;
    case 10:
      rpl_data_len = 0;
      break;
  }
  
  rpl_packet_len_rw1 = 2*(rpl_data_len + 4) + 3;
  rpl_packet_len_rw2 = 2*(rpl_data_len + 4) + 3;
  rpl_packet_len_rw3 = 2*(rpl_data_len + 4) + 3;
  rpl_packet_len_rw4 = 2*(rpl_data_len + 4) + 3;

  // rw1
  reqPayloadWriteSwitcher(com_id, &req_payload_rw1[0], &req_payload_len_rw1, &rw1);
  reqPacketProcess(&req_payload_rw1[0], &req_packet_rw1[0], &req_payload_len_rw1, &req_packet_len_rw1);
  reqSpiTransfer(&req_packet_rw1[0], &req_packet_len_rw1, SS1);

//  // rw2
//  reqPayloadWriteSwitcher(com_id, &req_payload_rw2[0], &req_payload_len_rw2, &rw2);
//  reqPacketProcess(&req_payload_rw2[0], &req_packet_rw2[0], &req_payload_len_rw2, &req_packet_len_rw2);
//  reqSpiTransfer(&req_packet_rw2[0], &req_packet_len_rw2, SS2);
//
//  // rw3
//  reqPayloadWriteSwitcher(com_id, &req_payload_rw3[0], &req_payload_len_rw3, &rw3);
//  reqPacketProcess(&req_payload_rw1[0], &req_packet_rw3[0], &req_payload_len_rw3, &req_packet_len_rw3);
//  reqSpiTransfer(&req_packet_rw3[0], &req_packet_len_rw3, SS3);
//
//  // rw4
//  reqPayloadWriteSwitcher(com_id, &req_payload_rw4[0], &req_payload_len_rw4, &rw4);
//  reqPacketProcess(&req_payload_rw4[0], &req_packet_rw4[0], &req_payload_len_rw4, &req_packet_len_rw4);
//  reqSpiTransfer(&req_packet_rw4[0], &req_packet_len_rw4, SS4);

  delay(SPI_TIMEOUT);

  // rw1
  rplSpiTransfer(&rpl_packet_rw1[0], &rpl_packet_len_rw1, SS1);
  rplPacketProcess(&rpl_payload_rw1[0], &rpl_packet_rw1[0], &rpl_payload_len_rw1, &rpl_packet_len_rw1);
  rplPayloadReadSwitcher(com_id, &rpl_payload_rw1[0], &rpl_payload_len_rw1, &rw1);
  
//  // rw2
//  rplSpiTransfer(&rpl_packet_rw2[0], &rpl_packet_len_rw2, SS2);
//  rplPacketProcess(&rpl_payload_rw2[0], &rpl_packet_rw2[0], &rpl_payload_len_rw2, &rpl_packet_len_rw2);
//  rplPayloadReadSwitcher(com_id, &rpl_payload_rw2[0], &rpl_payload_len_rw2, &rw2);
//
//  // rw3
//  rplSpiTransfer(&rpl_packet_rw3[0], &rpl_packet_len_rw3, SS3);
//  rplPacketProcess(&rpl_payload_rw3[0], &rpl_packet_rw3[0], &rpl_payload_len_rw3, &rpl_packet_len_rw3);
//  rplPayloadReadSwitcher(com_id, &rpl_payload_rw3[0], &rpl_payload_len_rw3, &rw3);
//
//  // rw4
//  rplSpiTransfer(&rpl_packet_rw4[0], &rpl_packet_len_rw4, SS4);
//  rplPacketProcess(&rpl_payload_rw4[0], &rpl_packet_rw4[0], &rpl_payload_len_rw4, &rpl_packet_len_rw4);
//  rplPayloadReadSwitcher(com_id, &rpl_payload_rw4[0], &rpl_payload_len_rw4, &rw4);

        if (debug_mode == 1){
          Serial.print("req_packet_rw1:\t\t");
          for (uint8_t yy = 0; yy < req_packet_len_rw1; yy++) {
            Serial.print(req_packet_rw1[yy], HEX);
            Serial.print("\t");
          }
          Serial.println(" ");
          Serial.print("rpl_packet_rw1:\t\t");
          for (uint8_t yy = 0; yy < rpl_packet_len_rw1; yy++) {
            Serial.print(rpl_packet_rw1[yy], HEX);
            Serial.print("\t");
          }
          Serial.println(" ");
          Serial.println(" ");
        }
}
