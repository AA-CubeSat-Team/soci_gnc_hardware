
#include "sun_wrap.h"

// sun sensor struct
sun_t Sun1;

uint8_t recv_buffer[17];

/* converts the response bytes into floats, but saves them as doubles */
/* since the data is stored as a double, and also checks data for errors */
/* takes a pointer to the data that will be written over (e.g. angles), */
/* a pointer to the error, and a pointer to isValid */
void readFloats(double* data, uint8_t* error, uint8_t* isValid){
   for (int x = 0; x < 3; x++){
      uint8_t bytes[4] = {recv_buffer[4*x+3], recv_buffer[4*x+4], recv_buffer[4*x+5], recv_buffer[4*x+6]};
      memcpy((data + x), &bytes, sizeof(float));
   }  
   /* next, take the total sum of all the bytes except the address byte */
   int totSum = 0;
   /* need to sum from command (2nd) byte to last data byte (2nd to last of array) */
   for(int x = 1; x < angleRespLength - 1; x++){
      totSum += (int)recv_buffer[x];
   }
   /* next, need to pare totSum down to its least significant byte, which can be accomplished by */
   /* comparing it to 255 */
   totSum = (totSum & 255);
   /* then compare it to the checkSum byte, which is the last byte */
   if(totSum != (int)recv_buffer[angleRespLength - 1]){
      *data = -1000.0;
      *(data + 1) = -1000.0;
      *(data + 2) = -1000.0;
      *(data + 3) = -1000.0;
      *isValid = 0;
      *error = 14;
      return;
   }
   /* lastly, if we're getting values for angles (floatsToRead is 3) then the 4th read value needs to be the error (2nd to last) byte */
   *error = (double)recv_buffer[angleRespLength - 2];
   /* check if the error is acceptable */
   if(*error == 0){
      /* if it is acceptable, then the validity flag is raised (is 1) */
      *isValid = 1;
   }else{
      /* otherwise, it is not (is 0) */
      *isValid = 0;
   }
   return;
}

/* getSunAngles() will issue the angles commmand to the sun sensor, read in the response, perform some checks */
/* and then call readFloats to interpret and check the data */
/* it needs the pointer to the sun structure */
void getSunAngles(sun_t * Sun){
   /* issue command */
   #if ARDUINO_CODE
      for(int i = 0; i < 4; i++){
         Serial1.write(anglesComm[i]);
      }
      delay(10);
   #else
      error = LPUART_RTOS_Send(&uart3_handle, &anglesComm, sizeof(anglesComm));
      if(error != kStatus_Success){
         *(Sun->angles) = -2000.0;
         *(Sun->angles + 1) = -2000.0;
         *(Sun->angles + 2) = -2000.0;
         *(Sun->angles + 3) = -2000.0
         *(Sun->error) = 16;
         *(Sun->isValid) = 0;
         return;
      }
      vTaskDelay(xDelay10ms);
   #endif
   /* read response */
   #if ARDUINO_CODE
      for(int i = 0; i < angleRespLength; i++){
         recv_buffer[i] = Serial1.read();
      }
   #else
      size_t numRecvBytes = 0;
      error = LPUART_ReadBlocking(LPUART3, &recv_buffer, angleRespLength);
      if(error != kStatus_Success){
         *(Sun->angles) = -2000.0;
         *(Sun->angles + 1) = -2000.0;
         *(Sun->angles + 2) = -2000.0;
         *(Sun->angles + 3) = -2000.0
         *(Sun->error) = 16;
         *(Sun->isValid) = 0;
         return;
      }
   #endif
   /* check response */
   if(anglesComm[1] == recv_buffer[1]){
      readFloats(Sun->angles, Sun->error, Sun->isValid);
   }else{
      *(Sun->angles) = -1000.0;
      *(Sun->angles + 1) = -1000.0;
      *(Sun->angles + 2) = -1000.0;
      *(Sun->angles + 3) = -1000.0;
      *(Sun->error) = 15;
      *(Sun->isValid) = 0;
      return;
   }
   return;
}