// rwa_process_functions

void reqPacketProcess(uint8_t *req_payload_pt, uint8_t *req_packet_pt, uint8_t *req_payload_len_pt, uint8_t *req_packet_len_pt) { // --- --- --- --- --- --- --- --- ---
  uint8_t req_array_A[16] = {0}; // need to allocate max possible size of uint8_t
  uint8_t req_array_B[16] = {0};
  uint8_t req_array_C[16] = {0};
  uint8_t req_array_D[16] = {0};
  uint8_t req_array_E[16] = {0};
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

  // A to B – adding two CRC uint8_t s to end of array (WORKS)
  uint8_t req_crc[2] = { };
  crc_value = 0xFFFF;
  
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
  
  uint8_t spi_buffer[14] = {0};
  uint8_t spi_buffer_len = 14;
  
  for (ii = 0; ii < *req_packet_len_pt; ii++) {
    spi_buffer[ii] = *(req_packet_pt+ii);
  }
  spi_buffer_len = *req_packet_len_pt;
  
  SPI.beginTransaction(spiSet);
  digitalWrite(SS_id, LOW);
  SPI.transfer(spi_buffer, spi_buffer_len);
  digitalWrite(SS_id, HIGH);
  SPI.endTransaction();
}


void rplSpiTransfer(uint8_t *rpl_packet_pt, uint8_t *rpl_packet_len_pt, uint8_t SS_id) { // --- --- --- --- --- --- --- --- ---
  int ii;
  
  uint8_t spi_buffer[14] = {0};
  
  for (ii = 0; ii < 14; ii++) {
    spi_buffer[ii] = 0x7E;
  }

  Serial.println(*rpl_packet_len_pt);
  SPI.beginTransaction(spiSet);
  digitalWrite(SS_id, LOW);
  SPI.transfer(spi_buffer, *rpl_packet_len_pt);
  digitalWrite(SS_id, HIGH);
  SPI.endTransaction();

//  for (ii = 0; ii < *rpl_packet_len_pt; ii++) {
//    *(rpl_packet_pt+ii) = spi_buffer[ii];
//  }

  uint8_t test_reply[] = {0x7E, 0x0A, 0x01, 0xE5, 0xE2, 0x7E};
  *rpl_packet_len_pt = 6;
  for (ii = 0; ii < *rpl_packet_len_pt; ii++) {
    *(rpl_packet_pt+ii) = test_reply[ii];
  }
}


void rplPacketProcess(uint8_t *rpl_payload_pt, uint8_t *rpl_packet_pt, uint8_t *rpl_payload_len_pt, uint8_t *rpl_packet_len_pt) { //(NEEDS WORK) --- --- --- --- --- --- --- --- ---
  uint8_t rpl_array_Z[16] = {0}; // need to allocate max possible size of uint8_t
  uint8_t rpl_array_Y[16] = {0};
  uint8_t rpl_array_X[16] = {0};
  uint8_t rpl_array_W[16] = {0};
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

//          Serial.print("rpl_array_Z:\t");
//          for (uint8_t yy = 0; yy < rpl_len_Z; yy++) {
//            Serial.print(rpl_array_Z[yy], HEX);
//            Serial.print("\t");
//          }
//          Serial.println(" ");

  // Z to Y – copies request array to new array without frame flags (WORKS)
  for (ii = 0; ii < (rpl_len_Z - 1); ii++) {
    rpl_array_Y[ii] = rpl_array_Z[ii + 1];
  }
  rpl_len_Y = rpl_len_Z - 2;

  // Y to X – runs XOR process to remove escape uint8_t s from reply array (WORKS)
  uint8_t fnd;
  fnd = 0;

  for (uint8_t ii = 0; ii < rpl_len_Y; ii++) {
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
  crc_value = 0xFFFF;
  
  for (ii = 0; ii < rpl_len_W; ii++) {
    crc_value = ((crc_value << 8) ^ crc_table[((crc_value >> 8) ^ rpl_array_W[ii]) & 0x00FF]);
  }
  rpl_crc_calc[0] = crc_value & 0xFF;
  rpl_crc_calc[1] = crc_value >> 8;

  if (rpl_crc_rwa[0] == rpl_crc_calc[0] && rpl_crc_rwa[1] == rpl_crc_calc[1]) {
    Serial.println("reply CRC good");
  }


  // copies to rpl_payload_pt
  for (ii = 0; ii < rpl_len_W; ii++) {
    *(rpl_payload_pt + ii) = rpl_array_W[ii];
  }
  *rpl_payload_len_pt = rpl_len_W;

//          Serial.print("*rpl_payload_pt:\t");
//          for (uint8_t yy = 0; yy < *rpl_payload_len_pt; yy++) {
//            Serial.print(*(rpl_payload_pt+yy), HEX);
//            Serial.print("\t");
//          }
//          Serial.println(" ");
}
