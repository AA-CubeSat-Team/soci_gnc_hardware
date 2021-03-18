// rwa_command_functions

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
}
